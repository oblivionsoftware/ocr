cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ocr VERSION 0.1 LANGUAGES C CXX)

add_library(ocr
    src/common.c
    src/log.c
    src/pool.c)

target_include_directories(ocr PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

if ((${CMAKE_COMPILER_IS_GNUC}) OR (${CMAKE_C_COMPILER_ID} MATCHES "Clang"))
    target_compile_options(ocr PRIVATE
        -std=c99 -Wall -Werror -pedantic)
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

target_link_libraries(ocr_test ocr)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ocr_test)

enable_testing()
add_test(unit ocr_test)
