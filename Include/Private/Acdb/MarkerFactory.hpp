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
    Generates Marker objects.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MarkerFactory_hpp
#define ACDB_MarkerFactory_hpp

#include "Acdb/MapMarker.hpp"
#include "Acdb/SearchMarker.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/TextHandle.hpp"

namespace Acdb {

TextHandle GetMarkerTypeTextHandle(ACDB_type_type aMarkerType);

MapMarkerPtr GetMapMarker(MarkerTableDataType& aMarkerData);

SearchMarkerPtr GetSearchMarker(ExtendedMarkerDataType& aMarkerData);

TextHandle GetYesNoUnknownNearbyTextHandle(const std::string& aYnubValue);

}  // end of namespace Acdb

#endif  // end of ACDB_MarkerFactory_hpp
