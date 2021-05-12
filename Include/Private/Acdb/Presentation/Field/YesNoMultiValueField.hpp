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
Definition of the Yes / No Multiple value field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_YesNoMultiValueField_hpp
#define ACDB_YesNoMultiValueField_hpp

#include "Acdb/Presentation/Field/YesNoUnknownNearbyField.hpp"

namespace Acdb {
namespace Presentation {

class YesNoMultiValueField : public YesNoUnknownNearbyField {
 public:
  // functions
  YesNoMultiValueField(YesNoUnknownNearbyField&& aYesNoUnknownNearbyField,
                       std::string&& aCsvString);

  bool operator==(const YesNoMultiValueField& aRhs) const;

  const std::string& GetCsvString() const;

 private:
  // Variables
  std::string mCsvString;

};  // end of class YesNoMultiValueField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_YesNoMultiValueField_hpp
