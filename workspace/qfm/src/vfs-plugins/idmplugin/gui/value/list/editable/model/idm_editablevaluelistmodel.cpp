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
#include "idm_editablevaluelistmodel.h"


IDM_PLUGIN_NS_BEGIN

EditableValueListModel::EditableValueListModel(const EntityValueReader &reader, QObject *parent) :
    ValueListModel(reader, parent)
{}

void EditableValueListModel::add(const List &list)
{
    ValueListModel::add(list);
}

QModelIndex EditableValueListModel::add(const EntityValue &value)
{
    return ValueListModel::add(value);
}

void EditableValueListModel::remove(const QModelIndex &index)
{
    ValueListModel::remove(index);
}

IDM_PLUGIN_NS_END
