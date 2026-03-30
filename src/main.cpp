#include "config/ConfigManager.h"
#include "agent/Agent.h"
#include "logging/Logger.h"

int main() {
    ConfigManager cm;
    auto config = cm.load("config.json");
    
    Logger::init(config.log_file);

    Agent agent(config);
    agent.run();

    Logger::shutdown();
    return 0;
}