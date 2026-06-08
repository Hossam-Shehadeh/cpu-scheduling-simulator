cmake_minimum_required(VERSION 3.15)

project(CPUSchedulerSimulator
    VERSION 1.0.0
    DESCRIPTION "CPU Scheduling Simulator — FCFS, SJF, Round Robin"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(scheduler
    src/main.cpp
    src/scheduler.cpp
    src/process.cpp
)

target_include_directories(scheduler PRIVATE include)

if(MSVC)
    target_compile_options(scheduler PRIVATE /W4)
else()
    target_compile_options(scheduler PRIVATE -Wall -Wextra -pedantic)
endif()

# Optional: copy examples to build dir for easy testing
file(COPY examples DESTINATION ${CMAKE_BINARY_DIR})
