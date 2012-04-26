#include "../filesystemrootnode.h"
#include "../../filesystempluginsmanager.h"


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode() :
	Node(m_items)
{}

RootNode::~RootNode()
{}

ICopyControl *RootNode::createControl(INodeView *view) const
{
	return NULL;
}

void RootNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void RootNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void RootNode::rename(const QModelIndex &index, INodeView *view)
{

}

void RootNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void RootNode::copy(const INodeView *source, INodeView *destination)
{

}

void RootNode::move(const INodeView *source, INodeView *destination)
{

}

void RootNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

QString RootNode::location() const
{
	return QString();
}

QString RootNode::location(const QString &fileName) const
{
	return QString();
}

void RootNode::refresh()
{

}

QString RootNode::title() const
{
	return QString();
}

QAbstractItemModel *RootNode::model() const
{
	return NULL;
}

QAbstractItemDelegate *RootNode::delegate() const
{
	return NULL;
}

const INodeView::MenuActionList &RootNode::actions() const
{
	return m_menuActions;
}

::History::Entry *RootNode::menuAction(QAction *action, INodeView *view)
{
	return NULL;
}

QModelIndex RootNode::rootIndex() const
{
	return QModelIndex();
}

Node *RootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return NULL;
}

Node *RootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return plugins->node(&m_container, &m_container, this);
}

RootNode::Container::size_type RootNode::Container::size() const
{
	return 0;
}

RootNode::Container::Item *RootNode::Container::at(size_type index) const
{
	return NULL;
}

RootNode::Container::size_type RootNode::Container::indexOf(Item *item) const
{
	return InvalidIndex;
}

FILE_SYSTEM_NS_END
