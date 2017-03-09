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

namespace ocr {

class Pool {
public:

    explicit Pool(size_t size, Pool *parent = nullptr);

    ~Pool();

    void clear();

    void *allocRaw(size_t size);

    template <typename T>
    T *alloc()
    {
        return reinterpret_cast<T*>(allocRaw(sizeof(T)));
    }

    template <typename T>
    T *allocExtra(size_t extra)
    {
        return reinterpret_cast<T*>(allocRaw(sizeof(T) + extra));
    }

private:

    size_t _size;
    size_t _offset;
    Pool *_parent;
    u8 *_memory;
};

}
