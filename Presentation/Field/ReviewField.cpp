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
ReviewField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewField"

#include "DBG_pub.h"

#include "Acdb/PubTypes.hpp"
#include "Acdb/Presentation/Field/ReviewField.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewField::ReviewField(std::string&& aTitle, int aRating, std::string&& aDateVisited,
                         std::string&& aCaptainName, std::string&& aText, bool aIncludeStars,
                         std::unique_ptr<LinkField> aVoteField, int aVotes, LinkField&& aLinkField,
                         std::unique_ptr<ResponseField> aResponseField,
                         std::vector<ReviewPhotoField>&& aReviewPhotoFields)
    : mTitle(std::move(aTitle)),
      mStarValues(),
      mDateVisited(std::move(aDateVisited)),
      mCaptainName(std::move(aCaptainName)),
      mText(std::move(aText)),
      mVoteField(std::move(aVoteField)),
      mVotes{aVotes},
      mLinkField(std::move(aLinkField)),
      mResponseField(std::move(aResponseField)),
      mReviewPhotoFields(std::move(aReviewPhotoFields)) {
  if (aIncludeStars) {
    SetReviewStars(aRating);
  }
}  // end of ReviewField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewField::operator==(const ReviewField& aRhs) const {
  return mTitle == aRhs.mTitle && mStarValues == aRhs.mStarValues &&
         mDateVisited == aRhs.mDateVisited && mCaptainName == aRhs.mCaptainName &&
         mText == aRhs.mText && CompareUniquePtr(mVoteField, aRhs.mVoteField) &&
         mVotes == aRhs.mVotes && mLinkField == aRhs.mLinkField &&
         CompareUniquePtr(mResponseField, aRhs.mResponseField) &&
         mReviewPhotoFields == aRhs.mReviewPhotoFields;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of review photos
//!
//----------------------------------------------------------------
const std::vector<ReviewPhotoField>& ReviewField::GetReviewPhotoFields() const {
  return mReviewPhotoFields;
}  // end of GetReviewPhotoFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& ReviewField::GetTitle() const { return mTitle; }  // end of GetTitle

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the review stars
//!
//----------------------------------------------------------------
const std::vector<StringField>& ReviewField::GetStarValues() const {
  return mStarValues;
}  // end of GetReviewStars

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the date visited
//!
//----------------------------------------------------------------
const std::string& ReviewField::GetDateVisited() const {
  return mDateVisited;
}  // end of GetDateVisited

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the captain name
//!
//----------------------------------------------------------------
const std::string& ReviewField::GetCaptainName() const {
  return mCaptainName;
}  // end of GetCaptainName

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the review text
//!
//----------------------------------------------------------------
const std::string& ReviewField::GetText() const { return mText; }  // end of GetText

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the vote field
//!
//----------------------------------------------------------------
const LinkField* ReviewField::GetVoteField() const {
  return mVoteField.get();
}  // end of GetVoteField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the vote count
//!
//----------------------------------------------------------------
int ReviewField::GetVotes() const { return mVotes; }  // end of GetVotes

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the link field
//!
//----------------------------------------------------------------
const LinkField& ReviewField::GetLinkField() const { return mLinkField; }  // end of GetLinkField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the response field
//!
//----------------------------------------------------------------
const ResponseField* ReviewField::GetResponseField() const {
  return mResponseField.get();
}  // end of GetResponseField

//----------------------------------------------------------------
//!
//!   @private
//!   @detail Construct vector of string representation
//!           of each graphical star.  Full and empty stars
//!           will be represented.  There will be one entry
//!           to the limit MaxReviewRating.
//!
//----------------------------------------------------------------
void ReviewField::SetReviewStars(const int aRating) {
  int fullStars = aRating;

  mStarValues.clear();

  // Draw stars.
  for (int i = 0; i < MaxReviewRating; i++) {
    if (fullStars > 0) {
      mStarValues.push_back(StringField("Full"));
      fullStars--;
    } else {
      mStarValues.push_back(StringField("Empty"));
    }
  }
}

}  // end of namespace Presentation
}  // end of namespace Acdb
