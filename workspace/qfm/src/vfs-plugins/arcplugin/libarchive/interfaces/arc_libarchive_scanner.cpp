/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "arc_libarchive_scanner.h"
#include "arc_libarchive_fileinfo.h"

#include <vfs/tools/vfs_uri.h>
#include <vfs/containers/vfs_snapshot.h>
#include <vfs/interfaces/vfs_ifileaccessor.h>

#include <string.h>
#include <archive.h>
#include <archive_entry.h>

#include <QtCore/QMap>


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class FileAccesor : public IFileAccessor
{
public:
	FileAccesor(struct archive *archive, struct archive_entry *entry) :
		m_archive(archive),
		m_permissions(archive_entry_perm(entry))
	{}

	virtual QString lastError() const
	{
		return QString::fromUtf8(archive_error_string(m_archive));
	}

	virtual int permissions() const
	{
		return
				flags_permissions[m_permissions & S_IRUSR] |
				flags_permissions[m_permissions & S_IWUSR] |
				flags_permissions[m_permissions & S_IXUSR] |
				flags_permissions[m_permissions & S_IRGRP] |
				flags_permissions[m_permissions & S_IWGRP] |
				flags_permissions[m_permissions & S_IXGRP] |
				flags_permissions[m_permissions & S_IROTH] |
				flags_permissions[m_permissions & S_IWOTH] |
				flags_permissions[m_permissions & S_IXOTH];
	}

	virtual size_type size()
	{
		return 0;
	}

	virtual bool setPermissions(int mode)
	{
		return false;
	}

	virtual size_type read(value_type *data, size_type size)
	{
		return archive_read_data(m_archive, data, size);
	}

	virtual size_type write(const value_type *data, size_type size)
	{
		return 0;
	}

	virtual size_type seek(size_type offset, Seek from)
	{
		return 0;
	}

private:
	struct archive *m_archive;
	mode_t m_permissions;
};


class Scanner::Enumerator : public Scanner::IEnumerator
{
public:
	Enumerator(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive) :
		m_reader(buffer, file, archive),
		m_archive(archive)
	{}

	virtual const IFileInfo *next()
	{
		if (archive_read_next_header(m_archive, &m_entry) == ARCHIVE_OK)
		{
			Info::Data data;

			data.path = QByteArray(archive_entry_pathname(m_entry));
			data.fileName = Info::location(data.path);
			data.fileSize = archive_entry_size(m_entry);
			data.lastModified = QDateTime::fromTime_t(archive_entry_mtime(m_entry));

			m_info = Info(data, archive_entry_filetype(m_entry) & AE_IFDIR);
			return &m_info;
		}

		return NULL;
	}

	virtual IFileInfo *info() const
	{
		return new Info(m_info);
	}

	virtual IFileAccessor *open(int mode, QString &error) const
	{
		return new FileAccesor(m_archive, m_entry);
	}

private:
	Info m_info;
	ReadArchive m_reader;
	struct archive *m_archive;
	struct archive_entry *m_entry;
};


class SnapshotItem : public ::VFS::SnapshotItem
{
public:
	typedef QMap<QByteArray, ::VFS::SnapshotItem *> Container;

public:
	SnapshotItem(const IFileContainer *container, const Location &fileName, struct archive_entry *entry, SnapshotItem *parent) :
		::VFS::SnapshotItem(container, archive_entry_size(entry), parent)
	{
		m_data.path = QByteArray(archive_entry_pathname(entry));
		m_data.fileName = fileName;
		m_data.fileSize = totalSize();
		m_data.lastModified = QDateTime::fromTime_t(archive_entry_mtime(entry));
	}
	virtual ~SnapshotItem()
	{
		qDeleteAll(m_map);
	}

	SnapshotItem *find(const QByteArray &fileName) const
	{
		return static_cast<SnapshotItem *>(m_map.value(fileName));
	}

	void insert(const QByteArray &fileName, SnapshotItem *item)
	{
		incTotalSize((m_map[fileName] = item)->totalSize());
	}

	void populateInfo()
	{
		if (m_map.isEmpty())
			info() = new Info(m_data, false);
		else
		{
			items().reserve(items().size() + m_map.size());

			for (Container::iterator i = m_map.begin(), e = m_map.end(); i != e; i = m_map.erase(i))
			{
				items().append(*i);
				static_cast<SnapshotItem *>(*i)->populateInfo();
			}

			m_data.fileSize += totalSize();
			info() = new Info(m_data, true);
		}
	}

private:
	Info::Data m_data;
	Container m_map;
};


