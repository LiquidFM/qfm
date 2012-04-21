#include "idmfoldernode.h"
#include "../../control/idmcopycontrol.h"
#include "../../gui/choose/chooseentitydialog.h"
#include "../../../../filesystem/filesystempluginsmanager.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmFolderNode::IdmFolderNode(IFileContainer::Holder &container, const IdmContainer &storage, Node *parent) :
	FolderNode(container, parent),
	m_container(storage)
{}

ICopyControl *IdmFolderNode::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseEntityDialog::chooseFile(m_container, Application::mainWindow()))
		return new IdmCopyControl(m_container, container(), entity);
	else
		return NULL;
}

void IdmFolderNode::rename(const QModelIndex &index, INodeView *view)
{

}

void IdmFolderNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void IdmFolderNode::move(const INodeView *source, INodeView *destination)
{

}

void IdmFolderNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

Node *IdmFolderNode::createNode(const IFileInfo *file, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(container(), file, const_cast<IdmFolderNode *>(this)))
		return res;
	else
		if (file->isDir())
		{
			QString error;
			IFileContainer::Holder folder(container()->open(file->fileName(), false, error));

			if (folder)
				return new IdmFolderNode(folder, m_container, const_cast<IdmFolderNode *>(this));
			else
				QMessageBox::critical(Application::mainWindow(), tr("Error"), error);
		}

	return NULL;
}

Node *IdmFolderNode::privateViewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return FolderNode::viewChild(fileName, plugins, selected);
}

IDM_PLUGIN_NS_END