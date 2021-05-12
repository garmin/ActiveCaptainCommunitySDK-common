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
    Handles all Marker data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MarkerQuery_hpp
#define ACDB_MarkerQuery_hpp

#include <vector>

#include "ACDB_pub_types.h"
#include "Acdb/MapMarkerFilter.hpp"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class MarkerQuery {
 public:
  // functions
  MarkerQuery(SQLite::Database& aDatabase);

  bool Delete(const ACDB_marker_idx_type aId);

  bool Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd);

  bool Get(const ACDB_marker_idx_type aId, MarkerTableDataType& aResultOut);

  bool GetFiltered(const MapMarkerFilter& aFilter, std::vector<MarkerTableDataType>& aResultOut);

  bool GetLastUpdate(uint64_t& aLastUpdateOut);

  bool GetIds(const uint32_t aPageNumber, const uint32_t aPageSize,
              std::vector<ACDB_marker_idx_type>& aResultOut);

  bool Write(const ACDB_marker_idx_type aId, MarkerTableDataType&& aMarkerTableData);

 private:
  std::unique_ptr<SQLite::Statement> mDelete;

  std::unique_ptr<SQLite::Statement> mDeleteGeohash;

  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mReadFiltered;

  std::unique_ptr<SQLite::Statement> mReadLastUpdate;

  std::unique_ptr<SQLite::Statement> mReadIds;

  std::unique_ptr<SQLite::Statement> mWrite;
};  // end of class MarkerQuery
}  // end of namespace Acdb

#endif  // end of ACDB_MarkerQuery_hpp
