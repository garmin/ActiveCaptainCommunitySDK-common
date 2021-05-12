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
    @brief Encapsulates all merge operations

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MergeAdapter"

#include <vector>

#include "DBG_pub.h"
#include "Acdb/MergeAdapter.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MergeAdapter::MergeAdapter(SQLite::Database& aDatabase)
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
      mRetail{aDatabase},
      mReview{aDatabase},
      mReviewPhoto{aDatabase},
      mServices{aDatabase},
      mTranslator{aDatabase} {}  // End of MergeAdapter

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Marker.  This contains all of the
//!       table data for the marker.
//!
//----------------------------------------------------------------
MarkerTableDataCollection MergeAdapter::GetMarker(const ACDB_marker_idx_type aIdx) {
  MarkerTableDataCollection marker;

  if (mMarker.Get(aIdx, marker.mMarker) && mMarkerMeta.Get(aIdx, marker.mMarkerMeta)) {
    AddressTableDataType address;
    AmenitiesTableDataType amenities;
    BusinessTableDataType business;
    BusinessProgramTableDataType businessProgram;
    ContactTableDataType contact;
    DockageTableDataType dockage;
    FuelTableDataType fuel;
    MooringsTableDataType moorings;
    NavigationTableDataType navigation;
    RetailTableDataType retail;
    ServicesTableDataType services;

    if (mAddress.Get(aIdx, address)) {
      marker.mAddress.reset(new AddressTableDataType(std::move(address)));
    }

    if (mAmenities.Get(aIdx, amenities)) {
      marker.mAmenities.reset(new AmenitiesTableDataType(std::move(amenities)));
    }

    if (mBusiness.Get(aIdx, business)) {
      marker.mBusiness.reset(new BusinessTableDataType(std::move(business)));
    }

    mBusinessPhoto.Get(aIdx, marker.mBusinessPhotos);

    if (mBusinessProgram.Get(aIdx, businessProgram)) {
      marker.mBusinessProgram.reset(new BusinessProgramTableDataType(std::move(businessProgram)));
    }

    mCompetitor.Get(aIdx, marker.mCompetitors);

    if (mContact.Get(aIdx, contact)) {
      marker.mContact.reset(new ContactTableDataType(std::move(contact)));
    }

    if (mDockage.Get(aIdx, dockage)) {
      marker.mDockage.reset(new DockageTableDataType(std::move(dockage)));
    }

    if (mFuel.Get(aIdx, fuel)) {
      marker.mFuel.reset(new FuelTableDataType(std::move(fuel)));
    }

    if (mMoorings.Get(aIdx, moorings)) {
      marker.mMoorings.reset(new MooringsTableDataType(std::move(moorings)));
    }

    if (mNavigation.Get(aIdx, navigation)) {
      marker.mNavigation.reset(new NavigationTableDataType(std::move(navigation)));
    }

    if (mRetail.Get(aIdx, retail)) {
      marker.mRetail.reset(new RetailTableDataType(std::move(retail)));
    }

    if (mServices.Get(aIdx, services)) {
      marker.mServices.reset(new ServicesTableDataType(std::move(services)));
    }
  }

  return marker;
}  // end of GetMarker

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns list of marker IDs
//!
//----------------------------------------------------------------
bool MergeAdapter::GetMarkerIds(const uint32_t aPageNumber, const uint32_t aPageSize,
                                std::vector<ACDB_marker_idx_type>& aResults) {
  return mMarker.GetIds(aPageNumber, aPageSize, aResults);
}  // end of GetMarkerIds

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns reviews for a specific Marker.  This contains
//!       all of the table data for the reviews.
//!
//----------------------------------------------------------------
std::vector<ReviewTableDataCollection> MergeAdapter::GetReviews(const ACDB_marker_idx_type aIdx) {
  std::vector<ReviewTableDataCollection> results;

  std::vector<ReviewTableDataType> reviews;
  std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>> reviewPhotos;

  mReview.GetList(aIdx, std::string(), 1, -1,
                  reviews);  // Review list starts with page 1.  Page size of -1 means all reviews
                             // will be returned.

  mReviewPhoto.GetListByMarkerId(aIdx, std::string{}, 1, -1,
                                 reviewPhotos);  // Pagination follow same rules as reviews.

  for (auto review : reviews) {
    ReviewTableDataCollection result;
    result.mReview = std::move(review);

    auto it = reviewPhotos.find(result.mReview.mId);
    if (it != reviewPhotos.end()) {
      result.mReviewPhotos = std::move(it->second);
    }

    results.emplace_back(result);
  }

  return results;
}  // end of GetReviews

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns list of search bits
//!
//----------------------------------------------------------------
bool MergeAdapter::GetSupportTableData(
    std::vector<LanguageTableDataType>& aLanguages,
    std::vector<MustacheTemplateTableDataType>& aMustacheTemplates,
    std::vector<TranslationTableDataType>& aTranslations) {
  mLanguage.GetAll(aLanguages);
  mMustacheTemplate.GetAll(aMustacheTemplates);
  mTranslator.GetAll(aTranslations);

  return !aLanguages.empty() && !aMustacheTemplates.empty() && !aTranslations.empty();
}  // end of GetSupportTableData

}  // end of namespace Acdb
