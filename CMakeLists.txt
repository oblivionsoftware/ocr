include(ExternalProject)

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(ocr VERSION 0.1 LANGUAGES C CXX)

find_package(Threads REQUIRED)

add_library(ocr
    src/buffer.c
    src/common.c
    src/file.c
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

# Testing

ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.8.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}/gtest
    )

SET(GTEST_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/gtest/include")
SET(GTEST_LIBRARY "${CMAKE_CURRENT_BINARY_DIR}/gtest/lib/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}")

add_executable(ocr_test
    test/main.cpp)
target_link_libraries(ocr_test ocr ${CMAKE_THREAD_LIBS_INIT})

add_dependencies(ocr_test googletest)
target_include_directories(ocr_test PRIVATE ${GTEST_INCLUDE_DIR})
target_link_libraries(ocr_test ocr)
target_link_libraries(ocr_test ${GTEST_LIBRARY})

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -V DEPENDS ocr_test)

enable_testing()
add_test(unit ocr_test)
