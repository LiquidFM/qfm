#ifndef FILESYSTEMINFO_H_
#define FILESYSTEMINFO_H_

#include "../tools/filesystemfileinfo.h"
#include "../interfaces/filesystemifile.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFile
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QString &filePath, bool identifyType);

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

	/* IFile */
	virtual bool rename(const QString &newName, QString &error);
	virtual IFileAccessor *open(int mode, QString &error) const;

public:
	bool isRoot() const { return m_isRoot; }
	bool shouldBeUpdatedBy(const Info &other) const
	{
		return
			m_info.lastModified != other.m_info.lastModified ||
			m_info.size != other.m_info.size ||
			m_info.type.name.isEmpty();
	}

private:
    bool m_isRoot;
    QString m_filePath;
    QString m_fileName;
    FileInfo m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFO_H_ */
