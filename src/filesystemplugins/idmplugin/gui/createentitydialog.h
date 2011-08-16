#ifndef CREATEENTITYDIALOG_H_
#define CREATEENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QLineEdit>
#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/entitieslistmodel.h"


class CreateEntityDialog : public QDialog
{
    Q_OBJECT

public:
    CreateEntityDialog(const FileSystem::IdmContainer *container, const QString &name = QString(), QWidget *parent = 0);

    QString name() const { return m_lineEdit.text().simplified(); }
    virtual void accept();

private Q_SLOTS:
	void activated(int index);

private:
	void setListEnabled(bool enabled);

private:
    QLabel m_label;
    QLineEdit m_lineEdit;
    QLabel m_label2;
    QComboBox m_comboBox;
    QTreeView m_view;
    QPushButton m_addEntity;
    QPushButton m_removeEntity;
    QGridLayout m_gridLayout;
    QGridLayout m_gridLayout2;
    QHBoxLayout m_horizontalLayout;
    QDialogButtonBox m_buttonBox;
    FileSystem::EntitiesListModel m_model;
};

#endif /* CREATEENTITYDIALOG_H_ */
