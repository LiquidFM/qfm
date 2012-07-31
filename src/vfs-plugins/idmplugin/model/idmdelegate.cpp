#include "idmdelegate.h"


IDM_PLUGIN_NS_BEGIN

IdmDelegate::IdmDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{}

void IdmDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}

QSize IdmDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QSize size = QStyledItemDelegate::sizeHint(option, index);
	size.setHeight(size.height() + 5);
	return size;
}

IDM_PLUGIN_NS_END