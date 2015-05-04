// Macros and functions for memory allocation.
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

#ifndef _LIBCUTIL_ALLOC_H_
#define _LIBCUTIL_ALLOC_H_

#include "types.h"
#include "stdlib.h"

// Destructors get a pointer to the object and return nothing.
typedef void (*destructor)(void *obj);

// Frees memory obtained with malloc.
void destroy_heap_object(void *obj);

// Use this to automatically free memory when the pointer goes out of scope.
// NOTE: use this only for pointers obtained through malloc. Never copy
// a pointer with this attribute, as you can end up with a dangling pointer.
// To be safe you should always make these pointers const, to avoid mutation.
#define LOCAL __attribute__(cleanup(destroy_heap_object))

// Sets a custom destructor to be called when the variable goes out of scope.
#define LOCAL_DESTRUCTOR(x) __attribute__(cleanup(x))

#endif  // _LIBCUTIL_ALLOC_H_
