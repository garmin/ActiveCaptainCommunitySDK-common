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
ReviewSummary from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewSummary"

#include <math.h>
#include "DBG_pub.h"

#include "Acdb/PubTypes.hpp"
#include "Acdb/Presentation/Field/ReviewSummary.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewSummary::ReviewSummary(const float aAverageStars, const int aReviewCount,
                             const bool aIncludeStars)
    : mStarValues(), mCount(aReviewCount) {
  if (aIncludeStars) {
    SetReviewStars(aAverageStars);
  }
}  // end of ReviewSummary

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewSummary::operator==(const ReviewSummary& aRhs) const {
  return mStarValues == aRhs.mStarValues && mCount == aRhs.mCount;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the multiple value attribute
//!
//----------------------------------------------------------------
int ReviewSummary::GetReviewCount() const { return mCount; }  // end of GetReviewCount

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the multiple value attribute
//!
//----------------------------------------------------------------
const std::vector<StringField>& ReviewSummary::GetStarValues() const {
  return mStarValues;
}  // end of GetReviewCount

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Construct vector of string representation
//!           of each graphical star.  Full, half, and
//!           empty stars will be represented.  There will
//!           be one entry to the limit MaxReviewRating.
//!
//----------------------------------------------------------------
void ReviewSummary::SetReviewStars(const float aAverageStars) {
  bool drawHalfStar = false;

  // Calculate number of full stars needed
  int fullStars = static_cast<int>(floor(aAverageStars));

  // Determine if we need to draw a half star.
  float ratingFractionalPortion = aAverageStars - fullStars;
  if (ratingFractionalPortion >= 0.50) {
    drawHalfStar = true;
  }

  mStarValues.clear();

  // Draw stars.
  for (int i = 0; i < MaxReviewRating; i++) {
    if (fullStars > 0) {
      mStarValues.push_back(StringField("Full"));
      fullStars--;
    } else if (drawHalfStar) {
      mStarValues.push_back(StringField("Half"));
      drawHalfStar = false;
    } else {
      mStarValues.push_back(StringField("Empty"));
    }
  }
}

}  // end of namespace Presentation
}  // end of namespace Acdb
