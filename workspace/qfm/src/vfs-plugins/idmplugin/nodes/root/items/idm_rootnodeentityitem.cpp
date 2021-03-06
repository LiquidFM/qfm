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
#include "idm_rootnodeentityitem.h"
#include "../../../containeres/idm_container.h"


IDM_PLUGIN_NS_BEGIN

RootNodeEntityItem::RootNodeEntityItem(const Entity &entity, Base *parent) :
    RootNodeListItem(parent),
    m_entity(entity)
{}

QVariant RootNodeEntityItem::data(qint32 column, qint32 role) const
{
    if (role == Qt::DisplayRole)
        return toUnicode(m_entity.name());
    else
        return QVariant();
}

bool RootNodeEntityItem::isEntity()
{
    return true;
}

IDM_PLUGIN_NS_END
