#include "filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

Node::Node(const ModelContainer &conteiner, Node *parent) :
	FileSystemModel(conteiner, parent),
	m_links(0)
{}

INode *Node::root() const
{
	if (INode *res = Node::parent())
		return res->root();
	else
		return (INode*)this;
}

int Node::columnsCount() const
{
	return columnCount(QModelIndex());
}

void Node::viewCloseAll()
{
	if (!m_view.isEmpty())
	{
		QModelIndex index;
		qint32 count = m_view.size();
		Node *parent = Node::parent();

		while (!parent->exists())
			parent = static_cast<Node*>(parent->Node::parent());

		for (ViewSet::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
			parent->viewThis(*it, index);

		removeLinks(count);
	}

	if (!isLinked())
		static_cast<Node*>(parent())->allChildLinksRemoved(this);
}

void Node::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

void Node::viewParent(INodeView *nodeView)
{
	Node *parent = Node::parent();

	if (static_cast<INode*>(parent) != root())
		if (parent->exists())
		{
			parent->viewThis(nodeView, m_parentEntryIndex);
			parent->refresh();

			removeView(nodeView);
		}
		else
			viewCloseAll();
}

void Node::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, proxyModel(), itemDelegate(), menuActions());

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

void Node::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex selected;

	if (Node *node = viewChild(idx, plugins, selected))
	{
		if (node == Node::parent())
			node->viewThis(nodeView, m_parentEntryIndex);
		else
			node->viewThis(nodeView, selected);

		node->refresh();
		removeView(nodeView);
	}
}

void Node::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	QModelIndex selected;

	if (Node *node = viewChild(*path, plugins, selected))
	{
		if ((++path).atEnd())
		{
			if (node == Node::parent())
				node->viewThis(nodeView, m_parentEntryIndex);
			else
				node->viewThis(nodeView, selected);

			node->refresh();
		}
		else
			node->viewChild(nodeView, path, plugins);

		removeView(nodeView);
	}
}

void Node::viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Path path(absoluteFilePath);
	Path::Iterator it = path.begin();

	if (!it.atEnd())
		if (path.isAbsolute())
		{
			removeView(nodeView);
			root()->viewChild(nodeView, it, plugins);
		}
		else
			viewChild(nodeView, it, plugins);
}

QStringList Node::toFileNameList(const InfoListItem *files) const
{
	QStringList res;
	res.reserve(files->size());

	for (InfoListItem::size_type i = 0, size = files->size(); i < size; ++i)
		res.push_back(files->at(i)->fileName());

	return res;
}

void Node::nodeRemoved(Node *node)
{}

void Node::addLink()
{
	++m_links;

	if (parent())
		static_cast<Node*>(parent())->addLink();
}

void Node::removeLink()
{
	removeLinks(1);

	if (!isLinked())
		static_cast<Node*>(parent())->allChildLinksRemoved(this);
}

bool Node::isLinked() const
{
	return m_links;
}

void Node::removeLinks(qint32 count)
{
	m_links -= count;

	if (parent())
		static_cast<Node*>(parent())->removeLinks(count);
}

void Node::allChildLinksRemoved(Node *child)
{
	if (isLinked())
	{
		nodeRemoved(child);
		delete child;
	}
	else
		if (parent())
		{
			delete child;
			static_cast<Node*>(parent())->allChildLinksRemoved(this);
		}
		else
		{
			nodeRemoved(child);
			delete child;
		}
}

void Node::addView(INodeView *view)
{
	m_view.insert(view);
	addLink();
}

void Node::removeView(INodeView *view)
{
	if (m_view.remove(view))
		removeLink();
}

FILE_SYSTEM_NS_END
