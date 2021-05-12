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
Reviews object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewDetail"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/ReviewDetail.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewDetail::ReviewDetail(std::string&& aTitle, std::unique_ptr<ReviewField> aFeaturedReview,
                           LinkField&& aEditField, LinkField&& aSeeAllField,
                           ReviewSummaryPtr aReviewSummary)
    : mTitle(std::move(aTitle)),
      mReviewSummary(std::move(aReviewSummary)),
      mFeaturedReview(std::move(aFeaturedReview)),
      mEditField(std::move(aEditField)),
      mSeeAllField(std::move(aSeeAllField)) {}  // end of ReviewDetail

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewDetail::operator==(const ReviewDetail& aRhs) const {
  return mTitle == aRhs.mTitle && CompareUniquePtr(mReviewSummary, aRhs.mReviewSummary) &&
         CompareUniquePtr(mFeaturedReview, aRhs.mFeaturedReview) && mEditField == aRhs.mEditField &&
         mSeeAllField == aRhs.mSeeAllField;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& ReviewDetail::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the featured review field
//!
//----------------------------------------------------------------
const ReviewField* ReviewDetail::GetFeaturedReview() const {
  return mFeaturedReview.get();
}  // end of GetFeaturedReview

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the see all field
//!
//----------------------------------------------------------------
const LinkField& ReviewDetail::GetSeeAllField() const {
  return mSeeAllField;
}  // end of GetSeeAllField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& ReviewDetail::GetTitle() const { return mTitle; }  // end of GetTitle

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the review summary
//!
//----------------------------------------------------------------
const ReviewSummary* ReviewDetail::GetReviewSummary() const {
  return mReviewSummary.get();
}  // end of GetReviewSummary()

}  // end of namespace Presentation
}  // end of namespace Acdb
