#include "idmqueryresultvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultValueItem::QueryResultValueItem(IdmEntityValue *value, IdmItem *parent) :
	QueryResultItem(parent),
	m_value(value)
{}

QVariant QueryResultValueItem::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_value->value();
	else
		return QVariant();
}

bool QueryResultValueItem::isRoot()
{
	return false;
}

bool QueryResultValueItem::isProperty()
{
	return false;
}

bool QueryResultValueItem::isValue()
{
	return true;
}

IDM_PLUGIN_NS_END