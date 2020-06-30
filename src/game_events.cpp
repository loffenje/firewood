
struct EntityDestroyed : public EventData {
    static const EventType event_type;

    const char *getName() const override;
    const EventType &getEventType() const override;
    EventDataPtr copy() const override;

    void testEventFn() { std::cout << "triggered\n"; }
};

const EventType EntityDestroyed::event_type{genEventID()};

const char *EntityDestroyed::getName() const { return "EntityDestroyed"; }

const EventType &EntityDestroyed::getEventType() const { return event_type; }

EventDataPtr EntityDestroyed::copy() const {
    return EventDataPtr(new EntityDestroyed());
}

internal u32 genEntityID() {
    local_var u32 entity_id = 0;

    return ++entity_id;
}

struct TestEntity {
    std::string id;

    void destroyEventDelegate(EventDataPtr event_data) {
	std::shared_ptr<EntityDestroyed> p_event_data =
	    std::static_pointer_cast<EntityDestroyed>(event_data);
	p_event_data->testEventFn();
    }

    void init() {
	id = std::to_string(genEntityID());
	EventDispatcher::instance()->addListener(
	    id, this, &TestEntity::destroyEventDelegate,
	    EntityDestroyed::event_type);
    }

    void destroy() {
	EventDispatcher::instance()->removeListener(
	    id, EntityDestroyed::event_type);
    }
};
