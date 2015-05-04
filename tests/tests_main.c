// Entry point for the libcutil test suite.
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

#include <fcntl.h>
#include <math.h>
#include <mcheck.h>
#include <string.h>

#include "log.h"
#include "raii.h"
#include "test.h"
#include "vector.h"

// To avoid raising aborts on mprobe we need to install a nop callback.
void mcheck_abortfunc(enum mcheck_status status) {(void)status;}

void init_tests(void) {
  // Restrict the logging to error messages to avoid cluttering test output.
  log_level = LL_LOG;
   // Install debug hooks into malloc to track heap allocations.
  mcheck(mcheck_abortfunc);
}

void cleanup_tests(void) {}

void reset_tests(void) {
  cleanup_tests();
  init_tests();
}

void raii_test(void) {
  char *fd_filename = malloc(L_tmpnam * sizeof(char));
  char *fp_filename = malloc(L_tmpnam * sizeof(char));
  char **deallocated_fd_test_filename = NULL;
  char **deallocated_fp_test_filename = NULL;
  FILE **deallocated_test_fp = NULL;
  char *fd_test_filename_copy = NULL;
  char *fp_test_filename_copy = NULL;
  FILE *test_fp_copy = NULL;

  int *deallocated_test_fd = 0;

  ASSERT_NOT_NULL(fd_filename);
  ASSERT_NOT_NULL(fp_filename);
  // This is a test so the security implications of tmpnam are irrelevant.
  ASSERT_NOT_NULL(tmpnam(fd_filename));
  ASSERT_NOT_NULL(tmpnam(fp_filename));
  // We use a local scope to test proper resource deallocation.
  {
    LOCAL char *fd_test_filename = strdup(fd_filename);
    LOCAL char *fp_test_filename = strdup(fp_filename);
    LOCAL_FD int test_fd = open(fd_test_filename, O_CREAT | O_EXCL, S_IRWXU);
    ASSERT_GREATER(test_fd, STDERR_FILENO);
    LOCAL_FP FILE *test_fp = fopen(fp_test_filename, "w+");
    ASSERT_NOT_NULL(test_fp);
    // Copy local pointers to enable heap diagnostics.
    fd_test_filename_copy = fd_test_filename;
    fp_test_filename_copy = fp_test_filename;
    test_fp_copy = test_fp;
    // Store location of local variables to be able to check if
    // their destructor set them to NULL/invalid.
    deallocated_fd_test_filename = &fd_test_filename;
    deallocated_fp_test_filename = &fp_test_filename;
    deallocated_test_fp = &test_fp;
    deallocated_test_fd = &test_fd;
  }
  // Now check if the memory and files that have left their scope
  // have been closed/deallocated.
  ASSERT_EQUAL(mprobe(fd_test_filename_copy), MCHECK_FREE);
  ASSERT_EQUAL(mprobe(fp_test_filename_copy), MCHECK_FREE);
  ASSERT_EQUAL(mprobe(test_fp_copy), MCHECK_FREE);
  ASSERT_NULL(*deallocated_fd_test_filename);
  ASSERT_NULL(*deallocated_fp_test_filename);
  ASSERT_NULL(*deallocated_test_fp);
  ASSERT_EQUAL(*deallocated_test_fd, invalid_fileno);
  // Make sure we don't leave tmpfiles lying around...
  ASSERT_ZERO(unlink(fd_filename));
  ASSERT_ZERO(unlink(fp_filename));
}

void vector_test_charp(void) {
  size_t test_initial_capacity = 16;
  char *test_data = "loremipsumdolor...";
  VECTOR vec;

  vector_init(&vec, sizeof(char *), VECTOR_DEFAULT_SIZE);
  ASSERT_EQUAL(vec.used_bytes, 0);
  ASSERT_EQUAL(vec.total_bytes, test_initial_capacity * vec.item_size);
  ASSERT_NOT_NULL(vector_push(&vec, &test_data, sizeof(&test_data)));
  // TODO: Pointer vectors are a bit clumsy, create a simpler specialization.
  char *element = *((char **)vector_get(&vec, 0));
  ASSERT_EQUAL_STRINGS(element, test_data);
  ASSERT_SUCCESS(vector_del(&vec, 0));
  ASSERT_EQUAL(vec.used_bytes, 0);
  ASSERT_EQUAL(vec.total_bytes, test_initial_capacity * vec.item_size);
}

void vector_test_lots_ints(void) {
  size_t test_size = 10000;
  size_t test_initial_capacity = 16;
  VECTOR vec;

  vector_init(&vec, sizeof(size_t), VECTOR_DEFAULT_SIZE);
  ASSERT_EQUAL(vec.used_bytes, 0);
  ASSERT_EQUAL(vec.total_bytes, test_initial_capacity * vec.item_size);

  for (size_t i = 0; i < test_size; ++i) {
    ASSERT_NOT_NULL(vector_push(&vec, &i, sizeof(i)));
  }
  ASSERT_EQUAL(vec.used_bytes, test_size * sizeof(size_t));
  ASSERT_EQUAL(vec.total_bytes, pow(2, 14) * sizeof(size_t));

  for (size_t i = 0; i < test_size; ++i) {
    size_t *number = vector_get(&vec, i);
    ASSERT_EQUAL(*number, i);
  }
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  init_tests();

  test_add(raii_test, "RAII resource management");
  test_add(vector_test_charp, "basic vector insertion/removal");
  test_add(vector_test_lots_ints, "vector storing/removing many integers");

  ERROR status = tests_run();
  cleanup_tests();

  return status;
}
