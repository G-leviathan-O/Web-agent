# WEB-AGENT

Кроссплатформенный фоновый агент для удаленного управления и выполнения задач.

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.15+-brightgreen.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## 📋 О проекте

WEB-AGENT - это фоновый клиент, который получает инструкции от сервера и выполняет локальные задачи.

Поддерживает Windows 10+, Linux (Ubuntu 20.04+) и macOS 12+.

## 🚀 Быстрый старт

```bash
git clone https://github.com/G-leviathan-O/Web-agent.git
cd Web-agent
mkdir build && cd build
cmake ..
cmake --build . --config Release  # для Windows
make                               # для Linux/macOS
```
## 📚 Документация

- [USER-GUIDE.md](USER-GUIDE.md) — установка, настройка, решение проблем
- [ARCHITECTURE.md](ARCHITECTURE.md) — устройство проекта для разработчиков
- [API.md](API.md) — взаимодействие с сервером
