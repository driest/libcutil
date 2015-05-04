// A dynamically growing array. It will double it's size when it hits it's
// internal capacity. Note data is actually stored in the array,
// so when you put() something it will be copied into the internal storage.
// This makes it easy to release memory for a group of iteams by calling
// the cleanup() function, it also avoids many small memory allocations.
// If you wan't to keep the elements stored somewhere else simply make a
// vector of pointers.
//
// NOTE: Be aware that you should never mutate the vector while keeping
// pointers to elements, as the underlying storage can move without you
// noticing due to realloc calls.
//
// Author: Johannes Stüttgen (johannes.stuettgen@gmail.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CUTIL_VECTOR_H
#define CUTIL_VECTOR_H

#include "stdlib.h"

#include "types.h"

typedef struct VECTOR_ {
  uint8_t *data;
  size_t used_bytes;
  size_t item_size;
  size_t total_bytes;
  size_t init_bytes;
} VECTOR;

static const size_t VECTOR_DEFAULT_SIZE = 16;

ERROR vector_init(VECTOR *vec, size_t item_size, size_t capacity);
void vector_destroy(VECTOR *vec);
void *vector_push(VECTOR *vec, const void * const value, size_t size);
void *vector_push_new(VECTOR *vec, size_t size);
void *vector_ptr(VECTOR *vec, size_t index);
void *vector_get(VECTOR *vec, size_t index);
ERROR vector_pop(VECTOR *vec);
void *vector_pop_copy(VECTOR *vec);

ERROR vector_del(VECTOR *vec, size_t index);
void *vector_del_copy(VECTOR *vec, size_t index);

#endif  // CUTIL_VECTOR_H
