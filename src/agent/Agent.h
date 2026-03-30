#pragma once
#include "config/Config.h"
#include "http/HttpClient.h"
#include "tasks/TaskExecutor.h"

class Agent {
    Config config;
    HttpClient http;
    TaskExecutor executor;

public:
    Agent(Config cfg);
    void registerAgent();
    void run();

private:
    void processCycle();
};