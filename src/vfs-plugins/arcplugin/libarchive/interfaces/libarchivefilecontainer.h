#ifndef LIBARCHIVEFILECONTAINER_H_
#define LIBARCHIVEFILECONTAINER_H_

#include <QtCore/QSharedData>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "libarchivescanner.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class FileContainer : public IFileContainer
{
public:
	static IFileContainer *create(const IFileContainer *container, const IFileInfo *file, QString &error);
	static QString extractDirectoryName(const IFileInfo *file);
	static QString extractArchivedFileName(const IFileInfo *file);

	/* IFileContainer */
	virtual bool isDefault() const;
	virtual IFileInfo::size_type freeSpace() const;
	virtual ICopyControl *createControl(INodeView *view) const;

	virtual const Location &location() const;
	virtual Location location(const IFileInfo *info) const;
	virtual Location location(const QString &fileName) const;

	virtual bool contains(const QString &fileName) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;

	virtual bool remove(const IFileInfo *info, QString &error) const;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const;

	virtual IFileContainer *open() const;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const;
	virtual IFileContainer *create(const QString &fileName, QString &error) const;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const;

	virtual const IFileContainerScanner *scanner() const;
	virtual const IApplications *applications() const;

private:
	FileContainer(IFileContainer::Holder &container, IFileAccessor::Holder &file, const Location &location);

private:
	struct Data : public QSharedData
	{
		Data(IFileContainer::Holder &container, IFileAccessor::Holder &file, const Location &location) :
			container(container.take()),
			location(location),
			scanner(this->container.data(), file)
		{}

		IFileContainer::Holder container;
		Location location;
		Scanner scanner;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVEFILECONTAINER_H_ */