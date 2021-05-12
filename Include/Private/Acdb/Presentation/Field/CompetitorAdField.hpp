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
Definition of the Competitor Ad Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_CompetitorAdField_hpp
#define ACDB_CompetitorAdField_hpp

#include "Acdb/PrvTypes.hpp"
#include "Acdb/Presentation/Field/ReviewSummary.hpp"

namespace Acdb {
namespace Presentation {

class CompetitorAdField {
 public:
  // functions
  CompetitorAdField(ACDB_marker_idx_type aMarkerId, std::string&& aName, std::string&& aText,
                    std::string&& aPhotoUrl, ReviewSummaryPtr aReviewSummaryField,
                    std::string&& aAdLabel);

  bool operator==(const CompetitorAdField& aRhs) const;

  const std::string& GetAdLabel() const;

  ACDB_marker_idx_type GetMarkerId() const;

  const std::string& GetName() const;

  const std::string& GetPhotoUrl() const;

  const ReviewSummary* GetReviewSummary() const;

  const std::string& GetText() const;

 private:
  // Variables
  ACDB_marker_idx_type mMarkerId;
  std::string mName;
  std::string mText;
  std::string mPhotoUrl;
  ReviewSummaryPtr mReviewSummaryField;
  std::string mAdLabel;
};  // end of class CompetitorAdField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_CompetitorAdField_hpp
