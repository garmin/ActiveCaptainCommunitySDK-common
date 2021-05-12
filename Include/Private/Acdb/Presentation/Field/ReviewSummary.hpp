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
Definition of the Review Summary field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewSummary_hpp
#define ACDB_ReviewSummary_hpp

#include <vector>
#include "Acdb/Presentation/Field/StringField.hpp"

namespace Acdb {
namespace Presentation {

class ReviewSummary {
 public:
  // functions
  ReviewSummary() = default;

  ReviewSummary(const float aAverageStars, const int aReviewCount, const bool aIncludeStars);

  bool operator==(const ReviewSummary& aRhs) const;

  int GetReviewCount() const;

  const std::vector<StringField>& GetStarValues() const;

 private:
  void SetReviewStars(const float aAverageStars);

  // Variables
  std::vector<StringField> mStarValues;
  int mCount;

};  // end of class ReviewSummary

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewSummary_hpp
