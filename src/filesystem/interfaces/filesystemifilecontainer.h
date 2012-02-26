#ifndef FILESYSTEMIFILECONTAINER_H_
#define FILESYSTEMIFILECONTAINER_H_

#include <QtCore/QString>
#include "filesystemifile.h"


FILE_SYSTEM_NS_BEGIN

class IFileContainer
{
public:
	virtual ~IFileContainer();

	virtual bool isPhysical() const = 0;
	virtual QString location() const = 0;
	virtual IFileInfo::size_type freeSpace() const = 0;

	virtual bool contains(const QString &fileName) const = 0;
	virtual bool remove(const QString &fileName, QString &error) const = 0;

	virtual IFile *open(const QString &fileName) const = 0;
	virtual IFileContainer *open(const QString &fileName, bool create, QString &error) const = 0;

	virtual bool copy(const IFileInfo *source, QString &error) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTAINER_H_ */
