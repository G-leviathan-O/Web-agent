#include "Config.h"

#include <spdlog/spdlog.h>

using json = nlohmann::json;

Config::Config(const std::string& path)
{
    std::ifstream config_file(path);

    json j;
    config_file >> j;

    config_path = path;
    uid = j["UID"];
    descr = j["descr"];
    access_code = j.value("access_code", "");
    server_url = j["server_url"];
    poll_interval_sec = j["poll_interval_sec"];
    task_directory = j["task_directory"];
    result_directory = j["result_directory"];
    log_file = j["log_file"];
}

std::string Config::getConfigPath() const
{
    return config_path;
}

std::string Config::getUID() const
{
    return uid;
}

std::string Config::getDescr() const
{
    return descr;
}

std::string Config::getAccessCode() const
{
    return access_code;
}

void Config::setAccessCode(const std::string& ac)
{
    access_code = ac;
    this->setValue("access_code", ac);
    spdlog::info("Config updated: access_code = {}", access_code);
}

std::string Config::getServerURL() const
{
    return server_url;
}

unsigned int Config::getPollInterval() const
{
    return poll_interval_sec;
}

std::string Config::getTaskDirectory() const
{
    return task_directory;
}

std::string Config::getResultDirectory() const
{
    return result_directory;
}

std::string Config::getLogFile() const
{
    return log_file;
}