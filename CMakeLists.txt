cmake_minimum_required(VERSION 3.16)

set(ENV{CXX} g++)
project(ccbench_base_silo)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

# file(GLOB_RECURSE PROJECT_SOURCES *.cpp)
# file(GLOB_RECURSE PROJECT_HEADERS *.h)

add_executable(silo.exe main.cpp worker.cpp logger.cpp util.cpp)

# add_executable(silo.exe ${PROJECT_SOURCES} ${PROJECT_HEADERS})

target_link_libraries(silo.exe PUBLIC "-pthread")