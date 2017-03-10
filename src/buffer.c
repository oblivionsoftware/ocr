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

#include "ocr/buffer.h"


ocr_buffer_t *ocr_buffer_create(ocr_pool_t *pool, size_t size)
{
    ocr_buffer_t *buffer = ocr_pool_alloc(pool, sizeof(*buffer) + size);
    buffer->size = size;

    return buffer;
}


