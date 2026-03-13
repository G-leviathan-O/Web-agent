#include "HttpClient.h"

#include <curl/curl.h>
#include <spdlog/spdlog.h>

using json = nlohmann::json;

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

HttpClient::HttpClient(const std::string& url)
{
    base_url = url;
}

json HttpClient::post(const std::string& endpoint, const json& data)
{
    CURL *curl = curl_easy_init();

    if(!curl)
        throw std::runtime_error("curl init failed");

    std::string url = base_url + endpoint;

    std::string response;

    std::string json_data = data.dump();

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);

    if(res != CURLE_OK)
        throw std::runtime_error("HTTP request failed");

    if(response.empty())
        return json();

    return json::parse(response);
}