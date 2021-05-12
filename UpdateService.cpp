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
    Contains update-related functions for ActiveCaptain
    functionality.

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#include "Acdb/Json/MarkerParser.hpp"
#include "Acdb/Json/ResponseParser.hpp"
#include "Acdb/Json/ReviewParser.hpp"
#include "Acdb/Json/WebViewResponseParser.hpp"
#include "Acdb/Repository.hpp"
#include "Acdb/UpdateService.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Constructor
//!
//----------------------------------------------------------------
UpdateService::UpdateService(RepositoryPtr aRepositoryPtr)
    : mRepositoryPtr(std::move(aRepositoryPtr)) {}  // end of UpdateService

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Destructor
//!
//----------------------------------------------------------------
UpdateService::~UpdateService() {}  // end of ~UpdateService

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Process Create Marker endpoint response
//!
//----------------------------------------------------------------
bool UpdateService::ProcessCreateMarkerResponse(const std::string& aResponseBody,
                                                ACDB_marker_idx_type& aMarkerId_out) {
  MarkerTableDataCollection marker;

  bool success =
      Json::ParseCreateMarkerResponse(aResponseBody.c_str(), aResponseBody.size(), marker);

  if (success) {
    aMarkerId_out = marker.mMarker.mId;

    // Set last updated time to 0 so we don't miss other updates.
    marker.mMarker.mLastUpdated = 0;

    std::vector<MarkerTableDataCollection> markerList;
    markerList.push_back(std::move(marker));

    success = mRepositoryPtr->ApplyMarkerUpdateToDb(markerList, nullptr /* aTileXY */);
  }

  return success;
}  // end of ProcessCreateMarkerResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Process Move Marker endpoint response
//!
//----------------------------------------------------------------
bool UpdateService::ProcessMoveMarkerResponse(const std::string& aResponseBody) {
  MarkerTableDataCollection marker;

  bool success = Json::ParseMoveMarkerResponse(aResponseBody.c_str(), aResponseBody.size(), marker);

  if (success) {
    // Set last updated time to 0 so we don't miss other updates.
    marker.mMarker.mLastUpdated = 0;

    std::vector<MarkerTableDataCollection> markerList;
    markerList.push_back(std::move(marker));

    success = mRepositoryPtr->ApplyMarkerUpdateToDb(markerList, nullptr /* aTileXY */);
  }

  return success;
}  // end of ProcessMoveMarkerResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Process Get Markers by DateLastModified endpoint response
//!
//----------------------------------------------------------------
bool UpdateService::ProcessSyncMarkersResponse(const std::string& aResponseBody,
                                               const TileXY& aTileXY,
                                               std::size_t& aResultCount_out) {
  std::vector<MarkerTableDataCollection> markers;

  bool success =
      Json::ParseMarkerSyncResponse(aResponseBody.c_str(), aResponseBody.size(), markers);

  aResultCount_out = markers.size();

  if (!markers.empty()) {
    success = success && mRepositoryPtr->ApplyMarkerUpdateToDb(markers, &aTileXY);
  }

  return success;
}  // end of ProcessSyncMarkersResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Process Get Reviews by DateLastModified endpoint response
//!
//----------------------------------------------------------------
bool UpdateService::ProcessSyncReviewsResponse(const std::string& aResponseBody,
                                               const TileXY& aTileXY,
                                               std::size_t& aResultCount_out) {
  std::vector<ReviewTableDataCollection> reviews;

  bool success =
      Json::ParseReviewSyncResponse(aResponseBody.c_str(), aResponseBody.size(), reviews);

  aResultCount_out = reviews.size();

  if (!reviews.empty()) {
    success = success && mRepositoryPtr->ApplyReviewUpdateToDb(reviews, &aTileXY);
  }

  return success;
}  // end of ProcessSyncReviewsResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Process Vote for Review endpoint response
//!
//----------------------------------------------------------------
bool UpdateService::ProcessVoteForReviewResponse(const std::string& aResponseBody) {
  ReviewTableDataCollection review;

  bool success =
      Json::ParseVoteForReviewResponse(aResponseBody.c_str(), aResponseBody.size(), review);

  // Set last updated time to 0 so we don't miss other updates.
  review.mReview.mLastUpdated = 0;

  std::vector<ReviewTableDataCollection> reviewList;
  reviewList.push_back(std::move(review));

  success = success && mRepositoryPtr->ApplyReviewUpdateToDb(reviewList, nullptr /* aTileXY */);

  return success;
}  // end of ProcessVoteForReviewResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Apply an updated marker from a webview response.
//!   @note
//!       This is a blocking call that will consume the thread
//!       until completion.
//!
//----------------------------------------------------------------
bool UpdateService::ProcessWebViewResponse(const std::string& aJsonMarker) {
  bool success = true;
  MarkerTableDataCollection marker;
  ReviewTableDataCollection review;

  Json::WebViewResponseParser::ResultType result =
      Json::WebViewResponseParser::ParseWebViewResponse(aJsonMarker.data(), aJsonMarker.size(),
                                                        marker, review);

  switch (result) {
    case Json::WebViewResponseParser::ResultType::MarkerUpdate: {
      // Set last updated time to 0 so we don't miss other updates.
      marker.mMarker.mLastUpdated = 0;

      std::vector<MarkerTableDataCollection> markerList;
      markerList.push_back(std::move(marker));

      mRepositoryPtr->ApplyMarkerUpdateToDb(markerList, nullptr /* aTileXY */);
      break;
    }

    case Json::WebViewResponseParser::ResultType::ReviewUpdate: {
      // Set last updated time to 0 so we don't miss other updates.
      review.mReview.mLastUpdated = 0;

      std::vector<ReviewTableDataCollection> reviewList;
      reviewList.push_back(std::move(review));

      mRepositoryPtr->ApplyReviewUpdateToDb(reviewList, nullptr /* aTileXY */);
      break;
    }

    default:
      success = false;
  }

  return success;
}  // end of ProcessWebViewResponse

}  // end of namespace Acdb
