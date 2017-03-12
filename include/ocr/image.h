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

#pragma once

#include "ocr/common.h"
#include "ocr/pool.h"

OCR_EXTERN_C_BEGIN

typedef struct {
    u32 width;
    u32 height;
    u8 pixels[1];
} ocr_image_t;

ocr_image_t *ocr_image_create(ocr_pool_t *pool, u32 width, u32 height);

ocr_image_t *ocr_image_load(ocr_pool_t *pool, const char *path);

void ocr_image_fill(ocr_image_t *image, u8 r, u8 g, u8 b, u8 a);

OCR_EXTERN_C_END
