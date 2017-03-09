#include "catch.hpp"

#include <ocr/ocr.h>

using namespace ocr;

TEST_CASE("pools can be created and destroyed", "[pool]")
{
    Pool pool{mb(32)};
}

TEST_CASE("read entire file uses a pool correctly", "[file]")
{
    Pool pool{mb(32)};

    Buffer buffer = read_file(__FILE__, pool);
    OCR_INFO("Read Buffer: %s", buffer.data);

    void *dataValue = buffer.data;

    pool.clear();

    buffer = read_file(__FILE__, pool);
    OCR_INFO("Read Buffer: %s", buffer.data);

    REQUIRE(dataValue == buffer.data);
}

TEST_CASE("all log levels compile", "[log]")
{
    OCR_TRACE("Test Message");
    OCR_DEBUG("Test Message");
    OCR_INFO("Test Message");
    OCR_WARN("Test Message");
    OCR_ERROR("Test Message");
}

TEST_CASE("event loop can run", "[event]")
{
    EventLoop loop;
    loop.run();
}

TEST_CASE("window runs", "[window]")
{
    WindowSettings settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "OCR Window";

    ocr::Window window {settings};
    GlContext gl_context {window};

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    while (!window.isClosed()) {
        window.doEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        gl_context.present();
    }
}
