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
Definition of the Moorings component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Moorings_hpp
#define ACDB_Moorings_hpp

#include <vector>
#include "Acdb/Presentation/Field/AttributeField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/YesNoPriceField.hpp"
#include "Acdb/Presentation/Field/YesNoUnknownNearbyField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class Moorings {
 public:
  // functions
  Moorings(std::string&& aTitle, std::vector<YesNoPriceField>&& aYesNoPriceFields,
           std::vector<AttributeField>&& aAttributeFields,
           std::unique_ptr<AttributeField> aSectionNote,
           std::vector<YesNoUnknownNearbyField>&& aYnubFields,
           std::vector<YesNoUnknownNearbyFieldPair>&& aYnubFieldPairs, LinkField&& aEditField,
           LinkField&& aSeeAllField);

  bool operator==(const Moorings& aRhs) const;

  const std::vector<AttributeField>& GetAttributeFields() const;

  const LinkField& GetEditField() const;

  const AttributeField* GetSectionNote() const;

  const LinkField& GetSeeAllField() const;

  const std::string& GetTitle() const;

  const std::vector<YesNoPriceField>& GetYesNoPriceFields() const;

  const std::vector<YesNoUnknownNearbyField>& GetYesNoUnknownNearbyFields() const;

  const std::vector<YesNoUnknownNearbyFieldPair>& GetYesNoUnknownNearbyFieldPairs() const;

 private:
  // Variables
  std::string mTitle;
  std::vector<YesNoPriceField> mYesNoPriceFields;
  std::vector<AttributeField> mAttributeFields;
  std::unique_ptr<AttributeField> mSectionNote;
  std::vector<YesNoUnknownNearbyField> mYnubFields;
  std::vector<YesNoUnknownNearbyFieldPair> mYnubFieldPairs;

  LinkField mEditField;
  LinkField mSeeAllField;

};  // end of class Moorings

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_Moorings_hpp
