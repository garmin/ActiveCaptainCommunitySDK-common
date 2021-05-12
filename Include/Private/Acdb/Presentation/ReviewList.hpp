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
    @brief Contains functionality related to
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewList_hpp
#define ACDB_ReviewList_hpp

#include <memory>
#include <vector>

#include "ACDB_pub_types.h"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/ReviewField.hpp"
#include "Acdb/Presentation/Field/ReviewSummary.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {
//! List of reviews for a marker
class ReviewList {
 public:
  // public functions
  ReviewList(std::string&& aTitle, ReviewSummaryPtr aReviewSummary,
             std::unique_ptr<ReviewField> aUserReview, std::vector<ReviewField>&& aReviews,
             LinkField&& aBackButtonField, LinkField&& aEditField,
             std::unique_ptr<LinkField> aPrevField, std::unique_ptr<LinkField> aNextField);

  const LinkField& GetBackButton() const;

  const LinkField& GetEditField() const;

  const LinkField* GetNextField() const;

  const LinkField* GetPrevField() const;

  const ReviewSummary* GetReviewSummary() const;

  const std::vector<ReviewField>& GetReviews() const;

  const std::string& GetTitle() const;

  const ReviewField* GetUserReview() const;

 private:
  std::string mTitle;
  ReviewSummaryPtr mReviewSummary;
  std::unique_ptr<ReviewField> mUserReview;
  std::vector<ReviewField> mReviews;
  LinkField mBackButtonField;
  LinkField mEditField;
  std::unique_ptr<LinkField> mPrevField;
  std::unique_ptr<LinkField> mNextField;
};  // end of class ReviewList

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewList_hpp
