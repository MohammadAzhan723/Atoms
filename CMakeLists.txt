cmake_minimum_required(VERSION 3.16)

project(AtomicSandbox)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

add_executable(AtomicSandbox src/main.cpp)

target_link_libraries(AtomicSandbox
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)
