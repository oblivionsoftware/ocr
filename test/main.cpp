#include <ocr/ocr.h>

int main(int argc, char **argv)
{
    ocr_pool_t *pool;
    if (OCR_SUCCEEDED(ocr_pool_create(4096, &pool))) {
        ocr_pool_destroy(pool);
    }

    OCR_TRACE("Test Message");
    OCR_DEBUG("Test Message");
    OCR_INFO("Test Message");
    OCR_WARN("Test Message");
    OCR_ERROR("Test Message");

    return 0;
}
