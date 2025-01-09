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

#define DBG_MODULE "ACDB"
#define DBG_TAG "MapMarkerFilter"

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/MapMarkerFilter.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MapMarkerFilter::MapMarkerFilter(const bbox_type& aBbox, uint32_t aTypesBitmask)
    : mBbox(aBbox), mTypesBitmask(aTypesBitmask) {}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Default Constructor
//!
//----------------------------------------------------------------
MapMarkerFilter::MapMarkerFilter() : mBbox{{0}}, mTypesBitmask(0) {}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return filter bounding box
//!
//----------------------------------------------------------------
const bbox_type& MapMarkerFilter::GetBbox() const { return mBbox; }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return database filter type value
//!
//----------------------------------------------------------------
uint32_t MapMarkerFilter::GetAllowedTypes() const {
  if (0 != mTypesBitmask) {
    return mTypesBitmask;
  } else {
    // no type filter is set => all types allowed
    return static_cast<uint32_t>(ACDB_ALL_TYPES);
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief set the filter's bounding box
//!
//----------------------------------------------------------------
void MapMarkerFilter::SetBbox(const bbox_type& aBoundingBox) { mBbox = aBoundingBox; }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief add another type to the filter's mask
//!
//----------------------------------------------------------------
void MapMarkerFilter::AddType(ACDB_type_type aType) {
  mTypesBitmask |= static_cast<uint32_t>(aType);
}

}  // end of namespace Acdb
