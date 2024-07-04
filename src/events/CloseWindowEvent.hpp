#pragma once

#include "Event.hpp"

class CloseWindowEvent : public Event
{
public:
    virtual EventType getType() const override
    {
        return type;
    }

    static const EventType type = EventType::CloseWindow;
};