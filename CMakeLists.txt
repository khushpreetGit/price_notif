cmake_minimum_required(VERSION 3.10)
project(CryptoAlert)

set(CMAKE_CXX_STANDARD 17)

# Add the executable
add_executable(crypto_alert notif.cpp)
add_executable(client client_notif.cpp)
# Enable threading support
find_package(nlohmann_json 3.11.2 REQUIRED)
#include_directories(${json_SOURCE_DIR}/include)
target_link_libraries(crypto_alert PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)