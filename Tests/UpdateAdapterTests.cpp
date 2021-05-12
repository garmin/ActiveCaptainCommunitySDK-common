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
    @brief Regression tests for the UpdateAdapter

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "UpdateAdapterTests"

#include "Acdb/UpdateAdapter.hpp"
#include "Acdb/PresentationAdapter.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/StringFormatter.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/Tests/SettingsUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/TextTranslator.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
using namespace Presentation;

namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test deleting markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.delete_markers") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  TranslationUtil translationUtil{state};

  MarkerTableDataCollection markerUpdate;
  markerUpdate.mMarker.mId = markerId;
  markerUpdate.mIsDeleted = true;

  std::vector<MarkerTableDataCollection> markerUpdates;
  markerUpdates.push_back(std::move(markerUpdate));

  uint64_t lastUpdateMax = 0;

  PresentationMarkerPtr expected = nullptr;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.UpdateMarkers(markerUpdates, lastUpdateMax), "Delete Markers");

  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Delete Markers: Expected nullptr");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test deleting reviews.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.delete_reviews") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  std::vector<ReviewTableDataCollection> reviewUpdates{
      ReviewTableDataCollection{ReviewTableDataType{1, markerId, 0, 0, "", "", "", "", 0, true,
                                                    std::string{}}},  // Will be deleted
  };

  ReviewDetail expected(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReviewsTitle)),
      std::unique_ptr<ReviewField>(new ReviewField(
          "Review Title 2", 2, "23-[175]-2018", "Test Captain 2", "Review Text 2", true,
          std::unique_ptr<LinkField>(new LinkField("vote/1/2", std::string())), 0,
          LinkField("report/1/2",
                    TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel))),
          nullptr,
          std::vector<ReviewPhotoField>{
              ReviewPhotoField("https://activecaptain.garmin.com/photos/10102.jpg"),
              ReviewPhotoField("https://activecaptain.garmin.com/photos/10102-2.jpg")})),
      LinkField("edit/1/Reviews",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::WriteReview))),
      LinkField("seeAll/1/Reviews/1",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll))),
      ReviewSummaryPtr(new ReviewSummary(2.75, 3, true)));

  std::vector<StringField> expectedStarValues{StringField("Full"), StringField("Full"),
                                              StringField("Half"), StringField("Empty"),
                                              StringField("Empty")};

  uint64_t lastUpdateMax = 0;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.UpdateReviews(reviewUpdates, lastUpdateMax), "Delete Reviews");

  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "Delete Reviews: Unexpected nullptr");
  TF_assert_msg(state, expected == *(actual->GetReviewDetail()), "Delete Reviews: ReviewDetail");
  TF_assert_msg(state, actual->GetMarkerDetail().GetReviewSummary() != nullptr,
                "Delete Reviews: ReviewSummary unexpected nullptr");
  TF_assert_msg(state,
                *(expected.GetReviewSummary()) == *(actual->GetMarkerDetail().GetReviewSummary()),
                "Delete Reviews: ReviewSummary");
  TF_assert_msg(state,
                expectedStarValues == actual->GetMarkerDetail().GetReviewSummary()->GetStarValues(),
                "Delete Reviews: ReviewSummary stars");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test deleting tile.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.delete_tile") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  MarkerTableDataCollection markerUpdate;
  markerUpdate.mMarker.mId = markerId;
  markerUpdate.mIsDeleted = true;

  std::vector<MarkerTableDataCollection> markerUpdates;
  markerUpdates.push_back(std::move(markerUpdate));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.DeleteTile(TileXY(1, 1)), "Delete tile");

  PresentationMarkerPtr actualDeleted = presentationAdapter.GetMarker(1);
  PresentationMarkerPtr actualNotDeleted = presentationAdapter.GetMarker(2);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actualDeleted == nullptr, "Delete tile: Expected nullptr");
  TF_assert_msg(state, actualNotDeleted != nullptr, "Delete tile: Unexpected nullptr");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test deleting tile reviews.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.delete_tile_reviews") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  MarkerTableDataCollection markerUpdate;
  markerUpdate.mMarker.mId = markerId;
  markerUpdate.mIsDeleted = true;

  std::vector<MarkerTableDataCollection> markerUpdates;
  markerUpdates.push_back(std::move(markerUpdate));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.DeleteTileReviews(TileXY(1, 1)), "Delete tile reviews");

  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);
  PresentationMarkerPtr actual2 = presentationAdapter.GetMarker(2);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual->GetReviewDetail() != nullptr,
                "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual->GetReviewDetail()->GetReviewSummary() != nullptr,
                "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual->GetReviewDetail()->GetReviewSummary()->GetReviewCount() == 0,
                "Delete tile reviews: Expected 0");
  TF_assert_msg(state, actual2 != nullptr, "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual2->GetReviewDetail() != nullptr,
                "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual2->GetReviewDetail()->GetReviewSummary() != nullptr,
                "Delete tile reviews: Unexpected nullptr");
  TF_assert_msg(state, actual2->GetReviewDetail()->GetReviewSummary()->GetReviewCount() > 0,
                "Delete tile reviews: Expected nonzero");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test updating markers.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.update_markers") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  MarkerTableDataType updatedMarkerTableData(markerId, ACDB_MARINA, 1527084000,
                                             "Updated test Marina 1", {100, 100}, 0, 0,
                                             ACDB_INVALID_BUSINESS_PROGRAM_TIER);
  MarkerMetaTableDataType updatedMarkerMetaTableData(
      "{ \"value\": \"Updated marker note here.\", \"isDistance\": false }",  // SectionNote
      (ACDB_text_handle_type)TextHandle::SummaryTitle                         // SectionTitle
  );

  MarkerTableDataCollection markerUpdate;
  markerUpdate.mMarker = updatedMarkerTableData;
  markerUpdate.mMarkerMeta = updatedMarkerMetaTableData;
  markerUpdate.mAddress = nullptr;
  markerUpdate.mAmenities = nullptr;
  markerUpdate.mBusiness = nullptr;
  markerUpdate.mContact = nullptr;
  markerUpdate.mDockage = nullptr;
  markerUpdate.mFuel = nullptr;
  markerUpdate.mMoorings = nullptr;
  markerUpdate.mNavigation = nullptr;
  markerUpdate.mRetail = nullptr;
  markerUpdate.mServices = nullptr;

  std::vector<MarkerTableDataCollection> markerUpdates;
  markerUpdates.push_back(std::move(markerUpdate));

  static const char* DEGREE_SYMBOL = "\xC2\xB0";

  MarkerDetail expected(
      "Updated test Marina 1", ReviewSummaryPtr(new ReviewSummary(2.25, 4, true)),
      AttributeField(
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::DateLastModifiedLabel)),
          "23-[175]-2018", "", ""),
      StringField(
          String::Format("00%s00.001'[148], 00%s00.001'[150]", DEGREE_SYMBOL, DEGREE_SYMBOL)),
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SummaryTitle)),
      AttributeField("",
                     TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MarinaValue)),
                     "", ""),
      std::unique_ptr<AttributeField>(new AttributeField("", "Updated marker note here.", "", "")),
      LinkField("edit/1/Summary", ""), nullptr, nullptr);

  uint64_t lastUpdateMax = 0;
  uint64_t expectedLastUpdateMax = 1527084000;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.UpdateMarkers(markerUpdates, lastUpdateMax), "Update Markers");

  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "Update Markers: Unexpected nullptr");
  TF_assert_msg(state, expected == actual->GetMarkerDetail(), "Update Markers: MarkerDetail");
  TF_assert_msg(state, expectedLastUpdateMax == lastUpdateMax, "Update Markers: lastUpdateMax");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test updating reviews.
