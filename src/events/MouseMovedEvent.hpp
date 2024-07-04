#pragma once

#include "Event.hpp"

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(double newX, double newY)
    {
        this->x = newX;
        this->y = newY;
    }

    unsigned int getX() { return x; }
    unsigned int getY() { return y; }

    virtual EventType getType() const override
    {
        return type;
    }

    static const EventType type = EventType::MouseMoved;

private:
    double x;
    double y;
};