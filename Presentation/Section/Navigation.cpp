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
Navigation object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Navigation"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Navigation.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Navigation::Navigation(std::string aTitle, std::vector<AttributeField>&& aAttributeFields,
                       std::unique_ptr<AttributeField> aSectionNote, LinkField&& aEditField)
    : mTitle(std::move(aTitle)),
      mAttributeFields(std::move(aAttributeFields)),
      mSectionNote(std::move(aSectionNote)),
      mEditField(std::move(aEditField)) {}  // end of Navigation

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Navigation::operator==(const Navigation& aRhs) const {
  return mTitle == aRhs.mTitle && mAttributeFields == aRhs.mAttributeFields &&
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
const std::vector<AttributeField>& Navigation::GetAttributeFields() const {
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
const LinkField& Navigation::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return pointer to section note field
//!
//----------------------------------------------------------------
const AttributeField* Navigation::GetSectionNote() const {
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
const std::string& Navigation::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
