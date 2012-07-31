#ifndef VFS_IFILETYPE_H_
#define VFS_IFILETYPE_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include "../vfs_ns.h"
#include "../filetypeinfo/vfs_filetypeinfo.h"


VFS_NS_BEGIN

class IFileType
{
public:
	virtual ~IFileType();

	virtual const FileTypeId &id() const = 0;
	virtual const QIcon &icon() const = 0;
	virtual const QString &name() const = 0;
	virtual const QString &description() const = 0;
};

VFS_NS_END

#endif /* VFS_IFILETYPE_H_ */