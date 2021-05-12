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
Definition of the Fuel component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Fuel_hpp
#define ACDB_Fuel_hpp

#include <vector>
#include "Acdb/Presentation/Field/AttributeField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/YesNoPriceField.hpp"
#include "Acdb/Presentation/Field/YesNoUnknownNearbyField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class Fuel {
 public:
  // functions
  Fuel(std::string aTitle, std::vector<YesNoPriceField>&& aYesNoPriceFields,
       std::vector<AttributeField>&& aAttributeFields,
       std::vector<YesNoUnknownNearbyField>&& aYnubFields,
       std::vector<YesNoUnknownNearbyFieldPair>&& aYnubFieldPairs,
       std::unique_ptr<AttributeField> aSectionNote, LinkField&& aEditField);

  bool operator==(const Fuel& aRhs) const;

  const std::vector<AttributeField>& GetAttributeFields() const;

  const LinkField& GetEditField() const;

  const AttributeField* GetSectionNote() const;

  const std::string& GetTitle() const;

  const std::vector<YesNoPriceField>& GetYesNoPriceFields() const;

  const std::vector<YesNoUnknownNearbyField>& GetYesNoUnknownNearbyFields() const;

  const std::vector<YesNoUnknownNearbyFieldPair>& GetYesNoUnknownNearbyFieldPairs() const;

 private:
  // Variables
  std::string mTitle;
  std::vector<YesNoPriceField> mYesNoPriceFields;
  std::vector<AttributeField> mAttributeFields;
  std::vector<YesNoUnknownNearbyField> mYnubFields;
  std::vector<YesNoUnknownNearbyFieldPair> mYnubFieldPairs;
  std::unique_ptr<AttributeField> mSectionNote;

  LinkField mEditField;

};  // end of class Fuel

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_Fuel_hpp
