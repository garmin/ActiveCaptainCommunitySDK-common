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
Definition of the RequestWriter.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_RequestWriter_hpp
#define ACDB_RequestWriter_hpp

#include <map>
#include <set>

#include "Acdb/PrvTypes.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Json {

std::string WriteCreateMarkerRequest(const std::string& aName, ACDB_type_type aType,
                                     const scposn_type& aPosn);

std::string WriteExportRequest(const std::set<TileXY>& aTile);

std::string WriteMoveMarkerRequest(const scposn_type& aPosn);

std::string WriteSyncStatusRequest(const std::map<TileXY, LastUpdateInfoType>& aTileLastUpdate);

std::string WriteTilesByBoundingBoxesRequest(const std::vector<bbox_type>& aBboxList);
}  // end of namespace Json
}  // end of namespace Acdb

#endif  // end of ACDB_RequestWriter_hpp
