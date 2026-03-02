cmake_minimum_required(VERSION 3.10)
project(Web-agent VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем директории с исходными файлами
include_directories(include)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME}
    src/main.cpp
)

# Устанавливаем свойства для более строгой компиляции
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)