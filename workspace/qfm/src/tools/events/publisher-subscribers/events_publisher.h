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
#ifndef EVENTS_PUBLISHER_H_
#define EVENTS_PUBLISHER_H_

#include <QtCore/QMap>
#include <QtCore/QLinkedList>
#include "events_subscriber.h"
#include "../../arguments/define.h"


#define PUBLISH_EVENT(Event) \
	publish(Event)

#define PUBLISH_EVENT_1(Event, arg1) \
	publish<Event##Arguments::Argument1>(Event, arg1)

#define PUBLISH_EVENT_2(Event, arg1, arg2) \
	publish<Event##Arguments::Argument1, Event##Arguments::Argument2>(Event, arg1, arg2)

#define PUBLISH_EVENT_3(Event, arg1, arg2, arg3) \
	publish<Event##Arguments::Argument1, Event##Arguments::Argument2, Event##Arguments::Argument3>(Event, arg1, arg2, arg3)


EVENTS_NS_BEGIN

class Publisher
{
private:
	typedef QLinkedList<Subscriber *> List;
	typedef QMap<int, List>           Container;

public:
	Publisher()
	{}
	virtual ~Publisher();

	void subscribe(Subscriber *subscriber)
	{
		Q_ASSERT(!m_subscribers[subscriber->event()].contains(subscriber));
		m_subscribers[subscriber->event()].push_back(subscriber);
	}

	void unsubscribe(Subscriber *subscriber)
	{
		Q_ASSERT(!m_subscribers.value(subscriber->event()).isEmpty());
		List &list = m_subscribers[subscriber->event()];

		for (List::iterator i = list.begin(), end = list.end(); i != end; ++i)
			if ((*i) == subscriber)
			{
				list.erase(i);
				break;
			}
	}

protected:
	inline void publish(int event)
	{
		Arguments arguments;
		publish(event, arguments);
	}

	template <TEMPLATE_ARGS_1>
	inline void publish(int event, ARGUMENTS_BY_VALUE_1)
	{
		Arguments1<ARGUMENTS_1> arguments(VALUES_1);
		publish(event, static_cast<Arguments &>(arguments));
	}

	template <TEMPLATE_ARGS_2>
	inline void publish(int event, ARGUMENTS_BY_VALUE_2)
	{
		Arguments2<ARGUMENTS_2> arguments(VALUES_2);
		publish(event, static_cast<Arguments &>(arguments));
	}

	template <TEMPLATE_ARGS_3>
	inline void publish(int event, ARGUMENTS_BY_VALUE_3)
	{
		Arguments3<ARGUMENTS_3> arguments(VALUES_3);
		publish(event, static_cast<Arguments &>(arguments));
	}

private:
	inline void publish(int event, Arguments &arguments)
	{
		List &list = m_subscribers[event];

		for (List::iterator i = list.begin(), end = list.end(); i != end; ++i)
			(*i)->receive(arguments);
	}

private:
	Container m_subscribers;
};

EVENTS_NS_END


#include "../../arguments/undefine.h"

#endif /* EVENTS_PUBLISHER_H_ */
