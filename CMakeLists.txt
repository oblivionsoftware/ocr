include(ExternalProject)

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ocr VERSION 0.1 LANGUAGES C CXX)

find_package(Threads REQUIRED)

set(OCR_SOURCES
    src/buffer.c
    src/common.c
    src/file.c
    src/log.c
    src/pool.c
    src/linux/event.c
    src/mac/event.c
    src/windows/event.c)

if (NOT WIN32)
    list(FILTER OCR_SOURCES EXCLUDE REGEX ".+/windows/.+")
endif()

if (NOT APPLE)
    list(FILTER OCR_SOURCES EXCLUDE REGEX ".+/mac/.+")
endif()

if ((NOT UNIX) OR APPLE)
    list(FILTER OCR_SOURCES EXCLUDE REGEX ".+/linux/.+")
endif()

add_library(ocr ${OCR_SOURCES})

target_include_directories(ocr PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

if ((${CMAKE_COMPILER_IS_GNUCC}) OR (${CMAKE_C_COMPILER_ID} MATCHES "Clang"))
    target_compile_options(ocr PRIVATE
        -std=c99 -Wall -Werror -pedantic)
else()
    target_compile_options(ocr PRIVATE
        /W3 /WX)
    target_compile_definitions(ocr PRIVATE
        -D_CRT_SECURE_NO_WARNINGS)
endif()

install(TARGETS ocr EXPORT OcrConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT OcrConfig DESTINATION share/ocr/cmake)
export(TARGETS ocr FILE OcrConfig.cmake)

add_executable(ocr_test
    test/main.cpp)
target_link_libraries(ocr_test ocr ${CMAKE_THREAD_LIBS_INIT})

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ocr_test)

enable_testing()
add_test(unit ocr_test)
