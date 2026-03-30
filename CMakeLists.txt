cmake_minimum_required(VERSION 3.15)

project(Web-agent VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)

set(SOURCES
    src/main.cpp
    src/agent/Agent.cpp
    src/config/ConfigManager.cpp
    src/http/HttpClient.cpp
    src/tasks/TaskExecutor.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        CURL::libcurl
        spdlog::spdlog
        nlohmann_json::nlohmann_json
)