/*------------------------------------------------------------------------------
Copyright 2021 Garmin Ltd. or its subsidiaries.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
------------------------------------------------------------------------------*/

/**
    @file
    @brief Garmin types stub file

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef GRM_PUB_H
#define GRM_PUB_H

#include <stdint.h>

#define cnt_of_array(a) (sizeof(a) / sizeof((a)[0]))
#define GRM_NOEXCEPT noexcept

typedef struct {
  int32_t lat;
  int32_t lon;
} scposn_type;

typedef struct {
  scposn_type nec; /* coords for Northeast corner */
  scposn_type swc; /* coords for Southwest corner */
} bbox_type;

/*---------------------------------------------------------------
Mark that a value (parameter/variable or return value) is unused.
---------------------------------------------------------------*/
#define GRM_unused(a) (void)(a)

#endif
