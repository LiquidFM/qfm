/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SETTINGSVISIBLECONTAINER_H_
#define SETTINGSVISIBLECONTAINER_H_

#include "settingscontainer.h"
#include "options/gui/page/settingspage.h"
#include "dialog/settings/settingsdialogsettings.h"


class QWidget;


SETTINGS_NS_BEGIN

class VisibleContainer : public Container
{
public:
	VisibleContainer(const QString &storage);

protected:
	using Container::manage;
	void manage(Page *option);

	void exec(const QString &title, DialogSettings &settings, QWidget *parent = 0);

protected:
	typedef QList<Page *> List;
	List m_pages;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLECONTAINER_H_ */
