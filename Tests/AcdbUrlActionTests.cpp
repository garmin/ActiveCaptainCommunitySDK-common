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
    @brief Regression tests for the AcdbUrlAction

    Copyright 2019 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "AcdbUrlActionTests"

#include <vector>
#include "Acdb/AcdbUrlAction.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing edit URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.edit") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://edit/9223372036854775807/Summary"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::Edit;
  std::string expectedUrl{"embed/poi/9223372036854775807/Summary/edit"};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  EditAction* editAction = static_cast<EditAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction Edit");
  TF_assert_msg(state, editAction != nullptr, "AcdbUrlAction: Edit");
  TF_assert_msg(state, expectedActionType == editAction->GetAction(), "AcdbUrlAction: Edit action");
  TF_assert_msg(state, expectedUrl == editAction->GetUrl(), "AcdbUrlAction: Edit markerId");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing invalid URLs.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.invalid") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::vector<std::string> invalidUrls = {
      // Invalid protocol or action will throw an assert.
      "acdb://edit",                                      // missing markerId and section
      "acdb://edit/9223372036854775807",                  // missing section
      "acdb://edit/9223372036854775807/summary/invalid",  // extra token
      "acdb://report",                                    // missing markerId and reviewId
      "acdb://report/9223372036854775807",                // missing reviewId
      "acdb://report/9223372036854775807/9223372036854775806/invalid",  // extra token
      "acdb://seeAll",                                        // missing section and markerId
      "acdb://seeAll/9223372036854775807",                    // missing section
      "acdb://seeAll/9223372036854775807/Amenities/invalid",  // extra token
      "acdb://seeAll/9223372036854775807/Reviews",            // missing page number
      "acdb://seeAll/9223372036854775807/Reviews/1/invalid",  // extra token
      "acdb://photos",                                        // missing markerId
      "acdb://photos/9223372036854775807/invalid",            // extra token
      "acdb://summary",                                       // missing markerId
      "acdb://summary/9223372036854775807/invalid",           // extra token
      "acdb://vote",                                          // missing markerId and reviewId
      "acdb://vote/9223372036854775807",                      // missing reviewId
      "acdb://vote/9223372036854775807/9223372036854775806/invalid"  // extra token
  };

  for (auto acdbUrl : invalidUrls) {
    // ----------------------------------------------------------
    // Act
    // ----------------------------------------------------------
    AcdbUrlActionPtr action;
    bool success = ParseAcdbUrl(acdbUrl, action);

    // ----------------------------------------------------------
    // Assert
    // ----------------------------------------------------------
    TF_assert_msg(state, !success, "AcdbUrlAction Invalid");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing report review URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.report_review") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://report/9223372036854775807/9223372036854775806"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::ReportReview;
  std::string expectedUrl{"embed/poi/9223372036854775807/reviews/edit/9223372036854775806"};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  ReportReviewAction* reportReviewAction = static_cast<ReportReviewAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction ReportReview");
  TF_assert_msg(state, reportReviewAction != nullptr, "AcdbUrlAction: ReportReview");
  TF_assert_msg(state, expectedActionType == reportReviewAction->GetAction(),
                "AcdbUrlAction: ReportReview action");
  TF_assert_msg(state, expectedUrl == reportReviewAction->GetUrl(),
                "AcdbUrlAction: ReportReview url");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing see all URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.see_all") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://seeAll/9223372036854775807/Amenities"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::SeeAll;
  ACDB_marker_idx_type expectedMarkerId = 9223372036854775807;
  std::string expectedSection = "Amenities";
  uint32_t expectedPageNumber = 0;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  SeeAllAction* seeAllAction = static_cast<SeeAllAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction SeeAll");
  TF_assert_msg(state, seeAllAction != nullptr, "AcdbUrlAction: SeeAll");
  TF_assert_msg(state, expectedActionType == seeAllAction->GetAction(),
                "AcdbUrlAction: SeeAll action");
  TF_assert_msg(state, expectedMarkerId == seeAllAction->GetMarkerId(),
                "AcdbUrlAction: SeeAll markerId");
  TF_assert_msg(state, expectedSection == seeAllAction->GetSection(),
                "AcdbUrlAction: SeeAll section");
  TF_assert_msg(state, expectedPageNumber == seeAllAction->GetPageNumber(),
                "AcdbUrlAction: SeeAll pageNumber");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing see all reviews URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.see_all_reviews") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://seeAll/9223372036854775807/Reviews/5"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::SeeAll;
  ACDB_marker_idx_type expectedMarkerId = 9223372036854775807;
  std::string expectedSection = "Reviews";
  uint32_t expectedPageNumber = 5;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  SeeAllAction* seeAllAction = static_cast<SeeAllAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction SeeAll");
  TF_assert_msg(state, seeAllAction != nullptr, "AcdbUrlAction: SeeAll");
  TF_assert_msg(state, expectedActionType == seeAllAction->GetAction(),
                "AcdbUrlAction: SeeAll action");
  TF_assert_msg(state, expectedMarkerId == seeAllAction->GetMarkerId(),
                "AcdbUrlAction: SeeAll markerId");
  TF_assert_msg(state, expectedSection == seeAllAction->GetSection(),
                "AcdbUrlAction: SeeAll section");
  TF_assert_msg(state, expectedPageNumber == seeAllAction->GetPageNumber(),
                "AcdbUrlAction: SeeAll pageNumber");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing show photos URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.show_photos") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://photos/9223372036854775807"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::ShowPhotos;
  ACDB_marker_idx_type expectedMarkerId = 9223372036854775807;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  ShowPhotosAction* showPhotosAction = static_cast<ShowPhotosAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction ShowPhotos");
  TF_assert_msg(state, showPhotosAction != nullptr, "AcdbUrlAction: ShowPhotos");
  TF_assert_msg(state, expectedActionType == showPhotosAction->GetAction(),
                "AcdbUrlAction: ShowPhotos action");
  TF_assert_msg(state, expectedMarkerId == showPhotosAction->GetMarkerId(),
                "AcdbUrlAction: ShowPhotos markerId");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing show summary URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.show_summary") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://summary/9223372036854775807"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::ShowSummary;
  ACDB_marker_idx_type expectedMarkerId = 9223372036854775807;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  ShowSummaryAction* showSummaryAction = static_cast<ShowSummaryAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction ShowSummary");
  TF_assert_msg(state, showSummaryAction != nullptr, "AcdbUrlAction: ShowSummary");
  TF_assert_msg(state, expectedActionType == showSummaryAction->GetAction(),
                "AcdbUrlAction: ShowSummary action");
  TF_assert_msg(state, expectedMarkerId == showSummaryAction->GetMarkerId(),
                "AcdbUrlAction: ShowSummary markerId");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test parsing vote review URL.
//!
//----------------------------------------------------------------
TF_TEST("acdb.urlaction.vote_review") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::string acdbUrl{"acdb://vote/9223372036854775807/9223372036854775806"};
  AcdbUrlAction::ActionType expectedActionType = AcdbUrlAction::ActionType::VoteReview;
  ACDB_marker_idx_type expectedMarkerId = 9223372036854775807;
  ACDB_review_idx_type expectedReviewId = 9223372036854775806;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  AcdbUrlActionPtr action;
  bool success = ParseAcdbUrl(acdbUrl, action);
  VoteReviewAction* voteReviewAction = static_cast<VoteReviewAction*>(action.get());

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, success, "AcdbUrlAction VoteReview");
  TF_assert_msg(state, voteReviewAction != nullptr, "AcdbUrlAction: VoteReview");
  TF_assert_msg(state, expectedActionType == voteReviewAction->GetAction(),
                "AcdbUrlAction: VoteReview action");
  TF_assert_msg(state, expectedMarkerId == voteReviewAction->GetMarkerId(),
                "AcdbUrlAction: VoteReview markerId");
  TF_assert_msg(state, expectedReviewId == voteReviewAction->GetReviewId(),
                "AcdbUrlAction: VoteReview reviewId");
}

}  // end of namespace Test
}  // end of namespace Acdb
