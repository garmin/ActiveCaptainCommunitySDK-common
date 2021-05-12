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
Dockage object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Dockage"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Dockage.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Dockage::Dockage(std::string&& aTitle, std::vector<YesNoMultiValueField>&& aYesNoMultiValueFields,
                 std::vector<AttributePriceField>&& aAttributePriceFields,
                 std::vector<AttributeField>&& aAttributeFields,
                 std::unique_ptr<AttributeField> aSectionNote,
                 std::vector<YesNoUnknownNearbyField>&& aYnubFields,
                 std::vector<YesNoUnknownNearbyFieldPair>&& aYnubFieldPairs, LinkField&& aEditField,
                 LinkField&& aSeeAllField)
    : mTitle(std::move(aTitle)),
      mYesNoMultiValueFields(std::move(aYesNoMultiValueFields)),
      mAttributePriceFields(std::move(aAttributePriceFields)),
      mAttributeFields(std::move(aAttributeFields)),
      mSectionNote(std::move(aSectionNote)),
      mYnubFields(std::move(aYnubFields)),
      mYnubFieldPairs(std::move(aYnubFieldPairs)),
      mEditField(std::move(aEditField)),
      mSeeAllField(std::move(aSeeAllField)) {}  // end of Dockage

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Dockage::operator==(const Dockage& aRhs) const {
  return mTitle == aRhs.mTitle && mYesNoMultiValueFields == aRhs.mYesNoMultiValueFields &&
         mAttributePriceFields == aRhs.mAttributePriceFields &&
         mAttributeFields == aRhs.mAttributeFields &&
         CompareUniquePtr(mSectionNote, aRhs.mSectionNote) && mYnubFields == aRhs.mYnubFields &&
         mYnubFieldPairs == aRhs.mYnubFieldPairs && mEditField == aRhs.mEditField &&
         mSeeAllField == aRhs.mSeeAllField;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute fields
//!
//----------------------------------------------------------------
const std::vector<AttributeField>& Dockage::GetAttributeFields() const {
  return mAttributeFields;
}  // end of GetAttributeFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute price fields
//!
//----------------------------------------------------------------
const std::vector<AttributePriceField>& Dockage::GetAttributePriceFields() const {
  return mAttributePriceFields;
}  // end of GetAttributePriceFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& Dockage::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return pointer to section note field
//!
//----------------------------------------------------------------
const AttributeField* Dockage::GetSectionNote() const {
  return mSectionNote.get();
}  // end of GetSectionNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the see all field
//!
//----------------------------------------------------------------
const LinkField& Dockage::GetSeeAllField() const { return mSeeAllField; }  // end of GetSeeAllField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& Dockage::GetTitle() const { return mTitle; }  // end of GetTitle

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the Yes / No multi-value fields
//!
//----------------------------------------------------------------
const std::vector<YesNoMultiValueField>& Dockage::GetYesNoMultiValueFields() const {
  return mYesNoMultiValueFields;
}  // end of GetYesNoMultiValueFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the Yes / No / Unknown / Nearby fields
//!
//----------------------------------------------------------------
const std::vector<YesNoUnknownNearbyField>& Dockage::GetYesNoUnknownNearbyFields() const {
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
const std::vector<YesNoUnknownNearbyFieldPair>& Dockage::GetYesNoUnknownNearbyFieldPairs() const {
  return mYnubFieldPairs;
}  // end of GetYesNoUnknownNearbyFieldPairs

}  // end of namespace Presentation
}  // end of namespace Acdb
