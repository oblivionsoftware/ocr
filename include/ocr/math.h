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

typedef struct {
    r32 x;
    r32 y;
} Vec2;

typedef struct {
    r32 x;
    r32 y;
    r32 z;
} Vec3;

typedef struct {
    r32 x;
    r32 y;
    r32 z;
    r32 w;
} Vec4;

typedef struct {
    r32 m[16];
} Mat4;

OCR_INLINE Vec4 vec4(r32 x, r32 y, r32 z, r32 w) {
    Vec4 result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;

    return result;
}

OCR_INLINE Mat4 mat4_ortho(r32 left, r32 right, r32 bottom, r32 top, r32 z_near, r32 z_far) {
    r32 tx = -(right + left) / (right - left);
    r32 ty = -(top + bottom) / (top - bottom);
    r32 tz = -(z_far + z_near) / (z_far - z_near);

    Mat4 result = {{0}};
    result.m[0] = 2 / (right - left);
    result.m[12] = tx;
    result.m[5] = 2 / (top - bottom);
    result.m[13] = ty;
    result.m[10] = -2 / (z_far - z_near);
    result.m[14] = tz;
    result.m[15] = 1.0f;

    return result;
}

}

