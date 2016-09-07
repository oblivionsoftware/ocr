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
#include "ocr/pool.h"

OCR_EXTERN_C_BEGIN

typedef struct ocr_event_loop ocr_event_loop_t;

ocr_status_t ocr_event_loop_create(ocr_pool_t *pool, ocr_event_loop_t **loop_out);

void ocr_event_loop_destroy(ocr_event_loop_t *loop);

void ocr_event_loop_run(ocr_event_loop_t *loop);

OCR_EXTERN_C_END
