#include "catch.hpp"

#include <ocr/ocr.h>

TEST_CASE("pools can be created and destroyed", "[pool]")
{
    ocr_pool_t *pool = ocr_pool_create(ocr_mb(32), NULL);
    REQUIRE(pool);
    ocr_pool_destroy(pool);
}

TEST_CASE("read entire file uses a pool correctly", "[file]")
{
    ocr_pool_t *pool = ocr_pool_create(ocr_mb(32), NULL);
    REQUIRE(pool);
    ocr_buffer_t *buffer;
    if (OCR_SUCCEEDED(ocr_read_file(pool, __FILE__, &buffer))) {
        OCR_INFO("Read Buffer: %s", buffer->data);
    }

    void *dataValue = buffer->data;

    ocr_pool_clear(pool);

    if (OCR_SUCCEEDED(ocr_read_file(pool, __FILE__, &buffer))) {
        OCR_INFO("Read Buffer: %s", buffer->data);
    }

    REQUIRE(dataValue == buffer->data);

    ocr_pool_destroy(pool);
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
    ocr_pool_t *pool = ocr_pool_create(ocr_mb(32), NULL);
    REQUIRE(pool);

    ocr_event_loop_t *loop = ocr_event_loop_create(pool);
    REQUIRE(loop);
    ocr_event_loop_run(loop);
    ocr_event_loop_destroy(loop);

    ocr_pool_destroy(pool);
}

TEST_CASE("window runs", "[window]")
{
    ocr_pool_t *pool = ocr_pool_create(ocr_mb(32), NULL);

    ocr_window_settings_t settings;
    settings.width = 1280;
    settings.height = 720;
    settings.title = "OCR Window";

    ocr_window_t *window = ocr_window_create(pool, &settings);
    ocr_gl_context_t *gl_context = ocr_gl_context_create(pool, window);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    while (!ocr_window_is_closed(window)) {
        ocr_window_do_events(window);

        glClear(GL_COLOR_BUFFER_BIT);

        ocr_gl_context_present(gl_context);
    }

    ocr_gl_context_destroy(gl_context);
    ocr_window_destroy(window);
    ocr_pool_destroy(pool);
}

TEST_CASE("image loads", "[image]")
{
    ocr_pool *pool = ocr_pool_create(ocr_mb(32), NULL);

    ocr_image_t *image = ocr_image_create(pool, 64, 128);
    REQUIRE(image);
    REQUIRE(image->width == 64);
    REQUIRE(image->height == 128);

    ocr_image_fill(image, 2, 4, 8, 16);
    u8 *pixel = image->pixels;
    for (u32 i = 0; i < (image->width * image->height); ++i) {
        REQUIRE(pixel[0] == 2);
        REQUIRE(pixel[1] == 4);
        REQUIRE(pixel[2] == 8);
        REQUIRE(pixel[3] == 16);

        pixel += 4;
    }

    OCR_INFO("image dimensions: %u x %u", image->width, image->height);

    ocr_pool_destroy(pool);
}

TEST_CASE("json parses", "[json]")
{
    ocr_pool *pool = ocr_pool_create(ocr_mb(32), NULL);

    ocr_json_t *json = ocr_json_parse(pool, "1234");
    REQUIRE(json);
    REQUIRE(json->type == OCR_JSON_NUMBER);
    REQUIRE((u32)json->data.number.value == 1234);

    ocr_pool_destroy(pool);
}
