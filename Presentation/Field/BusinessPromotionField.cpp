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
#define DBG_TAG "BusinessPromotionField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/BusinessPromotionField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessPromotionField::BusinessPromotionField(std::string&& aTitle, std::string&& aDetails,
                                               std::string&& aStartDate, std::string&& aEndDate)
    : mTitle(std::move(aTitle)),
      mDetails(std::move(aDetails)),
      mStartDate(std::move(aStartDate)),
      mEndDate(std::move(aEndDate)) {}  // end of BusinessPromotionsField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessPromotionField::operator==(const BusinessPromotionField& aRhs) const {
  return mTitle == aRhs.mTitle && mDetails == aRhs.mDetails && mStartDate == aRhs.mStartDate &&
         mEndDate == aRhs.mEndDate;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mDetails member
//!
//----------------------------------------------------------------
const std::string& BusinessPromotionField::GetDetails() const {
  return mDetails;
}  // end of GetDetails

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mEndDate member
//!
//----------------------------------------------------------------
const std::string& BusinessPromotionField::GetEndDate() const {
  return mEndDate;
}  // end of GetEndDate

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mStartDate member
//!
//----------------------------------------------------------------
const std::string& BusinessPromotionField::GetStartDate() const {
  return mStartDate;
}  // end of GetStartDate

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the mTitle member
//!
//----------------------------------------------------------------
const std::string& BusinessPromotionField::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
