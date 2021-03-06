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
#ifndef DEFAULT_NODE_H_
#define DEFAULT_NODE_H_

#include "base/default_basenode.h"


DEFAULT_PLUGIN_NS_BEGIN

class Node : public BaseNode
{
	Q_DISABLE_COPY(Node)

public:
	Node(IFileContainer::Holder &container, ::VFS::Node *parent = 0);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;

protected:
	virtual Snapshot updateFilesList() const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_NODE_H_ */
