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
YesNoPriceField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "YesNoPriceField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/YesNoPriceField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
YesNoPriceField::YesNoPriceField(YesNoUnknownNearbyField&& aYesNoUnknownNearbyField,
                                 std::string&& aPrice, std::string&& aUnit, std::string&& aDate)
    : YesNoUnknownNearbyField(std::move(aYesNoUnknownNearbyField)),
      mPrice(std::move(aPrice)),
      mUnit(std::move(aUnit)),
      mDate(std::move(aDate)) {}  // end of YesNoPriceField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool YesNoPriceField::operator==(const YesNoPriceField& aRhs) const {
  return YesNoUnknownNearbyField::operator==(aRhs) && mPrice == aRhs.mPrice &&
         mUnit == aRhs.mUnit && mDate == aRhs.mDate;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the price member
//!
//----------------------------------------------------------------
const std::string& YesNoPriceField::GetPrice() const { return mPrice; }  // end of GetPrice

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the unit member
//!
//----------------------------------------------------------------
const std::string& YesNoPriceField::GetUnit() const { return mUnit; }  // end of GetUnit

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the date member
//!
//----------------------------------------------------------------
const std::string& YesNoPriceField::GetDate() const { return mDate; }  // end of GetDate

}  // end of namespace Presentation
}  // end of namespace Acdb
