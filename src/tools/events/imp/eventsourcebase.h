#ifndef EVENTSOURCEBASE_H_
#define EVENTSOURCEBASE_H_

#include <QtGui/QWidget>
#include "../eventhandler.h"
#include "../../metatemplates.h"


#include "../../arguments/define.h"
#define INITIALISERS(Values) :         \
		BaseClass(Values),             \
	    m_eventHandler(eventHandler)   \
	{                                  \
		Q_ASSERT(m_eventHandler != 0); \
	}                                  \

template <typename BaseClass>
class EventSourceBase : public BaseClass
{
public:
	EventSourceBase(EventHandler *eventHandler) :
		BaseClass(),
	    m_eventHandler(eventHandler)
	{
		Q_ASSERT(m_eventHandler != 0);
	}

    template <TEMPLATE_ARGS_1>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_1) INITIALISERS(VALUES_1)

    template <TEMPLATE_ARGS_2>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_2) INITIALISERS(VALUES_2)

    template <TEMPLATE_ARGS_3>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_3) INITIALISERS(VALUES_3)

    template <TEMPLATE_ARGS_4>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_4) INITIALISERS(VALUES_4)

    template <TEMPLATE_ARGS_5>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_5) INITIALISERS(VALUES_5)

    template <TEMPLATE_ARGS_6>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_6) INITIALISERS(VALUES_6)

    template <TEMPLATE_ARGS_7>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_7) INITIALISERS(VALUES_7)

    template <TEMPLATE_ARGS_8>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_8) INITIALISERS(VALUES_8)

    template <TEMPLATE_ARGS_9>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_9) INITIALISERS(VALUES_9)

    template <TEMPLATE_ARGS_10>
    EventSourceBase(EventHandler *eventHandler, ARGUMENTS_10) INITIALISERS(VALUES_10)

protected:
    inline EventHandler *eventHandler() const { return m_eventHandler; }

private:
    EventHandler *m_eventHandler;
};

#undef INITIALISERS
#include "../../arguments/undefine.h"

#endif /* EVENTSOURCEBASE_H_ */