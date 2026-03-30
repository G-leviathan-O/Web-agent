#pragma once
#include "tasks/Task.h"
#include "config/Config.h"
#include <nlohmann/json.hpp>

class TaskExecutor {
public:
    nlohmann::json execute(const Task& task, Config& config);
};