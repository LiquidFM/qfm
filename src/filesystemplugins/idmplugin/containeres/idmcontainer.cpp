#include "idmcontainer.h"
#include "../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmContainer::IdmContainer(const Info &storage) :
	m_storage(storage)
{
	m_menuActions.push_back(new QAction(tr("Create"), 0));
	m_menuActions.last()->setData(Create);

	m_menuActions.push_back(new QAction(tr("Query"), 0));
	m_menuActions.last()->setData(Query);

	m_menuActions.push_back(new QAction(tr("List"), 0));
	m_menuActions.last()->setData(List);

	m_entityTypes[Database::Int]       = IdmEntityTypeDescription(tr("Int"),       tr("Integer type"));
	m_entityTypes[Database::String]    = IdmEntityTypeDescription(tr("String"),    tr("String type (max length is 256 characters)"));
	m_entityTypes[Database::Date]      = IdmEntityTypeDescription(tr("Date"),      tr("Date type"));
	m_entityTypes[Database::Time]      = IdmEntityTypeDescription(tr("Time"),      tr("Time type"));
	m_entityTypes[Database::DateTime]  = IdmEntityTypeDescription(tr("DateTime"),  tr("DateTime type"));
	m_entityTypes[Database::Memo]      = IdmEntityTypeDescription(tr("Memo"),      tr("Memo type"));
	m_entityTypes[Database::Composite] = IdmEntityTypeDescription(tr("Composite"), tr("Entity of this type consists of other entities"));
	m_entityTypes[Database::Rating]    = IdmEntityTypeDescription(tr("Rating"),    tr("Rating type (shown as stars)"));
	m_entityTypes[Database::Path]      = IdmEntityTypeDescription(tr("Path"),      tr("Path to file or folder"));
}

IdmContainer::~IdmContainer()
{
	qDeleteAll(m_menuActions);
}

IDM_PLUGIN_NS_END
