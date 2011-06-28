#ifndef FILESYSTEMFOLDERNODE_H_
#define FILESYSTEMFOLDERNODE_H_

#include "filesystemfolderdelegate.h"
#include "filesystemfolderproxymodel.h"
#include "events/filesystemmodelevent.h"
#include "info/filesystemfoldernodeinfo.h"
#include "containers/filesystemupdateslist.h"
#include "containers/filesystemfoldernodevalues.h"
#include "../filesystemnode.h"
#include "../../tools/metatemplates.h"
#include "../../tools/rangeintersection.h"


FILE_SYSTEM_NS_BEGIN

class FolderNode : public Node
{
	Q_DISABLE_COPY(FolderNode)

public:
	FolderNode(const Info &info, Node *parent = 0);

    virtual bool event(QEvent *e);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
	virtual INode *root() const;
	virtual int columnCount() const;
	virtual IFileControl *createControl() const;
	virtual QString absolutePath(const QModelIndex &idx) const;

	/* INode::IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual bool exists(IFileInfo *info) const;
	virtual void refresh();

	/* INode::IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

	/* INode::IFileNavigation */
	virtual void viewClosed(INodeView *nodeView);
	virtual void viewParent(INodeView *nodeView);
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected);
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	/* Node */
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }
	virtual void removeThis();
	virtual void switchTo(Node *node, const QModelIndex &selected);
	virtual void removeEntry(Node *entry);

protected:
	typedef QPair<Values::size_type, FolderNodeItem*> ProcessedValue;
	typedef QList<ProcessedValue>                     ProcessedList;
	ProcessedList processIndexList(const QModelIndexList &list);

protected:
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

protected:
	void updateFiles();
	void updateFilesEvent(const ModelEvent *event);

	void scanForRemove(const ProcessedList &entries);
	void scanForRemoveEvent(const ModelEvent *event);
	void removeCompleteEvent(const ModelEvent *event);

	void scanForSize(const ProcessedList &entries);
	void scanForSizeEvent(const ModelEvent *event);

	void scanForCopy(const ProcessedList &entries, INode *destination, bool move);
	void scanForCopyEvent(const ModelEvent *event);
	void copyCompleteEvent(const ModelEvent *event);

	void questionAnswerEvent(const ModelEvent *event);
	void updateProgressEvent(const ModelEvent *event);

private:
	QModelIndex index(int column, FolderNodeItem *item) const;
	Node *createNode(const Info &info, PluginsManager *plugins) const;
	QModelIndex indexForFile(FolderNodeItem *item) const;
	QModelIndex indexForFile(FolderNodeItem *item, Values::size_type index) const;
	QModelIndex rootIndex() const;

	void updateFirstColumn(FolderNodeItem *entry);
	void updateFirstColumn(const RangeIntersection &range);
	void updateFirstColumn(Values::size_type index, FolderNodeItem *entry);
	void updateSecondColumn(FolderNodeItem *entry);
	void updateSecondColumn(const RangeIntersection &range);
	void updateSecondColumn(Values::size_type index, FolderNodeItem *entry);
	void updateBothColumns(FolderNodeItem *entry);
	void updateBothColumns(const RangeIntersection &range);
	void updateBothColumns(Values::size_type index, FolderNodeItem *entry);
	void removeEntry(Values::size_type index);
	void removeEntry(const QModelIndex &index);

	void switchTo(Node *node, INodeView *nodeView, const QModelIndex &selected);
	bool isVisible() const;
	void addView(INodeView *view);
	void removeView(INodeView *view);


private:
	typedef QSet<INodeView*> SetView;

private:
	bool m_updating;
	Info m_info;
	Values m_items;
	SetView m_view;
	FolderProxyModel m_proxy;
	FolderDelegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODE_H_ */
