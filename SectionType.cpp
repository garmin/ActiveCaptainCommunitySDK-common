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
    @brief Contains functionality related to
    ActiveCaptain section types.

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "SectionType"

#include <map>
#include "ACDB_pub_types.h"
#include "Acdb/SectionType.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!       @public
//!       @brief constructor
//!
//----------------------------------------------------------------
SectionType::SectionType(Value aValue) : mValue(aValue) {}  // end of SectionType

//----------------------------------------------------------------
//!
//!       @public
//!       @brief equality operator
//!
//----------------------------------------------------------------
bool SectionType::operator==(SectionType aSectionType) const {
  return mValue == aSectionType.mValue;
}  // end of operator==

//----------------------------------------------------------------
//!
//!       @public
//!       @brief inequality operator
//!
//----------------------------------------------------------------
bool SectionType::operator!=(SectionType aSectionType) const {
  return mValue != aSectionType.mValue;
}  // end of operator!=

//----------------------------------------------------------------
//!
//!       @public
//!       @brief bitwise-AND operator
//!
//----------------------------------------------------------------
SectionType SectionType::operator&(SectionType aSectionType) const {
  return SectionType((Value)(mValue & aSectionType.mValue));
}  // end of operator&

//----------------------------------------------------------------
//!
//!       @public
//!       @brief bitwise-OR operator
//!
//----------------------------------------------------------------
SectionType SectionType::operator|(SectionType aSectionType) const {
  return SectionType((Value)(mValue | aSectionType.mValue));
}  // end of operator|

//----------------------------------------------------------------
//!
//!       @private
//!       @returns SectionType with required sections for marker type
//!
//----------------------------------------------------------------
SectionType SectionType::GetRequiredSections(ACDB_type_type aMarkerType) {
  SectionType requiredForAll((Value)(PointOfInterest | Summary | ReviewDetail));

  static const std::map<ACDB_type_type, SectionType> requiredSectionsMap = {
      {ACDB_ANCHORAGE, requiredForAll | Contact | Navigation | Amenities | Retail | Moorings},
      {ACDB_HAZARD, requiredForAll},
      {ACDB_MARINA, requiredForAll | Address | Contact | Business | Navigation | Amenities |
                        Services | Retail | Fuel | Dockage | Moorings},
      {ACDB_BOAT_RAMP, requiredForAll},
      {ACDB_BUSINESS,
       requiredForAll | Address | Contact | Business | Amenities | Services | Retail | Fuel},
      {ACDB_INLET, requiredForAll},
      {ACDB_BRIDGE, requiredForAll | Contact | Navigation},
      {ACDB_LOCK, requiredForAll | Contact | Navigation},
      {ACDB_DAM, requiredForAll},
      {ACDB_FERRY, requiredForAll | Contact}};

  SectionType requiredSections(None);

  auto requiredSectionsIter = requiredSectionsMap.find(aMarkerType);
  if (requiredSectionsIter != requiredSectionsMap.end()) {
    requiredSections = requiredSectionsIter->second;
  }

  return requiredSections;
}  // end of GetRequiredSections

//----------------------------------------------------------------
//!
//!       @private
//!       @returns String representation of SectionType value
//!
//----------------------------------------------------------------
std::string SectionType::ToString() const {
  std::map<SectionType::Value, std::string> sectionNameMap = {
      // NOTE -- These values must match the section names used by webviews.
      {PointOfInterest, "Summary"}, {Summary, "Summary"},
      {Address, "Address"},         {Contact, "ContactInfo"},
      {Business, "BusinessInfo"},   {Navigation, "Navigation"},
      {Amenities, "Amenities"},     {Services, "Services"},
      {Retail, "Retail"},           {Fuel, "Fuel"},
      {Dockage, "Dockage"},         {Moorings, "Moorings"},
      {ReviewDetail, "Reviews"}};

  std::string sectionName;

  auto sectionNameIter = sectionNameMap.find(mValue);
  if (sectionNameIter != sectionNameMap.end()) {
    sectionName = sectionNameIter->second;
  }

  return sectionName;
}  // end of ToString
}  // end of namespace Acdb
