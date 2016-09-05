#include <ocr/ocr.h>

int main(int argc, char **argv)
{
    ocr_pool_t *pool = ocr_pool_create(4096);
    ocr_pool_destroy(pool);

    return 0;
}
