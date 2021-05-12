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
    @brief contains functionality related to
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewList"

#include <map>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/Presentation/ReviewList.hpp"
#include "GRM_pub_lib.h"

#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewList::ReviewList(std::string&& aTitle, ReviewSummaryPtr aReviewSummary,
                       std::unique_ptr<ReviewField> aUserReview,
                       std::vector<ReviewField>&& aReviews, LinkField&& aBackButtonField,
                       LinkField&& aEditField, std::unique_ptr<LinkField> aPrevField,
                       std::unique_ptr<LinkField> aNextField)
    : mTitle(std::move(aTitle)),
      mReviewSummary(std::move(aReviewSummary)),
      mUserReview(std::move(aUserReview)),
      mReviews(std::move(aReviews)),
      mBackButtonField(std::move(aBackButtonField)),
      mEditField(std::move(aEditField)),
      mPrevField(std::move(aPrevField)),
      mNextField(std::move(aNextField)) {}  // end of ReviewList::ReviewList

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to back button field
//!
//----------------------------------------------------------------
const LinkField& ReviewList::GetBackButton() const {
  return mBackButtonField;
}  // end of ReviewList::GetBackButton()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to edit field
//!
//----------------------------------------------------------------
const LinkField& ReviewList::GetEditField() const {
  return mEditField;
}  // end of ReviewList::GetEditField()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to next field
//!
//----------------------------------------------------------------
const LinkField* ReviewList::GetNextField() const {
  return mNextField.get();
}  // end of ReviewList::GetNextField()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to prev field
//!
//----------------------------------------------------------------
const LinkField* ReviewList::GetPrevField() const {
  return mPrevField.get();
}  // end of ReviewList::GetPrevField()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to review summary
//!
//----------------------------------------------------------------
const ReviewSummary* ReviewList::GetReviewSummary() const {
  return mReviewSummary.get();
}  // end of ReviewList::GetReviewSummary()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to reviews
//!
//----------------------------------------------------------------
const std::vector<ReviewField>& ReviewList::GetReviews() const {
  return mReviews;
}  // end of ReviewList::GetReviews()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to title
//!
//----------------------------------------------------------------
const std::string& ReviewList::GetTitle() const { return mTitle; }  // end of ReviewList::GetTitle()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to user review
//!
//----------------------------------------------------------------
const ReviewField* ReviewList::GetUserReview() const {
  return mUserReview.get();
}  // end of ReviewList::GetUserReview()

}  // end of namespace Presentation
}  // end of namespace Acdb
