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

#include "ocr/file.h"

#include <stdio.h>

#include "ocr/log.h"

ocr_status_t ocr_read_file(const char *path, ocr_pool_t *pool, ocr_buffer_t *buffer)
{
    FILE *file = fopen(path, "rb");
    if (!file) {
        OCR_ERROR("unable to open file: %s", path);
        return OCR_IO_ERROR;
    }

    fseek(file, 0, SEEK_END);
    u64 size = ftell(file);
    fseek(file, 0, SEEK_SET);
    OCR_INFO("file size: %zu", size);

    ocr_buffer_init(buffer, pool, size + 1);
    if (fread(buffer->data, size, 1, file) != 1) {
        fclose(file);
        return OCR_IO_ERROR;
    }

    buffer->data[size] = '\0';
    fclose(file);

    return OCR_OK;
}
