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
Definition of the Dockage component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Dockage_hpp
#define ACDB_Dockage_hpp

#include <vector>
#include "Acdb/Presentation/Field/AttributeField.hpp"
#include "Acdb/Presentation/Field/AttributePriceField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/YesNoMultiValueField.hpp"
#include "Acdb/Presentation/Field/YesNoUnknownNearbyField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class Dockage {
 public:
  // functions
  Dockage(std::string&& aTitle, std::vector<YesNoMultiValueField>&& aYesNoMultiValueFields,
          std::vector<AttributePriceField>&& aAttributePriceFields,
          std::vector<AttributeField>&& aAttributeFields,
          std::unique_ptr<AttributeField> aSectionNote,
          std::vector<YesNoUnknownNearbyField>&& aYnubFields,
          std::vector<YesNoUnknownNearbyFieldPair>&& aYnubFieldPairs, LinkField&& aEditField,
          LinkField&& aSeeAllField);

  bool operator==(const Dockage& aRhs) const;

  const std::vector<AttributeField>& GetAttributeFields() const;

  const std::vector<AttributePriceField>& GetAttributePriceFields() const;

  const LinkField& GetEditField() const;

  const AttributeField* GetSectionNote() const;

  const LinkField& GetSeeAllField() const;

  const std::string& GetTitle() const;

  const std::vector<YesNoMultiValueField>& GetYesNoMultiValueFields() const;

  const std::vector<YesNoUnknownNearbyField>& GetYesNoUnknownNearbyFields() const;

  const std::vector<YesNoUnknownNearbyFieldPair>& GetYesNoUnknownNearbyFieldPairs() const;

 private:
  // Variables
  std::string mTitle;
  std::vector<YesNoMultiValueField> mYesNoMultiValueFields;
  std::vector<AttributePriceField> mAttributePriceFields;
  std::vector<AttributeField> mAttributeFields;
  std::unique_ptr<AttributeField> mSectionNote;
  std::vector<YesNoUnknownNearbyField> mYnubFields;
  std::vector<YesNoUnknownNearbyFieldPair> mYnubFieldPairs;

  LinkField mEditField;
  LinkField mSeeAllField;

};  // end of class Dockage

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_Dockage_hpp
