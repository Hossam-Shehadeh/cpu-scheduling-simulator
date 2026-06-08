cmake_minimum_required(VERSION 3.15)

project(CPUSchedulerSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(scheduler
    main.cpp
    scheduler.cpp
    process.cpp
)

if(MSVC)
    target_compile_options(scheduler PRIVATE /W4)
else()
    target_compile_options(scheduler PRIVATE -Wall -Wextra -pedantic)
endif()
