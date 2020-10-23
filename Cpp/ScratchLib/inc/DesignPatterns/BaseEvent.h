#ifndef BASEEVENT_H
#define BASEEVENT_H

#include "DesignPatterns/PubSub.h"

template<class DerivedEvent>
class BaseEvent {
public:
    BaseEvent() {
        coordinator; // force static constructor
    }

private:
    static EventCoordinator<DerivedEvent>* coordinator;
};

template<class DerivedEvent>
EventCoordinator<DerivedEvent>* BaseEvent<DerivedEvent>::coordinator = new EventCoordinator<DerivedEvent>();

#endif