#pragma once

#include "Event.hpp"

class ResizeWindowEvent : public Event
{
public:
    ResizeWindowEvent(unsigned int newWidth, unsigned int newHeight)
    {
        this->width = newWidth;
        this->height = newHeight;
    }

    unsigned int getWidth() { return width; }
    unsigned int getHeight() { return height; }

    virtual EventType getType() const override
    {
        return type;
    }

    static const EventType type = EventType::ResizeWindow;

private:
    unsigned int width;
    unsigned int height;
};