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
Definition of the ReviewDetail component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewDetail_hpp
#define ACDB_ReviewDetail_hpp

#include "Acdb/PrvTypes.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/ReviewField.hpp"
#include "Acdb/Presentation/Field/ReviewSummary.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class ReviewDetail {
 public:
  // functions
  ReviewDetail(std::string&& aTitle, std::unique_ptr<ReviewField> aFeaturedReview,
               LinkField&& aEditField, LinkField&& aSeeAllField, ReviewSummaryPtr aReviewSummary);

  bool operator==(const ReviewDetail& aRhs) const;

  const LinkField& GetEditField() const;

  const ReviewField* GetFeaturedReview() const;

  const LinkField& GetSeeAllField() const;

  const std::string& GetTitle() const;

  const ReviewSummary* GetReviewSummary() const;

 private:
  // Variables
  std::string mTitle;
  ReviewSummaryPtr mReviewSummary;
  std::unique_ptr<ReviewField> mFeaturedReview;

  LinkField mEditField;
  LinkField mSeeAllField;

};  // end of class ReviewDetail

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewDetail_hpp
