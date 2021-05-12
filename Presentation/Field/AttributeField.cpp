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
AttributeField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "AttributeField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/AttributeField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
AttributeField::AttributeField(std::string&& aLabel, std::string&& aValue, std::string&& aNote,
                               std::string&& aHyperLink)
    : mHyperLink(aHyperLink),
      mLabel(aLabel),
      mNote(aNote),
      mValue(aValue) {}  // end of AttributeField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool AttributeField::operator==(const AttributeField& aRhs) const {
  return mHyperLink == aRhs.mHyperLink && mLabel == aRhs.mLabel && mNote == aRhs.mNote &&
         mValue == aRhs.mValue;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the hyperlink member
//!
//----------------------------------------------------------------
std::string AttributeField::GetHyperLink() const { return mHyperLink; }  // end of GetHyperLink

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the label member
//!
//----------------------------------------------------------------
std::string AttributeField::GetLabel() const { return mLabel; }  // end of GetLabel

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the note member
//!
//----------------------------------------------------------------
std::string AttributeField::GetNote() const { return mNote; }  // end of GetNote

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute
//!
//----------------------------------------------------------------
std::string AttributeField::GetValue() const { return mValue; }  // end of GetValue

}  // end of namespace Presentation
}  // end of namespace Acdb
