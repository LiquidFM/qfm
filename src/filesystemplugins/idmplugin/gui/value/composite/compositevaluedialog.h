#ifndef COMPOSITEVALUEDIALOG_H_
#define COMPOSITEVALUEDIALOG_H_

#include "widgets/compositevaluewidget.h"
#include "../../tools/nestedplaindialog/nestedplaindialog.h"


class CompositeValueDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	CompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent = 0);

protected:
	virtual void addValue();
	virtual void removeValue();

protected:
    QModelIndex currentIndex() const { return m_mainWidget.currentIndex(); }

    const IdmContainer &container() const { return m_mainWidget.container(); }
	IdmContainer &container() { return m_mainWidget.container(); }

	const CompositeValueModel &model() const { return m_mainWidget.model(); }
	CompositeValueModel &model() { return m_mainWidget.model(); }

	void doAddValue(const QModelIndex &index) { m_mainWidget.addValue(index); }
	void doRemoveValue(const QModelIndex &index) { m_mainWidget.removeValue(index); }

private:
	typedef KeyboardEventHandler<
				EventHandlerBase<
					CompositeValueDialog
				>
			> TreeViewHandler;

private:
	TreeViewHandler m_handler;
	MainCompositeValueWidget m_mainWidget;
};

#endif /* COMPOSITEVALUEDIALOG_H_ */
