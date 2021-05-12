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
Fuel object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Fuel"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Fuel.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Fuel::Fuel(std::string aTitle, std::vector<YesNoPriceField>&& aYesNoPriceFields,
           std::vector<AttributeField>&& aAttributeFields,
           std::vector<YesNoUnknownNearbyField>&& aYnubFields,
           std::vector<YesNoUnknownNearbyFieldPair>&& aYnubFieldPairs,
           std::unique_ptr<AttributeField> aSectionNote, LinkField&& aEditField)
    : mTitle(std::move(aTitle)),
      mYesNoPriceFields(std::move(aYesNoPriceFields)),
      mAttributeFields(std::move(aAttributeFields)),
      mYnubFields(std::move(aYnubFields)),
      mYnubFieldPairs(std::move(aYnubFieldPairs)),
      mSectionNote(std::move(aSectionNote)),
      mEditField(std::move(aEditField)) {}  // end of Fuel

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Fuel::operator==(const Fuel& aRhs) const {
  return mTitle == aRhs.mTitle && mYesNoPriceFields == aRhs.mYesNoPriceFields &&
         mAttributeFields == aRhs.mAttributeFields && mYnubFields == aRhs.mYnubFields &&
         mYnubFieldPairs == aRhs.mYnubFieldPairs &&
         CompareUniquePtr(mSectionNote, aRhs.mSectionNote) && mEditField == aRhs.mEditField;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute fields
//!
//----------------------------------------------------------------
const std::vector<AttributeField>& Fuel::GetAttributeFields() const {
  return mAttributeFields;
}  // end of GetAttributeFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& Fuel::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return pointer to section note field
//!
//----------------------------------------------------------------
const AttributeField* Fuel::GetSectionNote() const {
  return mSectionNote.get();
}  // end of GetSectionNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& Fuel::GetTitle() const { return mTitle; }  // end of GetTitle

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the price attribute fields
//!
//----------------------------------------------------------------
const std::vector<YesNoPriceField>& Fuel::GetYesNoPriceFields() const {
  return mYesNoPriceFields;
}  // end of GetPrices

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the Yes / No / Unknown / Nearby fields
//!
//----------------------------------------------------------------
const std::vector<YesNoUnknownNearbyField>& Fuel::GetYesNoUnknownNearbyFields() const {
  return mYnubFields;
}  // end of GetYesNoUnknownNearbyFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the Yes / No / Unknown / Nearby field pairs
//!
//----------------------------------------------------------------
const std::vector<YesNoUnknownNearbyFieldPair>& Fuel::GetYesNoUnknownNearbyFieldPairs() const {
  return mYnubFieldPairs;
}  // end of GetYesNoUnknownNearbyFieldPairs

}  // end of namespace Presentation
}  // end of namespace Acdb
