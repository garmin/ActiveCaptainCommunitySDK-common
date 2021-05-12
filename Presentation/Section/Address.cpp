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
Address object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Address"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Address.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Address::Address(std::string&& aTitle, std::vector<StringField>&& aStringFields,
                 std::vector<AttributeField>&& aAttributeFields, LinkField&& aEditField)
    : mTitle(std::move(aTitle)),
      mStringFields(std::move(aStringFields)),
      mAttributeFields(std::move(aAttributeFields)),
      mEditField(std::move(aEditField)) {}  // end of Address

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Address::operator==(const Address& aRhs) const {
  return mTitle == aRhs.mTitle && mStringFields == aRhs.mStringFields &&
         mAttributeFields == aRhs.mAttributeFields && mEditField == aRhs.mEditField;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute fields
//!
//----------------------------------------------------------------
const std::vector<AttributeField>& Address::GetAttributeFields() const {
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
const LinkField& Address::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the string fields
//!
//----------------------------------------------------------------
const std::vector<StringField>& Address::GetStringFields() const {
  return mStringFields;
}  // end of GetStringFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& Address::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
