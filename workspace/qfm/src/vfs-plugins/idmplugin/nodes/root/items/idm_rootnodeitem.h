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
#ifndef IDM_ROOTNODEITEM_H_
#define IDM_ROOTNODEITEM_H_

#include <vfs/tasks/model/items/vfs_tasksnodeitem.h>
#include "../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeItem : public TasksNodeItem
{
public:
    RootNodeItem(Base *parent);

    virtual bool isRoot();
    virtual bool isFiles();
    virtual bool isEntity();
    virtual bool isProperty();
};


class RootNodeListItem : public RootNodeItem
{
public:
    typedef QList<RootNodeItem *> Container;

public:
    RootNodeListItem(Base *parent);
    virtual ~RootNodeListItem();

    /* Base */
    virtual Base *at(size_type index) const;
    virtual size_type size() const;
    virtual size_type indexOf(Base *item) const;

protected:
    Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEITEM_H_ */
