cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(tachyon VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${tachyon_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)


set(TACHYON_SOURCES
    src/glew.c
    src/core/context.cpp
    src/core/exception.cpp
    src/core/string.cpp
    src/platform/sdl_platform.cpp
    src/renderer/gl_buffer.cpp
    src/renderer/gl_program.cpp
    src/renderer/gl_renderer.cpp
    src/renderer/gl_shader.cpp
    src/renderer/gl_vertex_array.cpp
    src/renderer/image.cpp
)

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
    $<BUILD_INTERFACE:${SDL2_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${OPENGL_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

target_link_libraries(tachyon
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARY})

if (${CMAKE_COMPILER_IS_GNUCXX})
    set_source_files_properties(src/renderer/image.cpp PROPERTIES COMPILE_FLAGS -Wno-misleading-indentation)
endif()

if ((${CMAKE_COMPILER_IS_GNUCXX}) OR (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    target_compile_options(tachyon PRIVATE
        -Wall -Werror -pedantic
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++14>)
else()
    target_compile_options(tachyon PRIVATE
        /W3 /WX /GR-)
    target_compile_definitions(tachyon PRIVATE
        -D_CRT_SECURE_NO_WARNINGS -DGLEW_STATIC)
endif()

install(TARGETS tachyon EXPORT TachyonConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT TachyonConfig DESTINATION share/tachyon/cmake)
export(TARGETS tachyon FILE TachyonConfig.cmake)
