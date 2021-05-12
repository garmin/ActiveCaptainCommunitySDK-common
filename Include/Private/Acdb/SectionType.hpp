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
Definition of the SectionType.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_SectionType_hpp
#define ACDB_SectionType_hpp

#include <string>

namespace Acdb {
class SectionType {
 public:
  enum Value : uint32_t {
    None = 0,
    PointOfInterest = 1 << 0,
    Summary = 1 << 1,
    Address = 1 << 2,
    Contact = 1 << 3,
    Business = 1 << 4,
    Navigation = 1 << 5,
    Amenities = 1 << 6,
    Services = 1 << 7,
    Retail = 1 << 8,
    Fuel = 1 << 9,
    Dockage = 1 << 10,
    Moorings = 1 << 11,
    ReviewDetail = 1 << 12
  };

  SectionType(Value aValue);

  bool operator==(SectionType aSectionType) const;

  bool operator!=(SectionType aSectionType) const;

  SectionType operator&(SectionType aSectionType) const;

  SectionType operator|(SectionType aSectionType) const;

  static SectionType GetRequiredSections(ACDB_type_type aMarkerType);

  std::string ToString() const;

 private:
  Value mValue;
};
}  // end of namespace Acdb

#endif  // end of ACDB_SectionType_hpp
