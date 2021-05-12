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
CompetitorAdField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "CompetitorAdField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/CompetitorAdField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
CompetitorAdField::CompetitorAdField(ACDB_marker_idx_type aMarkerId, std::string&& aName,
                                     std::string&& aText, std::string&& aPhotoUrl,
                                     ReviewSummaryPtr aReviewSummaryField, std::string&& aAdLabel)
    : mMarkerId(aMarkerId),
      mName(std::move(aName)),
      mText(std::move(aText)),
      mPhotoUrl(std::move(aPhotoUrl)),
      mReviewSummaryField(std::move(aReviewSummaryField)),
      mAdLabel(std::move(aAdLabel)) {}  // end of CompetitorAdField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool CompetitorAdField::operator==(const CompetitorAdField& aRhs) const {
  return mMarkerId == aRhs.mMarkerId && mName == aRhs.mName && mText == aRhs.mText &&
         mPhotoUrl == aRhs.mPhotoUrl &&
         CompareUniquePtr(mReviewSummaryField, aRhs.mReviewSummaryField) &&
         mAdLabel == aRhs.mAdLabel;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
const std::string& CompetitorAdField::GetAdLabel() const { return mAdLabel; }  // end of GetAdLabel

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
ACDB_marker_idx_type CompetitorAdField::GetMarkerId() const {
  return mMarkerId;
}  // end of GetMarkerId

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
const std::string& CompetitorAdField::GetName() const { return mName; }  // end of GetName

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
const std::string& CompetitorAdField::GetPhotoUrl() const {
  return mPhotoUrl;
}  // end of GetPhotoUrl

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
const ReviewSummary* CompetitorAdField::GetReviewSummary() const {
  return mReviewSummaryField.get();
}  // end of GetReviewSummary

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the adLabel member
//!
//----------------------------------------------------------------
const std::string& CompetitorAdField::GetText() const { return mText; }  // end of GetText

}  // end of namespace Presentation
}  // end of namespace Acdb
