#ifndef EVENT_H
#define EVENT_H

struct EventData;

typedef unsigned long EventType;
typedef std::shared_ptr<EventData> EventDataPtr;

namespace {
    constexpr int32_t max_queues = 2;
    constexpr EventType max_GUID =  0xffffffff;
}

internal EventType genEventID()
{
    local_var EventType event_id = 0;
    assert(event_id < max_GUID);

    return ++event_id;
}

struct EventData
{
    virtual const char *getName() const = 0;
    virtual const EventType &getEventType() const = 0;
    virtual EventDataPtr copy() const = 0;
    virtual ~EventData() {}
    
    b32 handled = false;
    f32 timestamp = 0.0f;
};

struct BaseEventDelegate
{
    BaseEventDelegate(std::string id): id{id} {}

    virtual void delegate(EventDataPtr event) = 0; 
    virtual ~BaseEventDelegate() {}

    std::string id;
};

template <typename E>
struct EventDelegate : public BaseEventDelegate
{
    typedef void (E::*fn_ptr)(EventDataPtr);
    EventDelegate(E *event, fn_ptr func, std::string id): event{event}, func{func}, BaseEventDelegate{id} {}

    void delegate(EventDataPtr event) override;

    E *event;
    fn_ptr func;
};

template <typename E>
void EventDelegate<E>::delegate(EventDataPtr event_data)
{
    (event->*func)(event_data);
}

struct EventDispatcher
{
    
    typedef std::vector<BaseEventDelegate *> EventListenerList;
    typedef std::map<EventType, EventListenerList> EventListenerMap;
    typedef std::vector<EventDataPtr> EventQueue;
    
    static EventDispatcher *instance();

    template <typename E>
    bool addListener(const std::string &id, E *object, void (E::*func)(EventDataPtr), EventType type);
    
    bool removeListener(const std::string &id, EventType type);
    
    bool fireEvent(const EventDataPtr &event) const;
    bool queueEvent(const EventDataPtr &event);

    EventListenerMap event_listeners;
    EventQueue queues[max_queues];
    i32 active_queue;
private:
    ~EventDispatcher() {}
};


EventDispatcher *EventDispatcher::instance()
{
    local_var EventDispatcher *instance = new EventDispatcher();

    return instance;
}

template <typename E>
bool EventDispatcher::addListener(const std::string &id, E *object, void (E::*func)(EventDataPtr), EventType type)
{
    BaseEventDelegate *event_delegate = new EventDelegate<E>(object, func, id);
    event_listeners[type].push_back(event_delegate);

    return true;
}

bool EventDispatcher::removeListener(const std::string &id, EventType type)
{
    bool result = false;
    if (auto it = event_listeners.find(type); it != event_listeners.end()) {
        EventListenerList &listeners = it->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it) {
            //TODO: better way to remove listener ???
            if (id == (**it).id) {
                it = listeners.erase(it);
                result = true;
                break;
            }
        }
    }

    return result;
}


bool EventDispatcher::fireEvent(const EventDataPtr &event) const
{
    bool processed = false;
    auto it = event_listeners.find(event->getEventType());
    if (it != event_listeners.end()) {
        const EventListenerList &listeners = it->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it) {
            BaseEventDelegate *listener = *it;
            listener->delegate(event);
            processed = true;
        }
    }

    return processed;
}

bool EventDispatcher::queueEvent(const EventDataPtr &event)
{
    assert(active_queue >= 0);
    assert(active_queue < max_queues);
    
    if (auto it = event_listeners.find(event->getEventType()); it != event_listeners.end()) {
        queues[active_queue].push_back(event);

        return true;
    }

    return false;
}

#endif
