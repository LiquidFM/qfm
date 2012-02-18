#ifndef FILESYSTEMTASKPROGRESSEVENTS_H_
#define FILESYSTEMTASKPROGRESSEVENTS_H_

#include "../filesystembasetask.h"
#include "../../model/items/filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class UpdateProgressEvent : public BaseTask::Event
{
public:
	UpdateProgressEvent(const FileSystemItem *item, quint64 progress, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Progress),
		item(item),
		progress(progress),
		timeElapsed(timeElapsed)
	{}

	const FileSystemItem *item;
	quint64 progress;
	quint64 timeElapsed;
};


class CompletedProgressEvent : public BaseTask::Event
{
public:
	CompletedProgressEvent(const FileSystemItem *item, quint64 timeElapsed) :
		BaseTask::Event(NULL, BaseTask::Event::Completed),
		item(item),
		timeElapsed(timeElapsed)
	{}

	const FileSystemItem *item;
	quint64 timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKPROGRESSEVENTS_H_ */
