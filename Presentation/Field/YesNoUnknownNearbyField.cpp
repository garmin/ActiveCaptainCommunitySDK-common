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
YesNoUnknownNearbyField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "YesNoUnknownNearbyField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/YesNoUnknownNearbyField.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
YesNoUnknownNearbyField::YesNoUnknownNearbyField(std::string&& aLabel, std::string&& aValue,
                                                 std::string&& aNote, std::string&& aAltText)
    : mLabel(aLabel),
      mNote(aNote),
      mValue(aValue),
      mAltText(aAltText) {}  // end of YesNoUnknownNearbyField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool YesNoUnknownNearbyField::operator==(const YesNoUnknownNearbyField& aRhs) const {
  return mLabel == aRhs.mLabel && mNote == aRhs.mNote && mValue == aRhs.mValue &&
         mAltText == aRhs.mAltText;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the label member
//!
//----------------------------------------------------------------
std::string YesNoUnknownNearbyField::GetLabel() const { return mLabel; }  // end of GetLabel

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the note member
//!
//----------------------------------------------------------------
std::string YesNoUnknownNearbyField::GetNote() const { return mNote; }  // end of GetNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute
//!
//----------------------------------------------------------------
std::string YesNoUnknownNearbyField::GetValue() const { return mValue; }  // end of GetValue

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the alt text
//!
//----------------------------------------------------------------
std::string YesNoUnknownNearbyField::GetAltText() const { return mAltText; }  // end of GetAltText

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool YesNoUnknownNearbyFieldPair::operator==(const YesNoUnknownNearbyFieldPair& aRhs) const {
  return CompareSharedPtr(mLeftItem, aRhs.mLeftItem) &&
         CompareSharedPtr(mRightItem, aRhs.mRightItem);
}  // end of operator==

}  // end of namespace Presentation
}  // end of namespace Acdb