//!
//----------------------------------------------------------------
TF_TEST("acdb.updateadapter.update_reviews") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  UpdateAdapter updateAdapter{database};
  PresentationAdapter presentationAdapter{database};

  ACDB_marker_idx_type markerId = 1;

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  std::vector<ReviewTableDataCollection> reviewUpdates{
      ReviewTableDataCollection{
          ReviewTableDataType{5, markerId, 1527084000, 4, "Review Title 5", "2018-05-23T00:00:00Z",
                              "Test Captain 5", "Review Text 5", 1, false, std::string{}},
          std::vector<ReviewPhotoTableDataType>{ReviewPhotoTableDataType{
              5, 1, "https://activecaptain.garmin.com/photos/10105.jpg"}}},  // Will be added
      ReviewTableDataCollection{ReviewTableDataType{
          3, markerId, 1527084000, 4, "Review Title 3", "2018-05-24T00:00:00Z", "Test Captain 3",
          "Updated Review Text 3", 0, false, std::string{}}}  // Will be edited
  };

  ReviewDetail expected(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReviewsTitle)),
      std::unique_ptr<ReviewField>(new ReviewField(
          "Review Title 5", 4, "23-[175]-2018", "Test Captain 5", "Review Text 5", true,
          std::unique_ptr<LinkField>(new LinkField("vote/1/5", std::string())), 1,
          LinkField("report/1/5",
                    TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel))),
          nullptr,
          std::vector<ReviewPhotoField>{ReviewPhotoField(
              "https://activecaptain.garmin.com/photos/10105.jpg")})),  // Has most votes.
      LinkField("edit/1/Reviews",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::WriteReview))),
      LinkField("seeAll/1/Reviews/1",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll))),
      ReviewSummaryPtr(new ReviewSummary(2.8f, 5, true)));

  std::vector<StringField> expectedStarValues{StringField("Full"), StringField("Full"),
                                              StringField("Half"), StringField("Empty"),
                                              StringField("Empty")};

  uint64_t lastUpdateMax = 0;
  uint64_t expectedLastUpdateMax = 1527084000;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  TF_assert_msg(state, updateAdapter.UpdateReviews(reviewUpdates, lastUpdateMax), "Update Reviews");

  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "Update Reviews: Unexpected nullptr");
  TF_assert_msg(state, expected == *(actual->GetReviewDetail()), "Update Reviews: ReviewDetail");
  TF_assert_msg(state, actual->GetReviewDetail()->GetReviewSummary() != nullptr,
                "Update Reviews: ReviewSummary unexpected nullptr");
  TF_assert_msg(
      state, expectedStarValues == actual->GetReviewDetail()->GetReviewSummary()->GetStarValues(),
      "Update Reviews: ReviewSummary stars");
  TF_assert_msg(state, expectedLastUpdateMax == lastUpdateMax, "Update Markers: lastUpdateMax");
}

}  // end of namespace Test
}  // end of namespace Acdb
