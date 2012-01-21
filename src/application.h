#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <QtCore/QThread>
#include <QtGui/QApplication>
//#include <QtSingleInstance/QSingleApplication>
#include "tools/exceptionshandler/exceptionshandler.h"
#include "tools/taskspool/taskpool.h"
#include "settings/applicationsettings.h"
#include "mainwindow/mainwindow.h"
#include "filesystemplugins/pluginsmanager.h"
#include "de/deservice.h"


class Application : public QApplication, protected ExceptionHandler
{
	Q_OBJECT

public:
	Application(const QString &name, const QString &label, const QString &description, int &argc, char **argv, bool GUIenabled = true);
	virtual ~Application();

	virtual bool notify(QObject *receiver, QEvent *event);

	qint32 exec();
	QString version() const;

	static Application *instance() { return static_cast<Application*>(QApplication::instance()); }
	static ::DesktopEnvironment::Service *desktopService() { return &instance()->m_desktopService; }
	static ::Tools::TasksPool::TaskPool *taskPool() { return &instance()->m_taskPool; }
	static ApplicationSettings *config() { return &instance()->m_settings; }
	static MainWindow *mainWindow() { return &instance()->m_mainWindow; }
	static ::FileSystem::PluginsManager *plugins() { return &instance()->m_plugins; }

protected: /* ExceptionHandler */
	virtual void handleException(const char *where);
	virtual void handleException(const QString &message);
	virtual void handleException(const char *where, const char *what);

private:
	::DesktopEnvironment::Service m_desktopService;
	::Tools::TasksPool::TaskPool m_taskPool;
	ApplicationSettings m_settings;
    ::FileSystem::Plugins::MyPluginsManager m_plugins;
	MainWindow m_mainWindow;
};

#endif /* APPLICATION_H_ */
