#pragma once

#include "Task.h"
#include "../config/Config.h"
#include <string>

class Agent;

class TaskExecutor
{
private:

    std::string resultDir;

public:

    TaskExecutor(const std::string& dir);

    json execute(const Task& task, Agent* agent);
};