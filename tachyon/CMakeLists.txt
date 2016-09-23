cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tachyon VERSION 0.1 LANGUAGES CXX)

set(TACHYON_SOURCES
    src/core/common.cpp
    src/core/exception.cpp
    src/core/string.cpp)

if (NOT WIN32)
    list(FILTER TACHYON_SOURCES EXCLUDE REGEX ".+/windows/.+")
endif()

if (NOT APPLE)
    list(FILTER TACHYON_SOURCES EXCLUDE REGEX ".+/mac/.+")
endif()

if ((NOT UNIX) OR APPLE)
    list(FILTER TACHYON_SOURCES EXCLUDE REGEX ".+/linux/.+")
endif()

add_library(tachyon ${TACHYON_SOURCES})

target_include_directories(tachyon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

if ((${CMAKE_COMPILER_IS_GNUCXX}) OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    target_compile_options(tachyon PRIVATE
        -std=c++14 -Wall -Werror -pedantic)
else()
    target_compile_options(tachyon PRIVATE
        /W3 /WX)
    target_compile_definitions(tachyon PRIVATE
        -D_CRT_SECURE_NO_WARNINGS)
endif()

install(TARGETS tachyon EXPORT TachyonConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT TachyonConfig DESTINATION share/tachyon/cmake)
export(TARGETS tachyon FILE TachyonConfig.cmake)
