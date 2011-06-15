#ifndef DESTCONTROLABLETASK_H_
#define DESTCONTROLABLETASK_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include "controlabletask.h"


FILE_SYSTEM_NS_BEGIN

class DestControlableTask : public ControlableTask
{
public:
	struct Params : ControlableTask::Params
	{
		Params(INode *destination) :
	    	destination(destination)
		{}

    	INode *destination;

	private:
		friend class DestControlableTask;
	    friend class DeleteHandler;
		QMutex m_mutex;
	};

public:
	/*
	 * This class and it's subclasses should be created only
	 * in the same thread as "destination.object" because of DeleteHandler!
	 *
	 */
	DestControlableTask(Params *parameters, QObject *listener);
	virtual ~DestControlableTask();

protected:
	inline Params *parameters() const { return static_cast<Params*>(ControlableTask::parameters()); }

private:
    class DeleteHandler : public QObject
    {
    public:
    	DeleteHandler(DestControlableTask *task, QObject *parent) :
    		QObject(parent),
    		m_task(task),
    		m_params(m_task->params())
    	{}
    	virtual ~DeleteHandler()
    	{
    		QMutexLocker locker(&static_cast<Params*>(m_params.data())->m_mutex);

    		if (m_task != 0)
    		{
				m_task->m_handler = 0;
				m_task->m_listener = 0;
    		}
    	}

    private:
    	friend class DestControlableTask;
    	DestControlableTask *m_task;
    	/*
    	 * Just holds the reference to DestControlableTask::m_params,
    	 * so m_task->parameters() can not be deleted before this object.
    	 */
    	ParamsPointer m_params;
    };

private:
    QObject *m_listener;
    DeleteHandler *m_handler;
};

FILE_SYSTEM_NS_END

#endif /* DESTCONTROLABLETASK_H_ */
