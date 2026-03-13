#include "agent/Agent.h"
#include "config/Config.h"
#include "logging/Logger.h"

int main()
{
    Config config("config.json");

    Logger::init(config.getLogFile());

    spdlog::info("Logger initialized");

    Agent agent(config);

    agent.run();

    Logger::shutdown();

    return 0;
}