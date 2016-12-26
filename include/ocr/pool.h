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

OCR_EXTERN_C_BEGIN

typedef struct ocr_pool ocr_pool_t;

struct ocr_pool {
    size_t size;
    size_t offset;
    ocr_pool_t *parent;
    u8 memory[1];
};

/**
 * Creates a new pool.
 *
 * @param size The size of the pool.
 * @param parent The parent pool.
 * @return the newly created pool.
 */
ocr_pool_t *ocr_pool_create(size_t size, ocr_pool_t *parent);

/**
 * Destroys the specified pool.
 *
 * @param pool The pool to destroy.
 */
void ocr_pool_destroy(ocr_pool_t *pool);

/**
 * Allocates memory using the specified pool.
 *
 * @param pool The pool to use to allocate.
 * @param size The amount of memory to allocate.
 * @return The allocated memory, NULL if the pool was unable to allocate.
 */
void *ocr_pool_alloc(ocr_pool_t *pool, size_t size);

/**
 * Clears the allocated memory in the pool.
 *
 * @param pool The pool to clear.
 */
void ocr_pool_clear(ocr_pool_t *pool);

OCR_EXTERN_C_END
