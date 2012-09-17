#ifndef IDM_STORAGE_H_
#define IDM_STORAGE_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QVariant>
#include <QtCore/QCoreApplication>

#include <vfs/location/vfs_location.h>

#include "entities/idm_entityroot.h"
#include "queries/idm_query.h"
#include "queries/idm_querycontext.h"
#include "undo/idm_storage_undocommand.h"
#include "format/idm_shortformat.h"
#include "values/idm_entityvalue.h"
#include "../idm_ns.h"


typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;


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
	IdmStorage(const Location &storage, bool create);
	IdmStorage(const Location &newStorage, const Location &oldStorage);
	~IdmStorage();

	bool isValid() const { return m_valid; }
	const QString &lastError() const { return m_lastError; }

	IdmEntity *at(size_type index) const { return m_entities.at(index).entity; }
	size_type size() const { return m_entities.size(); }
	size_type indexOf(id_type id) const { return m_entities.indexOf(id); }

	bool transaction();
	bool commit();
	void rollback();

	bool savepoint(const QByteArray &name);
	bool release(const QByteArray &name);
	void rollback(const QByteArray &name);

	QueryContext prepare(const Query &query, QString &error) const;

	IdmEntity *createEntity(const QString &name, IdmEntity::Type type, const IdmShortFormat &shortFormat);
	bool updateEditorGeometry(IdmEntity *entity, const QRect &geometry);
	bool updateListGeometry(IdmEntity *entity, const QRect &geometry);
	bool removeEntity(IdmEntity *entity);

	bool addProperty(IdmEntity *entity, IdmEntity *property, const QString &name);
	bool renameProperty(IdmEntity *entity, IdmEntity *property, const QString &name);
	bool removeProperty(IdmEntity *entity, IdmEntity *property);

	IdmEntityValue::Holder addValue(IdmEntity *entity) const;
	bool addValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) const;
	bool addValue(const IdmEntityValue::Holder &entityValue, const IdmCompositeEntityValue::List &propertyValues) const;
	IdmEntityValue::Holder addValue(IdmEntity *entity, const QVariant &value) const;
	bool updateValue(const IdmEntityValue::Holder &value, const QVariant &newValue) const;
	bool removeValue(IdmEntity *entity, const IdsList &ids) const;
	bool removeValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) const;

private:
	id_type loadId(const QString &tableName) const;
	bool isThereCycles(IdmEntity *entity, IdmEntity *property) const;
	bool removeEntityValue(IdmEntity *entity, id_type id) const;
	bool removeEntityValues(IdmEntity *entity, const IdsList &ids) const;
	bool removeOverlappingIds(IdmEntity *entity, IdmEntity *property, IdsSet &ids) const;
	bool removeSelfOverlappingIds(IdmEntity *entity, const IdsList &entityIds, IdmEntity *property, IdsSet &propertyIds) const;
	bool cleanupParentsValues(IdmEntity *entity) const;
	bool cleanupParentsValues(IdmEntity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(IdmEntity *entity) const;
	bool cleanupPropertyValues(IdmEntity *entity, const IdsList &ids) const;
	bool cleanupPropertyValues(IdmEntity *entity, IdmEntity *property) const;

	void loadEntities();
	void loadProperties();
	void loadEntities(sqlite3_stmt *statement, IdmEntity *parent);
	void copyEntities(sqlite3 *oldDb, QByteArray &sqlQuery);
	void copyProperties(sqlite3 *oldDb, QByteArray &sqlQuery);
	void copySingleEntityValues(sqlite3 *oldDb, IdmEntity *entity, QByteArray &sqlQuery);
	void copyCompositeEntityValues(sqlite3 *oldDb, IdmEntity *entity, QByteArray &sqlQuery);
	void copyPropertyValues(sqlite3 *oldDb, IdmEntity *entity, IdmEntity *property, QByteArray &sqlQuery);

private:
	void performUndo();
	void performUndo(const QByteArray &name);
	void clearUndoStack();
	void clearUndoStack(const QByteArray &name);

private:
	QString failedToLoadId(const QString &tableName) const;
	QString failedToBind(const QByteArray &sqlQuery) const;
	QString failedToReset(const QByteArray &sqlQuery) const;

	void setLastError(const QByteArray &sqlQuery, sqlite3 *db) const;
	void setLastError(const char *sqlQuery) const;
	void setLastError(const char *sqlQuery, const char *errorMsg) const;
	void setLastError(const QByteArray &sqlQuery) const;
	void setLastError(const QByteArray &sqlQuery, const char *errorMsg) const;
	void setLastError(const QString &error) const;

private:
	typedef QList<IdmStorageUndoCommand*>                         UndoList;
	typedef ::Tools::Containers::HashedList<QByteArray, UndoList> UndoStack;

private:
	QMutex m_mutex;
	bool m_valid;
	sqlite3 *m_db;
	mutable UndoStack m_undo;
	IdmEntityRoot m_entities;
	mutable QString m_lastError;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_H_ */