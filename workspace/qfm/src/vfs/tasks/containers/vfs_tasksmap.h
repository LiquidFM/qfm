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
#ifndef VFS_TASKSMAP_H_
#define VFS_TASKSMAP_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "../vfs_basetask.h"
#include "../../containers/vfs_snapshot.h"


VFS_NS_BEGIN

class TasksMap
{
public:
	typedef QList<Item::Holder>            List;
	typedef QMap<BaseTask *, List>         Tasks;
	typedef QMap<Item::Holder, BaseTask *> Items;

public:
	TasksMap()
	{}

	void add(BaseTask *task, const Item::Holder &item)
	{
		m_tasks[task].push_back(item);
		m_items[item] = task;
	}

	void add(BaseTask *task, const Snapshot &snapshot)
	{
		List &items = m_tasks[task];
		items.reserve(snapshot.size());

		for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
		{
			items.push_back((*i).first);
			m_items[(*i).first] = task;
		}
	}

	void remove(const Item::Holder &item)
	{
		if (BaseTask *task = m_items.value(item, NULL))
		{
			List &list = m_tasks[task];
			List::size_type index;

			if ((index = list.indexOf(item)) != -1)
				list.removeAt(index);

			m_items.remove(item);

			if (list.isEmpty())
				m_tasks.remove(task);
		}
	}

	void removeAll(BaseTask *task)
	{
		List list = m_tasks.take(task);

		for (List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items.remove(list.at(i));
	}

	BaseTask *take(const Item::Holder &item)
	{
		if (BaseTask *task = m_items.value(item, 0))
		{
			List list = m_tasks.take(task);

			for (List::size_type i = 0, size = list.size(); i < size; ++i)
				m_items.remove(list.at(i));

			return task;
		}

		return NULL;
	}

	BaseTask *take(const Item::Holder &item, List &list)
	{
		if (BaseTask *task = m_items.value(item, NULL))
		{
			list = m_tasks.take(task);

			for (List::size_type i = 0, size = list.size(); i < size; ++i)
				m_items.remove(list.at(i));

			return task;
		}

		return NULL;
	}

	void resetTask(BaseTask *task, BaseTask *oldTask)
	{
		const List &list = (m_tasks[task] = m_tasks.take(oldTask));

		for (List::size_type i = 0, size = list.size(); i < size; ++i)
			m_items[list.at(i)] = task;
	}

private:
	Tasks m_tasks;
	Items m_items;
};

VFS_NS_END

#endif /* VFS_TASKSMAP_H_ */
