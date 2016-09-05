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

#include <stdlib.h>

typedef struct ocr_pool ocr_pool_t;

struct ocr_pool {
    void *(*alloc)(ocr_pool_t *pool, size_t size);
    void (*free)(ocr_pool_t *pool, void *memory);
};

/**
 * Allocates memory using the specified pool.
 *
 * @param pool The pool to use to allocate.
 * @param size The amount of memory to allocate.
 * @return The allocated memory, NULL if the pool was unable to allocate.
 */
void *ocr_alloc(ocr_pool_t *pool, size_t size);

/**
 * Frees memory allocated from the pool.
 *
 * @param pool The pool used to allocate the memory.
 * @param memory The memory to free.
 */
void ocr_free(ocr_pool_t *pool, void *memory);
