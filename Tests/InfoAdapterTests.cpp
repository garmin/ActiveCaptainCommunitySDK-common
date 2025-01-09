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
    @brief Regression tests for the InfoAdapter

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "InfoAdapterTests"

#include "Acdb/InfoAdapter.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/Version.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving last update info.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.infoadapter.get_last_update_info", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  InfoAdapter infoAdapter{database};

  PopulateDatabase(state, database);

  LastUpdateInfoType expected;
  expected.mMarkerLastUpdate = 1715842820;
  expected.mUserReviewLastUpdate = 1715842821;
  LastUpdateInfoType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, infoAdapter.GetLastUpdateInfo(actual), "LastUpdateInfo: Get");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.mMarkerLastUpdate == actual.mMarkerLastUpdate,
                "LastUpdateInfo: Marker");
  TF_assert_msg(state, expected.mUserReviewLastUpdate == actual.mUserReviewLastUpdate,
                "LastUpdateInfo: Review");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving version.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.infoadapter.get_version", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  InfoAdapter infoAdapter{database};
  VersionQuery versionQuery{database};

  PopulateDatabase(state, database);

  std::string invalidVersion = "9.9.9.9";

  std::string expected("1.2.3.4");
  Acdb::Version actual(invalidVersion);

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  infoAdapter.GetVersion(actual);
  TF_assert_msg(state, actual.ToString() == invalidVersion,
                "Version: get invalid");  // Shouldn't exist yet.
  versionQuery.Put(expected);
  infoAdapter.GetVersion(actual);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual.ToString(), "Version");
}

}  // end of namespace Test
}  // end of namespace Acdb
