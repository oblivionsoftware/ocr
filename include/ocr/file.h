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
#include "ocr/buffer.h"

OCR_EXTERN_C_BEGIN

/**
 * Reads the entire contents of a file into a buffer.
 *
 * @param path The path to the file.
 * @param pool The pool to use to allocate the buffer.
 * @param buffer The output buffer.
 * @return OK on success, IO_ERROR on failure.
 */
ocr_status_t ocr_read_file(const char *path, ocr_pool_t *pool, ocr_buffer_t *buffer);

OCR_EXTERN_C_END
