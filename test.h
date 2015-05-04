// Simple unit test framework that has no external dependencies except
// for the standard library and focuses on minimal complexity.
//
// USAGE: You can write your own test functions that use any of the ASSERT_X
// macros to check for problems. Add test functions to the queue with
// test_add(), and then run all tests using tests_run().
//
// NOTE: This framework uses longjmp to restore the stack into a working
// state after a test fails. This can leak memory, as acquired ressources
// cannot be released when a test failure occurs. If you have tests which
// use a lot of heap memory or other system ressources, try to allocate most
// of your ressources in a seperate init() and cleanup() function outside of
// your test function.
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

#ifndef _LIBCUTIL_UTEST_H
#define _LIBCUTIL_UTEST_H

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#include "types.h"

// To keep things simple there is a hardcoded limit to the number of
// tests you can run. If you hit it the framework will tell you, and you
// need to increase this number.
#define MAX_TESTS 4096

// Functions for unit tests return nothing and have no arguments.
typedef void (*TEST)(void);

// Adds a test to the queue to be run when tests_run() is called.
ERROR test_add(TEST test_function, const char * const test_name);

// Runs any queued tests and prints some statistics.
ERROR tests_run(void);

// Print a formatted error message and the exact location of the test failure.
#define TEST_FAILED(fmt, ...) do { \
  printf("FAILED: " fmt "\n\t(%s:%d %s())\n", \
  ##__VA_ARGS__, __FILE__, __LINE__, __FUNCTION__); \
  abort(); \
} while (0)

#define ASSERT_TRUE(expression) do { \
  if (!(expression)) \
    TEST_FAILED("%s not true", __STRING(expression)); \
} while (0)

#define ASSERT_SUCCESS(expression) do { \
  if ((expression) != SUCCESS) \
    TEST_FAILED("%s not successful", __STRING(expression)); \
} while (0)

#define ASSERT_ZERO(expression) do { \
  if (expression) \
    TEST_FAILED("%s not null", __STRING(expression)); \
} while (0)

#define ASSERT_NULL(expression) do { \
  if (expression != NULL) \
    TEST_FAILED("%s(%p) not NULL", __STRING(expression), (expression)); \
} while (0)

#define ASSERT_EQUAL(val1, val2) do { \
  if ((val1) != (val2)) \
    TEST_FAILED("%s(%zi) != %s(%zi)", \
      __STRING(val1), (ssize_t)(val1), __STRING(val2), (ssize_t)(val2)); \
} while (0)

#define ASSERT_EQUAL_UNSIGNED(val1, val2) do { \
  if ((val1) != (val2)) \
    TEST_FAILED("%s(%zu) != %s(%zu)", \
      __STRING(val1), (size_t)(val1), __STRING(val2), (size_t)(val2)); \
} while (0)

#define ASSERT_EQUAL_POINTERS(val1, val2) do { \
  if ((void *)(val1) != (void *)(val2)) \
    TEST_FAILED("%s(%p) != %s(%p)", \
      __STRING(val1), (void *)(val1), __STRING(val2), (void *)(val2)); \
} while (0)

#define ASSERT_EQUAL_STRINGS(val1, val2) do { \
  if (strcmp((val1), (val2))) \
    TEST_FAILED("%s(%s) != %s(%s)", \
      __STRING(val1), val1, __STRING(val2), val2); \
} while (0)

#define ASSERT_EQUAL_MEMORY(val1, val2, len) do { \
  if (memcmp((val1), (val2), (len))) \
    TEST_FAILED("%s != %s", \
      __STRING(val1), val1, __STRING(val2), val2); \
} while (0)

#define ASSERT_NOT_NULL(expression) do { \
  if (expression == NULL) \
    TEST_FAILED("%s(%p) is NULL", __STRING(expression), (expression)); \
} while (0)

#define ASSERT_NOT_EQUAL(val1, val2) do { \
  if ((val1) == (val2)) \
    TEST_FAILED("%s(%zi) == %s(%zi)", \
      __STRING(val1), (ssize_t)(val1), __STRING(val2), (ssize_t)(val2)); \
} while (0)

#define ASSERT_NOT_EQUAL_UNSIGNED(val1, val2) do { \
  if ((val1) == (val2)) \
    TEST_FAILED("%s(%zu) == %s(%zu)", \
      __STRING(val1), (size_t)(val1), __STRING(val2), (size_t)(val2)); \
} while (0)

#define ASSERT_NOT_EQUAL_POINTERS(val1, val2) do { \
  if ((val1) == (val2)) \
    TEST_FAILED("%s(%p) == %s(%p)", \
      __STRING(val1), (val1), __STRING(val2), (val2)); \
} while (0)

#define ASSERT_NOT_EQUAL_STRINGS(val1, val2) do { \
  if (!strcmp((val1), (val2))) \
    TEST_FAILED("%s(%s) == %s(%s)", \
      __STRING(val1), val1, __STRING(val2), val2); \
} while (0)

#define ASSERT_NOT_EQUAL_MEMORY(val1, val2, len) do { \
  if (!memcmp((val1), (val2), (len))) \
    TEST_FAILED("%s == %s", \
      __STRING(val1), val1, __STRING(val2), val2); \
} while (0)

#endif  // _LIBCUTIL_UTEST_H
