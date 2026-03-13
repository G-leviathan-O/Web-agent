#pragma once

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "../tasks/TaskExecutor.h"

class Config
{
private:
    friend class TaskExecutor;

    std::string config_path;

    std::string uid;
    std::string descr;
    std::string access_code;
    std::string server_url;
    unsigned int poll_interval_sec;
    std::string task_directory;
    std::string result_directory;
    std::string log_file;

public: 
    Config(const std::string& path);

    template<typename T>
    Config setValue(const std::string &key, const T &value)
    {
        std::ifstream in(config_path);
        json cfg;
        in >> cfg;
        in.close();

        cfg[key] = value;

        std::ofstream out(config_path);
        out << cfg.dump(4);
        out.close();
        
        return Config(config_path);
    }

    std::string getConfigPath() const;

    std::string getUID() const;

    std::string getDescr() const;

    std::string getAccessCode() const;

    void setAccessCode(const std::string& ac);

    std::string getServerURL() const;

    unsigned int getPollInterval() const;

    std::string getTaskDirectory() const;

    std::string getResultDirectory() const;

    std::string getLogFile() const;
};