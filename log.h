// Simple logging functionality to prettify output and manage verbosity.
//
// Adapted from the LMAP project.
// Copyright 2013 Google Inc. All Rights Reserved.
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

#ifndef _LIBCUTIL_LOG_H_
#define _LIBCUTIL_LOG_H_

typedef enum LOGLEVEL_T {
  LL_ERR = 0,  // Special error formatting (gets a '[-]' prefix)
  LL_NNL,  // No New Line
  LL_MSG,  // Clean messages without line prefixes
  LL_LOG,  // Regular log entries (gets a '[+]' prefix)
  LL_VER,  // Verbose logging
  LL_DBG   // Really verbose debug logging
} LOGLEVEL;

LOGLEVEL log_level;
void log_print(LOGLEVEL msg_level, const char *fmt, ...);

#endif  // _LIBCUTIL_LOG_H_
