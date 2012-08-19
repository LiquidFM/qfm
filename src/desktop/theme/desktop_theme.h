#ifndef DESKTOP_THEME_H_
#define DESKTOP_THEME_H_

#include <QtGui/QIcon>
#include <QtCore/QByteArray>
#include "../desktop_ns.h"


DESKTOP_NS_BEGIN

class Theme
{
public:

#if defined(Q_OS_UNIX)
	enum Type
	{
		DE_Cde,
		DE_Kde,
		DE_4Dwm,
		DE_Gnome,
		DE_Unknown
	};

	enum IconSize
	{
		Small = 16
	};
#elif defined(Q_OS_WIN32)
	enum Type
	{
		DE_Win32
	};

	enum IconSize
	{
		Small = 16
	};
#endif

public:
	Theme();
	~Theme();

	static Theme *current();

	const QByteArray &name() const { return m_name; }

	QIcon processingIcon(int iconSize = Small) const;
	QIcon cancelingIcon(int iconSize = Small) const;

	QIcon copyActionIcon(int iconSize = Small) const;
	QIcon cutActionIcon(int iconSize = Small) const;
	QIcon pasteActionIcon(int iconSize = Small) const;
	QIcon propertiesActionIcon(int iconSize = Small) const;

	QIcon packActionIcon(int iconSize = Small) const;
	QIcon unpackActionIcon(int iconSize = Small) const;

	QIcon searchIcon(int iconSize = Small) const;
	QIcon openDataIcon(int iconSize = Small) const;

	QIcon missingIcon(int iconSize = Small) const;

private:
	Type m_type;
	QByteArray m_name;
};

DESKTOP_NS_END

#endif /* DESKTOP_THEME_H_ */