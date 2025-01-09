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
    ActiveCaptain markers.

    Copyright 2017 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MapMarker_hpp
#define ACDB_MapMarker_hpp

#include <map>
#include <memory>

#include "ACDB_pub_types.h"
#include "Acdb/IMapMarker.hpp"
#include "Acdb/MapIconType.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/TableDataTypes.hpp"

namespace Acdb {
//! MapMarker represents a point from the ActiveCaptain database
class MapMarker : public IMapMarker {
 public:
  MapMarker(const ACDB_marker_idx_type aId, const ACDB_type_type aType, const uint64_t aLastUpdated,
            std::string&& aName, const int32_t aLat, const int32_t aLon,
            const MapIconType aMapIcon);

  ACDB_marker_idx_type GetId() const override;

  uint64_t GetLastUpdated() const override;

  MapIconType GetMapIcon() const override;

  std::string GetName() const override;

  scposn_type GetPosition() const override;

  ACDB_type_type GetType() const override;

  std::string ToString() const override;

  virtual ~MapMarker() = default;

 private:
  // private variables
  ACDB_marker_idx_type mId;
  ACDB_type_type mType;
  uint64_t mLastUpdated;
  std::string mName;
  scposn_type mPosn;
  MapIconType mMapIcon;

};  // end of class MapMarker
}  // end of namespace Acdb

#endif  // end of ACDB_MapMarker_hpp
