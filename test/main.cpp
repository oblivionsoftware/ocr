#include <gtest/gtest.h>

#include <ocr/ocr.h>

TEST(Pool, Creation)
{
    ocr_pool_t *pool;
    ASSERT_EQ(OCR_OK, ocr_pool_create(ocr_mb(32), NULL, &pool));
    ocr_pool_destroy(pool);
}

TEST(File, ReadFile)
{
    ocr_pool_t *pool;
    if (OCR_SUCCEEDED(ocr_pool_create(ocr_mb(32), NULL, &pool))) {
        ocr_buffer_t buffer;
        if (OCR_SUCCEEDED(ocr_read_file(__FILE__, pool, &buffer))) {
            OCR_INFO("Read Buffer: %s", buffer.data);
        }

        void *dataValue = buffer.data;

        ocr_pool_clear(pool);

        if (OCR_SUCCEEDED(ocr_read_file(__FILE__, pool, &buffer))) {
            OCR_INFO("Read Buffer: %s", buffer.data);
        }

        ASSERT_EQ(dataValue, buffer.data);

        ocr_pool_destroy(pool);
    }
}

TEST(General, LogLevels)
{
    OCR_TRACE("Test Message");
    OCR_DEBUG("Test Message");
    OCR_INFO("Test Message");
    OCR_WARN("Test Message");
    OCR_ERROR("Test Message");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
