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
    ActiveCaptain business promotions.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_BusinessPromotionField_hpp
#define ACDB_BusinessPromotionField_hpp

#include <string>

namespace Acdb {
namespace Presentation {
//! Business promotion
class BusinessPromotionField {
 public:
  // public functions
  BusinessPromotionField(std::string&& aTitle, std::string&& aDetails, std::string&& aStartDate,
                         std::string&& aEndDate);

  bool operator==(const BusinessPromotionField& aRhs) const;

  const std::string& GetDetails() const;

  const std::string& GetEndDate() const;

  const std::string& GetStartDate() const;

  const std::string& GetTitle() const;

 private:
  std::string mTitle;
  std::string mDetails;
  std::string mStartDate;
  std::string mEndDate;

};  // end of class BusinessPromotionField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_BusinessPromotionField_hpp
