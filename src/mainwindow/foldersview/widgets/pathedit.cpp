#include "pathedit.h"
#include <QKeyEvent>


PathEdit::PathEdit(::Tools::Events::EventHandler *eventHandler, QWidget *parent) :
	QLineEdit(parent),
	m_eventHandler(eventHandler)
{
	Q_ASSERT(m_eventHandler != 0);
}

void PathEdit::startEdit()
{
	m_backupText = text();
	setFocus();
	selectAll();
}

void PathEdit::cancelEdit()
{
	setText(m_backupText);
	end(false);
}

void PathEdit::mouseReleaseEvent(QMouseEvent *event)
{
	if (!m_eventHandler->mouseReleaseEvent(event))
		QLineEdit::mouseReleaseEvent(event);
}

void PathEdit::keyPressEvent(QKeyEvent *event)
{
	if (!m_eventHandler->keyboardEvent(event))
		QLineEdit::keyPressEvent(event);
}

void PathEdit::focusOutEvent(QFocusEvent *event)
{
	if (!m_eventHandler->focusOutEvent(event))
		QLineEdit::focusOutEvent(event);
}
