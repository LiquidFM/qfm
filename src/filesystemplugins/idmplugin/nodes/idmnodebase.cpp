#include "idmnodebase.h"
#include "idmnode.h"
#include "../items/idmroot.h"
#include "../items/idmmessage.h"
#include "../items/idmseparator.h"
#include "../gui/createentitydialog.h"
#include "../../../application.h"
#include <QtGui/QMessageBox>


FILE_SYSTEM_NS_BEGIN

IdmNodeBase::IdmNodeBase(IdmContainer *storage, const Info &info, Node *parent) :
	FolderNodeBase(info, parent),
	m_proxy(this),
	m_delegate(&m_proxy),
	m_container(storage)
{
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

IdmNodeBase::~IdmNodeBase()
{
	if (!m_items.isEmpty())
	{
		if (m_items.at(0)->isList())
			m_items.removeAt(0);

		qDeleteAll(m_items);
	}
}

int IdmNodeBase::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int IdmNodeBase::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant IdmNodeBase::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags IdmNodeBase::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant IdmNodeBase::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex IdmNodeBase::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex IdmNodeBase::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

IFileInfo *IdmNodeBase::info(const QModelIndex &idx) const
{
	return 0;
}

IFileControl *IdmNodeBase::createControl(const QModelIndex &idx, PluginsManager *plugins)
{
	return 0;
}

void IdmNodeBase::remove(const QModelIndexList &list)
{
	if (m_container->transaction())
	{
		IdmItem *item;
		QModelIndex index;

		for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
			if ((index = m_proxy.mapToSource(list.at(i))).isValid() &&
			    (item = static_cast<IdmItem*>(index.internalPointer()))->isEntityItem())
				if (item->parent()->isEntityItem())
				{
					if (QMessageBox::question(&Application::instance()->mainWindow(),
										  tr("Remove entity"),
										  tr("Do you really want to remove property \"%1\" of entity \"%2\"?").
										  arg(static_cast<IdmEntityItem*>(item)->entity()->name()).
										  arg(static_cast<IdmEntityItem*>(item->parent())->entity()->name()),
										  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
						if (m_container->removeProperty(static_cast<IdmEntityItem*>(item->parent())->entity(),
														static_cast<IdmEntityItem*>(item)->entity()))
						{
							beginRemoveRows(parent(index), index.row(), index.row());
							delete item;
							endRemoveRows();
						}
						else
						{
							QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
							m_container->rollback();
							return;
						}
				}
				else
				{
					if (QMessageBox::question(&Application::instance()->mainWindow(),
										  tr("Remove entity"),
										  tr("Do you really want to remove entity \"%1\"?").
										  arg(static_cast<IdmEntityItem*>(item)->entity()->name()),
										  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
						if (m_container->removeEntity(static_cast<IdmEntityItem*>(item)->entity()))
						{
							beginRemoveRows(parent(index), index.row(), index.row());
							delete item;
							endRemoveRows();
						}
						else
						{
							QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
							m_container->rollback();
							return;
						}
				}

		if (!m_container->commit())
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
	}
}

void IdmNodeBase::cancel(const QModelIndexList &list)
{

}

void IdmNodeBase::calculateSize(const QModelIndexList &list)
{

}

void IdmNodeBase::pathToClipboard(const QModelIndexList &list)
{

}

void IdmNodeBase::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeBase::move(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeBase::switchTo(Node *node, const QModelIndex &selected)
{
	FolderNodeBase::switchTo(node, selected);

//	Node *child;
//	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
//		if (child = m_items.at(i).node)
//			child->switchTo(node, selected);
}

QModelIndex IdmNodeBase::rootIndex() const
{
	return QModelIndex();
}

Node *IdmNodeBase::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<IdmItem*>(index.internalPointer())->isRoot())
		return static_cast<Node*>(Node::parent());
	else
		if (static_cast<IdmItem*>(index.internalPointer())->isMenuItem())
			switch (static_cast<IdmMenuItem*>(index.internalPointer())->id())
			{
				case IdmContainer::Create:
				{
					CreateEntityDialog dialog(m_container, QString(), &Application::instance()->mainWindow());

					if (dialog.exec() == CreateEntityDialog::Accepted)
						if (m_container->transaction())
							if (IdmEntity *entity = m_container->createEntity(dialog.name(), dialog.type()))
								if (entity->type() == IdmEntity::Composite)
								{
									bool ok = true;

									for (EntitiesListModel::size_type i = 0, size = dialog.entities().size(); i < size; ++i)
										if (!m_container->addProperty(entity, dialog.entities().at(i)))
										{
											ok = false;
											m_container->rollback();
											QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
											break;
										}

									if (ok && !m_container->commit())
									{
										m_container->rollback();
										QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
									}
								}
								else
								{
									if (!m_container->commit())
									{
										m_container->rollback();
										QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
									}
								}
							else
							{
								m_container->rollback();
								QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());
							}
						else
							QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container->lastError());

					break;
				}
				case IdmContainer::Query:
				{
					break;
				}
			}

	return 0;
}

Node *IdmNodeBase::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

UpdatesList::Map IdmNodeBase::updateFilesMap() const
{
	return UpdatesList::Map();
}

void IdmNodeBase::updateFilesEvent(const UpdatesList &updates)
{

}

void IdmNodeBase::scanForSizeEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::scanForCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move)
{

}

void IdmNodeBase::scanForRemoveEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::performCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, bool move)
{

}

void IdmNodeBase::performRemoveEvent(PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed)
{

}

void IdmNodeBase::completedProgressEvent(const QString &fileName, quint64 timeElapsed)
{

}

FILE_SYSTEM_NS_END
