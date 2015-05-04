// Enables Ressource Allocation Is Initialization (RAII) style
// resource management for heap memory and files.
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

#ifndef CUTIL_RAII_H
#define CUTIL_RAII_H

#include <stdio.h>
#include <stdlib.h>
#include <types.h>
#include <unistd.h>

// Automatically free memory when the pointer goes out of scope.
// NOTE: use this only for pointers obtained through malloc. Never copy
// a pointer with this attribute, as you can end up with a dangling pointer.
// To be safe you should always make these pointers const, to avoid mutation.
#define LOCAL __attribute__((cleanup(destroy_heap_object)))

// Automatically close files opened with the fopen() function,
// when the associated 'FILE *fp' goes out of scope.
// NOTE: Never use this on anything else or behaviour is undefined.
#define LOCAL_FP __attribute__((cleanup(close_fp)))

// Automatically close files opened with the open() function,
// when the associated 'int fd' goes out of scope.
// NOTE: Never use this on anything else or behaviour is undefined.
#define LOCAL_FD __attribute__((cleanup(close_fd)))

// Sets a custom destructor to be called when the variable goes out of scope.
#define LOCAL_DESTRUCTOR(x) __attribute__((cleanup(x)))

// Destructors get a pointer to the object and return nothing.
typedef void (*destructor)(uintptr_t *pobj);

// This is the standard error return value for open()
static const int invalid_fileno = -1;

// Frees memory obtained with malloc. You need to pass the object pointer
// by reference to allow the function to set it to NULL after freeing it.
//
// NOTE: Only use this on memory obtained with malloc,
// otherwise behaviour is undefined!
static inline void destroy_heap_object(void *pobj) {
  // cleanup must accept any pointer and 'void **' will not match this.
  // However, we still get a pointer to a pointer, so it's safe to dereference
  // this.
  void **ppobj = (void **)pobj;

  if (*ppobj) {
    free(*ppobj);
    *ppobj = NULL;
  }
}

// Close a file opened by fopen() and set the 'FILE *fp' to NULL.
static inline void close_fp(FILE **pfile) {
  if (*pfile) {
    fclose(*pfile);
    *pfile = NULL;
  }
}

// Close a file opened by open() and set the 'int *fd' to -1 (INVALID_FILENO).
static inline void close_fd(int *pfileno) {
  if (*pfileno != invalid_fileno) {
    close(*pfileno);
    *pfileno = invalid_fileno;
  }
}

#endif  // CUTIL_RAII_H
