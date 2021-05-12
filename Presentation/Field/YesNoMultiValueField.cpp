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
YesNoMultiValueField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "YesNoMultiValueField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/YesNoMultiValueField.hpp"

// Note -- We use mCsvString instead of reusing YesNoUnknownNearbyField's mValue property because
// the template requires mValue to be "Yes", "No", "Unknown", or "Nearby".

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
YesNoMultiValueField::YesNoMultiValueField(YesNoUnknownNearbyField&& aYesNoUnknownNearbyField,
                                           std::string&& aCsvString)
    : YesNoUnknownNearbyField(std::move(aYesNoUnknownNearbyField)),
      mCsvString(std::move(aCsvString)) {}  // end of YesNoMultiValueField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool YesNoMultiValueField::operator==(const YesNoMultiValueField& aRhs) const {
  return YesNoUnknownNearbyField::operator==(aRhs) && mCsvString == aRhs.mCsvString;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return comma-separated values string
//!
//----------------------------------------------------------------
const std::string& YesNoMultiValueField::GetCsvString() const {
  return mCsvString;
}  // end of GetCsvString

}  // end of namespace Presentation
}  // end of namespace Acdb
