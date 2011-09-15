#ifndef IDMSTORAGE_H_
#define IDMSTORAGE_H_

#include <sqlite3.h>
#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>
#include "entities/idmentityroot.h"
#include "queries/idmquery.h"
#include "queries/idmquerycontext.h"
#include "undo/idmstorageundocommand.h"
#include "format/idmshortformat.h"
#include "../idmplugin_ns.h"
#include "../../../filesystem/info/filesysteminfo.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorage
{
	Q_DECLARE_TR_FUNCTIONS(IdmStorage)

public:
	typedef IdmEntity::id_type        id_type;
	typedef IdmEntity::size_type      size_type;
	typedef Database::IdsSet          IdsSet;
	typedef Database::IdsList         IdsList;
	typedef QMap<IdmEntity*, IdsList> IdsMap;
	enum { InvalidId = IdmEntity::InvalidId };
	enum { InvalidIndex = IdmEntity::InvalidIndex };

public:
	IdmStorage(const Info &storage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	IdmEntity *at(size_type index) const { return m_entities.at(index).entity; }
	size_type size() const { return m_entities.size(); }
	size_type indexOf(IdmEntity *item) const { return m_entities.indexOf(item); }
	size_type indexOf(id_type id) const { return m_entities.indexOf(id); }

	bool transaction();
	bool commit();
	void rollback();

	bool savepoint(const QByteArray &name);
	bool release(const QByteArray &name);
	void rollback(const QByteArray &name);

	QueryContext prepare(const Query &query, QString &error) const;

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type, const IdmShortFormat &shortFormat);
	bool removeEntity(IdmEntity *entity);

	bool addProperty(IdmEntity *entity, IdmEntity *property, const QString &name);
	bool removeProperty(IdmEntity *entity, IdmEntity *property);

	bool addValue(IdmEntity *entity, const IdsMap &values) const;
	bool addValue(IdmEntity *entity, const QVariant &value) const;
	bool removeValue(IdmEntity *entity, const IdsList &ids) const;

private:
	IdmEntity::id_type loadId(const QString &tableName) const;
	bool isThereCycles(IdmEntity *entity, IdmEntity *property) const;
	bool removeEntityValues(IdmEntity *entity, const IdsList &ids) const;
	bool removeOverlappingIds(IdmEntity *entity, IdmEntity *property, IdsSet &ids) const;
	bool cleanupParentsValues(IdmEntity *entity) const;
	bool cleanupParentsValues(IdmEntity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(IdmEntity *entity) const;
	bool cleanupPropertyValues(IdmEntity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(IdmEntity *entity, IdmEntity *property) const;

	void loadEntities(sqlite3_stmt *statement, IdmEntity *parent);

private:
	void performUndo();
	void performUndo(const QByteArray &name);
	void clearUndoStack();
	void clearUndoStack(const QByteArray &name);

private:
	void setLastError(const char *sqlQuery) const;
	void setLastError(const char *sqlQuery, const char *errorMsg) const;
	void setLastError(const QByteArray &sqlQuery) const;
	void setLastError(const QByteArray &sqlQuery, const char *errorMsg) const;
	void setLastError(const QString &error) const;

private:
	typedef QList<IdmStorageUndoCommand*>    UndoList;
	typedef HashedList<QByteArray, UndoList> UndoStack;

private:
	QMutex m_mutex;
	Info m_info;
	bool m_valid;
	sqlite3 *m_db;
	UndoStack m_undo;
	IdmEntityRoot m_entities;
	mutable QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDMSTORAGE_H_ */
