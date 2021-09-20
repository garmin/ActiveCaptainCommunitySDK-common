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
Definition of the MapIconType.

Copyright 2018-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MapIconType_hpp
#define ACDB_MapIconType_hpp

namespace Acdb {
enum class MapIconType {
  Unknown,
  Anchorage,
  Hazard,
  Marina,
  BoatRamp,
  Business,
  Inlet,
  Bridge,
  Lock,
  Dam,
  Ferry,

  AnchorageSponsor,
  BusinessSponsor,
  MarinaSponsor,

  MapIconCount  // Used for compiler asserts, must be last.

};  // end of class MapIconType
}  // end of namespace Acdb

#endif  // end of ACDB_MapIconType_hpp
