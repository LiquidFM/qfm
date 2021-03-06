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
#ifndef ARC_NODE_H_
#define ARC_NODE_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include <tools/containers/union.h>
#include "arc_proxymodel.h"
#include "../model/arc_delegate.h"


ARC_PLUGIN_NS_BEGIN

class ArcNode : public TasksNode
{
public:
	ArcNode(IFileContainer::Holder &container, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual QString fileName(const QModelIndex &index) const;
	virtual bool shortcut(INodeView *view, QKeyEvent *event);

	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;
	virtual void menuAction(INodeView *view, QAction *action);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QModelIndex childIndex(const QString &fileName);
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncAction::FilesList &files, const QString &error);

	void scanCompleteEvent(BaseTask::Event *event);
	void copyCompleteEvent(BaseTask::Event *event);

protected:
	virtual void copy(const INodeView *source, INodeView *destination);

private:
	bool isUpdating() const;

private:
	enum { RootItemIndex = 0 };

	class ItemsContainer : public Container
	{
	public:
		typedef QList<VFS::Item::Holder> List;

	public:
		ItemsContainer();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class ArcNode;
		List m_container;
	};

private:
	typedef ::Tools::Containers::Union Union;
	void updateFirstColumn(const Item::Holder &entry);
	void updateSecondColumn(const Item::Holder &entry);

private:
	IFileContainer::Holder m_container;
	ItemsContainer m_itemsContainer;
    ItemsContainer::List &m_items;
    ProxyModel m_proxy;
    Delegate m_delegate;
    INodeView::MenuActionList m_actions;
};

ARC_PLUGIN_NS_END

#endif /* ARC_NODE_H_ */
