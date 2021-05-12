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
    @brief Encapsulates all update-related operations

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "UpdateAdapter"

#include <vector>

#include "DBG_pub.h"
#include "Acdb/UpdateAdapter.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
UpdateAdapter::UpdateAdapter(SQLite::Database& aDatabase)
    : mAddress{aDatabase},
      mAmenities{aDatabase},
      mBusiness{aDatabase},
      mBusinessPhoto{aDatabase},
      mBusinessProgram{aDatabase},
      mCompetitor{aDatabase},
      mContact{aDatabase},
      mDockage{aDatabase},
      mFuel{aDatabase},
      mLanguage{aDatabase},
      mMarker{aDatabase},
      mMarkerMeta{aDatabase},
      mMoorings{aDatabase},
      mMustacheTemplate{aDatabase},
      mNavigation{aDatabase},
      mPosition{aDatabase},
      mRetail{aDatabase},
      mReview{aDatabase},
      mReviewPhoto{aDatabase},
      mServices{aDatabase},
      mTileLastUpdate{aDatabase},
      mTiles{aDatabase},
      mTranslator{aDatabase} {}

//----------------------------------------------------------------
//!
//!       @public
//!       @brief delete all data for a tile
//!
//----------------------------------------------------------------
bool UpdateAdapter::DeleteTile(const TileXY& aTileXY) {
  bool success{true};

  TileTableDataType tileTableData;
  success = success && mTiles.Get(aTileXY.mX, aTileXY.mY, tileTableData);

  success = success && mMarkerMeta.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mAddress.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mAmenities.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mBusiness.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success =
      success && mBusinessPhoto.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success =
      success && mBusinessProgram.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mCompetitor.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mContact.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mDockage.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mFuel.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mMoorings.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mNavigation.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mPosition.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mRetail.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success =
      success && mReviewPhoto.Delete(tileTableData.mGeohashStart,
                                     tileTableData.mGeohashEnd);  // MUST BE DELETED BEFORE REVIEWS
  success = success && mReview.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success = success && mServices.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);
  success =
      success && mMarker.Delete(tileTableData.mGeohashStart,
                                tileTableData.mGeohashEnd);  // MUST BE AFTER MARKER ATTRIBUTES.

  success = success && mTileLastUpdate.Delete(aTileXY);

  return success;
}  // end of DeleteTile

//----------------------------------------------------------------
//!
//!       @public
//!       @brief delete all reviews for a tile
//!
//----------------------------------------------------------------
bool UpdateAdapter::DeleteTileReviews(const TileXY& aTileXY) {
  bool success{true};

  TileTableDataType tileTableData;
  success = success && mTiles.Get(aTileXY.mX, aTileXY.mY, tileTableData);

  success =
      success && mReviewPhoto.Delete(tileTableData.mGeohashStart,
                                     tileTableData.mGeohashEnd);  // MUST BE DELETED BEFORE REVIEWS

  success = success && mReview.Delete(tileTableData.mGeohashStart, tileTableData.mGeohashEnd);

  LastUpdateInfoType lastUpdateInfo;
  if (mTileLastUpdate.Get(aTileXY, lastUpdateInfo)) {
    // Only zero out mUserReviewLastUpdate if the read was successful.  If not, the row was deleted,
    // which is fine.

    lastUpdateInfo.mUserReviewLastUpdate = 0;
    success = success && mTileLastUpdate.Write(aTileXY, lastUpdateInfo);
  }

  return success;
}  // end of DeleteTileReviews

