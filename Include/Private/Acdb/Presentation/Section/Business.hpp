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
Definition of the Business component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Business_hpp
#define ACDB_Business_hpp

#include <memory>
#include "Acdb/Presentation/Field/AttributeField.hpp"
#include "Acdb/Presentation/Field/BusinessPromotionListField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class Business {
 public:
  // functions
  Business(std::string&& aTitle, std::vector<AttributeField>&& aAttributeFields,
           std::vector<AttributeField>&& aAttributeMultiValueFields, LinkField&& aEditField,
           std::unique_ptr<BusinessPromotionListField> aBusinessPromotionListField,
           std::unique_ptr<LinkField> aCallToActionField);

  bool operator==(const Business& aRhs) const;

  const std::vector<AttributeField>& GetAttributeFields() const;

  const std::vector<AttributeField>& GetAttributeMultiValueFields() const;

  const BusinessPromotionListField* GetBusinessPromotionListField() const;

  const LinkField* GetCallToActionField() const;

  const LinkField& GetEditField() const;

  const std::string& GetTitle() const;

 private:
  // Variables
  std::string mTitle;
  std::vector<AttributeField> mAttributeFields;
  std::vector<AttributeField> mAttributeMultiValueFields;

  LinkField mEditField;

  std::unique_ptr<BusinessPromotionListField> mBusinessPromotionListField;
  std::unique_ptr<LinkField> mCallToActionField;

};  // end of class Business

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_Business_hpp
