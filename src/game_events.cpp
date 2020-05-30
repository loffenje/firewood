
struct EntityDestroyed : public EventData
{
    static const EventType event_type;

    const char *getName() const override;
    const EventType &getEventType() const override;
    EventDataPtr copy() const override;

    void testEventFn()
    {
        std::cout << "triggered\n";
    }
};

const EventType EntityDestroyed::event_type{genEventID()};

const char *EntityDestroyed::getName() const
{
    return "EntityDestroyed";
}

const EventType &EntityDestroyed::getEventType() const
{
    return event_type;
}

EventDataPtr EntityDestroyed::copy() const
{
    return EventDataPtr(new EntityDestroyed());
}


struct TestEntity
{
    void destroyEventDelegate(EventDataPtr event_data)
    {
        std::shared_ptr<EntityDestroyed> p_event_data = std::static_pointer_cast<EntityDestroyed>(event_data);
        p_event_data->testEventFn();
    }

    void init()
    {
        EventDispatcher::instance()->addListener(
                std::to_string(EntityDestroyed::event_type),
                this, &TestEntity::destroyEventDelegate,
                EntityDestroyed::event_type);
    }
    
    void destroy()
    {
        EventDispatcher::instance()->removeListener(
                    std::to_string(EntityDestroyed::event_type),
                    this, &TestEntity::destroyEventDelegate,
                    EntityDestroyed::event_type
                );
    }
};
