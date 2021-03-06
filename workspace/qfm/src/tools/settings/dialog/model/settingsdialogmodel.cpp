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
#include "settingsdialogmodel.h"


SETTINGS_NS_BEGIN

Model::Model(const Pages &pages, QObject *parent) :
	Tools::Models::Tree::Model(m_container, parent),
	m_container(pages)
{}

Page *Model::pageAt(const QModelIndex &index) const
{
	return static_cast<ItemPage *>(index.internalPointer())->page();
}

Model::ItemPage::ItemPage(Page *page, Base *parent) :
	Tools::Models::Tree::Item(parent),
	m_page(page)
{
	Q_ASSERT(m_page);

	const Page::Pages &subpages = m_page->subpages();
	m_subpages.reserve(subpages.size());

	for (Page::Pages::size_type i = 0, size = subpages.size(); i < size; ++i)
		m_subpages.push_back(new ItemPage(subpages.at(i), this));
}

Model::ItemPage::~ItemPage()
{
	qDeleteAll(m_subpages);
}

Model::ItemPage::Base *Model::ItemPage::at(size_type index) const
{
	return m_subpages.at(index);
}

Model::ItemPage::size_type Model::ItemPage::size() const
{
	return m_subpages.size();
}

Model::ItemPage::size_type Model::ItemPage::indexOf(Base *item) const
{
	return m_subpages.indexOf(static_cast<ItemPage *>(item));
}

QVariant Model::ItemPage::data(qint32 column, qint32 role) const
{
	if (role == Qt::DisplayRole)
		return m_page->title();
	else
		return QVariant();
}

Model::Container::Container(const Pages &pages)
{
	m_items.reserve(pages.size());

	for (Pages::size_type i = 0, size = pages.size(); i < size; ++i)
		m_items.push_back(new ItemPage(pages.at(i)));
}

Model::Container::~Container()
{
	qDeleteAll(m_items);
}

Model::Container::size_type Model::Container::size() const
{
	return m_items.size();
}

Model::Container::Item *Model::Container::at(size_type index) const
{
	return m_items.at(index);
}

Model::Container::size_type Model::Container::indexOf(Item *item) const
{
	return m_items.indexOf(static_cast<ItemPage *>(item));
}

SETTINGS_NS_END
