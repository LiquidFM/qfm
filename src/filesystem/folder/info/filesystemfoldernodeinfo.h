#ifndef FILESYSTEMFOLDERNODEINFO_H_
#define FILESYSTEMFOLDERNODEINFO_H_

#include <QtCore/QFileInfo>
#include <QtGui/QIcon>
#include "../../filesystem_ns.h"
#include "../../interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFileInfo
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QFileInfo &info);
#ifndef Q_OS_WIN
    Info(const QString &filePath, uint userId, uint groupId);
    Info(const QFileInfo &info, uint userId, uint groupId);
#endif

	/* IFileInfo */
	virtual bool isDir() const { return m_info.isDir(); }
	virtual bool isFile() const { return m_info.isFile(); }
	virtual bool exists() const { return m_info.exists(); }
	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const { return m_info.lastModified(); }

	virtual IFile *open(IFile::OpenMode mode, QString &error) const;
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const { return 0; }

	virtual void refresh();

public:
	bool isRoot() const { return m_info.isRoot(); }
	qint64 size() const { return m_info.size(); }
    const QIcon &icon() const;
    const QString &displayType() const;
    bool isCaseSensitive() const;

    const QFile::Permissions &permissions() const { return m_permissions; }

private:
#ifndef Q_OS_WIN
    void translatePermissions(uint userId, uint groupId);
#endif

private:
    QFileInfo m_info;
    QFile::Permissions m_permissions;
    mutable QString m_displayType;
    mutable QIcon m_icon;
#ifndef Q_OS_WIN
    static QString root;
#endif
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEINFO_H_ */
