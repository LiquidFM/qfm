#include "performactiontask.h"


FILE_SYSTEM_NS_BEGIN

PerformActionTask::PerformActionTask(TasksNode *receiver, const AsyncFileAction *action, const AsyncFileAction::FilesList &files) :
	BaseTask(receiver),
	m_progress(receiver),
	m_action(action),
	m_files(files)
{}

void PerformActionTask::run(const volatile Flags &aborted)
{
	m_action->process(this, m_files, aborted);
}

FILE_SYSTEM_NS_END