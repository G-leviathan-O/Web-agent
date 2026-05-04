#include "config/ConfigManager.h"
#include <fstream>

using json = nlohmann::json;

Config ConfigManager::load(const std::string& path) {
    std::ifstream f(path);
    json j;
    f >> j;

    Config c;
    c.uid = j.value("UID", "");
    c.descr = j.value("descr", "");
    c.access_code = j.value("access_code", "");
    c.server_url = j.value("server_url", "");
    c.poll_interval_sec = j.value("poll_interval_sec", 10);
    c.task_directory = j.value("task_directory", "");
    c.result_directory = j.value("result_directory", "");
    c.log_file = j.value("log_file", "agent.log");
    c.task_program = j.value("task_program", "");

    return c;
}

void ConfigManager::save(const Config& cfg, const std::string& path) {
    json j;
    j["UID"] = cfg.uid;
    j["descr"] = cfg.descr;
    j["access_code"] = cfg.access_code;
    j["server_url"] = cfg.server_url;
    j["poll_interval_sec"] = cfg.poll_interval_sec;
    j["task_directory"] = cfg.task_directory;
    j["result_directory"] = cfg.result_directory;
    j["log_file"] = cfg.log_file;
    j["task_program"] = cfg.task_program;

    std::ofstream f(path);
    f << j.dump(4);
}