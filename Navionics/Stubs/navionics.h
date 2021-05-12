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

// navionics.h stub file

#ifndef NAVIONICS_H
#define NAVIONICS_H

#include "GRM_pub.h"

#ifndef DBG_MODULE
#define DBG_MODULE "ACDB"
#endif

#ifndef DBG_TAG
#define DBG_TAG "Navionics"
#endif

#include <climits>
#include "DBG_pub.h"

#define NavAssert(e) DBG_ASSERT(e, "")
#define number_of_elements(a) cnt_of_array(a)

#endif  // NAVIONICS_H
