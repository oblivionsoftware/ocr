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

#include <string.h>

#include "ocr/log.h"

static const char *parse_element(ocr_json_t *json, const char *text);

static bool is_ws(char c)
{
    return c == ' '
           || c == '\t'
           || c == '\r'
           || c == '\n'
           || c == ':'
           || c == ',';
}

static const char *skip_ws(const char *text)
{
    while (is_ws(text[0])) {
        ++text;
    }

    return text;
}

static const char *skip_until_ws(const char *text) {
    while (text && !is_ws(text[0])) {
        ++text;
    }

    return text;
}

static const char *parse_string(ocr_json_string_t *string, const char *text)
{
    assert(string);
    return text;
}

static const char *parse_number(ocr_json_number_t *number, const char *text)
{
    number->value = atof(text);
    return skip_until_ws(text);
}

const char *parse_boolean(ocr_json_boolean_t *boolean, const char *text)
{
    if (strncmp("true", text, 4) == 0) {
        boolean->value = true;
        text += 4;
    } else if (strncmp("false", text, 5) == 0) {
        boolean->value = false;
        text += 5;
    } else {
        OCR_PANIC("unable to parse boolean from text: %s", text);
    }

    return text;
}

static void ensure_capacity(ocr_json_array_t *array, u32 capacity)
{
    if (array->capacity < capacity) {
        u32 new_capacity = array->capacity * 2;
        if (new_capacity < capacity) {
            new_capacity = capacity;
        }

        array->items = realloc(array->items, new_capacity * sizeof(ocr_json_t));
    }
}

static const char *parse_array(ocr_json_array_t *array, const char *text)
{
    assert(text[0] == '[');
    ++text;

    array->size = 0;
    array->capacity = 0;
    array->items = NULL;

    text = skip_ws(text);

    while (text && text[0] != ']') {
        ensure_capacity(array, array->size + 1);

        text = parse_element(&array->items[array->size], text);
        ++array->size;

        text = skip_ws(text);
    }

    assert(text[0] == ']');

    ++text;
    return text;
}

static const char *parse_object(ocr_json_object_t *object, const char *text)
{
    assert(text[0] == '{');
    ++text;

    object->size = 0;

    while (text && text[0] != '}') {
        ++text;
    }

    assert(text[0] == '}');

    ++text;
    return text;
}

static const char *parse_element(ocr_json_t *json, const char *text)
{
    assert(json);

    OCR_INFO("parsing element from text: %s", text);

    char first = text[0];

    if (first == '\"') {
        json->type = OCR_JSON_STRING;
        text = parse_string(&json->data.string, text);
    } else if (first == '+' || first == '-' || ('0' <= first && first < '9')) {
        json->type = OCR_JSON_NUMBER;
        text = parse_number(&json->data.number, text);
    } else if (first == '[') {
        json->type = OCR_JSON_ARRAY;
        text = parse_array(&json->data.array, text);
    } else if (first == '{') {
        json->type = OCR_JSON_OBJECT;
        text = parse_object(&json->data.object, text);
    } else if (strncmp("true", text, 4) == 0) {
        json->type = OCR_JSON_BOOLEAN;
        text = parse_boolean(&json->data.boolean, text);
    } else if (strncmp("false", text, 5) == 0) {
        json->type = OCR_JSON_BOOLEAN;
        text = parse_boolean(&json->data.boolean, text);
    } else if (strncmp("null", text, 4) == 0) {
        json->type = OCR_JSON_NULL;
        text += 4;
    } else {
        OCR_PANIC("unrecognized input: %c", first);
    }

    return text;
}

ocr_json_t *ocr_json_parse(ocr_pool_t *pool, const char *text)
{
    text = skip_ws(text);
    if (text[0] == 0) {
        return NULL;
    }

    ocr_json_t *json = ocr_pool_alloc(pool, sizeof(ocr_json_t));
    parse_element(json, text);

    return json;
}

ocr_json_t *ocr_json_get(ocr_json_t *json, const char *key)
{
    assert(json->type == OCR_JSON_OBJECT);

    ocr_json_object_entry_t *entry = json->data.object.entries;

    for (u32 i = 0; i < json->data.object.size; ++i) {
        if (strcmp(key, entry->name) == 0) {
            return &entry->value;
        }
    }

    return NULL;
}
