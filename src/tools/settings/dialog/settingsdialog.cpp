#include "settingsdialog.h"
#include "../configoptions/configoptionvalue.h"


SettingsDialog::SettingsDialog(const QString &title, const SettingsList &settings, const ConstraintsList &constraints, QWidget *parent) :
	QDialog(parent),
    m_tabs(this),
	m_layout(this),
	m_buttonBox(this),
	m_constraints(constraints)
{
    setWindowTitle(title);

    m_buttonBox.setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
    connect(&m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(&m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_layout.setSpacing(6);
    m_layout.setContentsMargins(6, 6, 6, 6);
    m_layout.addWidget(&m_tabs);
    m_layout.addWidget(&m_buttonBox);

    QFont font = createFont();
	for (SettingsList::size_type i = 0, size = settings.size(); i < size; ++i)
		m_tabs.addTab(settings.at(i)->createGroup(font), settings.at(i)->title());
}

void SettingsDialog::accept()
{
	for (ConstraintsList::size_type i = 0, size = m_constraints.size(); i < size; ++i)
		if (!m_constraints.at(i)->check(this))
			return;

	QDialog::accept();
}

QFont SettingsDialog::createFont() const
{
    QFont font;
    font.setBold(true);
    font.setWeight(75);

    return font;
}