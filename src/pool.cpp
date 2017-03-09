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

namespace ocr {

Pool::Pool(size_t size, Pool *parent) {
    OCR_INFO("allocating pool of size: %zu", size);

    if (parent) {
        _memory = reinterpret_cast<u8*>(parent->allocRaw(size));
    } else {
        _memory = reinterpret_cast<u8*>(malloc(size));
    }

    if (!_memory) {
        OCR_ERROR("pool allocation failed, size: %zu", size);
        //return NULL;
    }

    _size = size;
    _offset = 0;
    _parent = parent;
}

Pool::~Pool() {
    OCR_INFO("destroying pool");

    if (_parent == NULL) {
        free(_memory);
    }
}

void *Pool::allocRaw(size_t size) {
    assert(_offset + size < _size);

    void *result = _memory + _offset;
    _offset += size;

    return result;
}


void Pool::clear() {
    _offset = 0;
}

}
