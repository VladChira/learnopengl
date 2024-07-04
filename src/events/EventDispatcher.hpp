#pragma once

#include "Events.hpp"

#include <functional>
#include <array>

class EventDispatcher
{
public:
    template <typename EventType>
    void addEventListener(std::function<void(EventType &)> callback)
    {
        auto baseCallback = [func = std::move(callback)](Event &e)
        {
            if (e.getType() == EventType::type)
            {
                func(static_cast<EventType &>(e));
            }
        };
        callbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
    }

    void dispatch(Event &e)
    {
        auto &callback = callbacks[static_cast<size_t>(e.getType())];
        if (callback)
            callback(e);
    }

private:
    std::array<std::function<void(Event &)>, static_cast<size_t>(EventType::EventCount)> callbacks;
};