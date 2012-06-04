#ifndef ARCPACKACTION_H_
#define ARCPACKACTION_H_

#include <QtCore/QCoreApplication>
#include "../libarchive_ns.h"
#include "../../actions/arcaction.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class PackAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(PackAction)

public:
	PackAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* ARCPACKACTION_H_ */