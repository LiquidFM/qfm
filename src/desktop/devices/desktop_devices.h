#ifndef DESKTOP_DEVICES_H_
#define DESKTOP_DEVICES_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "desktop_device.h"
#include "../../tools/events/publisher-subscribers/events_publisher.h"


class DevicesPrivate;


DESKTOP_NS_BEGIN

class Devices : public ::Tools::Events::Publisher
{
public:
	typedef QMap<Device::Id, Device *> Container;
	typedef Container::const_iterator  const_iterator;

public:
	enum Events
	{
		DeviceAdded,
		DeviceRemoved,
		DeviceChanged
	};

	DECLARE_EVENT_SUBSCRIBER_1(DeviceAdded, const Device *);
	DECLARE_EVENT_SUBSCRIBER_1(DeviceRemoved, const Device *);
	DECLARE_EVENT_SUBSCRIBER_1(DeviceChanged, const Device *);

public:
	Devices();
	virtual ~Devices();

	const_iterator begin() const;
	const_iterator end() const;

	static Devices *instance();

private:
	void slotDeviceAdded(const Device *device);
	void slotDeviceRemoved(const Device *device);
	void slotDeviceChanged(const Device *device);

private:
	friend class ::DevicesPrivate;
	::DevicesPrivate *d;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICES_H_ */