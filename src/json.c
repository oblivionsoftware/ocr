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

static size_t string_size(const char *text)
{
    assert(text[0] == '"');
    ++text;

    size_t size = 0;
    while (text && text[0] != '"') {
        ++size;
        ++text;
    }

    OCR_INFO("text size: %zu", size);

    assert(text[0] == '"');
    return size;
}

static const char *read_string(char *buffer, size_t buffer_size, const char *text)
{
    assert(text[0] == '"');
    ++text;

    size_t count = 0;
    while (text && text[0] != '"' && count < buffer_size) {
        char c = text[0];
        ++text;

        buffer[count++] = c;
    }

    buffer[count] = 0;

    OCR_INFO("read string: %s", buffer);

    if (text[0] != '"') {
        OCR_WARN("expected text to be quote, but was %c", text[0]);
    }

    ++text;
    return text;
}

static const char *parse_string(ocr_json_string_t *string, const char *text)
{
    size_t size = string_size(text);

    string->value = malloc(size + 1);
    text = read_string(string->value, size + 1, text);

    return text;
}

static const char *parse_number(ocr_json_number_t *number, const char *text)
{
    char *end;
    number->value = strtod(text, &end);

    return end;
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
        array->capacity = new_capacity;
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

    OCR_INFO("parsed array with %u elements", array->size);

    assert(text[0] == ']');

    ++text;
    return text;
}

static const char *parse_object(ocr_json_object_t *object, const char *text)
{
    assert(text[0] == '{');
    ++text;

    object->size = 0;

    text = skip_ws(text);

    while (text && text[0] != '}') {
        ocr_json_object_entry_t *entry = malloc(sizeof(ocr_json_object_entry_t));
        text = read_string(entry->name, sizeof(entry->name), text);
        OCR_INFO("property name: %s", entry->name);

        text = skip_ws(text);
        text = parse_element(&entry->value, text);

        if (object->entries == NULL) {
            object->entries = entry;
        } else {
            entry->next = object->entries;
            object->entries = entry;
        }
        ++object->size;
        OCR_INFO("added object property: %s", entry->name);

        text = skip_ws(text);
    }

    assert(text[0] == '}');

    ++text;
    return text;
}

static const char *parse_element(ocr_json_t *json, const char *text)
{
    assert(json);

    //OCR_INFO("parsing element from text: %s", text);

    char first = text[0];

    if (first == '"') {
        json->type = OCR_JSON_STRING;
        text = parse_string(&json->data.string, text);
    } else if (first == '+' || first == '-' || ('0' <= first && first <= '9')) {
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

    while (entry) {
        if (strcmp(key, entry->name) == 0) {
            return &entry->value;
        }

        entry = entry->next;
    }

    return NULL;
}

i32 ocr_json_get_int(ocr_json_t *json, const char *key)
{
    ocr_json_t *value = ocr_json_get(json, key);
    if (value == NULL) {
        return 0;
    }

    assert(value->type == OCR_JSON_NUMBER);

    return (int)value->data.number.value;
}

const char *ocr_json_get_string(ocr_json_t *json, const char *key)
{
    ocr_json_t *value = ocr_json_get(json, key);
    if (value == NULL) {
        return 0;
    }

    assert(value->type == OCR_JSON_STRING);

    return value->data.string.value;
}
