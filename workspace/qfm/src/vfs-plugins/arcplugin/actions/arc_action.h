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
#ifndef ARC_ACTION_H_
#define ARC_ACTION_H_

#include "../arc_ns.h"
#include <vfs/actions/asynchronous/vfs_asyncaction.h>


ARC_PLUGIN_NS_BEGIN

class Action : public AsyncAction
{
public:
	Action(const QString &lockReason, const QIcon &icon, const QString &text);

	virtual const QString &lockReason() const;

private:
	QString m_lockReason;
};

ARC_PLUGIN_NS_END

#endif /* ARC_ACTION_H_ */
