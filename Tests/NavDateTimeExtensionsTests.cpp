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
    @brief Regression tests for NavDateTimeExtensions

    Copyright 2019-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "Acdb"
#define DBG_TAG "NavDateTimeExtensionsTests"

#include "NavDateTime.h"
#include "NavDateTimeExtensions.hpp"
#include "TF_pub.h"

namespace Acdb {
namespace Test {
using namespace Navionics;

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test converting from epoch to NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.extensions.from_epoch") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const uint64_t secondsSinceUnixEpoch = 1527067801;

  NavDateTime expected{2018, 5, 23, 9, 30, 1, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime actualUnix =
      NavDateTimeExtensions::EpochToNavDateTime(UNIX_EPOCH, secondsSinceUnixEpoch);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actualUnix, "EpochToNavDateTime, Unix");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test converting from NavDateTime to epoch.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.extensions.to_epoch") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime dateTime{2018, 5, 23, 9, 30, 1, 0};

  const uint64_t expectedUnixEpoch = 1527067801;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  uint64_t actualUnixEpoch = NavDateTimeExtensions::NavDateTimeToEpoch(dateTime, UNIX_EPOCH);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expectedUnixEpoch == actualUnixEpoch, "NavDateTimeToEpoch, Unix");
}

}  // end of namespace Test
}  // end of namespace Acdb
