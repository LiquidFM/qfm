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
#ifndef CONFIGOPTIONVALUETEMPLATE_H_
#define CONFIGOPTIONVALUETEMPLATE_H_

#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>
#include <QtGui/QTimeEdit>
#include <QtGui/QCheckBox>
#include <limits.h>
#include "configoptionvaluedefault.h"
#include "../../templates/metatemplates.h"


SETTINGS_NS_BEGIN

template <typename Editor = QLineEdit>
class ConfigOptionValueTemplate: public ConfigOptionValueDefault
{
public:
	ConfigOptionValueTemplate(const qint32 &id, const QString &title, const QVariant &defaultValue, ConfigOption *parent = 0) :
		ConfigOptionValueDefault(id, title, defaultValue, parent),
		m_editor(0)
	{}

	virtual bool isEnabled() const { return m_editor->isEnabled(); }
	virtual void setEnabled(bool value) { m_editor->setEnabled(value); }
	virtual QWidget *createEditor(QWidget *parent) { return m_editor = static_cast<Editor*>(createEditor(parent, typename Templates::type_to_type<Editor>::type())); }
	virtual QVariant editorValue() const { return value(typename Templates::type_to_type<Editor>::type()); }
	virtual void setEditorValue(const QVariant &value) { setValue(value, typename Templates::type_to_type<Editor>::type()); }
	virtual void setLoadedEditorValue(const QVariant &value) { setValue(value, typename Templates::type_to_type<Editor>::type()); }

private:
	inline QWidget *createEditor(QWidget *parent, const Templates::type_to_type<QLineEdit>::type&) { return new QLineEdit(parent); }
	inline QWidget *createEditor(QWidget *parent, const Templates::type_to_type<QSpinBox>::type&)
	{
		QSpinBox *res = new QSpinBox(parent);
		res->setMinimum(0);
		res->setMaximum(INT_MAX);
		return res;
	}
	inline QWidget *createEditor(QWidget *parent, const Templates::type_to_type<QTimeEdit>::type&) { return new QTimeEdit(parent); }
	inline QWidget *createEditor(QWidget *parent, const Templates::type_to_type<QCheckBox>::type&) { return new QCheckBox(parent); }

	inline QVariant value(const Templates::type_to_type<QLineEdit>::type&) const { return m_editor->text(); }
	inline QVariant value(const Templates::type_to_type<QSpinBox>::type&) const { return m_editor->value(); }
	inline QVariant value(const Templates::type_to_type<QTimeEdit>::type&) const { return m_editor->time(); }
	inline QVariant value(const Templates::type_to_type<QCheckBox>::type&) const { return m_editor->isChecked(); }

	inline void setValue(const QVariant &value, const Templates::type_to_type<QLineEdit>::type&) const { return m_editor->setText(value.toString()); }
	inline void setValue(const QVariant &value, const Templates::type_to_type<QSpinBox>::type&) const { return m_editor->setValue(value.toInt()); }
	inline void setValue(const QVariant &value, const Templates::type_to_type<QTimeEdit>::type&) const { return m_editor->setTime(value.toTime()); }
	inline void setValue(const QVariant &value, const Templates::type_to_type<QCheckBox>::type&) const { return m_editor->setChecked(value.toBool()); }

private:
	Editor *m_editor;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONVALUETEMPLATE_H_ */
