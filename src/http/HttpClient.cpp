#include "http/HttpClient.h"
#include "http/CurlWrapper.h"
#include <curl/curl.h>
#include <stdexcept>
#include <logging/Logger.h>

using json = nlohmann::json;

namespace {
    size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        auto* s = static_cast<std::string*>(userp);
        s->append(static_cast<char*>(contents), size * nmemb);
        return size * nmemb;
    }

    struct CurlSList {
        curl_slist* list{nullptr};
        ~CurlSList() { if (list) curl_slist_free_all(list); }
        void append(const char* h) { list = curl_slist_append(list, h); }
        operator curl_slist*() { return list; }
    };
}

HttpClient::HttpClient(std::string url) : base_url(std::move(url)) {
    spdlog::info("HttpClient initialized with base_url={}", base_url);
}

json HttpClient::post(const std::string& endpoint, const json& data) {
    spdlog::debug("HTTP POST -> {}{}", base_url, endpoint);
    spdlog::debug("Payload: {}", data.dump());

    CurlHandle curl;
    std::string response;

    const std::string url = base_url + endpoint;
    const std::string body = data.dump();

    CurlSList headers;
    headers.append("Content-Type: application/json");

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, (curl_slist*)headers);
    curl_easy_setopt(curl.get(), CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    // NOTE: enable verification in real prod (set CA bundle path if needed)
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 2L);

    const CURLcode res = curl_easy_perform(curl.get());
    if (res != CURLE_OK) {
        spdlog::error("HTTP POST failed: {}", curl_easy_strerror(res));
        throw std::runtime_error(curl_easy_strerror(res));
    }

    spdlog::debug("Response: {}", response);

    if (response.empty()) return json{};
    return json::parse(response);
}

json HttpClient::postMultipart(const std::string& endpoint,
                               int result_code,
                               const json& result,
                               const std::vector<std::string>& files) {
    spdlog::debug("HTTP MULTIPART POST -> {}{}", base_url, endpoint);
    spdlog::debug("Result code: {}", result_code);
    spdlog::debug("Result payload: {}", result.dump());
    spdlog::debug("Files count: {}", files.size());

    CurlHandle curl;
    std::string response;

    const std::string url = base_url + endpoint;

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &response);

    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYHOST, 2L);

    curl_mime* mime = curl_mime_init(curl.get());
    if (!mime) throw std::runtime_error("curl_mime_init failed");

    auto add_field = [&](const char* name, const std::string& val) {
        curl_mimepart* part = curl_mime_addpart(mime);
        curl_mime_name(part, name);
        curl_mime_data(part, val.c_str(), CURL_ZERO_TERMINATED);
    };

    add_field("result_code", std::to_string(result_code));
    add_field("result", result.dump());

    for (size_t i = 0; i < files.size(); ++i) {
        curl_mimepart* part = curl_mime_addpart(mime);
        std::string field = "file" + std::to_string(i + 1);
        curl_mime_name(part, field.c_str());
        curl_mime_filedata(part, files[i].c_str());
    }

    curl_easy_setopt(curl.get(), CURLOPT_MIMEPOST, mime);

    const CURLcode res = curl_easy_perform(curl.get());

    curl_mime_free(mime);

    if (res != CURLE_OK) {
        spdlog::error("HTTP multipart failed: {}", curl_easy_strerror(res));
        throw std::runtime_error(curl_easy_strerror(res));
    }

    if (response.empty()) return json{};
    return json::parse(response);
}