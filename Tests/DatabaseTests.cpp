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
    @brief Regression tests for the database

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "DatabaseTests"

#include "Acdb/Queries/AddressQuery.hpp"
#include "Acdb/Queries/AmenitiesQuery.hpp"
#include "Acdb/Queries/BusinessQuery.hpp"
#include "Acdb/Queries/BusinessPhotoQuery.hpp"
#include "Acdb/Queries/BusinessProgramQuery.hpp"
#include "Acdb/Queries/CompetitorQuery.hpp"
#include "Acdb/Queries/ContactQuery.hpp"
#include "Acdb/Queries/DockageQuery.hpp"
#include "Acdb/Queries/FuelQuery.hpp"
#include "Acdb/Queries/LanguageQuery.hpp"
#include "Acdb/Queries/MarkerQuery.hpp"
#include "Acdb/Queries/MarkerMetaQuery.hpp"
#include "Acdb/Queries/MooringsQuery.hpp"
#include "Acdb/Queries/MustacheTemplateQuery.hpp"
#include "Acdb/Queries/NavigationQuery.hpp"
#include "Acdb/Queries/PositionQuery.hpp"
#include "Acdb/Queries/RetailQuery.hpp"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "Acdb/Queries/ReviewPhotoQuery.hpp"
#include "Acdb/Queries/ReviewSummaryQuery.hpp"
#include "Acdb/Queries/ServicesQuery.hpp"
#include "Acdb/Queries/TileLastUpdateQuery.hpp"
#include "Acdb/Queries/TilesQuery.hpp"
#include "Acdb/Queries/TranslatorQuery.hpp"
#include "Acdb/Queries/VersionQuery.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"
#include "UTL_pub_lib_cnvt.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test address operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_address") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  AddressQuery addressQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  AddressTableDataType addressTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::AddressTitle),
      "[ { \"value\": \"1200 W 151st St\" }, { \"value\": \"Olathe, KS 66062\" }, { \"value\": \"US\" } ]",
      "[ { \"value\": \"www.garmin.com\", \"hyperlink\": \"http://www.garmin.com\", \"isDistance\": false } ]");

  AddressTableDataType expected = addressTableData;
  AddressTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, addressQuery.Get(markerId, actual) == false,
                "Address Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, addressQuery.Write(markerId, std::move(addressTableData)), "Address Write");
  TF_assert_msg(state, addressQuery.Get(markerId, actual), "Address Get");
  TF_assert_msg(state, addressQuery.Delete(markerId), "Address Delete");
  TF_assert_msg(state, addressQuery.Get(markerId, actual) == false, "Address not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Address");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test amenities operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_amenities") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  AmenitiesQuery amenitiesQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  AmenitiesTableDataType amenitiesTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::AmenitiesTitle),
      "{ \"fieldTextHandle\": 29, \"value\": \"Section Note\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 30, \"value\": \"Yes\", \"note\": \"Nearby\" }, { \"fieldTextHandle\": 31, \"value\": \"No\" } ]");

  AmenitiesTableDataType expected = amenitiesTableData;
  AmenitiesTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, amenitiesQuery.Get(markerId, actual) == false,
                "Amenities Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, amenitiesQuery.Write(markerId, std::move(amenitiesTableData)),
                "Amenities Write");
  TF_assert_msg(state, amenitiesQuery.Get(markerId, actual), "Amenities Get");
  TF_assert_msg(state, amenitiesQuery.Delete(markerId), "Amenities Delete");
  TF_assert_msg(state, amenitiesQuery.Get(markerId, actual) == false, "Amenities not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Amenities");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test business operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_business") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  BusinessQuery businessQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  BusinessTableDataType businessTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::BusinessTitle),
      "[ { \"fieldTextHandle\": 45, \"valueTextHandle\": 0, \"isDistance\": false }, { \"fieldTextHandle\": 46, \"valueTextHandle\": 48, \"isDistance\": false } ]",
      "[ { \"valueTextHandles\": [ 50, 52 ], \"fieldTextHandle\": 49, \"isDistance\": false } ]",
      std::string(), std::string());

  BusinessTableDataType expected = businessTableData;
  BusinessTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, businessQuery.Get(markerId, actual) == false,
                "Business Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, businessQuery.Write(markerId, std::move(businessTableData)),
                "Business Write");
  TF_assert_msg(state, businessQuery.Get(markerId, actual), "Business Get");
  TF_assert_msg(state, businessQuery.Delete(markerId), "Business Delete");
  TF_assert_msg(state, businessQuery.Get(markerId, actual) == false, "Business not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Business");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test business photo operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_business_photo") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  BusinessPhotoQuery businessPhotoQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  std::vector<BusinessPhotoTableDataType> businessPhotoTableData{
      {markerId, 1, "https://activecaptain.garmin.com/photos/12345-1.png"},
      {markerId, 2, "https://activecaptain.garmin.com/photos/12345-2.png"},
      {markerId, 3, "https://activecaptain.garmin.com/photos/12345-3.png"}};

  std::vector<BusinessPhotoTableDataType> expected = businessPhotoTableData;
  std::vector<BusinessPhotoTableDataType> actual;
  std::vector<BusinessPhotoTableDataType> actual2;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, businessPhotoQuery.Get(markerId, actual) == false,
                "BusinessPhoto Get: expected false.");  // Should not exist yet.
  for (auto businessPhoto : businessPhotoTableData) {
    TF_assert_msg(state, businessPhotoQuery.Write(markerId, std::move(businessPhoto)),
                  "BusinessPhoto Write");
  }
  TF_assert_msg(state, businessPhotoQuery.Get(markerId, actual), "BusinessPhoto Get");
  TF_assert_msg(state, businessPhotoQuery.Delete(markerId), "BusinessPhoto Delete");
  TF_assert_msg(state, businessPhotoQuery.Get(markerId, actual2) == false,
                "BusinessPhoto not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "BusinessPhoto");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test review photo operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_review_photo") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ReviewPhotoQuery reviewPhotoQuery{database};
  ACDB_review_idx_type reviewId = 12345;

  std::vector<ReviewPhotoTableDataType> reviewPhotoTableData{
      {reviewId, 1, "https://activecaptain.garmin.com/photos/111-1.png"},
      {reviewId, 2, "https://activecaptain.garmin.com/photos/111-2.png"},
      {reviewId, 3, "https://activecaptain.garmin.com/photos/111-3.png"}};

  std::vector<ReviewPhotoTableDataType> expected = reviewPhotoTableData;
  std::vector<ReviewPhotoTableDataType> actual;
  std::vector<ReviewPhotoTableDataType> actual2;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, reviewPhotoQuery.Get(reviewId, actual) == false,
                "ReviewPhoto Get: expected false.");  // Should not exist yet.
  for (auto reviewPhoto : reviewPhotoTableData) {
    TF_assert_msg(state, reviewPhotoQuery.Write(reviewId, std::move(reviewPhoto)),
                  "BusinessPhoto Write");
  }
  TF_assert_msg(state, reviewPhotoQuery.Get(reviewId, actual), "ReviewPhoto Get");
  TF_assert_msg(state, reviewPhotoQuery.Delete(reviewId), "ReviewPhoto Delete");
  TF_assert_msg(state, reviewPhotoQuery.Get(reviewId, actual2) == false, "ReviewPhoto not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "ReviewPhoto");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test business program operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_business_program") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  BusinessProgramQuery businessProgramQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  BusinessProgramTableDataType businessProgramTableData{
      markerId,
      "{\"text\": \"Voted best marina of 2019!\", \"photoUrl\": \"https://activecaptain.garmin.com/photos/999.jpg\" }",
      3};

  BusinessProgramTableDataType expected = businessProgramTableData;
  BusinessProgramTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, businessProgramQuery.Get(markerId, actual) == false,
                "BusinessProgram Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, businessProgramQuery.Write(markerId, std::move(businessProgramTableData)),
                "BusinessProgram Write");
  TF_assert_msg(state, businessProgramQuery.Get(markerId, actual), "BusinessProgram Get");
  TF_assert_msg(state, businessProgramQuery.Delete(markerId), "BusinessProgram Delete");
  TF_assert_msg(state, businessProgramQuery.Get(markerId, actual) == false,
                "BusinessProgram not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "BusinessProgram");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test competitor operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_competitor") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  CompetitorQuery competitorQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  std::vector<CompetitorTableDataType> competitorTableData{
      {markerId, 50001, 1}, {markerId, 50002, 2}, {markerId, 50003, 3}};

  std::vector<CompetitorTableDataType> expected = competitorTableData;
  std::vector<CompetitorTableDataType> actual;
  std::vector<CompetitorTableDataType> actual2;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, competitorQuery.Get(markerId, actual) == false,
                "Competitor Get: expected false.");  // Should not exist yet.
  for (auto competitor : competitorTableData) {
    TF_assert_msg(state, competitorQuery.Write(markerId, std::move(competitor)),
                  "Competitor Write");
  }
  TF_assert_msg(state, competitorQuery.Get(markerId, actual), "Competitor Get");
  TF_assert_msg(state, competitorQuery.Delete(markerId), "Competitor Delete");
  TF_assert_msg(state, competitorQuery.Get(markerId, actual2) == false, "Competitor not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Competitor");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test contact operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_contact") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ContactQuery contactQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  ContactTableDataType contactTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::ContactTitle),
      "[ { \"fieldTextHandle\": 24, \"value\": \"913-397-8200\", \"hyperlink\": \"tel:913-397-8200\", \"isDistance\": false }, { \"fieldTextHandle\": 25, \"value\": \"Channel 16 VHF\", \"isDistance\": false } ]",
      "913-397-8200", "Channel 16 VHF");

  ContactTableDataType expected = contactTableData;
  ContactTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, contactQuery.Get(markerId, actual) == false,
                "Contact Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, contactQuery.Write(markerId, std::move(contactTableData)), "Contact Write");
  TF_assert_msg(state, contactQuery.Get(markerId, actual), "Contact Get");
  TF_assert_msg(state, contactQuery.Delete(markerId), "Contact Delete");
  TF_assert_msg(state, contactQuery.Get(markerId, actual) == false, "Contact not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Contact");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test dockage operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_dockage") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  DockageQuery dockageQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  DockageTableDataType dockageTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::DockageTitle),
      "[ { \"valueTextHandles\": [ 99, 101, 103 ], \"fieldTextHandle\": 97, \"value\": \"Yes\", \"note\": \"Test note.\" } ]",
      "[ { \"price\": \"3.75 USD\", \"pricingUnitTextHandle\": 4, \"priceDate\": \"2018-10-13T10:32:29Z\", \"fieldTextHandle\": 78, \"isDistance\": false } ]",
      "[ { \"fieldTextHandle\": 79, \"value\": \"50\", \"isDistance\": false }, { \"fieldTextHandle\": 80, \"value\": \"40\", \"isDistance\": false } ]",
      "{ \"fieldTextHandle\": 75, \"value\": \"Test section note.\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 91, \"value\": \"Unknown\" }, { \"fieldTextHandle\": 92, \"value\": \"No\" } ]",
      ACDB_METER);

  DockageTableDataType expected = dockageTableData;
  DockageTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, dockageQuery.Get(markerId, actual) == false,
                "Dockage Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, dockageQuery.Write(markerId, std::move(dockageTableData)), "Dockage Write");
  TF_assert_msg(state, dockageQuery.Get(markerId, actual), "Dockage Get");
  TF_assert_msg(state, dockageQuery.Delete(markerId), "Dockage Delete");
  TF_assert_msg(state, dockageQuery.Get(markerId, actual) == false, "Dockage not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Dockage");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test fuel operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_fuel") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  FuelQuery fuelQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  FuelTableDataType fuelTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::FuelTitle),
      "[ { \"fieldTextHandle\": 67, \"value\": \"No\" }, { \"fieldTextHandle\": 69, \"value\": \"Yes\" } ]",
      "[ { \"fieldTextHandle\": 68, \"value\": \"Unknown\" }, { \"fieldTextHandle\": 70, \"value\": \"No\" } ]",
      "[ { \"fieldTextHandle\": 71, \"value\": \"3.66\", \"isDistance\": true } ]",
      "{ \"fieldTextHandle\": 72, \"value\": \"Test section note.\", \"isDistance\": false }",
      ACDB_METER, std::string(), 0, 0, ACDB_LITER);

  FuelTableDataType expected = fuelTableData;
  FuelTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, fuelQuery.Get(markerId, actual) == false,
                "Fuel Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, fuelQuery.Write(markerId, std::move(fuelTableData)), "Fuel Write");
  TF_assert_msg(state, fuelQuery.Get(markerId, actual), "Fuel Get");
  TF_assert_msg(state, fuelQuery.Delete(markerId), "Fuel Delete");
  TF_assert_msg(state, fuelQuery.Get(markerId, actual) == false, "Fuel not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Fuel");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test Language operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_language") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  LanguageQuery languageQuery{database};

  std::vector<LanguageTableDataType> languageTableData{{1, "en_US"}, {2, "pt_BR"}, {3, "ro_RO"}};

  std::vector<LanguageTableDataType> expected = languageTableData;
  std::vector<LanguageTableDataType> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, languageQuery.GetAll(actual) == false,
                "Language Get: expected false.");  // Should not exist yet.
  for (auto language : languageTableData) {
    TF_assert_msg(state, languageQuery.Write(std::move(language)), "Language Write");
  }
  TF_assert_msg(state, languageQuery.GetAll(actual), "Language Get");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Language");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test marker and position operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_marker") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  MarkerQuery markerQuery{database};
  PositionQuery positionQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  std::vector<MarkerTableDataType> markerTableDataList{
      MarkerTableDataType(markerId, ACDB_MARINA, 1527084000, "Test Marker 1",
                          {static_cast<int32_t>(10.0 * UTL_DEG_TO_SEMI),
                           static_cast<int32_t>(10.0 * UTL_DEG_TO_SEMI)},
                          2333322233144231124UL, 9223372036854775809UL,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER),
      MarkerTableDataType(markerId + 1, ACDB_MARINA, 1527084001, "Test Marker 2",
                          {static_cast<int32_t>(20.0 * UTL_DEG_TO_SEMI),
                           static_cast<int32_t>(20.0 * UTL_DEG_TO_SEMI)},
                          2333222313444131442UL, 9223372036854775809UL,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER),
      MarkerTableDataType(markerId + 2, ACDB_MARINA, 1527084002, "Test Marker 3",
                          //{ 357913941, 357913941 },  // 30, 30
                          {static_cast<int32_t>(30.0 * UTL_DEG_TO_SEMI),
                           static_cast<int32_t>(30.0 * UTL_DEG_TO_SEMI)},
                          2332321432143432321UL, 9223372036854775809UL,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER),
      MarkerTableDataType(markerId + 3, ACDB_MARINA, 1527084003, "Test Marker 4",
                          {static_cast<int32_t>(40.0 * UTL_DEG_TO_SEMI),
                           static_cast<int32_t>(40.0 * UTL_DEG_TO_SEMI)},
                          2332221134423132421UL, 9223372036854775809UL,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER),
      MarkerTableDataType(markerId + 4, ACDB_HAZARD, 1527084004, "Test Hazard 1",
                          {static_cast<int32_t>(25.0 * UTL_DEG_TO_SEMI),
                           static_cast<int32_t>(25.0 * UTL_DEG_TO_SEMI)},
                          2332331442311224113UL, 9223372036854775808UL,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER)};

  MarkerTableDataType markerTableData = markerTableDataList[0];
  scposn_type posn = markerTableData.mPosn;
  MarkerTableDataType expected = markerTableData;
  MarkerTableDataType actual;

  std::vector<MarkerTableDataType> expectedFiltered;
  expectedFiltered.push_back(markerTableDataList[1]);
  expectedFiltered.push_back(markerTableDataList[2]);
  // 0 and 4 are outside of bbox, 5 is inside bbox but wrong type.
  std::vector<MarkerTableDataType> actualFiltered;

  ACDB_marker_filter_type filter;
  filter.bbox = {
      {static_cast<int32_t>(35.0 * UTL_DEG_TO_SEMI), static_cast<int32_t>(35.0 * UTL_DEG_TO_SEMI)},
      {static_cast<int32_t>(15.0 * UTL_DEG_TO_SEMI), static_cast<int32_t>(15.0 * UTL_DEG_TO_SEMI)}};
  filter.include_marinas = true;
  filter.include_hazards = false;
  filter.include_anchorages = false;
  filter.include_local_knowledge = false;
  filter.search_string = nullptr;
  filter.max_num_results = 5;

  MapMarkerFilter markerFilter(&filter);

  uint64_t expectedLastUpdate = markerTableDataList[4].mLastUpdated;
  uint64_t actualLastUpdate;

  std::vector<ACDB_marker_idx_type> expectedIds;
  for (auto data : markerTableDataList) {
    expectedIds.push_back(data.mId);
  };
  std::vector<ACDB_marker_idx_type> actualIds;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, markerQuery.Get(markerId, actual) == false,
                "Marker Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, markerQuery.Write(markerId, std::move(markerTableData)), "Marker Write");
  TF_assert_msg(state, positionQuery.Write(markerId, posn), "Position Write");
  TF_assert_msg(state, markerQuery.Get(markerId, actual), "Marker Get");
  TF_assert_msg(state, markerQuery.Delete(markerId), "Marker Delete");
  TF_assert_msg(state, positionQuery.Delete(markerId), "Position Delete");
  TF_assert_msg(state, markerQuery.Get(markerId, actual) == false, "Marker not deleted");

  for (std::size_t i = 0; i < markerTableDataList.size(); i++) {
    posn = markerTableDataList[i].mPosn;
    TF_assert_msg(state,
                  markerQuery.Write(markerTableDataList[i].mId, std::move(markerTableDataList[i])),
                  "Marker Write list");
    TF_assert_msg(state, positionQuery.Write(markerTableDataList[i].mId, posn),
                  "Position Write list");
  }

  TF_assert_msg(state, markerQuery.GetFiltered(markerFilter, actualFiltered),
                "Marker get filtered");

  TF_assert_msg(state, markerQuery.GetLastUpdate(actualLastUpdate), "Marker get last update");

  TF_assert_msg(state, markerQuery.GetIds(0, -1, actualIds), "Marker get IDs");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Marker");
  TF_assert_msg(state, expectedFiltered == actualFiltered, "Marker Filtered");
  TF_assert_msg(state, expectedLastUpdate == actualLastUpdate, "Marker last update");
  TF_assert_msg(state, expectedIds == actualIds, "Marker get IDs");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test marker meta operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_marker_meta") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  MarkerMetaQuery markerMetaQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  MarkerMetaTableDataType markerMetaTableData(
      "{ \"value\": \"Test marker notes.\", \"isDistance\": false }",
      static_cast<ACDB_text_handle_type>(TextHandle::SummaryTitle));

  MarkerMetaTableDataType expected = markerMetaTableData;
  MarkerMetaTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, markerMetaQuery.Get(markerId, actual) == false,
                "MarkerMeta Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, markerMetaQuery.Write(markerId, std::move(markerMetaTableData)),
                "MarkerMeta Write");
  TF_assert_msg(state, markerMetaQuery.Get(markerId, actual), "MarkerMeta Get");
  TF_assert_msg(state, markerMetaQuery.Delete(markerId), "MarkerMeta Delete");
  TF_assert_msg(state, markerMetaQuery.Get(markerId, actual) == false, "MarkerMeta not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "MarkerMeta");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test moorings operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_moorings") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  MooringsQuery mooringsQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  MooringsTableDataType mooringsTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::MooringsTitle),
      "{ \"fieldTextHandle\": 106, \"value\": \"No moorings.\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 109, \"value\": \"No\" } ]",
      "[ { \"fieldTextHandle\": 79, \"value\": \"0\", \"isDistance\": false }, { \"fieldTextHandle\": 80, \"value\": \"0\", \"isDistance\": false } ]",
      "[ { \"fieldTextHandle\": 107, \"value\": \"Yes\" }, { \"fieldTextHandle\": 108, \"value\": \"Unknown\" } ]");

  MooringsTableDataType expected = mooringsTableData;
  MooringsTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, mooringsQuery.Get(markerId, actual) == false,
                "Moorings Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, mooringsQuery.Write(markerId, std::move(mooringsTableData)),
                "Moorings Write");
  TF_assert_msg(state, mooringsQuery.Get(markerId, actual), "Moorings Get");
  TF_assert_msg(state, mooringsQuery.Delete(markerId), "Moorings Delete");
  TF_assert_msg(state, mooringsQuery.Get(markerId, actual) == false, "Moorings not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Moorings");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test Mustache template operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_mustache_templates") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  MustacheTemplateQuery mustacheTemplateQuery{database};
  std::string templateName = "template1";

  std::vector<MustacheTemplateTableDataType> mustacheTemplateTableData{
      {"template1", "{{ID}} - {{Name}}"},
      {"template2", "{{Field}}: {{Value}}"},
      {"template3", "This is a test."}};

  std::string expected = mustacheTemplateTableData[0].mTemplate;
  std::string actual;

  std::vector<MustacheTemplateTableDataType> expectedAll = mustacheTemplateTableData;
  std::vector<MustacheTemplateTableDataType> actualAll;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, mustacheTemplateQuery.Get(templateName, actual) == false,
                "MustacheTemplate Get: expected false.");  // Should not exist yet.
  for (auto mustacheTemplate : mustacheTemplateTableData) {
    TF_assert_msg(state, mustacheTemplateQuery.Write(std::move(mustacheTemplate)),
                  "MustacheTemplate Write");
  }
  TF_assert_msg(state, mustacheTemplateQuery.Get(templateName, actual), "MustacheTemplate Get");

  TF_assert_msg(state, mustacheTemplateQuery.GetAll(actualAll), "MustacheTemplate GetAll");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "MustacheTemplate");
  TF_assert_msg(state, expectedAll == actualAll, "MustacheTemplate");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test navigation operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_navigation") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  NavigationQuery navigationQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  NavigationTableDataType navigationTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::NavigationTitle),
      "{ \"fieldTextHandle\": 65, \"value\": \"Test section note.\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 54, \"value\": \"5.4864\", \"isDistance\": true }, { \"fieldTextHandle\": 55, \"value\": \"0.3048\", \"isDistance\": true } ]",
      ACDB_METER);

  NavigationTableDataType expected = navigationTableData;
  NavigationTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, navigationQuery.Get(markerId, actual) == false,
                "Navigation Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, navigationQuery.Write(markerId, std::move(navigationTableData)),
                "Navigation Write");
  TF_assert_msg(state, navigationQuery.Get(markerId, actual), "Navigation Get");
  TF_assert_msg(state, navigationQuery.Delete(markerId), "Navigation Delete");
  TF_assert_msg(state, navigationQuery.Get(markerId, actual) == false, "Navigation not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Navigation");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retail operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_retail") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  RetailQuery retailQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  RetailTableDataType retailTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::RetailTitle),
      "{ \"fieldTextHandle\": 111, \"value\": \"Test section note.\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 112, \"value\": \"Yes\" }, { \"fieldTextHandle\": 113, \"value\": \"Unknown\" } ]");

  RetailTableDataType expected = retailTableData;
  RetailTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, retailQuery.Get(markerId, actual) == false,
                "Retail Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, retailQuery.Write(markerId, std::move(retailTableData)), "Retail Write");
  TF_assert_msg(state, retailQuery.Get(markerId, actual), "Retail Get");
  TF_assert_msg(state, retailQuery.Delete(markerId), "Retail Delete");
  TF_assert_msg(state, retailQuery.Get(markerId, actual) == false, "Retail not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Retail");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test review operations (single review).
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_review_single") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ReviewQuery reviewQuery{database};
  ACDB_marker_idx_type markerId = 12345;
  ACDB_review_idx_type reviewId = 23456;

  ReviewTableDataType reviewTableData(reviewId, markerId, 1527084000, 1, "Test Review 1",
                                      "2018-05-23T00:00:00", "Test Captain 1", "This is a review.",
                                      0, false, std::string());

  ReviewTableDataType expected = reviewTableData;
  ReviewTableDataType actual;
  ReviewTableDataType actualDeleted;  // Should remain empty.

  uint64_t expectedLastUpdate = expected.mLastUpdated;
  uint64_t actualLastUpdate;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, reviewQuery.Get(markerId, actual) == false,
                "Review Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, reviewQuery.Write(reviewId, std::move(reviewTableData)), "Review Write");
  TF_assert_msg(state, reviewQuery.Get(markerId, actual), "Review Get");
  TF_assert_msg(state, reviewQuery.GetLastUpdate(actualLastUpdate), "Review get last update");
  TF_assert_msg(state, reviewQuery.Delete(reviewId), "Review Delete");
  TF_assert_msg(state, reviewQuery.Get(markerId, actualDeleted) == false, "Review not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Review");
  TF_assert_msg(state, expectedLastUpdate == actualLastUpdate, "Review last update");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test review list operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_review_list") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ReviewQuery reviewQuery{database};
  ACDB_marker_idx_type markerId = 12345;
  ACDB_review_idx_type reviewId = 23456;

  std::vector<ReviewTableDataType> reviewTableDataList{
      ReviewTableDataType(reviewId, markerId, 1527084000, 1, "Test Review 1", "2018-05-23T00:00:00",
                          "Test Captain 1", "This is a review.", 0, false, std::string()),
      ReviewTableDataType(reviewId + 1, markerId, 1527084001, 2, "Test Review 2",
                          "2018-05-23T00:00:00", "Test Captain 2", "This is a review.", 3, false,
                          std::string()),
      ReviewTableDataType(reviewId + 2, markerId, 1527084002, 5, "Test Review 3",
                          "2018-05-23T00:00:01", "Test Captain 3", "This is a review.", 0, false,
                          std::string()),
      ReviewTableDataType(reviewId + 3, markerId, 1527084002, 5, "Test Review 4",
                          "2018-05-23T00:00:02", "Test Captain 4", "This is a review.", 0, false,
                          std::string())};

  std::vector<ReviewTableDataType> expected{
      reviewTableDataList[3],  // User's own review
      reviewTableDataList[1],  // Most votes
      reviewTableDataList[2],  // Visit date more recent than 0
      reviewTableDataList[0]};
  std::vector<ReviewTableDataType> actual;
  std::vector<ReviewTableDataType> actualDeleted;  // Should remain empty.

  uint64_t expectedLastUpdate = reviewTableDataList[2].mLastUpdated;
  uint64_t actualLastUpdate;

  std::string captainName = "Test Captain 4";

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, reviewQuery.GetList(markerId, captainName, 1, 10, actual) == false,
                "Review Get list: expected false.");  // Should not exist yet.

  for (std::size_t i = 0; i < reviewTableDataList.size(); i++) {
    TF_assert_msg(state,
                  reviewQuery.Write(reviewTableDataList[i].mId, std::move(reviewTableDataList[i])),
                  "Review Write multiple");
  }

  TF_assert_msg(state, reviewQuery.GetList(markerId, captainName, 1, 10, actual),
                "Review Get list");
  TF_assert_msg(state, reviewQuery.GetLastUpdate(actualLastUpdate),
                "Review get last update multiple");
  TF_assert_msg(state, reviewQuery.DeleteMarker(markerId), "Review DeleteMarker");
  TF_assert_msg(state, reviewQuery.GetList(markerId, captainName, 1, 10, actualDeleted) == false,
                "Review for marker not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Review multiple");
  TF_assert_msg(state, expectedLastUpdate == actualLastUpdate, "Review last update multiple");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test review summary operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_review_summary") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ReviewQuery reviewQuery{database};
  ReviewSummaryQuery reviewSummaryQuery{database};
  ACDB_marker_idx_type markerId = 12345;
  ACDB_review_idx_type reviewId = 23456;

  std::vector<ReviewTableDataType> reviewTableDataList{
      ReviewTableDataType(reviewId, markerId, 1527084000, 1, "Test Review 1", "2018-05-23T00:00:00",
                          "Test Captain 1", "This is a review.", 0, false, std::string()),
      ReviewTableDataType(reviewId + 1, markerId, 1527084001, 2, "Test Review 2",
                          "2018-05-23T00:00:00", "Test Captain 2", "This is a review.", 3, false,
                          std::string()),
      ReviewTableDataType(reviewId + 2, markerId, 1527084002, 5, "Test Review 3",
                          "2018-05-23T00:00:01", "Test Captain 3", "This is a review.", 0, false,
                          std::string()),
      ReviewTableDataType(reviewId + 3, markerId, 1527084003, 1, "Test Review 4",
                          "2018-05-23T00:00:00", "Test Captain 4", "This is a review.", 0, false,
                          std::string())};

  ReviewSummaryTableDataType expected(2.25, 4);

  ReviewSummaryTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  for (std::size_t i = 0; i < reviewTableDataList.size(); i++) {
    TF_assert_msg(state,
                  reviewQuery.Write(reviewTableDataList[i].mId, std::move(reviewTableDataList[i])),
                  "ReviewSummary Write list");
  }

  TF_assert_msg(state, reviewSummaryQuery.Get(markerId, actual), "ReviewSummary Get");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "ReviewSummary");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test services operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_services") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  ServicesQuery servicesQuery{database};
  ACDB_marker_idx_type markerId = 12345;

  ServicesTableDataType servicesTableData(
      static_cast<ACDB_text_handle_type>(TextHandle::ServicesTitle),
      "{ \"fieldTextHandle\": 118, \"value\": \"Test section note.\", \"isDistance\": false }",
      "[ { \"fieldTextHandle\": 119, \"value\": \"No\" }, { \"fieldTextHandle\": 120, \"value\": \"Yes\" } ]");

  ServicesTableDataType expected = servicesTableData;
  ServicesTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, servicesQuery.Get(markerId, actual) == false,
                "Services Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, servicesQuery.Write(markerId, std::move(servicesTableData)),
                "Services Write");
  TF_assert_msg(state, servicesQuery.Get(markerId, actual), "Services Get");
  TF_assert_msg(state, servicesQuery.Delete(markerId), "Services Delete");
  TF_assert_msg(state, servicesQuery.Get(markerId, actual) == false, "Services not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Services");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test tile last update operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_tile_last_update") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TileLastUpdateQuery tileLastUpdateQuery{database};
  TileXY tileXY{4, 6};

  LastUpdateInfoType lastUpdateInfoType;
  lastUpdateInfoType.mMarkerLastUpdate = 1527084005;
  lastUpdateInfoType.mUserReviewLastUpdate = 1527084006;

  LastUpdateInfoType expected = lastUpdateInfoType;
  LastUpdateInfoType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, tileLastUpdateQuery.Get(tileXY, actual) == false,
                "TileLastUpdate Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, tileLastUpdateQuery.Write(tileXY, std::move(lastUpdateInfoType)),
                "TileLastUpdate Write");
  TF_assert_msg(state, tileLastUpdateQuery.Get(tileXY, actual), "TileLastUpdate Get");
  TF_assert_msg(state, tileLastUpdateQuery.Delete(tileXY), "TileLastUpdate Delete");
  TF_assert_msg(state, tileLastUpdateQuery.Get(tileXY, actual) == false,
                "TileLastUpdate not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "TileLastUpdate");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test tile last update operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_tile_last_update_bbox") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TileLastUpdateQuery tileLastUpdateQuery{database};
  TileXY tileXY{4, 6};
  bbox_type bbox{{1599, 1599}, {0, 0}};

  PopulateTilesTable(state, database);

  LastUpdateInfoType lastUpdateInfoType;
  lastUpdateInfoType.mMarkerLastUpdate = 1527084005;
  lastUpdateInfoType.mUserReviewLastUpdate = 1527084006;

  std::map<TileXY, LastUpdateInfoType> expected;
  std::map<TileXY, LastUpdateInfoType> expectedDefault;

  for (int x = 0; x < 16; x++) {
    for (int y = 0; y < 16; y++) {
      TileXY expectedTileXY{x, y};

      if (x == tileXY.mX && y == tileXY.mY) {
        expected[expectedTileXY] = lastUpdateInfoType;
      } else {
        expected[expectedTileXY] = LastUpdateInfoType();
      }
      expectedDefault[expectedTileXY] = LastUpdateInfoType();
    }
  }

  std::map<TileXY, LastUpdateInfoType> actual;
  std::map<TileXY, LastUpdateInfoType> actualDefault;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, tileLastUpdateQuery.GetBbox(bbox, actualDefault),
                "TileLastUpdate GetBbox default");  // Will return zeroes for all tiles.
  TF_assert_msg(state, tileLastUpdateQuery.Write(tileXY, std::move(lastUpdateInfoType)),
                "TileLastUpdate Bbox Write");
  TF_assert_msg(state, tileLastUpdateQuery.GetBbox(bbox, actual), "TileLastUpdate GetBbox");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "TileLastUpdate Bbox");
  TF_assert_msg(state, expectedDefault == actualDefault, "TileLastUpdate Bbox Defaults");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test tiles operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_tiles") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TilesQuery tilesQuery{database};

  PopulateDatabase(state, database);

  TileTableDataType tileTableData(3, 4, 67000, 67999);

  TileTableDataType expected = tileTableData;
  TileTableDataType actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, tilesQuery.Get(3, 4, actual), "Tiles Get.");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Tiles");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test tiles bbox operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_tiles_bbox") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TilesQuery tilesQuery{database};

  PopulateDatabase(state, database);

  std::vector<TileTableDataType> expected{
      TileTableDataType(8, 8, 136000, 136999),  TileTableDataType(9, 8, 137000, 137999),
      TileTableDataType(10, 8, 138000, 138999), TileTableDataType(8, 9, 152000, 152999),
      TileTableDataType(9, 9, 153000, 153999),  TileTableDataType(10, 9, 154000, 154999),
      TileTableDataType(8, 10, 168000, 168999), TileTableDataType(9, 10, 169000, 169999),
      TileTableDataType(10, 10, 170000, 170999)};

  std::vector<TileTableDataType> actual;

  bbox_type bbox;
  bbox.nec = {1050, 1050};
  bbox.swc = {850, 850};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, tilesQuery.GetBbox(bbox, actual), "Tiles bbox Get.");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Tiles bbox");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test translation operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_translations") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  LanguageQuery languageQuery{database};
  TranslatorQuery translatorQuery{database};
  std::string isoCode = "en_US";

  std::vector<LanguageTableDataType> languageTableData{{1, "en_US"}};

  std::vector<TranslationTableDataType> translationTableData{
      {1, 1, "[1]"}, {2, 1, "[2]"}, {3, 1, "[3]"}};

  std::vector<TranslationDataType> expected;
  for (auto translation : translationTableData) {
    expected.push_back(TranslationDataType{translation.mId, translation.mTranslation});
  }
  std::vector<TranslationDataType> actual;

  std::vector<TranslationTableDataType> expectedAll = translationTableData;
  std::vector<TranslationTableDataType> actualAll;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  for (auto language : languageTableData) {
    TF_assert_msg(state, languageQuery.Write(std::move(language)), "Translation Write Language");
  }

  TF_assert_msg(state, translatorQuery.Get(isoCode, actual) == false,
                "Translation Get: expected false.");  // Should not exist yet.
  for (auto translation : translationTableData) {
    TF_assert_msg(state, translatorQuery.Write(std::move(translation)), "Translation Write");
  }
  TF_assert_msg(state, translatorQuery.Get(isoCode, actual), "Translation Get");

  TF_assert_msg(state, translatorQuery.GetAll(actualAll), "Translation GetAll");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Translation");
  TF_assert_msg(state, expectedAll == actualAll, "Translation");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test version operations.
//!
//----------------------------------------------------------------
TF_TEST("acdb.database_version") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  VersionQuery versionQuery{database};

  std::string version = "1.2.3.4";

  std::string expected = version;
  std::string actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, versionQuery.Get(actual) == false,
                "Version Get: expected false.");  // Should not exist yet.
  TF_assert_msg(state, versionQuery.Put(std::move(version)), "Version Write");
  TF_assert_msg(state, versionQuery.Get(actual), "Version Get");
  TF_assert_msg(state, versionQuery.Delete(), "Version Delete");
  TF_assert_msg(state, versionQuery.Get(actual) == false, "Version not deleted");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Version");
}

}  // end of namespace Test
}  // end of namespace Acdb
