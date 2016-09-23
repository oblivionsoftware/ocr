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

#include <cstdint>

#define TACHYON_PLATFORM_WINDOWS 0
#define TACHYON_PLATFORM_LINUX 1
#define TACHYON_PLATFORM_MAC 2

#ifdef _WIN32
    #define TACHYON_PLATFORM TACHYON_PLATFORM_WINDOWS
    #define TACHYON_PLATFORM_NAME "Windows"
#elif __APPLE__
    #define TACHYON_PLATFORM TACHYON_PLATFORM_MAC
    #define TACHYON_PLATFORM_NAME "Mac OSX"
#elif __linux__
    #define TACHYON_PLATFOMR TACHYON_PLATFORM_LINUX
    #define TACHYON_PLATFORM_NAME "Linux"
#else
    #error "Unsupported Platform"
#endif

namespace tachyon {

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
typedef float r32;
typedef double r64;

}

#define OCR_PLATFORM_WINDOWS 0
#define OCR_PLATFORM_LINUX 1
#define OCR_PLATFORM_MAC 2

#ifdef _WIN32
#define OCR_PLATFORM OCR_PLATFORM_WINDOWS
#define OCR_PLATFORM_NAME "Windows"
#elif __APPLE__
#define OCR_PLATFORM OCR_PLATFORM_MAC
#define OCR_PLATFORM_NAME "Mac OSX"
#elif __linux__
#define OCR_PLATFOMR OCR_PLATFORM_LINUX
#define OCR_PLATFORM_NAME "Linux"
#else
#error "Unsupported Platform"
#endif
