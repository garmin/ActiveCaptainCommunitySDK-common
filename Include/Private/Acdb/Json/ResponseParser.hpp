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
Definition of the ResponseParser.

Copyright 2018 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ResponseParser_hpp
#define ACDB_ResponseParser_hpp

#include <map>
#include <set>
#include <vector>
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Json {
bool ParseExportResponse(const char* aJson, size_t aLength,
                         std::vector<ExportFile>& aExportFiles_out);

bool ParseSyncStatusResponse(const char* aJson, size_t aLength,
                             std::map<TileXY, TileUpdateOperation>& aTileUpdateOperations_out);

bool ParseTilesByBoundingBoxesResponse(const char* aJson, size_t aLength,
                                       std::set<TileXY>& aTileList_out);

}  // end of namespace Json
}  // end of namespace Acdb

#endif  // end of ACDB_ResponseParser_hpp
