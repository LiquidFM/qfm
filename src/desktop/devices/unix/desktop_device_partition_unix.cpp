#include "desktop_device_partition_unix.h"
#include "desktop_devices_udisks.h"


DESKTOP_NS_BEGIN

PartitionUnix::PartitionUnix(const Id &id,
			  	  	  	  	 const QIcon &icon,
			  	  	  	  	 const QString &label,
			  	  	  	  	 bool hidden,
			  	  	  	  	 Drive *parent,
			  	  	  	  	 int number,
			  	  	  	  	 quint64 size,
			  	  	  	  	 const QStringList &mountPaths) :
	Partition(id, icon, label, hidden, parent, number, size, mountPaths),
	m_mountedByThis(false)
{}

PartitionUnix::~PartitionUnix()
{
	if (m_mountedByThis)
	{
		QString error;
		unmount(error);
	}
}

bool PartitionUnix::mount(QString &error)
{
	QDBusInterface interface(QString::fromLatin1(UD_DBUS_SERVICE), id(), QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE), QDBusConnection::systemBus());

	if (interface.isValid())
	{
		static const QString filesystem = QString::fromLatin1("filesystem");
		QString idUsage = interface.property("IdUsage").toString();

		if (idUsage == filesystem)
		{
			QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(UD_DBUS_SERVICE),
																  id(),
																  QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE),
																  QString::fromLatin1("FilesystemMount"));

			message.setArguments(QList<QVariant>()
								 << interface.property("IdType").toString()
								 << QStringList());

			QDBusReply<QString> reply = QDBusConnection::systemBus().call(message);

			if (reply.isValid())
			{
				m_mountPaths.push_back(reply.value());
				m_mountedByThis = true;
				return true;
			}
			else
				error = reply.error().message();
		}
		else
			error = tr("Unknown type of partition (IdUsage: \"%1\").").arg(idUsage);
	}
	else
		error = interface.lastError().message();

	return false;
}

bool PartitionUnix::unmount(QString &error)
{
	QDBusMessage message = QDBusMessage::createMethodCall(QString::fromLatin1(UD_DBUS_SERVICE),
														  id(),
														  QString::fromLatin1(UD_DBUS_INTERFACE_DISKS_DEVICE),
														  QString::fromLatin1("FilesystemUnmount"));

	message.setArguments(QList<QVariant>() << QStringList());

	QDBusReply<void> reply = QDBusConnection::systemBus().call(message);

	if (reply.isValid())
		return true;
	else
		error = reply.error().message();

	return false;
}

DESKTOP_NS_END
