#pragma once
#include <curl/curl.h>
#include <stdexcept>

class CurlHandle {
    CURL* handle;
public:
    CurlHandle() {
        handle = curl_easy_init();
        if (!handle) throw std::runtime_error("curl init failed");
    }
    ~CurlHandle() {
        curl_easy_cleanup(handle);
    }
    CURL* get() { return handle; }
};