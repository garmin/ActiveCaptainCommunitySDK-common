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
Definition of the Review Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewField_hpp
#define ACDB_ReviewField_hpp

#include <vector>
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/ResponseField.hpp"
#include "Acdb/Presentation/Field/StringField.hpp"
#include "Acdb/Presentation/Field/ReviewPhotoField.hpp"

namespace Acdb {
namespace Presentation {

class ReviewField {
 public:
  // Functions
  ReviewField(std::string&& aTitle, int aRating, std::string&& aDateVisited,
              std::string&& aCaptainName, std::string&& aText, bool aIncludeStars,
              std::unique_ptr<LinkField> aVoteField, int aVotes, LinkField&& aLinkField,
              std::unique_ptr<ResponseField> aResponseField,
              std::vector<ReviewPhotoField>&& aReviewPhotoFields);

  bool operator==(const ReviewField& aRhs) const;

  const std::string& GetTitle() const;

  const std::vector<StringField>& GetStarValues() const;

  const std::string& GetDateVisited() const;

  const std::string& GetCaptainName() const;

  const std::string& GetText() const;

  const LinkField* GetVoteField() const;

  int GetVotes() const;

  const LinkField& GetLinkField() const;

  const ResponseField* GetResponseField() const;

  const std::vector<ReviewPhotoField>& GetReviewPhotoFields() const;

 private:
  // Functions
  void SetReviewStars(const int aRating);

  // Variables
  std::string mTitle;
  std::vector<StringField> mStarValues;
  std::string mDateVisited;
  std::string mCaptainName;
  std::string mText;

  std::unique_ptr<LinkField> mVoteField;
  int mVotes;

  LinkField mLinkField;

  std::unique_ptr<ResponseField> mResponseField;

  std::vector<ReviewPhotoField> mReviewPhotoFields;

};  // end of class ReviewField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewField_hpp
