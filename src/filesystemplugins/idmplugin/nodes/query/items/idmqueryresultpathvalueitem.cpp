#include "idmqueryresultpathvalueitem.h"


IDM_PLUGIN_NS_BEGIN

QueryResultPathValueItem::QueryResultPathValueItem(IdmEntityValue *value, Base *parent) :
	QueryResultValueItem(value, parent),
	m_info(value->value().toString())
{}

IDM_PLUGIN_NS_END
