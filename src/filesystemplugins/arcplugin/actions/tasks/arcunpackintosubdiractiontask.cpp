#include "arcunpackintosubdiractiontask.h"
#include "../../../../tools/pointers/pscopedpointer.h"
#include <QtGui/QMessageBox>


ARC_PLUGIN_NS_BEGIN

UnPackIntoSubdirActionTask::UnPackIntoSubdirActionTask(TasksNode *receiver, const IFileContainer *container, const AsyncFileAction::FilesList &files) :
	PerformActionTask(receiver, files),
	m_overwriteAll(false),
	m_skipAllIfNotCopy(false),
	m_progress(receiver),
	m_container(container)
{}

IFileAccessor::value_type *UnPackIntoSubdirActionTask::buffer() const
{
	return const_cast<IFileAccessor::value_type *>(m_buffer);
}

IFileAccessor::size_type UnPackIntoSubdirActionTask::bufferSize() const
{
	return FileReadWriteGranularity;
}

void UnPackIntoSubdirActionTask::progressInit(const NodeItem *item)
{
	m_progress.init(item);
}

void UnPackIntoSubdirActionTask::progressUpdate(quint64 progressIncrement)
{
	m_progress.update(progressIncrement);
}

void UnPackIntoSubdirActionTask::progresscomplete()
{
	m_progress.complete();
}

bool UnPackIntoSubdirActionTask::overwriteAll() const
{
	return m_overwriteAll;
}

bool UnPackIntoSubdirActionTask::skipAllIfNotCopy() const
{
	return m_skipAllIfNotCopy;
}

void UnPackIntoSubdirActionTask::askForOverwrite(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::No |
						QMessageBox::NoToAll |
						QMessageBox::Cancel,
						aborted);

	switch (answer)
	{
		case QMessageBox::Yes:
			tryAgain = true;
			break;

		case QMessageBox::YesToAll:
			m_overwriteAll = tryAgain = true;
			break;

		case QMessageBox::No:
			tryAgain = false;
			break;

		case QMessageBox::NoToAll:
			m_overwriteAll = tryAgain = false;
			break;

		case QMessageBox::Cancel:
			cancel();
			break;
	}
}

void UnPackIntoSubdirActionTask::askForSkipIfNotCopy(const QString &text, volatile bool &tryAgain, const volatile Flags &aborted)
{
	qint32 answer = askUser(
						tr("Extracting..."),
						text,
						QMessageBox::Yes |
						QMessageBox::YesToAll |
						QMessageBox::Retry |
						QMessageBox::Cancel,
						aborted);

	if (answer == QMessageBox::YesToAll)
		m_skipAllIfNotCopy = true;
	else
		if (answer == QMessageBox::Retry)
			tryAgain = true;
		else
			if (answer == QMessageBox::Cancel)
				cancel();
}

void UnPackIntoSubdirActionTask::process(const volatile Flags &aborted)
{
	QString error;
	bool tryAgain = false;
	const IFileInfo *file;
	Archive::State *state;
	IFileContainer::Holder folder;

	for (AsyncFileAction::FilesList::size_type i = 0, size = files().size(); i < size && !aborted; ++i)
	{
		file = files().at(i).second;

    	do
    	{
			if (folder = m_container->open(folderName(file->fileName()), true, error))
		    	if (const Archive *archive = Archive::archive(m_container->location(file->fileName()), &state))
		    	{
					archive->extractAll(state, folder.data(), this, aborted);
					archive->endRead(state);
		    	}
				else
					if (m_skipAllIfNotCopy)
						break;
					else
						askForSkipIfNotCopy(
								tr("Unknown archive format \"%1\". Skip it?").
									arg(m_container->location(file->fileName())),
								tryAgain = false,
								aborted);
			else
				if (m_skipAllIfNotCopy)
					break;
				else
					askForSkipIfNotCopy(
							tr("Failed to open/create directory \"%1\" (%2). Skip it?").
								arg(m_container->location(folderName(file->fileName()))).
								arg(state->error),
							tryAgain = false,
							aborted);
    	}
    	while (tryAgain && !aborted);
	}
}

QString UnPackIntoSubdirActionTask::folderName(const QString &fileName) const
{
	return fileName.mid(0, fileName.indexOf(QChar('.')));
}

ARC_PLUGIN_NS_END