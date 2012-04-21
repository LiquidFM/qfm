#ifndef IDMQUERYRESULTITEM_H_
#define IDMQUERYRESULTITEM_H_

#include "../../../idmplugin_ns.h"
#include "../../../../../filesystem/tasks/items/filesystemtasknodeitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultItem : public TaskNodeItem
{
public:
	QueryResultItem(Base *parent);

	virtual bool isRoot() = 0;
	virtual bool isProperty() = 0;
	virtual bool isValue() = 0;
	virtual bool isPath() = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTITEM_H_ */