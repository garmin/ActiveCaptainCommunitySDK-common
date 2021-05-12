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
    @brief contains functionality related to
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MapMarker"

#include <map>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/MapMarker.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/StringUtil.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MapMarker::MapMarker(const ACDB_marker_idx_type aId, const ACDB_type_type aType,
                     const uint64_t aLastUpdated, std::string&& aName, const int32_t aLat,
                     const int32_t aLon, const MapIconType aMapIcon)
    : mId(aId),
      mType(aType),
      mLastUpdated(aLastUpdated),
      mName(std::move(aName)),
      mMapIcon(aMapIcon) {
  mPosn.lat = aLat;
  mPosn.lon = aLon;
}  // end of MapMarker::MapMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return copy of the marker id
//!
//----------------------------------------------------------------
ACDB_marker_idx_type MapMarker::GetId() const { return mId; }  // end of MapMarker::GetId()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return copy of the marker LastUpdated value
//!
//----------------------------------------------------------------
uint64_t MapMarker::GetLastUpdated() const { return mLastUpdated; }  // end of GetLastUpdated

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return copy of the marker map icon
//!
//----------------------------------------------------------------
MapIconType MapMarker::GetMapIcon() const { return mMapIcon; }  // end of MapMarker::GetMapIcon

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return copy of the marker name
//!
//----------------------------------------------------------------
std::string MapMarker::GetName() const { return mName; }  // end of MapMarker::GetName

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return copy of the marker position
//!
//----------------------------------------------------------------
scposn_type MapMarker::GetPosition() const { return mPosn; }  // end of MapMarker::GetPosition

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return copy of the marker position
//!
//----------------------------------------------------------------
ACDB_type_type MapMarker::GetType() const { return mType; }  // end of MapMarker::GetType

//----------------------------------------------------------------
//!
//!   @public
//!   @detail If a memory allocation failure occurs an empty string
//!           will be returned to the caller.
//!
//!   @return All of the member data combined into a std::string
//!
//----------------------------------------------------------------
std::string MapMarker::ToString() const {
  return String::Format("Id: %d , Name: %s , Type: %u , Lat(sc) %i , Lon(sc) %i", mId,
                        mName.c_str(), mType, mPosn.lat, mPosn.lon);
}

}  // end of namespace Acdb
