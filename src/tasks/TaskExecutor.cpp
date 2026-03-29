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

	auto set_ok = [&](const std::string& msg) {
		result["status"] = "OK";
		result["message"] = msg;
		result["code_responce"] = 0;
	};

	auto set_error = [&](const std::string& msg, int code) {
		result["status"] = "ERROR";
		result["message"] = msg;
		result["code_responce"] = code;
	};

    switch(task.getType())
    {
        case TaskType::TIMEOUT:
        {
            try {
                unsigned int new_interval = std::stoul(task.getOptions());
                Config config = agent->getConfig();
                agent->updateConfig(config.setValue("poll_interval_sec", new_interval));

                set_ok("Poll interval updated");
            } catch(...) {
                set_error("Invalid poll interval value", -11);
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

                set_ok(key + " updated");
            } else {
                set_error("Invalid config option", -21);
            }
            break;
        }
        case TaskType::FILE:
        {
            std::string filename = task.getOptions();
            Config config = agent->getConfig();
            std::ifstream file(config.getResultDirectory() + "/" + filename, std::ios::binary);
            if(file)
            {
                std::string content((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());

                result["file_content"] = content;
                set_ok("File read successfully");
            }
            else
            {
                set_error("File not found", -31);
            }
            break;
        }
        default:
            set_error("Unknown task type", -4);
    }
    return result;
}