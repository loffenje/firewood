#ifndef EVENT_H
#define EVENT_H

struct EventData;

typedef u64 EventType;
typedef std::shared_ptr<EventData> EventDataPtr;

namespace {
    constexpr int32_t max_queues = 2;
    constexpr u64 infinite = 0xffffffff;
}

internal u64 genEventID()
{
    local_var u64 event_id = 0;
    assert(event_id < infinite);

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
    typedef std::deque<EventDataPtr> EventQueue;
    
    static EventDispatcher *instance();

    template <typename E>
    bool addListener(const std::string &id, E *object, void (E::*func)(EventDataPtr), EventType type);
    
    bool removeListener(const std::string &id, EventType type);
    
    bool fireEvent(const EventDataPtr &event) const;
    bool queueEvent(const EventDataPtr &event);
    bool abortEvent(EventType type, bool all_types = false);
    bool broadcast(u64 max_millis);

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
    if (auto it = event_listeners.find(type); it != event_listeners.end()) {
        EventListenerList &listeners = it->second;
        for (auto it = listeners.begin(); it != listeners.end(); ++it) {
            //TODO: better way to remove listener ???
            if (id == (**it).id) {
                it = listeners.erase(it);
                return true;
            }
        }
    }

    return false;
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

bool EventDispatcher::abortEvent(EventType type, bool all_types)
{
    assert(active_queue >= 0);
    assert(active_queue < max_queues);

    bool result = false;
    if (auto it = event_listeners.find(type); it != event_listeners.end()) {
        EventQueue &event_queue = queues[active_queue];
        auto queue_it = event_queue.begin();
        while (queue_it != event_queue.end()) {
            if ((*queue_it)->getEventType() == type) {
                queue_it = event_queue.erase(queue_it);
                result = true;
                if (!all_types) return result; 
            }
        } 
    }

    return result;
}

bool EventDispatcher::broadcast(u64 max_ms)
{
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    u64 current_ms = ms.count();
    u64 max_ms_limit = max_ms == infinite ? infinite : current_ms + max_ms;

    int queue_to_process = active_queue;
    active_queue = (active_queue + 1) % max_queues;
    queues[active_queue].clear();

    while (!queues[queue_to_process].empty()) {
        EventDataPtr event = queues[queue_to_process].front();
        queues[queue_to_process].pop_front();
        EventType type = event->getEventType();

        auto it = event_listeners.find(type);
        if (it != event_listeners.end()) {
            const EventListenerList &listeners = it->second;
            for (auto it = listeners.begin(); it != listeners.end(); ++it) {
                BaseEventDelegate *listener = *it;
                listener->delegate(event);
            }
        }


        milliseconds elapsed_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
        current_ms = elapsed_ms.count();
        if (max_ms != infinite && current_ms >= max_ms_limit) {
            fprintf(stderr, "Aborting event processing. Time ran out\n");
            break;
        }
    }

    bool queue_flushed = queues[queue_to_process].empty();
    if (!queue_flushed) {
        while (!queues[queue_to_process].empty()) {
            EventDataPtr event = queues[queue_to_process].back();
            queues[queue_to_process].pop_back();
            queues[active_queue].push_front(event);
        }
    }

    return queue_flushed;
}


#endif
