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
Definition of the ReviewParser.

Copyright 2018-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewParser"

#include <map>
#include <string>
#include <vector>

#include "DBG_pub.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Json/JsonParser.hpp"
#include "Acdb/Json/ReviewParser.hpp"
#include "rapidjson/document.h"
#include "UTL_pub_lib_cnvt.h"

namespace Acdb {
namespace Json {

static bool GetReviewStatus(const rapidjson::Value& aDocument, const char* aNodeName,
                            bool& aOutput);

static bool ParseReview(const rapidjson::Value& aDocument, ReviewTableDataCollection& aOutput);

static bool ParseReviewPhotos(const rapidjson::Value& aDocument,
                              std::vector<ReviewPhotoTableDataType>& aOutput);

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse webview response
//!
//----------------------------------------------------------------
bool ParseReviewWebViewResponse(rapidjson::Document& aJsonDocument,
                                ReviewTableDataCollection& aReview_out) {
  static const char idNode[] = "idStr";
  std::string resultTypeStr;
  auto resultTypeIt = aJsonDocument.FindMember("resultType");
  if (resultTypeIt == aJsonDocument.MemberEnd()) {
    return false;
  }

  resultTypeStr = String::ToUpper(resultTypeIt->value.GetString());

  auto dataIt = aJsonDocument.FindMember("data");
  bool success = (dataIt != aJsonDocument.MemberEnd());

  if (resultTypeStr == "REVIEWSUCCESS") {
    success = success && ParseReview(dataIt->value, aReview_out);
  } else if (resultTypeStr == "REVIEWDELETE" || resultTypeStr == "REVIEWFLAGGED") {
    auto reviewIdIt = dataIt->value.FindMember(idNode);
    if (reviewIdIt == dataIt->value.MemberEnd()) {
      success = false;
    } else {
      success = GetUint64(reviewIdIt->value, aReview_out.mReview.mId);
      aReview_out.mReview.mIsDeleted = true;
    }
  } else if (resultTypeStr == "ERROR") {
    success = false;
  } else {
    DBG_ASSERT_ALWAYS("Unknown review webview resultType");
    success = false;
  }

  return success;
}  // end of ParseReviewWebViewResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse review sync response
//!
//----------------------------------------------------------------
bool ParseReviewSyncResponse(const char* aJson, size_t aLength,
                             std::vector<ReviewTableDataCollection>& aReviews_out) {
  rapidjson::Document document;
  document.Parse(aJson, aLength);

  if (!document.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = document.GetArray().begin(); success && it != document.GetArray().end(); it++) {
    ReviewTableDataCollection reviewTableData;
    success = success && it->IsObject();
    success = success && ParseReview((*it), reviewTableData);
    if (success) {
      aReviews_out.push_back(std::move(reviewTableData));
    }
  }

  return success;
}  // end of ParseReviewSyncResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse vote for review response
//!
//----------------------------------------------------------------
bool ParseVoteForReviewResponse(const char* aJson, size_t aLength,
                                ReviewTableDataCollection& aReview_out) {
  rapidjson::Document document;
  document.Parse(aJson, aLength);
  if (!document.IsObject()) {
    return false;
  }

  return ParseReview(document.GetObject(), aReview_out);
}  // end of ParseVoteForReviewResponse

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse a review object
//!
//----------------------------------------------------------------
static bool ParseReview(const rapidjson::Value& aDocument, ReviewTableDataCollection& aOutput) {
  static const char captainNameNode[] = "captainName";
  static const char dateLastModifiedNode[] = "dateLastModified";
  static const char dateVisitedNode[] = "dateVisited";
  static const char idNode[] = "idStr";
  static const char markerIdNode[] = "poiIdStr";
  static const char photosNode[] = "photos";
  static const char ratingNode[] = "rating";
  static const char statusNode[] = "status";
  static const char textNode[] = "text";
  static const char titleNode[] = "title";
  static const char votesNode[] = "votes";
  static const char responseNode[] = "response";

  aOutput.mReview.mIsDeleted = false;

  bool success = GetUint64(aDocument, idNode, aOutput.mReview.mId);
  success = success && GetReviewStatus(aDocument, statusNode, aOutput.mReview.mIsDeleted);

  success =
      success && GetDateTimeEpoch(aDocument, dateLastModifiedNode, aOutput.mReview.mLastUpdated);

  // Short circuit -- if parsing failed before this point, or if the review was deleted, skip
  // further processing.
  if (!success || aOutput.mReview.mIsDeleted) {
    return success;
  }

  success = success && GetUint64(aDocument, markerIdNode, aOutput.mReview.mMarkerId);
  success = success && GetString(aDocument, captainNameNode, aOutput.mReview.mCaptain);
  success = success && GetString(aDocument, dateVisitedNode, aOutput.mReview.mDate);
  success = success && GetSint32(aDocument, ratingNode, aOutput.mReview.mRating);
  success = success && GetString(aDocument, textNode, aOutput.mReview.mReview);
  success = success && GetString(aDocument, titleNode, aOutput.mReview.mTitle);
  success = success && GetSint32(aDocument, votesNode, aOutput.mReview.mVotes);
  GetString(aDocument, responseNode, aOutput.mReview.mResponse);  // Optional

  auto photosIt = aDocument.FindMember(photosNode);
  if (photosIt != aDocument.MemberEnd()) {
    success = success && ParseReviewPhotos(photosIt->value, aOutput.mReviewPhotos);
  }

  return success;
}  // end of ParseReview

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse photos section.
//!
//----------------------------------------------------------------
static bool ParseReviewPhotos(const rapidjson::Value& aDocument,
                              std::vector<ReviewPhotoTableDataType>& aOutput) {
  const char ordinalNode[] = "ordinal";
  const char downloadUrlNode[] = "downloadUrl";

  if (!aDocument.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = aDocument.GetArray().begin(); it != aDocument.GetArray().end(); it++) {
    ReviewPhotoTableDataType reviewPhotoTableData;

    if (it->IsObject()) {
      success = success && Json::GetSint32(*it, ordinalNode, reviewPhotoTableData.mOrdinal);
      success = success && Json::GetString(*it, downloadUrlNode, reviewPhotoTableData.mDownloadUrl);
      if (success) {
        aOutput.emplace_back(std::move(reviewPhotoTableData));
      }
    }
  }

  return success;
}  // end of ParseReviewPhotos

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Determine if review is deleted.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
static bool GetReviewStatus(const rapidjson::Value& aDocument, const char* aNodeName,
                            bool& aOutput) {
  const std::string deletedStatus = "Deleted";

  std::string status;
  if (!GetString(aDocument, aNodeName, status)) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  if (status == deletedStatus) {
    aOutput = true;
  }

  return true;
}  // end of GetReviewStatus

}  // end of namespace Json
}  // end of namespace Acdb
