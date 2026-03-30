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

    nlohmann::json postMultipart(
        const std::string& endpoint,
        int result_code,
        const nlohmann::json& result,
        const std::vector<std::string>& files
    );
};