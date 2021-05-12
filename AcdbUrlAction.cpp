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
    Functions for processing acdb:// URLs.

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "AcdbUrlAction"

#include <inttypes.h>
#include <map>
#include "Acdb/AcdbUrlAction.hpp"
#include "Acdb/StringUtil.hpp"
#include "DBG_pub.h"

namespace Acdb {
const std::string ReviewsSection{"Reviews"};

bool ParseActionType(const std::string& aUrl, AcdbUrlAction::ActionType& aActionType_out,
                     std::string& aRemainingUrl_out);

bool ParseRemainingTokens(const AcdbUrlAction::ActionType aActionType,
                          const std::string& aRemainingUrl, AcdbUrlActionPtr& aAction_out);

// AcdbUrlAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
AcdbUrlAction::AcdbUrlAction(ActionType aAction) : mAction{aAction} {}  // end of AcdbUrlAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
AcdbUrlAction::ActionType AcdbUrlAction::GetAction() const { return mAction; }  // end of GetAction

// EditAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
EditAction::EditAction(std::string&& aUrl)
    : AcdbUrlAction(ActionType::Edit), mUrl(std::move(aUrl)) {}  // end of EditAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
std::string EditAction::GetUrl() const { return mUrl; }  // end of GetUrl

// ReportReviewAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReportReviewAction::ReportReviewAction(std::string&& aUrl)
    : AcdbUrlAction(ActionType::ReportReview), mUrl(aUrl) {}  // end of ReportReviewAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
std::string ReportReviewAction::GetUrl() const { return mUrl; }  // end of GetUrl

// SeeAllAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
SeeAllAction::SeeAllAction(const ACDB_marker_idx_type aMarkerId, const std::string&& aSection,
                           const uint32_t aPageNumber)
    : AcdbUrlAction(ActionType::SeeAll),
      mMarkerId(aMarkerId),
      mPageNumber(aPageNumber),
      mSection(aSection) {}  // end of SeeAllAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
ACDB_marker_idx_type SeeAllAction::GetMarkerId() const { return mMarkerId; }  // end of GetMarkerId

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
std::string SeeAllAction::GetSection() const { return mSection; }  // end of GetSection

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
uint32_t SeeAllAction::GetPageNumber() const { return mPageNumber; }  // end of GetPageNumber

// ShowPhotosAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ShowPhotosAction::ShowPhotosAction(const ACDB_marker_idx_type aMarkerId)
    : AcdbUrlAction(ActionType::ShowPhotos), mMarkerId(aMarkerId) {}  // end of ShowPhotosAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
ACDB_marker_idx_type ShowPhotosAction::GetMarkerId() const {
  return mMarkerId;
}  // end of GetMarkerId

// ShowSummaryAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ShowSummaryAction::ShowSummaryAction(const ACDB_marker_idx_type aMarkerId)
    : AcdbUrlAction(ActionType::ShowSummary), mMarkerId(aMarkerId) {}  // end of ShowSummaryAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
ACDB_marker_idx_type ShowSummaryAction::GetMarkerId() const {
  return mMarkerId;
}  // end of GetMarkerId

// ReportReviewAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
VoteReviewAction::VoteReviewAction(const ACDB_marker_idx_type aMarkerId,
                                   const ACDB_review_idx_type aReviewId)
    : AcdbUrlAction(ActionType::VoteReview),
      mMarkerId(aMarkerId),
      mReviewId(aReviewId) {}  // end of VoteReviewAction

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
ACDB_marker_idx_type VoteReviewAction::GetMarkerId() const {
  return mMarkerId;
}  // end of GetMarkerId

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//----------------------------------------------------------------
ACDB_review_idx_type VoteReviewAction::GetReviewId() const {
  return mReviewId;
}  // end of GetReviewId

// Parser

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Parse an "acdb://" URL into an AcdbUrlAction type.
//!
//----------------------------------------------------------------
bool ParseAcdbUrl(const std::string& aUrl, AcdbUrlActionPtr& aAction_out) {
  const std::string expectedProtocol{"acdb://"};

  // Check protocol
  size_t foundPosition = aUrl.find(expectedProtocol);
  if (foundPosition != 0) {
    DBG_I("Not an ACDB URL (protocol mismatch)");
    return false;
  }

  // Get action
  std::string remainingUrl = aUrl.substr(expectedProtocol.size());

  AcdbUrlAction::ActionType actionType;
  bool success = ParseActionType(remainingUrl, actionType, remainingUrl);

  success = success && ParseRemainingTokens(actionType, remainingUrl, aAction_out);

  return success;
}  // end of ParseAcdbUrl

//----------------------------------------------------------------
//!
//!   @public
//!   @brief checks if a section name matches the "reviews" section
//!
//----------------------------------------------------------------
bool IsReviewsSection(const std::string& aSectionName) {
  return (aSectionName == ReviewsSection);
}  // end of ParseAcdbUrl

//----------------------------------------------------------------
//!
//!   @private
//!   @brief Parse the action type from an "acdb://" URL.
//!
//----------------------------------------------------------------
bool ParseActionType(const std::string& aUrl, AcdbUrlAction::ActionType& aActionType_out,
                     std::string& aRemainingUrl_out) {
  static const std::map<std::string, AcdbUrlAction::ActionType> ActionStringMap = {
      {"edit", AcdbUrlAction::ActionType::Edit},
      {"report", AcdbUrlAction::ActionType::ReportReview},
      {"seeAll", AcdbUrlAction::ActionType::SeeAll},
      {"photos", AcdbUrlAction::ActionType::ShowPhotos},
      {"summary", AcdbUrlAction::ActionType::ShowSummary},
      {"vote", AcdbUrlAction::ActionType::VoteReview}};

  // Get action
  std::string actionStr = aUrl.substr(0, aUrl.find('/'));
  auto actionIt = ActionStringMap.find(actionStr);
  if (actionIt == ActionStringMap.end()) {
    DBG_ASSERT_ALWAYS("Invalid URL (unknown action)");
    return false;
  }

  aActionType_out = actionIt->second;
  if (aUrl.length() > actionStr.length()) {
    aRemainingUrl_out = aUrl.substr(actionStr.length() + 1);
  } else {
    aRemainingUrl_out.clear();
  }

  return true;
}  // end of ParseActionType

//----------------------------------------------------------------
//!
//!   @private
//!   @brief Parse tokens after action type from an "acdb://" URL.
//!   @output Unique ptr to AcdbUrlAction subclass for the
//!           requested action.
//!
//----------------------------------------------------------------
bool ParseRemainingTokens(const AcdbUrlAction::ActionType aActionType,
                          const std::string& aRemainingUrl, AcdbUrlActionPtr& aAction_out) {
  std::vector<std::string> tokens = String::Split(aRemainingUrl, '/');

  bool success{true};

  switch (aActionType) {
    case AcdbUrlAction::ActionType::Edit: {
      // Format: <markerId>/<sectionName>
      success = (tokens.size() == 2);
      if (success) {
        const std::string FormatString = "embed/poi/%" PRIu64 "/%s/edit";
        std::string url =
            String::Format(FormatString.c_str(), String::ToUInt64(tokens[0]), tokens[1].c_str());

        aAction_out.reset(new EditAction(std::move(url)));
      }
    } break;

    case AcdbUrlAction::ActionType::ReportReview: {
      // Format: <markerId>/<reviewId>
      success = (tokens.size() == 2);
      if (success) {
        const std::string FormatString = "embed/poi/%" PRIu64 "/reviews/edit/%" PRIu64;
        std::string url = String::Format(FormatString.c_str(), String::ToUInt64(tokens[0]),
                                         String::ToUInt64(tokens[1]));

        aAction_out.reset(new ReportReviewAction(std::move(url)));
      }
    } break;

    case AcdbUrlAction::ActionType::SeeAll: {
      // Format: <markerId>/<sectionName> or <markerId>/Reviews/<pageNumber>
      uint32_t pageNumber = 0;

      // Must have markerId and section.  If Reviews section, must have page number; otherwise, must
      // not.
      if (tokens.size() == 3 && tokens[1] == ReviewsSection) {
        pageNumber = String::ToUInt(tokens[2]);
      } else if (tokens.size() != 2 || tokens[1] == ReviewsSection) {
        success = false;
      }

      if (success) {
        aAction_out.reset(
            new SeeAllAction(String::ToUInt64(tokens[0]), std::move(tokens[1]), pageNumber));
      }
    } break;

    case AcdbUrlAction::ActionType::ShowPhotos:
      // Format: <markerId>
      success = (tokens.size() == 1);
      if (success) {
        aAction_out.reset(new ShowPhotosAction(String::ToUInt64(tokens[0])));
      }
      break;

    case AcdbUrlAction::ActionType::ShowSummary:
      // Format: <markerId>
      success = (tokens.size() == 1);
      if (success) {
        aAction_out.reset(new ShowSummaryAction(String::ToUInt64(tokens[0])));
      }
      break;

    case AcdbUrlAction::ActionType::VoteReview:
      // Format: <markerId>/<reviewId>
      success = (tokens.size() == 2);
      if (success) {
        aAction_out.reset(
            new VoteReviewAction(String::ToUInt64(tokens[0]), String::ToUInt64(tokens[1])));
      }
      break;
  }

  return success;

}  // end of ParseRemainingTokens
}  // end of namespace Acdb
