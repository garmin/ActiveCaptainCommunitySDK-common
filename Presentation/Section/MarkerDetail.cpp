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
@brief Represents the ActiveCaptain community database
MarkerDetail object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MarkerDetail"

#include "DBG_pub.h"

#include <unordered_map>
#include "Acdb/Presentation/Section/MarkerDetail.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MarkerDetail::MarkerDetail(std::string&& aName, ReviewSummaryPtr aReviewSummary,
                           AttributeField&& aLastModified, StringField&& aLocation,
                           std::string&& aTitle, AttributeField&& aMarkerType,
                           std::unique_ptr<AttributeField> aSectionNote, LinkField&& aEditField,
                           std::unique_ptr<BusinessPhotoField> aBusinessPhoto,
                           std::unique_ptr<LinkField> aSeeAllPhotosField)
    : mName(std::move(aName)),
      mReviewSummary(std::move(aReviewSummary)),
      mLastModified(std::move(aLastModified)),
      mLocation(std::move(aLocation)),
      mTitle(std::move(aTitle)),
      mMarkerType(std::move(aMarkerType)),
      mSectionNote(std::move(aSectionNote)),
      mEditField(std::move(aEditField)),
      mBusinessPhoto(std::move(aBusinessPhoto)),
      mSeeAllPhotosField(std::move(aSeeAllPhotosField)) {}  // end of MarkerDetail

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool MarkerDetail::operator==(const MarkerDetail& aRhs) const {
  return mName == aRhs.mName && CompareUniquePtr(mReviewSummary, aRhs.mReviewSummary) &&
         mLastModified == aRhs.mLastModified && mLocation == aRhs.mLocation &&
         mTitle == aRhs.mTitle && mMarkerType == aRhs.mMarkerType &&
         CompareUniquePtr(mSectionNote, aRhs.mSectionNote) && mEditField == aRhs.mEditField &&
         CompareUniquePtr(mBusinessPhoto, aRhs.mBusinessPhoto) &&
         CompareUniquePtr(mSeeAllPhotosField, aRhs.mSeeAllPhotosField);
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the business photo field
//!
//----------------------------------------------------------------
const BusinessPhotoField* MarkerDetail::GetBusinessPhoto() const {
  return mBusinessPhoto.get();
}  // end of GetBusinessPhoto

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& MarkerDetail::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to last modified field
//!
//----------------------------------------------------------------
const AttributeField& MarkerDetail::GetLastModified() const {
  return mLastModified;
}  // end of GetLastModified

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to location field
//!
//----------------------------------------------------------------
const StringField& MarkerDetail::GetLocation() const { return mLocation; }  // end of GetLocation

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to marker type field
//!
//----------------------------------------------------------------
const AttributeField& MarkerDetail::GetMarkerType() const {
  return mMarkerType;
}  // end of GetMarkerType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return name of the marker
//!
//----------------------------------------------------------------
const std::string& MarkerDetail::GetName() const { return mName; }  // end of GetName

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return pointer to review summary field
//!
//----------------------------------------------------------------
const ReviewSummary* MarkerDetail::GetReviewSummary() const {
  return mReviewSummary.get();
}  // end of GetReviewSummary

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return pointer to section note field
//!
//----------------------------------------------------------------
const AttributeField* MarkerDetail::GetSectionNote() const {
  return mSectionNote.get();
}  // end of GetSectionNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the see all photos field
//!
//----------------------------------------------------------------
const LinkField* MarkerDetail::GetSeeAllPhotosField() const {
  return mSeeAllPhotosField.get();
}  // end of GetSeeAllPhotosField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return title of the POI summary section
//!
//----------------------------------------------------------------
const std::string& MarkerDetail::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
