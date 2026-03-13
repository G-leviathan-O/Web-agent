#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
public:

    static void init(const std::string& file)
    {
        auto logger = spdlog::basic_logger_mt("agent", file);

        spdlog::set_default_logger(logger);

        spdlog::set_level(spdlog::level::debug);

        spdlog::flush_on(spdlog::level::debug);
    }
    
    static void shutdown()
    {
        spdlog::shutdown();
    }
};