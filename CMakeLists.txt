cmake_minimum_required(VERSION 3.16)

set(ENV{CXX} g++)
project(ccbench_base_silo)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

# file(GLOB_RECURSE PROJECT_SOURCES *.cpp)
# file(GLOB_RECURSE PROJECT_HEADERS *.h)

file(GLOB SILO_SOURCES
    "main.cpp" "silo_result.cpp" "transaction.cpp" "util.cpp" "log_buffer.cpp" "logger.cpp" "notifier.cpp" "result.cpp" "result_log.cpp"
)

#set(CMAKE_CXX_FLAGS "-pthread -fpermissive -Wreturn-type")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wreturn-type")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winvalid-memory-model")


add_executable(silo.exe ${SILO_SOURCES})

# add_executable(silo.exe ${PROJECT_SOURCES} ${PROJECT_HEADERS})



# target_link_libraries(silo.exe PUBLIC "-pthread -fpermissive")