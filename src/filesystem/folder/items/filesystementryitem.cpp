#include "filesystementryitem.h"
#include "../../tools/filesystemcommontools.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

FileSystemEntryItem::FileSystemEntryItem(const Info &info, Node *node, Item *parent) :
	FileSystemBaseItem(info, parent),
	m_locked(false)
{
	setNode(node);
}

QVariant FileSystemEntryItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return fileName();
				case Qt::DecorationRole:
					if (m_locked)
						return Application::style()->standardIcon(QStyle::SP_BrowserReload);
					else
						return info().icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					if (m_locked)
						return m_lockReason;
					else
						break;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					if (isFile())
						return Tools::humanReadableSize(info().size());
					else
						if (m_totalSize.isNull())
							return QString::fromLatin1("<DIR>");
						else
							return Tools::humanReadableSize(m_totalSize.toULongLong());
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return lastModified();
//					case Qt::DecorationRole:
//						return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
//					case Qt::ToolTipRole:
//						return QDir::toNativeSeparators(QDir::cleanPath(m_info.fileInfo().absoluteDir().absolutePath()));
			}
			break;
		}
	}

	return QVariant();
}

bool FileSystemEntryItem::isRootItem() const
{
	return false;
}

bool FileSystemEntryItem::isListItem() const
{
	return false;
}

void FileSystemEntryItem::lock(const QString &reason, quint64 totalSize)
{
	m_locked = true;
	m_lockReason = reason;
	m_totalSize = totalSize;
	m_doneSize = 0;
	m_timeElapsed = 0;
}

void FileSystemEntryItem::lock(const QString &reason)
{
	m_locked = true;
	m_lockReason = reason;
}

void FileSystemEntryItem::unlock()
{
	m_locked = false;
	m_lockReason.clear();
	m_doneSize.clear();
	m_timeElapsed.clear();
}

FILE_SYSTEM_NS_END
