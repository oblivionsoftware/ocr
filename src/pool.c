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

#include <ocr/pool.h>

#include <assert.h>
#include <stdio.h>

ocr_pool_t *ocr_pool_create(size_t size)
{
    ocr_pool_t *pool = malloc(sizeof(ocr_pool_t) + size);
    if (!pool) {
        fprintf(stderr, "pool allocation failed of size: %zu\n", size);
        abort();
    }

    pool->size = size;
    pool->offset = 0;

    return pool;
}

void ocr_pool_destroy(ocr_pool_t *pool)
{
    free(pool);
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
