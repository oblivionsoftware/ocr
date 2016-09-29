cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(sof VERSION 0.1 LANGUAGES CXX)

set(SOF_SOURCES
    src/main.cpp)

add_executable(sof WIN32 ${SOF_SOURCES})

if ((${CMAKE_COMPILER_IS_GNUCXX}) OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    target_compile_options(sof PRIVATE
        -std=c++14 -Wall -Werror -pedantic)
else()
    target_compile_options(sof PRIVATE
        /W3 /WX /GR-)
    target_compile_definitions(sof PRIVATE
        -D_CRT_SECURE_NO_WARNINGS)
endif()

target_link_libraries(sof tachyon)

add_custom_target(run
    COMMAND sof
    WORKING_DIRECTORY ${sof_SOURCE_DIR})
