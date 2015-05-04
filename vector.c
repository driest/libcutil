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

#include <string.h>
#include <strings.h>

#include "vector.h"

static const uint8_t growth_factor = 2;

// Create a new vector and allocate enough memory to hold an initial
// amount of data members.
//
// Args:
//  vec: pointer to the vector to initialize
//  item_size: size of an individual item in the vector
//  capacity: number of items to allocate initially
ERROR vector_init(VECTOR *vec, size_t item_size, size_t capacity) {
  vec->item_size = item_size;
  vec->used_bytes = 0;
  vec->total_bytes = capacity * item_size;
  vec->init_bytes = vec->total_bytes;
  vec->data = calloc(capacity, item_size);
  if (vec->data) {
    return SUCCESS;
  }
  return FAILURE;
}

// release all memory the vector holds.
void vector_destroy(VECTOR *vec) {
  free(vec->data);
}

// try to grow the vector using the growth_factor (2 by default).
static ERROR vector_grow(VECTOR *vec) {
  uint8_t *new_data = realloc(vec->data, vec->total_bytes * growth_factor);
  if (new_data == NULL) {
    return FAILURE;
  }
  vec->data = new_data;
  vec->total_bytes *= growth_factor;
  return SUCCESS;
}

// try to shrink the vector using the growth_factor (2 by default).
static ERROR vector_shrink(VECTOR *vec) {
  void *new_data = realloc(vec->data, vec->total_bytes / growth_factor);
  if (new_data == NULL) {
    return FAILURE;
  }
  vec->data = new_data;
  vec->total_bytes /= growth_factor;
  return SUCCESS;
}

// Add a new element to the vector by copying it into the next free slot.
// If the vector is full it will automatically grow by its growth_factor.
//
// NOTE: while we could get the size directly from the vector we prefer
// you supply it here. The supplied size must equal the vectors item_size.
// This allows us to catch bugs where you try to push an object of the wrong
// type to the vector. Of course this doesn't work all the time, for example
// it won't catch you pushing something of equal size but wrong type.
void *vector_push(VECTOR *vec, const void * const value, size_t size) {
  void *element = vector_push_new(vec, size);
  if (!element) {
    return NULL;
  }
  memcpy(element, value, vec->item_size);
  return element;
}

// Allocate a new element in the vector and return a pointer to it.
void *vector_push_new(VECTOR *vec, const size_t size) {
  if (size != vec->item_size) {
    return NULL;
  }
  if (vec->used_bytes >= vec->total_bytes) {
    if (vector_grow(vec) != SUCCESS) {
      return NULL;
    }
  }
  void *element = vec->data + vec->used_bytes;
  vec->used_bytes += vec->item_size;
  return element;
}

// Calculates a pointer to the desired element in the underlying array.
inline void *vector_ptr(VECTOR *vec, size_t index) {
  return (void *)(vec->data + index * vec->item_size);
}

// Get a pointer to an element in the vector. Implements bounds checking,
// so the pointer is guaranteed to be valid or NULL in case of an invalid index.
inline void *vector_get(VECTOR *vec, size_t index) {
  size_t offset = index * vec->item_size;
  if (offset >= vec->total_bytes) {
    return NULL;
  }
  return (void *)(vec->data + offset);
}

// Shortcut to delete the last element.
inline ERROR vector_pop(VECTOR *vec) {
  return vector_del(vec, vec->used_bytes / vec->item_size);
}

// Shortcut to delete the last element and return a copy.
inline void *vector_pop_copy(VECTOR *vec) {
  return vector_del_copy(vec, vec->used_bytes / vec->item_size);
}

// Remove an element from the vector by deleting its data.
// NOTE: This is O(n) for all elements except the last one which is O(1).
ERROR vector_del(VECTOR *vec, size_t index) {
  const void * const element = vector_get(vec, index);
  if (!element) {
    return FAILURE;
  }
  vec->used_bytes -= vec->item_size;
  // Check if we can shrink the vector.
  if (vec->total_bytes > vec->init_bytes &&
      vec->used_bytes <= (vec->total_bytes / growth_factor)) {
    vector_shrink(vec);
  }
  return SUCCESS;
}

// Remove an element from the vector and return a copy.
void *vector_del_copy(VECTOR *vec, size_t index) {
  void *copy = malloc(vec->item_size);
  void *element = vector_get(vec, index);
  if (!(element && copy)) {
    return NULL;
  }
  memcpy(copy, element, vec->item_size);
  vector_del(vec, index);
  return copy;
}
