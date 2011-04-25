#ifndef PERFORMREMOVEENTRYTASK_H_
#define PERFORMREMOVEENTRYTASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../controlabletask.h"
#include "../scan/scanfilestask.h"


class PerformRemoveEntryTask : public PerformTask<ControlableTask>
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveEntryTask)

public:
	typedef PerformTask<ControlableTask>                     parent_class;
	typedef FileSystemModelEvents::RemoveFilesCompletedEvent CompletedEvent;
	typedef FileSystemModelEvents::RemoveFilesCanceledEvent  CanceledEvent;
	typedef FileSystemModelEvents::QuestionAnswerEvent       QuestionAnswerEvent;

public:
	PerformRemoveEntryTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

protected:
	void removeEntry(FileSystemEntry *entry, bool &tryAgain, const volatile bool &stopedFlag);

protected:
	bool m_shoulRemoveEntry;
	bool m_skipAllIfNotRemove;
	bool m_skipAllIfNotExists;
};

#endif /* PERFORMREMOVEENTRYTASK_H_ */