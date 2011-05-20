#include "filesystemfoldernode.h"
#include "items/filesystemfoldernoderoot.h"
#include "items/filesystemfoldernodeentry.h"
#include "events/filesystemmodelevents.h"
#include "tasks/scan/updatefilestask.h"
#include "tasks/scan/scanfilestasks.h"
#include "../filesystempluginsmanager.h"
#include "../../tools/rangeintersection.h"
#include "../../application.h"
#include <iostream>


FILE_SYSTEM_NS_BEGIN

FolderNode::FolderNode(const Info &info, Node *parent) :
	FolderNodeBase(parent),
	m_updating(false),
	m_info(info),
	m_proxy(this),
	m_delegate(&m_proxy)
{
	if (!m_info.isRoot())
		items().add(new FolderNodeRoot());

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

bool FolderNode::event(QEvent *e)
{
	switch (static_cast<ModelEvent::Type>(e->type()))
	{
		case ModelEvent::ListFiles:
		{
			e->accept();
			listEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdateFiles:
		{
			e->accept();
			updateFilesEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForRemove:
		{
			e->accept();
			scanForRemoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCompleted:
		{
			e->accept();
			removeCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::RemoveFilesCanceled:
		{
			e->accept();
			removeCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForSize:
		{
			e->accept();
			scanForSizeEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForCopy:
		{
			e->accept();
			scanForCopyEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCompleted:
		{
			e->accept();
			copyCompleteEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::CopyFilesCanceled:
		{
			e->accept();
			copyCanceledEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::ScanFilesForMove:
		{
			e->accept();
			scanForMoveEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::QuestionAnswer:
		{
			e->accept();
			questionAnswerEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		case ModelEvent::UpdatePerformProgress:
		{
			e->accept();
			updateProgressEvent(static_cast<ModelEvent*>(e)->parameters());
			return true;
		}
		default:
			break;
	}

	return QAbstractItemModel::event(e);
}

int FolderNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
        return 0;
	else
    	return items().size();
}

int FolderNode::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QVariant FolderNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<FolderNodeItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return items().at(index.row()).item->data(index.column(), role);
}

Qt::ItemFlags FolderNode::flags(const QModelIndex &index) const
{
    if (index.isValid())
    	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else
        return Qt::NoItemFlags;
}

QVariant FolderNode::headerData(int section, Qt::Orientation orientation, int role) const
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

QModelIndex FolderNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, items().at(row).item);
    else
        return QModelIndex();
}

QModelIndex FolderNode::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

bool FolderNode::isDir() const
{
	return true;
}

bool FolderNode::isFile() const
{
	return false;
}

bool FolderNode::exists() const
{
	((FolderNode*)this)->m_info.refresh();
	return m_info.exists();
}

QString FolderNode::fileName() const
{
	return m_info.fileName();
}

QString FolderNode::absolutePath() const
{
	return m_info.absolutePath();
}

QString FolderNode::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString FolderNode::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime FolderNode::lastModified() const
{
	return m_info.lastModified();
}

IFile *FolderNode::open(IFile::OpenMode mode, QString &error) const
{
	return 0;
}

IFileInfo *FolderNode::create(const QString &fileName, FileType type, QString &error) const
{
	return 0;
}

void FolderNode::refresh()
{
	if (isUpdating())
		return;

	if (isRootNode())
		updateFiles();
	else
		if (exists())
			updateFiles();
		else
		{
			Node *node = static_cast<Node*>(Node::parent());

			while (!node->exists())
				node = static_cast<Node*>(node->Node::parent());

			switchTo(node);
		}
}

void FolderNode::remove(const QModelIndexList &list)
{
	processIndexList(list, Functors::callTo(this, &FolderNode::removeFunctor));
}

void FolderNode::calculateSize(const QModelIndexList &list)
{
	processIndexList(list, Functors::callTo(this, &FolderNode::calculateSizeFunctor));
}

void FolderNode::copy(const QModelIndexList &list, Node *destination)
{
	processIndexList(list, Functors::callTo(this, &FolderNode::copyFunctor, destination));
}

void FolderNode::move(const QModelIndexList &list, Node *destination)
{
	processIndexList(list, Functors::callTo(this, &FolderNode::moveFunctor, destination));
}

void FolderNode::view(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, &m_proxy, &m_delegate);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

void FolderNode::view(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
	{
		removeView(nodeView);
		static_cast<Node*>(Node::parent())->view(nodeView, parentEntryIndex());
		static_cast<Node*>(Node::parent())->refresh();
	}
	else
		if (!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(index.internalPointer());
			entry->refresh();

			if (entry->exists())
			{
				Q_ASSERT(items().indexOf(entry) != Values::InvalidIndex);
				Values::Value &value = items()[items().indexOf(entry)];

				if (value.node != 0)
					value.node->setParentEntryIndex(idx);
				else
					if (value.node = createNode(*value.item, plugins))
						value.node->setParentEntryIndex(idx);

				if (value.node)
				{
					removeView(nodeView);
					value.node->view(nodeView, QModelIndex());
					value.node->refresh();
				}
			}
			else
				removeEntry(index);
		}
}

void FolderNode::view(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
	Node *node;
	Values::Value *value;
	Values::size_type index = items().indexOf(*path);

	if (index == Values::InvalidIndex)
	{
		items().add(createNode(*path, plugins, node));
		value = &items().last();
	}
	else
	{
		value = &items()[index];

		if (value->node)
			node = value->node;
		else
			value->node = node = createNode(*(value->item), plugins);
	}

	if (node)
	{
		removeView(nodeView);

		if ((++path).atEnd())
			node->view(nodeView, QModelIndex());
		else
			node->view(nodeView, path, plugins);
	}
	else
		if (value->item->isFile())
			view(nodeView, indexForFile(value->item));
}

void FolderNode::view(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Node *node = this;
	removeView(nodeView);

	while (node->parent())
		node = static_cast<Node*>(node->parent());

	node->view(nodeView, Path(absoluteFilePath).begin(), plugins);
}

QModelIndex FolderNode::rootIndex() const
{
	if (isRootNode())
		return QModelIndex();
	else
		return m_proxy.mapFromSource(createIndex(0, 0, items().at(0).item));
}

bool FolderNode::isRootIndex(const QModelIndex &index) const
{
	if (isRootNode())
		return false;
	else
		return static_cast<FolderNodeItem*>(m_proxy.mapToSource(index).internalPointer())->isRootItem();
}

void FolderNode::processIndexList(const QModelIndexList &list, const Functors::Functor &functor)
{
	QModelIndex index;
	FolderNodeItem *entry;
	QSet<FolderNodeItem*> done;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (!done.contains(entry = static_cast<FolderNodeItem*>((index = m_proxy.mapToSource(list.at(i))).internalPointer())))
		{
			done.insert(entry);

			if (!entry->isRootItem() && !static_cast<FolderNodeEntry*>(entry)->isLocked())
			{
				entry->refresh();

				if (entry->exists())
					functor(entry);
				else
					removeEntry(index);
			}
		}
}

void FolderNode::removeFunctor(FolderNodeItem *entry)
{
	if (entry->isDir())
		scanForRemove(entry);
	else
		removeEntry(entry);
}

void FolderNode::calculateSizeFunctor(FolderNodeItem *entry)
{
	if (entry->isDir())
		scanForSize(entry);
	else
		updateSecondColumn(entry);
}

void FolderNode::copyFunctor(FolderNodeItem *entry, Node *destination)
{
	if (entry->isDir())
		scanForCopy(entry, destination);
	else
		if (entry->isFile())
			copyEntry(entry, destination);
}

void FolderNode::moveFunctor(FolderNodeItem *entry, Node *destination)
{
	if (entry->isDir())
		scanForMove(entry, destination);
	else
		if (entry->isFile())
			moveEntry(entry, destination);
}

void FolderNode::list(FolderNodeItem *fileSystemTree)
{

}

void FolderNode::listEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateFiles()
{
	QScopedPointer<UpdateFilesTask::Params> params(new UpdateFilesTask::Params());
	params->node = this;
	params->list = makeChangeSet();

	setUpdating(true);
	Application::instance()->taskPool().handle(new UpdateFilesTask(params.take()));
}

void FolderNode::updateFilesEvent(const ModelEvent::Params *p)
{
	typedef const UpdateFilesTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	ChangesList list = params->updates;
	RangeIntersection updateRange(1);

	for (ChangesList::size_type i = 0; i < list.size();)
		if (list.at(i).type() == Change::Updated)
		{
			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(list.at(i).entry());
			Values::size_type index = items().indexOf(entry);
			(*entry) = list.at(i).info();
			updateRange.add(index, index);
			list.removeAt(i);
		}
		else
			if (list.at(i).type() == Change::Deleted)
			{
				removeEntry(items().indexOf(list.at(i).entry()));
				list.removeAt(i);
			}
			else
				++i;

	for (RangeIntersection::RangeList::size_type i = 0, size = updateRange.size(); i < size; ++i)
		emit dataChanged(createIndex(updateRange.at(i).top(), 0),
						 createIndex(updateRange.at(i).bottom(), columnCount() - 1));

	if (!list.isEmpty())
	{
		beginInsertRows(QModelIndex(), items().size(), items().size() + list.size() - 1);
		for (ChangesList::size_type i = 0, size = list.size(); i < size; ++i)
			items().add(new FolderNodeEntry(list.at(i).info()));
		endInsertRows();
	}

	if (params->isLastEvent)
		setUpdating(false);
}

void FolderNode::removeEntry(FolderNodeItem *entry)
{
	if (QMessageBox::question(
			&Application::instance()->mainWindow(),
			tr("Remove file..."),
			tr("Would you like to remove \"%1\" file?").arg(static_cast<FolderNodeEntry*>(entry)->absoluteFilePath()),
			QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
//		QScopedPointer<PerformRemoveEntryTask::Params> params(new PerformRemoveEntryTask::Params());
//		params->source.object = (QObject*)this;
//		params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//		params->source.entry = static_cast<FileSystemEntry*>(entry);
//
//		static_cast<FileSystemEntry*>(entry)->lock(tr("Removing..."));
//		updateFirstColumn(fileSystemTree, entry);
//
//		Application::instance()->taskPool().handle(new PerformRemoveEntryTask(params.take()));
	}
}

void FolderNode::scanForRemove(FolderNodeItem *entry)
{
	QScopedPointer<ScanFilesForRemoveTask::Params> params(new ScanFilesForRemoveTask::Params());
	params->source.node = this;
	params->source.entry = static_cast<FolderNodeEntry*>(entry);

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for remove..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(new ScanFilesForRemoveTask(params.take()));
}

void FolderNode::scanForRemoveEvent(const ModelEvent::Params *p)
{

}

void FolderNode::removeCompleteEvent(const ModelEvent::Params *p)
{

}

void FolderNode::removeCanceledEvent(const ModelEvent::Params *p)
{

}

void FolderNode::scanForSize(FolderNodeItem *entry)
{
	QScopedPointer<ScanFilesForSizeTask::Params> params(new ScanFilesForSizeTask::Params());
	params->source.node = this;
	params->source.entry = static_cast<FolderNodeEntry*>(entry);

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for size..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(new ScanFilesForSizeTask(params.take()));
}

void FolderNode::scanForSizeEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForSizeTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = items().indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
		items()[index].node = params->subnode;
		params->snapshot.entry->setTotalSize(params->size);
		params->snapshot.entry->unlock();
		updateBothColumns(params->snapshot.entry);
	}
	else
		delete params->subnode;
}

void FolderNode::copyEntry(FolderNodeItem *entry, Node *destination)
{
//	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
//	params->source.object = (QObject*)this;
//	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//	params->source.entry = static_cast<FileSystemEntry*>(entry);
//	params->destination.object = destination;
//	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
//	params->removeSource = false;
//
//	static_cast<FileSystemEntry*>(entry)->lock(tr("Copying..."), params->source.entry->fileInfo().size());
//	updateFirstColumn(fileSystemTree, entry);
//
//	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FolderNode::scanForCopy(FolderNodeItem *entry, Node *destination)
{
	QScopedPointer<ScanFilesForCopyTask::Params> params(new ScanFilesForCopyTask::Params());
	params->source.node = this;
	params->source.entry = static_cast<FolderNodeEntry*>(entry);
	params->destination.node = destination;

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for copy..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(new ScanFilesForCopyTask(params.take()));
}

void FolderNode::moveEntry(FolderNodeItem *entry, Node *destination)
{
//	QScopedPointer<PerformCopyEntryTask::Params> params(new PerformCopyEntryTask::Params());
//	params->source.object = (QObject*)this;
//	params->source.fileSystemTree = static_cast<FileSystemTree*>(fileSystemTree);
//	params->source.entry = static_cast<FileSystemEntry*>(entry);
//	params->destination.object = destination;
//	params->destination.fileSystemTree = static_cast<FileSystemTree*>(destination->m_currentFsTree);
//	params->removeSource = true;
//
//	static_cast<FileSystemEntry*>(entry)->lock(tr("Moving..."), params->source.entry->fileInfo().size());
//	updateFirstColumn(fileSystemTree, entry);
//
//	Application::instance()->taskPool().handle(new PerformCopyEntryTask(params.take()));
}

void FolderNode::scanForMove(FolderNodeItem *entry, Node *destination)
{
	QScopedPointer<ScanFilesForMoveTask::Params> params(new ScanFilesForMoveTask::Params());
	params->source.node = this;
	params->source.entry = static_cast<FolderNodeEntry*>(entry);
	params->destination.node = destination;

	static_cast<FolderNodeEntry*>(entry)->lock(tr("Scanning folder for move..."));
	updateFirstColumn(entry);

	Application::instance()->taskPool().handle(new ScanFilesForMoveTask(params.take()));
}

void FolderNode::scanForCopyEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForCopyTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = items().indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
		items()[index].node = params->subnode;
		params->snapshot.entry->lock(tr("Copying..."), params->size);
		updateSecondColumn(params->snapshot.entry);

//		Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, false)));
	}
	else
		delete params->subnode;
}

void FolderNode::scanForMoveEvent(const ModelEvent::Params *p)
{
	typedef const ScanFilesForMoveTask::Event::Params *ParamsType;
	ParamsType params = static_cast<ParamsType>(p);
	Values::size_type index = items().indexOf(params->snapshot.entry);

	if (index != Values::InvalidIndex)
	{
		items()[index].node = params->subnode;
		params->snapshot.entry->lock(tr("Moving..."), params->size);
		updateSecondColumn(params->snapshot.entry);

//		Application::instance()->taskPool().handle(new PerformCopyTreeTask(new PerformCopyTreeTask::Params((QObject*)this, *params, true)));
	}
	else
		delete params->subnode;
}

void FolderNode::copyCompleteEvent(const ModelEvent::Params *p)
{

}

void FolderNode::copyCanceledEvent(const ModelEvent::Params *p)
{

}

void FolderNode::questionAnswerEvent(const ModelEvent::Params *p)
{

}

void FolderNode::updateProgressEvent(const ModelEvent::Params *p)
{

}

ChangesList FolderNode::makeChangeSet() const
{
	ChangesList list;

	if (isRootNode())
	{
		list.reserve(items().size());

		for (Values::size_type i = 0, size = items().size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, items().at(i).item));
	}
	else
	{
		list.reserve(items().size() - 1);

		for (Values::size_type i = 1, size = items().size(); i < size; ++i)
			list.push_back(Change(Change::NoChange, items().at(i).item));
	}

	return list;
}

QModelIndex FolderNode::index(int column, FolderNodeItem *item) const
{
	int index = items().indexOf(item);

	if (index != -1)
		return createIndex(index, column, item);
	else
		return QModelIndex();
}

Node *FolderNode::createNode(const Info &info, PluginsManager *plugins) const
{
	if (Node *res = plugins->node(&info, (FolderNode*)this))
		return res;
	else
		if (info.isDir())
			return new FolderNode(info, (FolderNode*)this);
		else
			if (info.isFile())
				return 0;

	return 0;
}

FolderNode::Values::Value FolderNode::createNode(const QString &fileName, PluginsManager *plugins, Node *&node) const
{
	Info info(absoluteFilePath(fileName));
	return Values::Value(new FolderNodeEntry(info), node = createNode(info, plugins));
}

QModelIndex FolderNode::indexForFile(FolderNodeItem *item)
{
	Q_ASSERT(items().indexOf(item) != Values::InvalidIndex);
	Values::size_type index = items().indexOf(item);
	return m_proxy.mapFromSource(createIndex(index, 0, item));
}

void FolderNode::updateFirstColumn(FolderNodeItem *entry)
{
	QModelIndex index = createIndex(items().indexOf(entry), 0, entry);
	emit dataChanged(index, index);
}

void FolderNode::updateSecondColumn(FolderNodeItem *entry)
{
	QModelIndex index = createIndex(items().indexOf(entry), 1, entry);
	emit dataChanged(index, index);
}

void FolderNode::updateBothColumns(FolderNodeItem *entry)
{
	Values::size_type index = items().indexOf(entry);
	emit dataChanged(createIndex(index, 0, entry), createIndex(index, 1, entry));
}

void FolderNode::removeEntry(Values::size_type index)
{
	beginRemoveRows(QModelIndex(), index, index);
	items().remove(index);
	endRemoveRows();
}

void FolderNode::removeEntry(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	items().remove(index.row());
	endRemoveRows();
}

void FolderNode::switchTo(Node *node)
{
	QModelIndex index;

	for (SetView::const_iterator it = m_view.constBegin(), end = m_view.constEnd(); it != end; ++it)
		node->view(*it, index);

	node->refresh();
}

void FolderNode::addView(INodeView *view)
{
	m_view.insert(view);
}

void FolderNode::removeView(INodeView *view)
{
	m_view.remove(view);
}

FILE_SYSTEM_NS_END
