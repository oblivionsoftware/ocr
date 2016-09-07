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

#include "ocr/common.h"


const char *ocr_strerror(ocr_status_t status)
{
    switch (status) {
    case OCR_OK: return "ok";
    case OCR_NO_MEMORY: return "out of memory";
    case OCR_IO_ERROR: return "i/o error";
    case OCR_GENERAL_ERROR: return "general error";
    default: return "unknown error";
    }
}
