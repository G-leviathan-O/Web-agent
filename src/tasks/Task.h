#pragma once

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class TaskType {
    TIMEOUT,
    CONF,
    FILE,
    TASK,
    UNKNOWN
};

class Task
{
private:
    std::string session_id;
    std::string options;
    std::string task_code;
    TaskType type;

public:
    Task(const json& res)
    {
        session_id = res["session_id"];
        options = res["options"];
        task_code = res["task_code"];

        if(task_code == "TIMEOUT") type = TaskType::TIMEOUT;
        else if(task_code == "CONF") type = TaskType::CONF;
        else if(task_code == "FILE") type = TaskType::FILE;
        else type = TaskType::UNKNOWN;
    }

    const std::string& getOptions() const { return options; }
    TaskType getType() const { return type; }
    const std::string& getSessionID() const { return session_id; }
};