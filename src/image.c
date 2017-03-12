/*
 * Copyright 2016 Jeff Upton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ocr/image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ocr/log.h"

static size_t image_size(u32 width, u32 height)
{
    return sizeof(ocr_image_t) + (width * height * 4);
}

ocr_image_t *ocr_image_create(ocr_pool_t *pool, u32 width, u32 height)
{
    ocr_image_t *image = ocr_pool_alloc(pool, image_size(width, height));
    image->width = width;
    image->height = height;

    return image;
}


ocr_image_t *ocr_image_load(ocr_pool_t *pool, const char *path)
{
    int width;
    int height;
    int channels;
    u8 *pixels = stbi_load(path, &width, &height, &channels, 4);

    if (!pixels) {
        OCR_ERROR("error loading image: %s", path);
        return NULL;
    }

    OCR_INFO("loaded image '%s' - %d x %d", path, width, height);

    ocr_image_t *image = ocr_pool_alloc(pool, image_size(width, height));
    image->width = (u32)width;
    image->height = (u32)height;
    memcpy(image->pixels, pixels, width * height * 4);

    free(pixels);

    return image;
}

void ocr_image_fill(ocr_image_t *image, u8 r, u8 g, u8 b, u8 a)
{
    u8 *pixel = image->pixels;

    for (u32 y = 0; y < image->height; ++y) {
        for (u32 x = 0; x < image->width; ++x) {
            pixel[0] = r;
            pixel[1] = g;
            pixel[2] = b;
            pixel[3] = a;

            pixel += 4;
        }
    }
}
