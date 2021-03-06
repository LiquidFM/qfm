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
#ifndef IDM_ROOTNODE_H_
#define IDM_ROOTNODE_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include "idm_rootnodedelegate.h"
#include "../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN

class RootNode : public TasksNode
{
public:
    RootNode(IFileContainer::Holder &container, Node *parent = 0);
    virtual ~RootNode();

    /* FileSystemModel */
    virtual int columnCount(const QModelIndex &parent) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
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
    virtual void nodeRemoved(Node *node);

protected:
    /* TasksNode */
    virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
    virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
    virtual void performActionEvent(const AsyncAction::FilesList &files, const QString &error);

protected:
    virtual void create(const QModelIndex &index, INodeView *view);
    virtual void remove(const QModelIndexList &list, INodeView *view);
    virtual void search(const QModelIndex &index, INodeView *view);

private:
    enum MenuId
    {
        Create,
        Remove,
        AddProperty,
        RemoveProperty,
        Find
    };

    enum Items
    {
        RootItemIndex = 0,
        FilesItemIndex = 1
    };

private:
    class Container : public TasksNode::Container
    {
    public:
        typedef QList<Item*> List;

    public:
        Container();
        virtual ~Container();

        virtual size_type size() const;
        virtual Item *at(size_type index) const;
        virtual size_type indexOf(Item *item) const;

    private:
        friend class RootNode;
        List m_container;
    };

private:
    void createEntity();
    void removeEntity(const QModelIndex &index);
    void addProperty(const QModelIndex &index);
    void removeProperty(const QModelIndex &index);

private:
    void doAdd(const Entity &entity);
    void doRemove(const Entity &entity);
    void doAdd(const QModelIndex &index, Container::Item *item, const Entity &property, const ::EFC::String &propertyName);
    void doRemove(const QModelIndex &index, Container::Item *item, Container::Item *property);
    void expand(Container::Item *parent);

private:
    enum ShortcutType
    {
        NoShortcut,

        CreateShortcut,
        RemoveShortcut,
        SearchShortcut,

        SizeOf_ShortcutType
    };

    typedef QMap<quint32, ShortcutType> Shortcuts;

private:
    typedef QMap<Entity, Container::List> EntitiesMap;

private:
    INodeView::MenuActionList m_actions;
    Container m_itemsContainer;
    Container::List &m_items;
    EntitiesMap m_entities;
    Shortcuts m_shortcuts;
    IdmContainer m_container;
    RootNodeDelegate m_delegate;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODE_H_ */
