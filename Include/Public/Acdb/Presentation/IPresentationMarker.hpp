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
    Active Captain markers.

    Copyright 2017-2018 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_IPresentationMarker_hpp
#define ACDB_IPresentationMarker_hpp

#include "ACDB_pub_types.h"
#include "GRM_pub.h"

namespace Acdb {
//! PresentationMarker represents a point from the active captain database
class IPresentationMarker {
 public:
  // public functions
  virtual ACDB_marker_idx_type GetId() const = 0;

  virtual ~IPresentationMarker() = default;

 protected:
 private:
};  // end of class IPresentationMarker
}  // end of namespace Acdb

#endif  // end of ACDB_IPresentationMarker_hpp
