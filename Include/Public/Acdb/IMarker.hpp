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

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_IMarker_hpp
#define ACDB_IMarker_hpp

#include <string>
#include "ACDB_pub_types.h"
#include "Acdb/MapIconType.hpp"

namespace Acdb {
//! Marker represents a point from the ActiveCaptain database
class IMarker {
 public:
  // public functions
  virtual ACDB_marker_idx_type GetId() const = 0;

  virtual uint64_t GetLastUpdated() const = 0;

  virtual MapIconType GetMapIcon() const = 0;

  virtual std::string GetName() const = 0;

  virtual scposn_type GetPosition() const = 0;

  virtual ACDB_type_type GetType() const = 0;

  virtual std::string ToString() const = 0;

  virtual ~IMarker() = default;

};  // end of class IMarker
}  // end of namespace Acdb

#endif  // end of ACDB_IMarker_hpp
