#ifndef FILEINFOGATHERERTASK_H_
#define FILEINFOGATHERERTASK_H_

#include "filestask.h"
#include "../controlabletask.h"


class ListFilesTask : public TemplateFilesTask<ControlableTask>
{
public:
	typedef TemplateFilesTask<ControlableTask> parent_class;

public:
	struct Params : public parent_class::Params
	{
		QObject *object;
		FileSystemTree *fileSystemTree;
		ChangesList list;
	};
	typedef FileSystemModelEvents::ListFilesEvent Event;

public:
	ListFilesTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }
};

#endif /* FILEINFOGATHERERTASK_H_ */