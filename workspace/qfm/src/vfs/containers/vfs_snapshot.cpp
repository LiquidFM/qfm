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
#include "vfs_snapshot.h"


VFS_NS_BEGIN

Snapshot::Snapshot()
{}

Snapshot::Snapshot(const Files &files) :
	m_data(files.m_data)
{}

Snapshot::Snapshot(const Updates &updates) :
	m_data(updates.m_data)
{}

Snapshot::Snapshot(const IFileContainer *container) :
	m_data(new Data(container))
{}

Snapshot::Data::Data(const IFileContainer *container) :
	container(container)
{}

Snapshot::Data::~Data()
{
	for (Container::iterator it = map.begin(), end = map.end(); it != end; it = map.erase(it))
		delete (*it).second;
}

VFS_NS_END
