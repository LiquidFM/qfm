#ifndef PERFORMREMOVEENTRYTASK_H_
#define PERFORMREMOVEENTRYTASK_H_

#include <QtCore/QCoreApplication>
#include "performtask.h"
#include "../taskprogress.h"
#include "../controlabletask.h"
#include "../scan/scanfilestask.h"


FILE_SYSTEM_NS_BEGIN

class PerformRemoveEntryTask : public PerformTask<ControlableTask>
{
	Q_DECLARE_TR_FUNCTIONS(PerformRemoveEntryTask)

public:
	typedef PerformTask<ControlableTask>           parent_class;
	typedef ModelEvents::RemoveFilesCompletedEvent CompletedEvent;
	typedef ModelEvents::RemoveFilesCanceledEvent  CanceledEvent;
	typedef ModelEvents::QuestionAnswerEvent       QuestionAnswerEvent;

public:
	struct Params : public parent_class::Params
	{
		Params(Node *node, FolderNodeEntry *entry)
		{
			source.node = node;
			source.entry = entry;
		}
	};

public:
	PerformRemoveEntryTask(Node *node, FolderNodeEntry *entry);

	virtual void run(const volatile bool &stopedFlag);

protected:
	PerformRemoveEntryTask(Params *params);

	inline Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

	void removeEntry(FileSystemItem *entry, bool &tryAgain, const volatile bool &stopedFlag);
	void postCompletedEvent() const;
	void postCanceledEvent() const;

protected:
	bool m_removeParentEntry;
	bool m_skipAllIfNotRemove;
	bool m_skipAllIfNotExists;
	TaskProgress m_progress;
};

FILE_SYSTEM_NS_END

#endif /* PERFORMREMOVEENTRYTASK_H_ */