Scanner::Scanner(const IFileContainer *container, IFileAccessor::Holder &file) :
	m_container(container),
	m_file(file.take()),
	m_archive(archive_read_new())
{
	archive_read_support_compression_all(m_archive);
	archive_read_support_format_all(m_archive);
}

Scanner::~Scanner()
{
	archive_read_finish(m_archive);
}

Scanner::IEnumerator *Scanner::enumerate(QString &error) const
{
	return new Enumerator(const_cast<Scanner *>(this)->m_buffer, m_file, m_archive);
}

void Scanner::scan(const ScanArguments &arguments, QString &error) const
{
	ReadArchive archive(const_cast<Scanner *>(this)->m_buffer, m_file, m_archive);
    QMap<QByteArray, SnapshotItem *> parents;
    IFileContainer::Holder container;
    SnapshotItem *parent;
    SnapshotItem *entry;
    struct archive_entry *e;
    QByteArray fileName;
    Location fileNameLocation;
    const char *path;
    char *sep;
    int res;

    while ((res = archive_read_next_header(m_archive, &e)) == ARCHIVE_OK && !arguments.aborted)
    {
    	if (path = archive_entry_pathname(e))
			if ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
			{
				(*sep) = 0;
				SnapshotItem *&p = parents[fileName = path];
				(*sep) = '/';

				if (p == NULL)
				{
					fileNameLocation = Info::location(fileName);
					arguments.snapshot.insert(fileNameLocation, p = parent = new SnapshotItem(m_container, fileNameLocation, e, NULL));
				}
				else
					parent = p;

				path = (++sep);

				while ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
				{
					(*sep) = 0;

					if (entry = parent->find(fileName = path))
						parent = entry;
					else
					{
						parent->insert(fileName, entry = new SnapshotItem(m_container, Info::location(fileName), e, parent));
						parent = entry;
					}

					(*sep) = '/';
					path = (++sep);
				}

				if (!(fileName = path).isEmpty() &&
					parent->find(fileName) == NULL)
				{
					parent->insert(fileName, new SnapshotItem(m_container, Info::location(fileName), e, parent));
				}
			}
			else
			{
				SnapshotItem *&p = parents[fileName = path];

				if (p == NULL)
				{
					fileNameLocation = Info::location(fileName);
					arguments.snapshot.insert(fileNameLocation, p = new SnapshotItem(m_container, fileNameLocation, e, NULL));
				}
			}

		archive_read_data_skip(m_archive);
    }

	if (!arguments.aborted)
    	if (res == ARCHIVE_EOF)
			for (Snapshot::const_iterator i = arguments.snapshot.begin(), end = arguments.snapshot.end(); i != end; ++i)
				static_cast<SnapshotItem *>((*i).second)->populateInfo();
    	else
    		error = QString::fromUtf8(archive_error_string(m_archive));
}

void Scanner::search(const SearchArguments &argument, QString &error) const
{

}

Scanner::ReadArchive::ReadArchive(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive) :
	m_buffer(buffer),
	m_file(file),
	m_archive(archive)
{
	m_mutex.lock();
	archive_read_open2(m_archive, this, open, read, skip, close);
}

Scanner::ReadArchive::~ReadArchive()
{
	archive_read_close(m_archive);
	m_mutex.unlock();
}

int	Scanner::ReadArchive::open(struct archive *archive, void *_client_data)
{
	static_cast<ReadArchive *>(_client_data)->m_file->seek(0, IFileAccessor::FromBeggining);
	return ARCHIVE_OK;
}

ssize_t Scanner::ReadArchive::read(struct archive *archive, void *_client_data, const void **_buffer)
{
	ReadArchive *self = static_cast<ReadArchive *>(_client_data);
	(*_buffer) = self->m_buffer;
	return self->m_file->read(self->m_buffer, BlockSize);
}

int64_t Scanner::ReadArchive::skip(struct archive *archive, void *_client_data, int64_t request)
{
	IFileAccessor *file = static_cast<ReadArchive *>(_client_data)->m_file.data();
	int64_t res = file->seek(0, IFileAccessor::FromCurrent);
	return file->seek(request, IFileAccessor::FromCurrent) - res;
}

int	Scanner::ReadArchive::close(struct archive *archive, void *_client_data)
{
	return ARCHIVE_OK;
}

ARC_PLUGIN_LIBARCHIVE_NS_END
