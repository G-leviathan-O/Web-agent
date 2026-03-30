#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

class HttpClient {
    std::string base_url;
public:
    explicit HttpClient(std::string url);

    nlohmann::json post(const std::string& endpoint, const nlohmann::json& data);

    // Flexible multipart: sends result_code + JSON 'result' + optional files
    nlohmann::json postMultipart(const std::string& endpoint,
                                 int result_code,
                                 const nlohmann::json& result,
                                 const std::vector<std::string>& files);
};