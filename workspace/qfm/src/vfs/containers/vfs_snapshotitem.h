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
#ifndef VFS_SNAPSHOTITEM_H_
#define VFS_SNAPSHOTITEM_H_

#include <QtCore/QList>
#include "../interfaces/vfs_ifilecontainer.h"


VFS_NS_BEGIN

class SnapshotItem
{
	Q_DISABLE_COPY(SnapshotItem)

public:
	typedef ::Tools::Memory::ScopedPointer<SnapshotItem> Holder;
	typedef QList<SnapshotItem *>        Container;
	typedef Container::iterator          iterator;
	typedef Container::const_iterator    const_iterator;
	typedef Container::size_type         size_type;

public:
	SnapshotItem();
	SnapshotItem(const IFileContainer *container, IFileInfo::Holder &info, SnapshotItem *parent);
	virtual ~SnapshotItem();

	bool isValid() const { return m_container != NULL; }

	bool isEmpty() const { return m_items.isEmpty(); }
	size_type size() const { return m_items.size(); }

	const_iterator begin() const { return m_items.begin(); }
	iterator begin() { return m_items.begin(); }

	const_iterator end() const { return m_items.end(); }
	iterator end() { return m_items.end(); }

	void append(SnapshotItem *item)
	{
		m_items.append(item);
		incTotalSize(item->totalSize());
	}

	iterator erase(iterator i)
	{
		Holder item(*i);
		decTotalSize(item->totalSize());
		return m_items.erase(i);
	}

	const IFileContainer::Holder &thisContainer() const { return m_thisContainer; }
	IFileContainer::Holder &thisContainer() { return m_thisContainer; }

	const IFileContainer *container() const { return m_container; }
	IFileInfo::size_type totalSize() const { return m_totalSize; }

	bool isRemoved() const { return m_removed; }
	void setRemoved(bool value) { m_removed = value; }

	const IFileInfo::Holder &info() const { return m_info; }
	IFileInfo::Holder &info() { return m_info; }

protected:
	SnapshotItem(const IFileContainer *container, SnapshotItem *parent);
	SnapshotItem(const IFileContainer *container, IFileInfo::size_type totalSize, SnapshotItem *parent);

protected:
	Container &items() { return m_items; }

	void incTotalSize(IFileInfo::size_type size)
	{
		SnapshotItem *parent = this;

		do
			parent->m_totalSize += size;
		while (parent = parent->m_parent);
	}

	void decTotalSize(IFileInfo::size_type size)
	{
		SnapshotItem *parent = this;

		do
			parent->m_totalSize -= size;
		while (parent = parent->m_parent);
	}

private:
	IFileContainer::Holder m_thisContainer;
	const IFileContainer *m_container;
	IFileInfo::size_type m_totalSize;
	IFileInfo::Holder m_info;
	SnapshotItem *m_parent;
	Container m_items;
	bool m_removed;
};

VFS_NS_END

#endif /* VFS_SNAPSHOTITEM_H_ */
