#include "tasks/TaskExecutor.h"
#include "config/ConfigManager.h"
#include <fstream>
#include <logging/Logger.h>
#include <filesystem>

using json = nlohmann::json;

json TaskExecutor::execute(const Task& task, Config& config) {
    spdlog::info("Executing task...");
    spdlog::debug("Task type: {}", static_cast<int>(task.getType()));
    spdlog::debug("Task options: {}", task.getOptions());

    json result;

    auto ok = [&](const std::string& msg){
        result["status"] = "OK";
        result["message"] = msg;
        result["code_response"] = 0;
    };
    auto err = [&](const std::string& msg, int code){
        result["status"] = "ERROR";
        result["message"] = msg;
        result["code_response"] = code;
    };

    switch (task.getType()) {
        case TaskType::TIMEOUT: {
            // format: X second
            try {
                const std::string key = "poll_interval_sec";
                const unsigned value = std::stoul(task.getOptions());
                
                spdlog::info("Updating config: {}", key);
                ConfigManager cm;
                cm.setValue("config.json", key, value);
                config = cm.load("config.json");

                spdlog::info("Config successfully updated: {}", key);
                ok("Poll interval updated");
            } catch (...) {
                err("Invalid poll interval value", -11);
            }
            break;
        }

        case TaskType::CONF: {
            // format: key=value
            const std::string opt = task.getOptions();
            const auto pos = opt.find('=');
            if (pos == std::string::npos) {
                err("Invalid config option", -21);
                break;
            }
            const std::string key = opt.substr(0, pos);
            const std::string value = opt.substr(pos + 1);

            spdlog::info("Updating config: {}", key);
            ConfigManager cm;
            cm.setValue("config.json", key, value);
            config = cm.load("config.json");

            spdlog::info("Config successfully updated: {}", key);
            ok("Config updated: " + key);
            break;
        }

        case TaskType::FILE: {
            // format: filename
            const std::string path = config.result_directory + "/" + task.getOptions();
            std::ifstream f(path, std::ios::binary);
            if (f) {
                result["file_names"] = { task.getOptions() };
                ok("File ready");
            } else {
                err("File not found", -31);
            }
            break;
        }

        case TaskType::TASK: {
            if (config.task_program.empty()) {
                err("Task program not set", -40);
                break;
            }

            spdlog::info("Starting external program: {}", config.task_program);

            int exit_code = std::system(config.task_program.c_str());

            spdlog::info("Program finished with code {}", exit_code);

            if (exit_code != 0) {
                err("Program execution failed", -41);
                break;
            }

            // собираем файлы из result_directory
            std::vector<std::string> files;
            for (const auto& entry : std::filesystem::directory_iterator(config.result_directory)) {
                if (entry.is_regular_file()) {
                    files.push_back(entry.path().string());
                }
            }

            result["file_names"] = json::array();

            for (const auto& f : files) {
                result["file_names"].push_back(std::filesystem::path(f).filename().string());
            }

            ok("Task executed, files collected");

            for (const auto& f : files) {
                std::filesystem::remove(f);
            }

            break;
        }

        default:
            err("Unknown task type", -4);
    }
    return result;
}