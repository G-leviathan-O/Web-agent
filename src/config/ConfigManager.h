#pragma once
#include "config/Config.h"
#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

class ConfigManager {
public:
    Config load(const std::string& path);

    void save(const Config& cfg, const std::string& path);

    template<typename T>
    void setValue(const std::string& path, const std::string& key, const T& value)
    {
        std::ifstream in(path);
        nlohmann::json j;
        if (in) in >> j;
        in.close();

        j[key] = value; // add or update

        std::ofstream out(path);
        out << j.dump(4);
    }
};