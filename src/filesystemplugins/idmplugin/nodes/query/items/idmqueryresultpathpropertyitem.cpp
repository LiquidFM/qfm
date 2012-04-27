#include "idmqueryresultpathpropertyitem.h"
#include "idmqueryresultrootpathvalueitem.h"
#include "../../../../../filesystem/filesystemproxymodel.h"


IDM_PLUGIN_NS_BEGIN

inline static bool lessThan(const QueryResultItem *v1, const QueryResultItem *v2)
{
	return ProxyModel::compareByFileNames(static_cast<const QueryResultRootPathValueItem *>(v1), static_cast<const QueryResultRootPathValueItem *>(v2));
}


QueryResultPathPropertyItem::QueryResultPathPropertyItem(const IdmEntity::Property &property, Base *parent) :
	QueryResultPropertyItem(property, parent)
{}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmEntityValue::Holder &value)
{
	m_items.push_back(new QueryResultRootPathValueItem(container, value, this));
	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
		m_items.push_back(new QueryResultRootPathValueItem(container, values.at(i), this));

	qSort(m_items.begin(), m_items.end(), lessThan);
}

void QueryResultPathPropertyItem::add(Snapshot::Files &files, const IFileContainer *container, const IdmCompositeEntityValue::List &values)
{
	QueryResultRootPathValueItem *item;

	for (IdmCompositeEntityValue::List::size_type i = 0, size = values.size(); i < size; ++i)
	{
		m_items.push_back(item = new QueryResultRootPathValueItem(container, values.at(i), this));
		files.add(item->fileName(), item);
	}

	qSort(m_items.begin(), m_items.end(), lessThan);
}

IDM_PLUGIN_NS_END
