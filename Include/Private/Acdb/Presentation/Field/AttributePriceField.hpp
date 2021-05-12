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
Definition of the Attribute Price Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_AttributePriceField_hpp
#define ACDB_AttributePriceField_hpp

#include "Acdb/Presentation/Field/AttributeField.hpp"

namespace Acdb {
namespace Presentation {

class AttributePriceField : public AttributeField {
 public:
  // functions
  AttributePriceField(AttributeField&& aAttributeField, std::string&& aPrice, std::string&& aUnit,
                      std::string&& aDate);

  bool operator==(const AttributePriceField& aRhs) const;

  const std::string& GetPrice() const;

  const std::string& GetUnit() const;

  const std::string& GetDate() const;

 private:
  // Variables
  std::string mPrice;
  std::string mUnit;
  std::string mDate;

};  // end of class AttributePriceField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_AttributePriceField_hpp
