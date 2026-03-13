#pragma once

#include <string>
#include <nlohmann/json.hpp>

class HttpClient
{
private:

    std::string base_url;

public:

    HttpClient(const std::string& url);

    nlohmann::json post(
        const std::string& endpoint,
        const nlohmann::json& data
    );
};