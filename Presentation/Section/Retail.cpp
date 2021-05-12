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
Retail object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Retail"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Retail.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Retail::Retail(std::string&& aTitle, std::vector<YesNoUnknownNearbyField>&& aYnubFields,
               std::vector<YesNoUnknownNearbyFieldPair>&& aYnubPairFields,
               std::unique_ptr<AttributeField> aNote, LinkField&& aEditField,
               LinkField&& aSeeAllField)
    : mTitle(std::move(aTitle)),
      mYnubFields(std::move(aYnubFields)),
      mYnubPairFields(std::move(aYnubPairFields)),
      mNote(std::move(aNote)),
      mEditField(aEditField),
      mSeeAllField(aSeeAllField) {}  // end of Retail

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Retail::operator==(const Retail& aRhs) const {
  return mTitle == aRhs.mTitle && mYnubFields == aRhs.mYnubFields &&
         mYnubPairFields == aRhs.mYnubPairFields && CompareUniquePtr(mNote, aRhs.mNote) &&
         mEditField == aRhs.mEditField && mSeeAllField == aRhs.mSeeAllField;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the Yes / No / Unknown / Nearby fields
//!
//----------------------------------------------------------------
const std::vector<YesNoUnknownNearbyField>& Retail::GetYesNoUnknownNearbyFields() const {
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
const std::vector<YesNoUnknownNearbyFieldPair>& Retail::GetYesNoUnknownNearbyFieldPairs() const {
  return mYnubPairFields;
}  // end of GetYesNoUnknownNearbyFieldPairs

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& Retail::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the note
//!
//----------------------------------------------------------------
const AttributeField* Retail::GetNote() const { return mNote.get(); }  // end of GetNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the see all field
//!
//----------------------------------------------------------------
const LinkField& Retail::GetSeeAllField() const { return mSeeAllField; }  // end of GetSeeAllField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& Retail::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
