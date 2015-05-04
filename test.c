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

#include "test.h"

typedef struct TESTS_ {
  TEST functions[MAX_TESTS];
  const char *names[MAX_TESTS];
  size_t size;
  size_t max_size;
} TESTS;

static const int TEST_FAILED = 1;
static unsigned int current_test = 0;
static unsigned int passed_tests = 0;
static jmp_buf test_exception;
static TESTS tests = {
  .functions = {0},
  .names = {0},
  .size = 0,
  .max_size = MAX_TESTS
};

// Signal handler that catches aborted tests and restores the stack
// to the last known good state.
static void test_abort(int signum __attribute__((unused))) {
  longjmp(test_exception, TEST_FAILED);
}

// This function runs unit test functions and recovers from errors.
// It will always return after printing information on test failure,
// so your tests can continue.
static void test_run(TEST test_function, const char * const test_name,
    size_t test_index) {
  printf("[TEST %zu] %-50s ", test_index, test_name);
  signal(SIGABRT, test_abort);
  current_test++;
  if (setjmp(test_exception) != TEST_FAILED) {
    test_function();
    printf("[%s]\n", "OK");
    passed_tests++;
  }
}

// Adds a test to the queue to be run when tests_run() is called.
ERROR test_add(TEST test_function, const char * const test_name) {
  if (tests.size < tests.max_size) {
    tests.functions[tests.size] = test_function;
    tests.names[tests.size] = test_name;
    ++tests.size;
    return SUCCESS;
  }
  return FAILURE;
}

// Runs any queued tests and prints some statistics.
ERROR tests_run(void) {
  for (size_t i = 0; i < tests.size; ++i) {
    test_run(tests.functions[i], tests.names[i], i);
  }
  if (current_test == passed_tests) {
    puts("ALL TESTS PASSED");
    return SUCCESS;
  }
  printf("%d TESTS FAILED\n", current_test - passed_tests);
  return FAILURE;
}
