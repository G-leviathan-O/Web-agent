cmake_minimum_required(VERSION 3.15)
project(Web-agent VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)

include_directories(src)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/agent/Agent.cpp
        src/config/Config.cpp
        src/http/HttpClient.cpp
        src/tasks/TaskExecutor.cpp
)

target_link_libraries(${PROJECT_NAME}
        CURL::libcurl
        spdlog::spdlog
)