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
    @brief Regression tests for the MergeAdapter

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MergeAdapterTests"

#include "Acdb/MergeAdapter.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving marker collection.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.mergeadapter.get_marker", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MergeAdapter mergeAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  MarkerTableDataCollection expected = GetMarkerTableDataCollection();

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  MarkerTableDataCollection actual = mergeAdapter.GetMarker(markerId);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, (expected == actual), "MergeAdapter: GetMarker");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving reviews.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.mergeadapter.get_reviews", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MergeAdapter mergeAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  std::vector<ReviewTableDataCollection> expected = GetReviewsTableDataCollection();

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<ReviewTableDataCollection> actual = mergeAdapter.GetReviews(markerId);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "MergeAdapter: GetReviews count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, (expected[i] == actual[i]), "MergeAdapter: GetReviews[%i]", i);
  }
}

}  // end of namespace Test
}  // end of namespace Acdb
