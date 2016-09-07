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

#include "ocr/pool.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "ocr/log.h"


ocr_status_t ocr_pool_create(size_t size, ocr_pool_t *parent, ocr_pool_t **pool_out)
{
    OCR_INFO("allocating pool of size: %zu", size);

    size_t pool_size = sizeof(ocr_pool_t) + size;

    ocr_pool_t *pool;
    if (parent) {
        pool = ocr_alloc(parent, pool_size);
    } else {
        pool = malloc(pool_size);
    }

    if (!pool) {
        OCR_ERROR("pool allocation failed, size: %zu", size);
        return OCR_NO_MEMORY;
    }

    pool->size = size;
    pool->offset = 0;
    pool->parent = parent;

    *pool_out = pool;

    return OCR_OK;
}


void ocr_pool_destroy(ocr_pool_t *pool)
{
    OCR_INFO("destroying pool");

    if (pool->parent == NULL) {
        free(pool);
    }
}


void *ocr_alloc(ocr_pool_t *pool, size_t size)
{
    assert(pool->offset + size < pool->size);

    void *result = pool->memory + pool->offset;
    pool->offset += size;

    return result;
}


void ocr_pool_clear(ocr_pool_t *pool)
{
    pool->offset = 0;
}
