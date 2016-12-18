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
    size_t size;
    u8 data[1];
} ocr_buffer_t;

ocr_buffer_t *ocr_buffer_create(ocr_pool_t *pool, size_t size);

void ocr_buffer_destroy(ocr_buffer_t *buffer);

OCR_EXTERN_C_END
