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

typedef enum {
    OCR_JSON_NUMBER,
    OCR_JSON_STRING,
    OCR_JSON_ARRAY,
    OCR_JSON_OBJECT
} ocr_json_type_t;

struct ocr_json;
struct ocr_json_object_entry;

typedef struct {
    r64 value;
} ocr_json_number_t;

typedef struct {
    const char *value;
} ocr_json_string_t;

typedef struct {
    struct ocr_json *items;
    u32 item_count;
} ocr_json_array_t;

typedef struct {
    struct ocr_json_object_entry *entries;
    u32 entry_count;
} ocr_json_object_t;

typedef struct {
    ocr_json_type_t type;
    union {
        ocr_json_number_t number;
        ocr_json_string_t string;
        ocr_json_array_t array;
        ocr_json_object_t object;
    };
} ocr_json_t;

typedef struct ocr_json_object_entry {
    char name[256];
    ocr_json_t value;
} ocr_json_object_entry_t;

ocr_json_t *ocr_json_parse(ocr_pool_t *pool, const char *text);

OCR_EXTERN_C_END
