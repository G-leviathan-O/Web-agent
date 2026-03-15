# Руководство пользователя WEB-AGENT

# Установка

### Windows
1. Установите Visual Studio с C++
2. Установите CMake
3. Скачайте проект: `git clone https://github.com/G-leviathan-O/Web-agent.git`
4. Соберите:
cd Web-agent
mkdir build
cd build
cmake ..
cmake --build . --config Release

### Linux
1. Установите компилятор и зависимости:
sudo apt update
sudo apt install build-essential cmake libcurl4-openssl-dev libboost-all-dev
2. Скачайте проект: `git clone https://github.com/G-leviathan-O/Web-agent.git`
3. Соберите:
cd Web-agent
mkdir build
cd build
cmake ..
make

### macOS
1. Установите Homebrew (если нет): `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
2. Установите зависимости: `brew install cmake curl spdlog boost`
3. Скачайте проект: `git clone https://github.com/G-leviathan-O/Web-agent.git`
4. Соберите:
cd Web-agent
mkdir build
cd build
cmake ..
make
