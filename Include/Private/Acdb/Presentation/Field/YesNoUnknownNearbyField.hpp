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
Definition of the Yes / No / Unknown / Nearby Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_YesNoUnknownNearbyField_hpp
#define ACDB_YesNoUnknownNearbyField_hpp

#include <memory>
#include <string>

namespace Acdb {
namespace Presentation {

class YesNoUnknownNearbyField {
 public:
  // functions
  YesNoUnknownNearbyField(std::string&& aLabel, std::string&& aValue, std::string&& aNote,
                          std::string&& aAltText);

  bool operator==(const YesNoUnknownNearbyField& aRhs) const;

  std::string GetLabel() const;

  std::string GetNote() const;

  std::string GetValue() const;

  std::string GetAltText() const;

 private:
  // Variables
  std::string mLabel;
  std::string mNote;
  std::string mValue;
  std::string mAltText;

};  // end of class YesNoUnknownNearbyField

struct YesNoUnknownNearbyFieldPair {
  bool operator==(const YesNoUnknownNearbyFieldPair& aRhs) const;

  std::shared_ptr<YesNoUnknownNearbyField> mLeftItem{nullptr};
  std::shared_ptr<YesNoUnknownNearbyField> mRightItem{nullptr};
};

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_YesNoUnknownNearbyField_hpp
