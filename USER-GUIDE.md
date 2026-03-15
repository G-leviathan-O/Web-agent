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
sudo apt update
sudo apt install build-essential cmake libcurl4-openssl-dev libboost-all-dev
git clone https://github.com/G-leviathan-O/Web-agent.git
cd Web-agent
mkdir build && cd build
cmake ..
make

### macOS
brew install cmake curl spdlog boost
git clone https://github.com/G-leviathan-O/Web-agent.git
cd Web-agent
mkdir build && cd build
cmake ..
make
