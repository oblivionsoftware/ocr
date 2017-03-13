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

#include "ocr/json.h"

#include "ocr/log.h"

static const char *skip_ws(const char *text)
{
    while (text[0] == ' ' || text[0] == '\t' || text[0] == '\r' || text[0] == '\n' || text[0] == ':' || text[0] == ',')
        ++text;

    return text;
}

ocr_json_t *ocr_json_parse(ocr_pool_t *pool, const char *text)
{
    text = skip_ws(text);
    if (text[0] == 0) {
        return NULL;
    }

    ocr_json_t *json = ocr_pool_alloc(pool, sizeof(ocr_json_t));

    char first = text[0];

    if (first == '\"') {
        json->type = OCR_JSON_STRING;
        OCR_INFO("parsing string");
    } else if ('0' <= first && first < '9') {
        json->type = OCR_JSON_NUMBER;
        OCR_INFO("parsing number");
    } else if (first == '[') {
        json->type = OCR_JSON_ARRAY;
        OCR_INFO("parsing array");
    } else if (first == '{') {
        json->type = OCR_JSON_OBJECT;
        OCR_INFO("parsing object");
    } else {
        OCR_PANIC("unrecognized input: %c", first);
    }

    return json;
}
