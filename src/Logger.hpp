#pragma once

#include <memory>

#include "spdlog/spdlog.h"

class Logger
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetLogger() { return logger; }

private:
    static std::shared_ptr<spdlog::logger> logger;
};