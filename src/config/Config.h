#pragma once
#include <string>

struct Config {
    std::string uid;
    std::string descr;
    std::string access_code;
    std::string server_url;
    unsigned int poll_interval_sec;
    std::string task_directory;
    std::string result_directory;
    std::string log_file;
    std::string task_program;
};