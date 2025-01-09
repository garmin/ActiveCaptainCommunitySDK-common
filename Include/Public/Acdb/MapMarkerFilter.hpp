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

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MapMarkerFilter_hpp
#define ACDB_MapMarkerFilter_hpp

#include "ACDB_pub_types.h"

namespace Acdb {
//! Creates a type marker type filter for ActiveCaptain point searches
class MapMarkerFilter {
 public:
  MapMarkerFilter(const bbox_type& aBbox, uint32_t aTypesBitmask);

  MapMarkerFilter();

  const bbox_type& GetBbox() const;

  uint32_t GetAllowedTypes() const;

  void SetBbox(const bbox_type& aBoundingBox);

  void AddType(ACDB_type_type aType);

 protected:
  // Variables
  bbox_type mBbox;
  uint32_t mTypesBitmask;
};  // end of class MapMarkerFilter
}  // end of namespace Acdb

#endif  // end of ACDB_MapMarkerFilter_hpp
