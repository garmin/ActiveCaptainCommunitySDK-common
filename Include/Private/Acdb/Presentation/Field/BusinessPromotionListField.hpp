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
Definition of the Business Promotion field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_BusinessPromotionListField_hpp
#define ACDB_BusinessPromotionListField_hpp

#include <string>
#include <vector>
#include "Acdb/Presentation/Field/BusinessPromotionField.hpp"

namespace Acdb {
namespace Presentation {

class BusinessPromotionListField {
 public:
  // functions
  BusinessPromotionListField(std::string&& aLabel,
                             std::vector<BusinessPromotionField>&& aBusinessPromotion);

  bool operator==(const BusinessPromotionListField& aRhs) const;

  const std::vector<BusinessPromotionField>& GetBusinessPromotions() const;

  const std::string& GetLabel() const;

 private:
  // Variables
  std::string mLabel;
  std::vector<BusinessPromotionField> mBusinessPromotions;

};  // end of class BusinessPromotionField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_BusinessPromotionsListField_hpp
