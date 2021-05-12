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
    @brief Contains functionality related to filtering
    ActiveCaptain searches

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_SearchMarkerFilter_hpp
#define ACDB_SearchMarkerFilter_hpp

#include <stdint.h>
#include <string>
#include "Acdb/MapMarkerFilter.hpp"

namespace Acdb {
//! Creates an extended marker type filter, adding categories and name search
class SearchMarkerFilter {
 public:
  typedef enum : uint32_t {
    MatchBeginningOfWord,
    MatchSubstring,
  } StringMatchMode;

  typedef enum : uint64_t {
    MarinasAndMoorings = 0x01,
    FuelStation = 0x02,
    RestaurantsAndBars = 0x04,
    RetailShopping = 0x08,
    TaxiAndLaunch = 0x10,
    Repair = 0x20,
    Dealer = 0x40,

    // special value: the highest significant bit represents "Any"
    // It is set for every marker, making sure that the category bitmask for a marker will never be
    // 0 If no specific search category is set in a filter, "Any" is automatically used as default
    // and all markers are found.
    Any = 0x8000000000000000  // set the 64th bit only (unfortunately "1 << 63" is not accepted by
                              // the compiler)
  } MarkerCategory;

  explicit SearchMarkerFilter();

  const bbox_type& GetBbox() const;

  uint32_t GetAllowedTypes() const;

  uint64_t GetAllowedCategories() const;

  const std::string& GetSearchString() const;
  StringMatchMode GetStringMatchMode() const;

  void SetBbox(const bbox_type& aBoundingBox);

  void AddType(ACDB_type_type aType);

  void AddCategory(MarkerCategory aCategory);

  void SetSearchString(const std::string& aSearchString,
                       StringMatchMode aMatchMode = MatchSubstring);

  int32_t GetMaxResults() const;

  void SetMaxResults(const int32_t aMaxResults);

 private:
  MapMarkerFilter mBaseFilter;
  uint64_t mCategoriesBitmask = 0;
  int32_t mMaxResults = -1;

  std::string mSearchString;
  StringMatchMode mStringMatchMode = MatchSubstring;

};  // end of class SearchMarkerFilter
}  // end of namespace Acdb

#endif  // end of ACDB_SearchMarkerFilter_hpp
