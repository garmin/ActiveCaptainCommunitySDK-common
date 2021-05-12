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
BusinessPromotionField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "BusinessPromotionListField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/BusinessPromotionListField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessPromotionListField::BusinessPromotionListField(
    std::string&& aLabel, std::vector<BusinessPromotionField>&& aBusinessPromotions)
    : mLabel(std::move(aLabel)),
      mBusinessPromotions(std::move(aBusinessPromotions)) {}  // end of BusinessPromotionListField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessPromotionListField::operator==(const BusinessPromotionListField& aRhs) const {
  return mLabel == aRhs.mLabel && mBusinessPromotions == aRhs.mBusinessPromotions;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mBusinessPromotion member
//!
//----------------------------------------------------------------
const std::vector<BusinessPromotionField>& BusinessPromotionListField::GetBusinessPromotions()
    const {
  return mBusinessPromotions;
}  // end of GetBusinessPromotions

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mLabel member
//!
//----------------------------------------------------------------
const std::string& BusinessPromotionListField::GetLabel() const {
  return mLabel;
}  // end of GetLabel

}  // end of namespace Presentation
}  // end of namespace Acdb
