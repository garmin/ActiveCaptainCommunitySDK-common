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
    @brief Regression tests for the MarkerAdapter

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MarkerAdapterTests"

#include <algorithm>

#include "Acdb/MarkerAdapter.hpp"
#include "Acdb/MapMarker.hpp"
#include "Acdb/SearchMarker.hpp"
#include "Acdb/TableDataTypes.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextTranslator.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_marker") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};

  IMapMarkerPtr expected(new MapMarker(1, ACDB_MARINA, 1527084000, "Test Marina 1", 100, 100,
                                       MapIconType::MarinaSponsor));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  IMapMarkerPtr actual = markerAdapter.GetMapMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, nullptr != actual, "Marker: unexpected nullptr");
  TF_assert_msg(state, expected->GetId() == actual->GetId(), "Marker: ID");
  TF_assert_msg(state, expected->GetLastUpdated() == actual->GetLastUpdated(),
                "Marker: LastUpdated");
  TF_assert_msg(state, expected->GetMapIcon() == actual->GetMapIcon(), "Marker: MapIcon");
  TF_assert_msg(state, expected->GetName() == actual->GetName(), "Marker: Name");
  TF_assert_msg(state, expected->GetPosition().lat == actual->GetPosition().lat,
                "Marker: Position lat");
  TF_assert_msg(state, expected->GetPosition().lon == actual->GetPosition().lon,
                "Marker: Position lon");
  TF_assert_msg(state, expected->GetType() == actual->GetType(), "Marker: Type");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving invalid marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_marker_invalid") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};

  IMapMarkerPtr expected = nullptr;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  IMapMarkerPtr actual = markerAdapter.GetMapMarker(ACDB_INVALID_MARKER_IDX);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Marker: expected nullptr");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving average stars.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_marker_avg_stars") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};

  float expected = 2.25;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  float actual = markerAdapter.GetAverageStars(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Average rating: expected %f, actual = %f", expected,
                actual);
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving average stars for an invalid marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_marker_avg_stars_invalid_marker") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};

  float expected = ACDB_INVALID_STAR_COUNT;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  float actual = markerAdapter.GetAverageStars(ACDB_INVALID_MARKER_IDX);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Average rating: expected %f, actual = %f", expected,
                actual);
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving nearest markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_marker_nearest") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};

  ACDB_marker_filter_type filter;
  filter.bbox.nec = {350, 350};
  filter.bbox.swc = {150, 150};
  filter.include_marinas = true;
  filter.include_hazards = false;
  filter.include_anchorages = false;
  filter.include_local_knowledge = false;
  filter.search_string = nullptr;
  filter.max_num_results = 10;

  MapMarkerFilter markerFilter(&filter);

  // Expected:
  // - 1 and 4 are outside bbox
  // - 5 is inside bbox but wrong type
  // - 2, 3, 21 and 22 should be good
  const std::vector<ACDB_marker_idx_type> expected = {2, 3, 21, 22};
  std::vector<IMapMarkerPtr> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  markerAdapter.GetMapMarkersByFilter(markerFilter, actual);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(),
                "Nearest markers count: expected %d, actual = %d", expected.size(), actual.size());
  for (std::vector<IMapMarkerPtr>::iterator it = actual.begin(); it != actual.end(); ++it) {
    TF_assert_msg(state, find(expected.begin(), expected.end(), (*it)->GetId()) != expected.end(),
                  "Nearest markers: Unexpected result %d", (*it)->GetId());
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  ReviewStatsData reviewStatsData(2.25f,  // aAverageRating,
                                  4       // aNumberOfReviews
  );
  ContactData contactData("555-1234",  // aPhoneNumber
                          "789"        // aVhfChannel
  );
  FuelData fuelData(1.6f,       // aGasPrice,
                    1.2f,       // aDieselPrice
                    "USD",      // aFuelPriceCurrency
                    ACDB_LITER  // aFuelPriceUnit
  );

  ISearchMarkerPtr expected(new SearchMarker(
      1, ACDB_MARINA, 1527084000, "Test Marina 1", 100, 100, MapIconType::MarinaSponsor,
      std::move(contactData), std::move(fuelData), std::move(reviewStatsData), "[10]"));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  ISearchMarkerPtr actual = markerAdapter.GetSearchMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, nullptr != actual, "SearchMarker: unexpected nullptr");
  TF_assert_msg(state, expected->GetId() == actual->GetId(), "SearchMarker: ID");
  TF_assert_msg(state, expected->GetLastUpdated() == actual->GetLastUpdated(),
                "SearchMarker: LastUpdated");
  TF_assert_msg(state, expected->GetMapIcon() == actual->GetMapIcon(), "SearchMarker: MapIcon");
  TF_assert_msg(state, expected->GetName() == actual->GetName(), "SearchMarker: Name");
  TF_assert_msg(state, expected->GetPosition().lat == actual->GetPosition().lat,
                "SearchMarker: Position lat");
  TF_assert_msg(state, expected->GetPosition().lon == actual->GetPosition().lon,
                "SearchMarker: Position lon");
  TF_assert_msg(state, expected->GetType() == actual->GetType(), "SearchMarker: Type");

  TF_assert_msg(state, expected->GetAverageRating() == actual->GetAverageRating(),
                "SearchMarker: Average Rating");
  TF_assert_msg(state, expected->GetNumberOfReviews() == actual->GetNumberOfReviews(),
                "SearchMarker: Number of reviews");
  TF_assert_msg(state, expected->GetPhoneNumber() == actual->GetPhoneNumber(),
                "SearchMarker: Phone Number");
  TF_assert_msg(state, expected->GetVhfChannel() == actual->GetVhfChannel(),
                "SearchMarker: VHF Channel");

  // check fuel info...
  ISearchMarker::FuelPriceUnit expectedPriceUnit;
  std::string expectedCurrency;
  float expectedPrice;

  ISearchMarker::FuelPriceUnit actualPriceUnit;
  std::string actualCurrency;
  float actualPrice;

  // check gas
  expected->GetFuelPriceInfo(ISearchMarker::FuelType::Gas, expectedPrice, expectedCurrency,
                             expectedPriceUnit);
  actual->GetFuelPriceInfo(ISearchMarker::FuelType::Gas, actualPrice, actualCurrency,
                           actualPriceUnit);
  TF_assert_msg(state, expectedPriceUnit == ISearchMarker::FuelPriceUnit::PerLiter,
                "SearchMarker: Price Unit - raw value to enum conversion");
  TF_assert_msg(state, expectedPrice == actualPrice, "SearchMarker: Gas Price");
  TF_assert_msg(state, expectedCurrency == actualCurrency, "SearchMarker: Gas Currency");
  TF_assert_msg(state, expectedPriceUnit == actualPriceUnit, "SearchMarker: Gas Unit");

  // check Diesel
  expected->GetFuelPriceInfo(ISearchMarker::FuelType::Diesel, expectedPrice, expectedCurrency,
                             expectedPriceUnit);
  actual->GetFuelPriceInfo(ISearchMarker::FuelType::Diesel, actualPrice, actualCurrency,
                           actualPriceUnit);
  TF_assert_msg(state, expectedPrice == actualPrice, "SearchMarker: Diesel Price");
  TF_assert_msg(state, expectedCurrency == actualCurrency, "SearchMarker: Diesel Currency");
  TF_assert_msg(state, expectedPriceUnit == actualPriceUnit, "SearchMarker: Diesel Unit");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker_noExtraInfo") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  ReviewStatsData reviewStatsData(3, 1);
  ContactData contactData;
  FuelData fuelData;

  ISearchMarkerPtr expected(new SearchMarker(
      2, ACDB_MARINA, 1527084001, "Test Marina 2", 200, 200, MapIconType::Marina,
      std::move(contactData), std::move(fuelData), std::move(reviewStatsData), "[10]"));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  ISearchMarkerPtr actual = markerAdapter.GetSearchMarker(2);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, nullptr != actual, "SearchMarker: unexpected nullptr");
  TF_assert_msg(state, expected->GetId() == actual->GetId(), "SearchMarker: ID");
  TF_assert_msg(state, expected->GetLastUpdated() == actual->GetLastUpdated(),
                "SearchMarker: LastUpdated");
  TF_assert_msg(state, expected->GetMapIcon() == actual->GetMapIcon(), "SearchMarker: MapIcon");
  TF_assert_msg(state, expected->GetName() == actual->GetName(), "SearchMarker: Name");
  TF_assert_msg(state, expected->GetPosition().lat == actual->GetPosition().lat,
                "SearchMarker: Position lat");
  TF_assert_msg(state, expected->GetPosition().lon == actual->GetPosition().lon,
                "SearchMarker: Position lon");
  TF_assert_msg(state, expected->GetType() == actual->GetType(), "SearchMarker: Type");

  TF_assert_msg(state, expected->GetAverageRating() == actual->GetAverageRating(),
                "SearchMarker: Average Rating");
  TF_assert_msg(state, expected->GetNumberOfReviews() == actual->GetNumberOfReviews(),
                "SearchMarker: Number of reviews");
  TF_assert_msg(state, expected->GetPhoneNumber() == actual->GetPhoneNumber(),
                "SearchMarker: Phone Number");
  TF_assert_msg(state, expected->GetVhfChannel() == actual->GetVhfChannel(),
                "SearchMarker: VHF Channel");

  // check fuel info...
  ISearchMarker::FuelPriceUnit expectedPriceUnit;
  std::string expectedCurrency;
  float expectedPrice;

  ISearchMarker::FuelPriceUnit actualPriceUnit;
  std::string actualCurrency;
  float actualPrice;

  // check gas
  expected->GetFuelPriceInfo(ISearchMarker::FuelType::Gas, expectedPrice, expectedCurrency,
                             expectedPriceUnit);
  actual->GetFuelPriceInfo(ISearchMarker::FuelType::Gas, actualPrice, actualCurrency,
                           actualPriceUnit);
  TF_assert_msg(state, expectedPrice == actualPrice, "SearchMarker: Gas Price");
  TF_assert_msg(state, expectedCurrency == actualCurrency, "SearchMarker: Gas Currency");
  TF_assert_msg(state, expectedPriceUnit == actualPriceUnit, "SearchMarker: Gas Unit");

  // check Diesel
  expected->GetFuelPriceInfo(ISearchMarker::FuelType::Diesel, expectedPrice, expectedCurrency,
                             expectedPriceUnit);
  actual->GetFuelPriceInfo(ISearchMarker::FuelType::Diesel, actualPrice, actualCurrency,
                           actualPriceUnit);
  TF_assert_msg(state, expectedPrice == actualPrice, "SearchMarker: Diesel Price");
  TF_assert_msg(state, expectedCurrency == actualCurrency, "SearchMarker: Diesel Currency");
  TF_assert_msg(state, expectedPriceUnit == actualPriceUnit, "SearchMarker: Diesel Unit");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving invalid marker.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker_invalid") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  ISearchMarkerPtr expected = nullptr;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  ISearchMarkerPtr actual = markerAdapter.GetSearchMarker(ACDB_INVALID_MARKER_IDX);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "SearchMarker: expected nullptr");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving nearest markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker_filter_by_bbox_only") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  bbox_type filterBbox;
  filterBbox.nec = {350, 350};
  filterBbox.swc = {150, 150};

  SearchMarkerFilter markerFilter;
  markerFilter.AddType(ACDB_MARINA);
  markerFilter.SetBbox(filterBbox);

  // Expected:
  // - 1 and 4 are outside bbox
  // - 5 is inside bbox but wrong type
  // - 2, 3, 21 and 22 should be good
  const std::vector<ACDB_marker_idx_type> expected = {2, 3, 21, 22};
  std::vector<ISearchMarkerPtr> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  markerAdapter.GetSearchMarkersByFilter(markerFilter, actual);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(),
                "Search markers by Bbox: expected %d, actual = %d", expected.size(), actual.size());
  for (std::vector<ISearchMarkerPtr>::iterator it = actual.begin(); it != actual.end(); ++it) {
    TF_assert_msg(state, find(expected.begin(), expected.end(), (*it)->GetId()) != expected.end(),
                  "Search markers by Bbox: Unexpected result %d", (*it)->GetId());
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving nearest markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker_filter_by_bbox_and_name") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  bbox_type filterBbox;
  filterBbox.nec = {350, 350};
  filterBbox.swc = {150, 150};
  std::string searchString("Yet Another");

  SearchMarkerFilter markerFilter;
  markerFilter.AddType(ACDB_MARINA);
  markerFilter.SetBbox(filterBbox);
  markerFilter.SetSearchString(searchString);

  // Expected:
  // - 1 and 4 are outside bbox
  // - 5 is inside bbox but wrong type
  // - 2 and 3 are inside, but the name does not match
  // - 21 and 22 should be good
  const std::vector<ACDB_marker_idx_type> expected = {21, 22};
  std::vector<ISearchMarkerPtr> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  markerAdapter.GetSearchMarkersByFilter(markerFilter, actual);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(),
                "Search markers by Bbox: expected %d, actual = %d", expected.size(), actual.size());
  for (std::vector<ISearchMarkerPtr>::iterator it = actual.begin(); it != actual.end(); ++it) {
    TF_assert_msg(state, find(expected.begin(), expected.end(), (*it)->GetId()) != expected.end(),
                  "Search markers by Bbox: Unexpected result %d", (*it)->GetId());
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving nearest markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.markeradapter.get_searchmarker_filter_by_bbox_and_name_and_category") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  MarkerAdapter markerAdapter{database};
  TranslationUtil translationUtil{state};

  bbox_type filterBbox;
  filterBbox.nec = {350, 350};
  filterBbox.swc = {150, 150};
  std::string searchString("Yet Another");

  SearchMarkerFilter markerFilter;
  markerFilter.AddType(ACDB_MARINA);
  markerFilter.SetBbox(filterBbox);
  markerFilter.SetSearchString(searchString);
  markerFilter.AddCategory(SearchMarkerFilter::MarinasAndMoorings);

  // Expected:
  // - 1 and 4 are outside bbox
  // - 5 is inside bbox but wrong type
  // - 2 and 3 are inside, but the name does not match
  // - 22 is inside, name matches, but category is not set
  // - 21 should be good
  const std::vector<ACDB_marker_idx_type> expected = {21};
  std::vector<ISearchMarkerPtr> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  markerAdapter.GetSearchMarkersByFilter(markerFilter, actual);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(),
                "Search markers by Bbox: expected %d, actual = %d", expected.size(), actual.size());
  for (std::vector<ISearchMarkerPtr>::iterator it = actual.begin(); it != actual.end(); ++it) {
    TF_assert_msg(state, find(expected.begin(), expected.end(), (*it)->GetId()) != expected.end(),
                  "Search markers by Bbox: Unexpected result %d", (*it)->GetId());
  }
}

}  // end of namespace Test
}  // end of namespace Acdb
