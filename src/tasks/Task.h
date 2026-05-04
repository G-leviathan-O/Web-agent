#pragma once
#include <string>
#include <nlohmann/json.hpp>

enum class TaskType { TIMEOUT, CONF, FILE, TASK, UNKNOWN };

class Task {
    std::string session_id;
    std::string options;
    TaskType type;

public:
    Task(const nlohmann::json& j) {
        session_id = j.value("session_id", "");
        options = j.value("options", "");

        std::string code = j.value("task_code", "");
        if (code == "TIMEOUT") type = TaskType::TIMEOUT;
        else if (code == "CONF") type = TaskType::CONF;
        else if (code == "FILE") type = TaskType::FILE;
        else if (code == "TASK") type = TaskType::TASK;
        else type = TaskType::UNKNOWN;
    }

    const std::string& getOptions() const { return options; }
    TaskType getType() const { return type; }
    const std::string& getSessionID() const { return session_id; }
};