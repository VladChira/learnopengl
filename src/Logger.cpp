#include "Logger.hpp"

#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::logger;

void Logger::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    logger = spdlog::stdout_color_mt("Main Logger");
    logger->set_level(spdlog::level::trace);
}