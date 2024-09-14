#include "log_system.h"

#include <filesystem>

#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include "config/config_manager.h"

void LogSystem::initialize() {
    std::filesystem::path log_path = ConfigManager::get()->getValue("settings", "log_path");
    spdlog::init_thread_pool(8192, 1);

    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    stdout_sink->set_level(spdlog::level::warn);

    auto rotating_sink =
        std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_path / "video-app-log.log", 1024 * 1024 * 10, 3);
    rotating_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};
    m_logger = std::make_shared<spdlog::async_logger>(
        "video-app-logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block
    );
    m_logger->set_level(spdlog::level::trace);
}

void LogSystem::shutdown() {
    m_logger.reset();
}