/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "default_searchnode.h"
#include "tasks/default_searchtask.h"
#include "items/default_searchnodeitem.h"
#include "../../model/items/default_rootnodeitem.h"
#include "../../../../application.h"


DEFAULT_PLUGIN_NS_BEGIN

SearchNode::SearchNode(IFileContainer::Holder &container, IFileContainerScanner::Filter::Holder &filter, Node *parent) :
	TasksNode(m_items, parent),
	m_container(container.take()),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	RootNodeItem::Holder root(new RootNodeItem());

	m_items.list.push_back(root);
	root->lock(tr("Searching..."));

	addTask(new SearchTask(Snapshot(m_container), filter, this), m_items.list.at(RootItemIndex));

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);

	m_shortcuts[Qt::NoModifier + Qt::Key_F5] = CopyShortcut;
	m_shortcuts[Qt::NoModifier + Qt::Key_F6] = MoveShortcut;
}

bool SearchNode::event(QEvent *e)
{
	switch (static_cast<FilesBaseTask::Event::Type>(e->type()))
	{
		case FilesBaseTask::Event::SearchFiles_NewFile:
		{
			e->accept();
			searchNewFileEvent(static_cast<BaseTask::Event *>(e));
			return true;
		}

		case FilesBaseTask::Event::SearchFiles_Done:
		{
			e->accept();
			searchCompleteEvent(static_cast<BaseTask::Event *>(e));
			return true;
		}

		default:
			return TasksNode::event(e);
	}
}

int SearchNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant SearchNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		switch (section)
		{
			case 0:
			{
				return tr("Name");
				break;
			}
			case 1:
			{
				return tr("Size");
				break;
			}
			case 2:
			{
				return tr("Modified");
				break;
			}
		}

	return QVariant();
}

void SearchNode::refresh()
{

}

QString SearchNode::title() const
{
	return tr("Search results");
}

QString SearchNode::location() const
{
	return m_container->location();
}

QString SearchNode::fileName(const QModelIndex &index) const
{
	return QString();
}

bool SearchNode::shortcut(INodeView *view, QKeyEvent *event)
{
	switch (m_shortcuts.value(event->modifiers() + event->key(), NoShortcut))
	{
		case CopyShortcut:
			copy(view, view->opposite());
			return true;

		case MoveShortcut:
			move(view, view->opposite());
			return true;

		default:
			return false;
	}
}

QAbstractItemModel *SearchNode::model() const
{
	return const_cast<ProxyModel *>(&m_proxy);
}

QAbstractItemDelegate *SearchNode::delegate() const
{
	return const_cast<Delegate *>(&m_delegate);
}

const INodeView::MenuActionList &SearchNode::actions() const
{
	return m_menuActions;
}

void SearchNode::menuAction(INodeView *view, QAction *action)
{

}

ICopyControl *SearchNode::createControl(INodeView *view) const
{
	return NULL;
}

void SearchNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

}

void SearchNode::cancel(const QModelIndexList &list, INodeView *view)
{
	QString reason = tr("Canceling...");
	SearchNodeItem *item;
	TasksMap::List items;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
	{
		item = static_cast<SearchNodeItem *>(m_proxy.mapToSource(list.at(i)).internalPointer());
		items = cancelTaskAndTakeItems(NodeItem::Holder(item));

		for (TasksMap::List::size_type i = 0, size = items.size(); i < size; ++i)
		{
			items.at(i).as<SearchNodeItem>()->cancel(reason);
			updateFirstColumn(m_items.list.indexOf(items.at(i)), items.at(i).as<NodeItem>());
		}
	}
}

void SearchNode::copy(const INodeView *source, INodeView *destination)
{
//	QModelIndex index = m_proxy.mapToSource(source->currentIndex());
//
//	if (index.isValid())
//	{
//		ICopyControl::Holder control(destination->node()->createControl(destination));
//
//		if (control)
//		{
//			SearchNodeItem *item = static_cast<SearchNodeItem *>(index.internalPointer());
//
//			item->lock(tr("Extracting..."));
//			updateFirstColumn(item);
//			addTask(new PerformCopyTask(m_info.absoluteFilePath(), item, control, false, this), TasksItemList() << item);
//		}
//	}
}

void SearchNode::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex SearchNode::rootIndex() const
{
	return m_proxy.mapFromSource(createIndex(0, 0, m_items.list.at(RootItemIndex).data()));
}

QModelIndex SearchNode::childIndex(const QString &fileName)
{
	return QModelIndex();
}

Node *SearchNode::viewChild(const QModelIndex &idx, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<NodeItem *>(index.internalPointer())->isRootItem())
		return parentNode();
	else
		if (!static_cast<NodeItem *>(index.internalPointer())->isLocked())
		{
			SearchNodeItem *entry = static_cast<SearchNodeItem *>(index.internalPointer());

			if (entry->node())
				return entry->node();
			else
				if (Node *node = Application::rootNode()->open(entry->container(), entry->info(), const_cast<SearchNode *>(this)))
				{
					entry->setNode(node);
					return node;
				}
				else
					if (entry->info()->isFile())
						Application::open(entry->container(), entry->info());
		}

	return NULL;
}

Node *SearchNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	return NULL;
}

void SearchNode::updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed)
{

}

void SearchNode::completedProgressEvent(const Item::Holder &item, quint64 timeElapsed)
{

}

void SearchNode::performActionEvent(const AsyncAction::FilesList &files, const QString &error)
{

}

void SearchNode::searchNewFileEvent(BaseTask::Event *e)
{
	SearchTask::NewFileEvent *event = static_cast<SearchTask::NewFileEvent *>(e);
	NodeItem::Holder item(new SearchNodeItem(event->file()));

	beginInsertRows(QModelIndex(), m_items.list.size(), m_items.list.size());
	m_items.list.push_back(item);
	endInsertRows();
}

void SearchNode::searchCompleteEvent(BaseTask::Event *e)
{
	SearchTask::DoneEvent *event = static_cast<SearchTask::DoneEvent *>(e);

	m_items.list.at(RootItemIndex).as<SearchNodeItem>()->unlock();
	updateFirstColumn(RootItemIndex, m_items.list.at(RootItemIndex).as<SearchNodeItem>());
	removeAllTaskLinks(event->task);
}

SearchNode::Container::size_type SearchNode::Container::size() const
{
	return list.size();
}

SearchNode::Container::Item *SearchNode::Container::at(size_type index) const
{
	return list.at(index).data();
}

SearchNode::Container::size_type SearchNode::Container::indexOf(Item *item) const
{
	NodeItem::Holder itemHolder(static_cast<NodeItem *>(item));
	return list.indexOf(itemHolder);
}

void SearchNode::updateFirstColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx(createIndex(index, 0, entry));
	emit dataChanged(idx, idx);
}

void SearchNode::updateSecondColumn(Container::size_type index, NodeItem *entry)
{
	QModelIndex idx(createIndex(index, 1, entry));
	emit dataChanged(idx, idx);
}

DEFAULT_PLUGIN_NS_END
