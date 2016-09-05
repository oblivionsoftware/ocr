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

#include <ocr/log.h>

#include <stdarg.h>
#include <stdio.h>

static const char *ocr_strlevel(ocr_log_level_t level)
{
    switch (level) {
    case OCR_LOG_TRACE: return "TRACE";
    case OCR_LOG_DEBUG: return "DEBUG";
    case OCR_LOG_INFO: return "INFO";
    case OCR_LOG_WARN: return "WARN";
    case OCR_LOG_ERROR: return "ERROR";
    default: return "INVALID";
    }
}

void ocr_log(ocr_log_level_t level, const char *file, int line, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);

    va_end(args);

    printf("%s - %s:%d - %s\n", ocr_strlevel(level), file, line, buffer);
}
