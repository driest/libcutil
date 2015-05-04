// Simple error logging macro to output function and line numbers together with
// an error message. Not as powerful as log.h and macro based, but easy.
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

#ifndef CUTIL_ERROR_LOG_H
#define CUTIL_ERROR_LOG_H

#define ERROR_LOG(fmt, ...) do { \
  printf("Error in %s(%d): " fmt, __func__, __LINE__, ##__VA_ARGS__); \
} while (0)

#endif  // CUTIL_ERROR_LOG_H
