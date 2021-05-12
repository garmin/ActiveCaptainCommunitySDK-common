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
    Extensions on NavDateTime class.

    Copyright 2019-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef Navionics_DateTimeExtensions_hpp
#define Navionics_DateTimeExtensions_hpp

#include "NavDateTime.h"
#include "NavString.h"

namespace Acdb {
using namespace Navionics;

//! Unix Epoch base date
static constexpr int UnixEpochDay = 1;
static constexpr int UnixEpochMonth = 1;
static constexpr int UnixEpochYear = 1970;

//! @enum EpochType
//! Differentiates between different varieties of epoch base years
enum EpochType { UNIX_EPOCH };

class NavDateTimeExtensions {
 public:
  static NavDateTime EpochToNavDateTime(EpochType aEpochType, uint64_t aEpochSeconds);

  static NavDateTime GetCurrentDateTime();

  static uint64_t NavDateTimeToEpoch(const NavDateTime& aDateTime, EpochType aEpochType);
};
}  // namespace Acdb

#endif  // Navionics_DateTimeExtensions_hpp
