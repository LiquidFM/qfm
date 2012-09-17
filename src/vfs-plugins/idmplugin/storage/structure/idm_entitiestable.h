#ifndef IDM_ENTITIESTABLE_H_
#define IDM_ENTITIESTABLE_H_

#include "idm_databasetypes.h"


IDM_PLUGIN_NS_BEGIN

struct EntitiesTable
{
	enum Fields
	{
		Id = 0,
		Type = 1,
		Name = 2,
		ShortFormat = 3
	};

	static QString tableName();

	static QByteArray create();
	static QByteArray select();
	static QByteArray select(Database::id_type id);
	static QByteArray insert();
	static QByteArray insertIntoSingle(Database::id_type id);
	static QByteArray insertIntoComposite(Database::id_type id);
	static QByteArray create(Database::EntityType type, Database::id_type id);
	static QByteArray insert(Database::EntityType type, Database::id_type id, const QString &name, const QString &shortFormat);
	static QByteArray updateEditorGeometry(Database::id_type entity);
	static QByteArray updateListGeometry(Database::id_type entity);
	static QByteArray remove(Database::id_type entity);
	static QByteArray selectValues(Database::id_type entity, Database::id_type property);
	static QByteArray addCompositeValue(Database::id_type entity, Database::id_type id);
	static QByteArray addValue(Database::id_type entity, Database::id_type id);
	static QByteArray updateValue(Database::id_type entity, Database::id_type id);
	static QByteArray removeValue(Database::id_type entity, Database::id_type value);
	static QByteArray removeValues(Database::id_type entity, const Database::IdsList &ids);
	static QByteArray removeProperty(Database::id_type entity, Database::id_type property);
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESTABLE_H_ */