//----------------------------------------------------------------
//!
//!       @public
//!       @brief apply marker updates to database
//!
//----------------------------------------------------------------
bool UpdateAdapter::UpdateMarkers(std::vector<MarkerTableDataCollection>& aMarkers,
                                  uint64_t& aLastUpdateMax_out) {
  bool success{true};

  aLastUpdateMax_out = 0;

  for (auto& marker : aMarkers) {
    ACDB_marker_idx_type id = marker.mMarker.mId;

    if (marker.mMarker.mLastUpdated > aLastUpdateMax_out) {
      aLastUpdateMax_out = marker.mMarker.mLastUpdated;
    }

    if (marker.mIsDeleted) {
      success = success && mAddress.Delete(id);
      success = success && mAmenities.Delete(id);
      success = success && mBusiness.Delete(id);
      success = success && mBusinessPhoto.Delete(id);
      success = success && mBusinessProgram.Delete(id);
      success = success && mCompetitor.Delete(id);
      success = success && mContact.Delete(id);
      success = success && mDockage.Delete(id);
      success = success && mFuel.Delete(id);
      success = success && mMarkerMeta.Delete(id);
      success = success && mMoorings.Delete(id);
      success = success && mPosition.Delete(id);
      success = success && mNavigation.Delete(id);
      success = success && mRetail.Delete(id);
      success = success && mReviewPhoto.DeleteMarker(id);  // MUST BE DELETED BEFORE REVIEWS
      success = success && mReview.DeleteMarker(id);
      success = success && mServices.Delete(id);
      success = success && mMarker.Delete(id);  // MUST BE LAST.
    } else {
      // Save these since we call std::move on marker.Marker.
      std::string name = marker.mMarker.mName;
      scposn_type posn = marker.mMarker.mPosn;

      success = success && mMarker.Write(id, std::move(marker.mMarker));
      success = success && mPosition.Write(id, posn);
      success = success && mMarkerMeta.Write(id, std::move(marker.mMarkerMeta));
      if (marker.mAddress) {
        success = success && mAddress.Write(id, std::move(*(marker.mAddress)));
      }

      if (marker.mAmenities) {
        success = success && mAmenities.Write(id, std::move(*(marker.mAmenities)));
      }

      if (marker.mBusiness) {
        success = success && mBusiness.Write(id, std::move(*(marker.mBusiness)));
      }

      // Always need to call Delete to ensure any all old photos are deleted.  If updated marker has
      // photos, marker.BusinessPhotos will be the complete set.
      success = success && mBusinessPhoto.Delete(id);
      for (auto businessPhoto : marker.mBusinessPhotos) {
        success = success && mBusinessPhoto.Write(id, std::move(businessPhoto));
      }

      if (marker.mBusinessProgram) {
        success = success && mBusinessProgram.Write(id, std::move(*(marker.mBusinessProgram)));
      } else {
        success = success && mBusinessProgram.Delete(id);
      }

      // Always need to call Delete to ensure any all old competitors are deleted.  If updated
      // marker has competitors, marker.Competitors will be the complete set.
      success = success && mCompetitor.Delete(id);
      for (auto competitor : marker.mCompetitors) {
        success = success && mCompetitor.Write(id, std::move(competitor));
      }

      if (marker.mContact) {
        success = success && mContact.Write(id, std::move(*(marker.mContact)));
      }

      if (marker.mDockage) {
        success = success && mDockage.Write(id, std::move(*(marker.mDockage)));
      }

      if (marker.mFuel) {
        success = success && mFuel.Write(id, std::move(*(marker.mFuel)));
      }

      if (marker.mMoorings) {
        success = success && mMoorings.Write(id, std::move(*(marker.mMoorings)));
      }

      if (marker.mNavigation) {
        success = success && mNavigation.Write(id, std::move(*(marker.mNavigation)));
      }

      if (marker.mRetail) {
        success = success && mRetail.Write(id, std::move(*(marker.mRetail)));
      }

      if (marker.mServices) {
        success = success && mServices.Write(id, std::move(*(marker.mServices)));
      }
    }
  }

  return success;
}  // end of UpdateMarkers

//----------------------------------------------------------------
//!
//!       @public
//!       @brief apply review updates to database
//!
//----------------------------------------------------------------
bool UpdateAdapter::UpdateReviews(std::vector<ReviewTableDataCollection>& aReviews,
                                  uint64_t& aLastUpdateMax_out) {
  bool success{true};

  aLastUpdateMax_out = 0;

  for (auto& review : aReviews) {
    ACDB_review_idx_type id = review.mReview.mId;

    if (review.mReview.mLastUpdated > aLastUpdateMax_out) {
      aLastUpdateMax_out = review.mReview.mLastUpdated;
    }

    if (review.mReview.mIsDeleted) {
      success = success && mReviewPhoto.Delete(id);  // MUST BE DELETED BEFORE REVIEWS
      success = success && mReview.Delete(id);
    } else {
      success = success && mReview.Write(id, std::move(review.mReview));

      // Always need to call Delete to ensure any all old photos are deleted.  If updated
      // review has photos, review.mReviewPhotos will be the complete set.
      success = success && mReviewPhoto.Delete(id);

      for (auto reviewPhoto : review.mReviewPhotos) {
        success = success && mReviewPhoto.Write(id, std::move(reviewPhoto));
      }
    }
  }

  return success;
}  // end of UpdateReviews

//----------------------------------------------------------------
//!
//!       @public
//!       @brief apply language updates to database
//!
//----------------------------------------------------------------
bool UpdateAdapter::UpdateSupportTables(
    std::vector<LanguageTableDataType>& aLanguages,
    std::vector<MustacheTemplateTableDataType>& aMustacheTemplates,
    std::vector<TranslationTableDataType>& aTranslations) {
  bool success{true};

  for (auto& language : aLanguages) {
    success = success && mLanguage.Write(std::move(language));
  }

  for (auto& mustacheTemplate : aMustacheTemplates) {
    success = success && mMustacheTemplate.Write(std::move(mustacheTemplate));
  }

  for (auto& translation : aTranslations) {
    success = success && mTranslator.Write(std::move(translation));
  }

  return success;
}  // end of UpdateSearchBits
}  // end of namespace Acdb
