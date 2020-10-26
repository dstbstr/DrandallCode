#ifndef __BASEEVENT_H__
#define __BASEEVENT_H__

#include "DesignPatterns/PubSub.h"

template<class DerivedEvent>
class BaseEvent {
private:
    static inline EventCoordinator<DerivedEvent>* coordinator;
};
#endif // __BASEEVENT_H__