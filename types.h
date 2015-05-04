// Standard types for integers and error checking.
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

#ifndef CUTIL_TYPES_H
#define CUTIL_TYPES_H

#include "stdbool.h"
#include "stdint.h"
#include <sys/types.h>

// Macro to determine the number of elements in an array at compile time.
// This does not work on pointers, as the compiler has no way of knowing
// what they point to.

// Adapted from Jorg Brown's implementation in the Chromium source.
// The following notes are from the Chromium documentation on the macro:
//
// ARRAYSIZE(arr) works by inspecting sizeof(arr) (the # of bytes in
// the array) and sizeof(*(arr)) (the # of bytes in one array
// element).  If the former is divisible by the latter, perhaps arr is
// indeed an array, in which case the division result is the # of
// elements in the array.  Otherwise, arr cannot possibly be an array,
// and we generate a compiler error to prevent the code from
// compiling.
//
// Since the size of bool is implementation-defined, we need to cast
// !(sizeof(a) & sizeof(*(a))) to size_t in order to ensure the final
// result has type size_t.
//
// This macro is not perfect as it wrongfully accepts certain
// pointers, namely where the pointer size is divisible by the pointee
// size.  Since all our code has to go through a 32-bit compiler,
// where a pointer is 4 bytes, this means all pointers to a type whose
// size is 3 or greater than 4 will be (righteously) rejected.
//
#define ARRAYSIZE(x) \
  ((sizeof(x) / sizeof(*(x))) / \
  (size_t)(!(sizeof(x) % sizeof(*(x)))))

// Standard error type for function return value checking.
typedef enum ERROR_ {
  SUCCESS = 0,
  FAILURE
} ERROR;

#endif  // CUTIL_TYPES_H
