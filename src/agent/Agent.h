#pragma once

#include "../config/Config.h"
#include "../http/HttpClient.h"
#include "../tasks/TaskExecutor.h"

using json = nlohmann::json;

class Agent
{
private:

    Config config;
    HttpClient http;

public:

    Agent(const Config& cfg);

    Config getConfig() const;

    void updateConfig(Config cfg);

    void registerAgent();

    void pollTasks();

    void run();
};