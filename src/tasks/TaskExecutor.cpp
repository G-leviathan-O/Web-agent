#include "TaskExecutor.h"
#include "../agent/Agent.h"
#include <boost/process.hpp>

namespace bp = boost::process;

TaskExecutor::TaskExecutor(const std::string& dir)
{
    resultDir = dir;
}

json TaskExecutor::execute(const Task& task, Agent* agent)
{
    json result;
    switch(task.getType())
    {
        case TaskType::TIMEOUT:
        {
            try {
                unsigned int new_interval = std::stoul(task.getOptions());
                Config config = agent->getConfig();
                agent->updateConfig(config.setValue("poll_interval_sec", new_interval));
                result["status"] = "OK";
                result["message"] = "Poll interval updated";
            } catch(...) {
                result["status"] = "ERROR";
                result["message"] = "Invalid poll interval value";
            }
            break;
        }
        case TaskType::CONF:
        {
            // Для простоты можно изменить любое поле Config через options в формате key=value
            std::string opt = task.getOptions();
            auto eq_pos = opt.find('=');
            if(eq_pos != std::string::npos)
            {
                std::string key = opt.substr(0, eq_pos);
                std::string value = opt.substr(eq_pos + 1);

                Config config = agent->getConfig();
                agent->updateConfig(config.setValue(key, value));
                result["status"] = "OK";
                result["message"] = key + " updated";
            } else {
                result["status"] = "ERROR";
                result["message"] = "Invalid config option";
            }
            break;
        }
        // case TaskType::FILE:
        // {
        //     std::string filename = task.getOptions();
        //     std::ifstream file(config.getResultDirectory() + "/" + filename, std::ios::binary);
        //     if(file)
        //     {
        //         std::string content((std::istreambuf_iterator<char>(file)),
        //                              std::istreambuf_iterator<char>());
        //         result["status"] = "OK";
        //         result["file_content"] = content; // можно закодировать в base64 для безопасности
        //     }
        //     else
        //     {
        //         result["status"] = "ERROR";
        //         result["message"] = "File not found";
        //     }
        //     break;
        // }
        // default:
        //     result["status"] = "ERROR";
        //     result["message"] = "Unknown task type";
    }
    return result;
}