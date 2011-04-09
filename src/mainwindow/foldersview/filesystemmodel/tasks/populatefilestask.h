#ifndef POPULATEFILESTASK_H_
#define POPULATEFILESTASK_H_

#include "filestask.h"


class PopulateFilesTask : public FilesTask
{
public:
	struct EventParams : public FilesTask::EventParams
	{
		quint64 size;
		FileSystemEntry *entry;
		FileSystemTree *subtree;
	};
	typedef FileSystemModelEventTemplate<EventParams> Event;

public:
	PopulateFilesTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);

	quint64 size() const { return m_size; }
	FileSystemEntry *entry() const { return m_entry; }
	FileSystemTree *subtree() const { return m_subtree; }

private:
	void populate(FileSystemTree *tree, const volatile bool &stopedFlag);

private:
	quint64 m_size;
	FileSystemEntry *m_entry;
	FileSystemTree *m_subtree;
};


class PopulateFilesForRemoveTask : public PopulateFilesTask
{
public:
	PopulateFilesForRemoveTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);
};


class PopulateFilesForSizeTask : public PopulateFilesTask
{
public:
	PopulateFilesForSizeTask(FileSystemTree *tree, FileSystemEntry *entry, QObject *receiver);

	virtual void run(const volatile bool &stopedFlag);
};

#endif /* POPULATEFILESTASK_H_ */
