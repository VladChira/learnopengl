#pragma once

enum class EventType
{
    ResizeWindow,
    CloseWindow,

    KeyPressed,
    KeyReleased,

    MouseMoved,
    MousePressed,
    MouseReleased,

    EventCount // a simple trick to count how many elements there are in an enum
};

class Event
{
public:
    virtual ~Event() = default;
    virtual EventType getType() const = 0;
};