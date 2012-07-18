#ifndef ARCNODE_H_
#define ARCNODE_H_

#include <vfs/tasks/vfs_tasksnode.h>
#include "arcnodeproxymodel.h"
#include "../model/arcdelegate.h"
#include "../../../tools/containers/union.h"


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
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);
	virtual ::History::Entry *search(const QModelIndex &index, INodeView *view);

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual QString location() const;
	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files, const QString &error);

	void scanCompleteEvent(BaseTask::Event *event);
	void copyCompleteEvent(BaseTask::Event *event);

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
    ArcNodeProxyModel m_proxy;
    ArcDelegate m_delegate;
    INodeView::MenuActionList m_actions;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODE_H_ */
