#define CATCH_CONFIG_MAIN
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
    if (pool) {
        ocr_buffer_t *buffer;
        if (OCR_SUCCEEDED(ocr_read_file(__FILE__, pool, &buffer))) {
            OCR_INFO("Read Buffer: %s", buffer->data);
        }

        void *dataValue = buffer->data;

        ocr_pool_clear(pool);

        if (OCR_SUCCEEDED(ocr_read_file(__FILE__, pool, &buffer))) {
            OCR_INFO("Read Buffer: %s", buffer->data);
        }

        REQUIRE(dataValue == buffer->data);

        ocr_pool_destroy(pool);
    }
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
