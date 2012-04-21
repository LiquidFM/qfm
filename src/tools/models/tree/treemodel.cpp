#include "treemodel.h"
#include "items/treemodelitem.h"


MODELS_TREE_NS_BEGIN

Model::Model(const Container &conteiner, QObject *parent) :
	QAbstractItemModel(parent),
	m_conteiner(conteiner)
{}

int Model::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<Item*>(parent.internalPointer())->size();
	else
    	return m_conteiner.size();
}

int Model::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<Item*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_conteiner.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags Model::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex Model::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid())
			return createIndex(row, column, static_cast<Item*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_conteiner.at(row));
    else
        return QModelIndex();
}

QModelIndex Model::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (Item *parent = static_cast<Item*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<Item*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_conteiner.indexOf(parent), 0, parent);

    return QModelIndex();
}

QModelIndex Model::index(Item *item) const
{
	if (Item *parent = item->parent())
		return createIndex(static_cast<Item*>(parent)->indexOf(item), 0, item);
	else
		return createIndex(m_conteiner.indexOf(item), 0, item);
}

QModelIndex Model::parent(Item *item) const
{
	if (Item *parent = item->parent())
		if (Item *parentParent = parent->parent())
			return createIndex(static_cast<Item*>(parentParent)->indexOf(parent), 0, parent);
		else
			return createIndex(m_conteiner.indexOf(parent), 0, parent);
	else
		return QModelIndex();
}

QModelIndex Model::parent(Item *item, Item::size_type &row) const
{
	if (Item *parent = item->parent())
		if (Item *parentParent = parent->parent())
		{
			row = static_cast<Item*>(parent)->indexOf(item);
			return createIndex(static_cast<Item*>(parentParent)->indexOf(parent), 0, parent);
		}
		else
		{
			row = static_cast<Item*>(parent)->indexOf(item);
			return createIndex(m_conteiner.indexOf(parent), 0, parent);
		}
	else
	{
		row = m_conteiner.indexOf(item);
		return QModelIndex();
	}
}

MODELS_TREE_NS_END