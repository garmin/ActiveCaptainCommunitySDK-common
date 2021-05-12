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
Definition of the Attribute Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_AttributeField_hpp
#define ACDB_AttributeField_hpp

#include <string>

namespace Acdb {
namespace Presentation {

class AttributeField {
 public:
  // functions
  AttributeField(std::string&& aLabel, std::string&& aValue, std::string&& aNote,
                 std::string&& aHyperLink);

  bool operator==(const AttributeField& aRhs) const;

  std::string GetHyperLink() const;

  std::string GetLabel() const;

  std::string GetNote() const;

  std::string GetValue() const;

 private:
  // Variables
  std::string mHyperLink;
  std::string mLabel;
  std::string mNote;
  std::string mValue;

};  // end of class AttributeField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_AttributeField_hpp
