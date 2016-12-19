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

#include <ocr/ocr.h>

namespace tachyon {

using vec2 = ocr_vec2_t;
using vec3 = ocr_vec3_t;
using vec4 = ocr_vec4_t;
using mat4 = ocr_mat4_t;

struct rect {

    rect() = default;

    rect(r32 left, r32 right, r32 top, r32 bottom) {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
    }


    r32 left;
    r32 right;
    r32 top;
    r32 bottom;
};

}

