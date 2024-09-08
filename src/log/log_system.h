#pragma once

#include <stdexcept>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include "base/singleton.h"

class LogSystem : public Singleton<LogSystem> {
    friend Singleton<LogSystem>;

public:
    virtual ~LogSystem() { shutdown(); }

    void initialize();
    void shutdown();

    std::shared_ptr<spdlog::logger> getLogger() { return m_logger; }

private:
    std::shared_ptr<spdlog::logger> m_logger;

    LogSystem() = default;
};

#define TRACE(...) SPDLOG_LOGGER_TRACE(LogSystem::get()->getLogger(), __VA_ARGS__)
#define DEBUG(...) SPDLOG_LOGGER_DEBUG(LogSystem::get()->getLogger(), __VA_ARGS__)
#define INFO(...) SPDLOG_LOGGER_INFO(LogSystem::get()->getLogger(), __VA_ARGS__)
#define WARN(...) SPDLOG_LOGGER_WARN(LogSystem::get()->getLogger(), __VA_ARGS__)
#define ERROR(...) SPDLOG_LOGGER_ERROR(LogSystem::get()->getLogger(), __VA_ARGS__)
#define FATAL(...) \
    do { \
        SPDLOG_LOGGER_CRITICAL(LogSystem::get()->getLogger(), __VA_ARGS__); \
        LogSystem::get()->getLogger()->flush(); \
        std::string message = fmt::format(__VA_ARGS__); \
        throw std::runtime_error(message); \
    } while (0)