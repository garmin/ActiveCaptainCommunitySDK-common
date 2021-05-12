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

#define DBG_MODULE "ACDB"
#define DBG_TAG "NavDateTimeExtensions"

#include <chrono>
#include "DBG_pub.h"
#include "NavDateTimeExtensions.hpp"

namespace Acdb {
using namespace Navionics;

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Convert epoch value to NavDateTime
//!   @return
//!       NavDateTime corresponding to given epoch value.
//!
//----------------------------------------------------------------
NavDateTime NavDateTimeExtensions::EpochToNavDateTime(EpochType aEpochType,
                                                      uint64_t aEpochSeconds) {
  NavDateTime dateTime;

  if (aEpochType == UNIX_EPOCH) {
    dateTime.SetDate(UnixEpochDay, UnixEpochMonth, UnixEpochYear);
  }

  dateTime += NavTimeSpan(static_cast<int>(aEpochSeconds));

  return dateTime;
}  // end of function EpochToNavDateTime()

//----------------------------------------------------------------
//!
//!   @public
//!   @return
//!       NavDateTime corresponding to current date time.
//!
//----------------------------------------------------------------
NavDateTime NavDateTimeExtensions::GetCurrentDateTime() {
  std::chrono::seconds currentDateTime = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch());

  Navionics::NavDateTime now = NavDateTimeExtensions::EpochToNavDateTime(
      UNIX_EPOCH, static_cast<uint64_t>(currentDateTime.count()));

  return now;
}  // end of function GetCurrentDateTime()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Convert NavDateTime to epoch value
//!   @return
//!       Epoch value (seconds) corresponding to given NavDateTime.
//!
//----------------------------------------------------------------
uint64_t NavDateTimeExtensions::NavDateTimeToEpoch(const NavDateTime& aDateTime,
                                                   EpochType aEpochType) {
  NavDateTime base;
  if (aEpochType == UNIX_EPOCH) {
    base.SetDate(UnixEpochDay, UnixEpochMonth, UnixEpochYear);
  }

  NavTimeSpan timeOffset = aDateTime - base;
  return static_cast<uint64_t>(timeOffset.GetTotalSeconds());
}  // end of function NavDateTimeToEpoch()

}  // end namespace Acdb
