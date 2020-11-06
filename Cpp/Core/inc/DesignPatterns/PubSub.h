#ifndef __PUBSUB_H__
#define __PUBSUB_H__

#include "DesignPatterns/ServiceLocator.h"

#include <vector>

template<class EventType>
class EventCoordinator;

template<class EventType>
class ISubscriber;

template<class TEventType>
class IPublisher {
    // only EventCoordinator can add/remove subscribers
    friend class EventCoordinator<TEventType>;

public:
    IPublisher() : head(nullptr) {
        if(!ServiceLocator<EventCoordinator<TEventType>>::IsSet()) {
            new EventCoordinator<TEventType>();
        }
        ServiceLocator<EventCoordinator<TEventType>>::Get().AddPublisher(*this);
    }

    virtual ~IPublisher() {
        ServiceLocator<EventCoordinator<TEventType>>::Get().RemovePublisher(*this);
    }

    virtual void RaiseEvent(const TEventType& event) {
        ISubscriber<TEventType>* current = head;
        while(current) {
            current->Notify(event);
            current = current->next;
        }
    }

private:
    virtual void AddSubscriber(ISubscriber<TEventType>& subscriber) {
        subscriber.next = head;
        head = &subscriber;
    }

    virtual void RemoveSubscriber(const ISubscriber<TEventType>& subscriber) {
        ISubscriber<TEventType>* current = head;
        ISubscriber<TEventType>* previous = nullptr;
        while(current != nullptr) {
            if(current == &subscriber) {
                if(previous != nullptr) {
                    previous->next = current->next;
                } else {
                    head = head->next;
                }

                break;
            }

            previous = current;
            current = current->next;
        }
    }

protected:
    bool HasSubscribers() {
        return head != nullptr;
    }

private:
    ISubscriber<TEventType>* head{nullptr};
};

/////////////////////////////////////////////////////////////////

template<class TEventType>
class ISubscriber {
    template<class TEvent>
    friend class IPublisher;

public:
    ISubscriber() : next(nullptr) {
        if(!ServiceLocator<EventCoordinator<TEventType>>::IsSet()) {
            new EventCoordinator<TEventType>();
        }
        ServiceLocator<EventCoordinator<TEventType>>::Get().AddSubscriber(*this);
    }

    virtual ~ISubscriber() {
        ServiceLocator<EventCoordinator<TEventType>>::Get().RemoveSubscriber(*this);
    }

    virtual void Notify(const TEventType& event) = 0;

private:
    ISubscriber* next{nullptr};
};

//////////////////////////////////////////////////

template<class TEvent>
class EventCoordinator {
public:
    EventCoordinator() {
        ServiceLocator<EventCoordinator<TEvent>>::Set(*this);
    }

    void AddSubscriber(ISubscriber<TEvent>& subscriber) {
        for(IPublisher<TEvent>* publisher: publishers) {
            publisher->AddSubscriber(subscriber);
        }

        subscribers.push_back(&subscriber);
    }
    void RemoveSubscriber(const ISubscriber<TEvent>& subscriber) {
        for(IPublisher<TEvent>* publisher: publishers) {
            publisher->RemoveSubscriber(subscriber);
        }

        auto where = std::find(subscribers.begin(), subscribers.end(), &subscriber);
        if(where != subscribers.end()) {
            subscribers.erase(where);
        }
    }

    void AddPublisher(IPublisher<TEvent>& publisher) {
        for(ISubscriber<TEvent>* subscriber: subscribers) {
            publisher.AddSubscriber(*subscriber);
        }

        publishers.push_back(&publisher);
    }

    void RemovePublisher(const IPublisher<TEvent>& publisher) {
        auto where = std::find(publishers.begin(), publishers.end(), &publisher);
        if(where != publishers.end()) {
            publishers.erase(where);
        }
    }

private:
    std::vector<ISubscriber<TEvent>*> subscribers{};
    std::vector<IPublisher<TEvent>*> publishers{};
};

#endif // __PUBSUB_H__