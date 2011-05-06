#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include <QtCore/QMap>
#include "filesystem_ns.h"
#include "filesystempluginsmanager.h"
#include "folder/filesystemfoldernode.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public FolderNode
{
public:
	RootNode(PluginsManager *plugins);

	void view(const QString &absoluteFilePath, QAbstractItemView *itemView);

protected:
	virtual bool isRoot() const { return true; }

private:
	PluginsManager *m_plugins;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
