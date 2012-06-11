#include "filesystembasetask.h"
#include "filesystemtasksnode.h"
#include "tools/filesystemtaskdialog.h"
#include "../../application.h"

#include <QtCore/QThread>


FILE_SYSTEM_NS_BEGIN

BaseTask::BaseTask(TasksNode *receiver) :
	Task(),
	m_receiver(receiver),
	m_canceled(1, flags())
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

BaseTask::BaseTask(TasksNode *receiver, ICopyControl::Holder &destination) :
	Task(),
	m_receiver(receiver),
	m_canceled(1, flags()),
	m_destination(destination.take())
{
	Q_ASSERT(m_receiver != 0);
	Q_ASSERT(m_destination.data());
	Q_ASSERT(m_receiver->thread() == QThread::currentThread());
}

void BaseTask::postEvent(Event *event) const
{
	Application::postEvent(m_receiver, event);
}

qint32 BaseTask::askUser(const QString &title, const QString &question, qint32 buttons, const volatile Flags &aborted) const
{
	QuestionEvent::Result result;
	postEvent(new QuestionEvent(title, question, buttons, &result));
	result.waitFor(aborted);
	return result.answer();
}

qint32 BaseTask::askForUserInput(const QString &title, const QString &question, qint32 buttons, QString &value, const volatile Flags &aborted) const
{
	UserInputEvent::Result result;
	postEvent(new UserInputEvent(title, question, buttons, &result));
	result.waitFor(aborted);
	value = result.value();
	return result.answer();
}

FILE_SYSTEM_NS_END
