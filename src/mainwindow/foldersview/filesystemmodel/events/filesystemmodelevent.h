#ifndef FILESYSTEMMODELEVENT_H_
#define FILESYSTEMMODELEVENT_H_

#include <QtCore/QEvent>


class FileSystemModelEvent : public QEvent
{
public:
	enum Type
	{
		ListFiles = QEvent::User + 1,
		UpdateFiles = QEvent::User + 2,
		ScanFilesForRemove = QEvent::User + 3,
		RemoveFilesCompleted = QEvent::User + 4,
		RemoveFilesCanceled = QEvent::User + 5,
		ScanFilesForSize = QEvent::User + 6,
		ScanFilesForCopy = QEvent::User + 7,
		ScanFilesForMove = QEvent::User + 8,
		CopyFilesCompleted = QEvent::User + 9,
		CopyFilesCanceled = QEvent::User + 10,
		QuestionAnswer = QEvent::User + 11,
		UpdatePerformProgress = QEvent::User + 12
	};

public:
	struct Params {};
	virtual const Params *parameters() const = 0;

protected:
	FileSystemModelEvent(Type type) :
		QEvent(static_cast<QEvent::Type>(type))
	{}
};

#endif /* FILESYSTEMMODELEVENT_H_ */