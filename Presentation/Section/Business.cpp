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
Business object from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Business"

#include "DBG_pub.h"

#include "Acdb/Presentation/Section/Business.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Business::Business(std::string&& aTitle, std::vector<AttributeField>&& aAttributeFields,
                   std::vector<AttributeField>&& aAttributeMultiValueFields, LinkField&& aEditField,
                   std::unique_ptr<BusinessPromotionListField> aBusinessPromotionListField,
                   std::unique_ptr<LinkField> aCallToActionField)
    : mTitle(std::move(aTitle)),
      mAttributeFields(std::move(aAttributeFields)),
      mAttributeMultiValueFields(std::move(aAttributeMultiValueFields)),
      mEditField(std::move(aEditField)),
      mBusinessPromotionListField(std::move(aBusinessPromotionListField)),
      mCallToActionField(std::move(aCallToActionField)) {}  // end of Business

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool Business::operator==(const Business& aRhs) const {
  return mTitle == aRhs.mTitle && mAttributeFields == aRhs.mAttributeFields &&
         mAttributeMultiValueFields == aRhs.mAttributeMultiValueFields &&
         mEditField == aRhs.mEditField &&
         CompareUniquePtr(mBusinessPromotionListField, aRhs.mBusinessPromotionListField) &&
         CompareUniquePtr(mCallToActionField, aRhs.mCallToActionField);
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute fields
//!
//----------------------------------------------------------------
const std::vector<AttributeField>& Business::GetAttributeFields() const {
  return mAttributeFields;
}  // end of GetAttributeFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the attribute multi-value fields
//!
//----------------------------------------------------------------
const std::vector<AttributeField>& Business::GetAttributeMultiValueFields() const {
  return mAttributeMultiValueFields;
}  // end of GetAttributeMultiValueFields

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the business promotion list field
//!
//----------------------------------------------------------------
const BusinessPromotionListField* Business::GetBusinessPromotionListField() const {
  return mBusinessPromotionListField.get();
}  // end of GetBusinessPromotionListField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the call-to-action field
//!
//----------------------------------------------------------------
const LinkField* Business::GetCallToActionField() const {
  return mCallToActionField.get();
}  // end of GetCallToActionField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the edit field
//!
//----------------------------------------------------------------
const LinkField& Business::GetEditField() const { return mEditField; }  // end of GetEditField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the title
//!
//----------------------------------------------------------------
const std::string& Business::GetTitle() const { return mTitle; }  // end of GetTitle

}  // end of namespace Presentation
}  // end of namespace Acdb
