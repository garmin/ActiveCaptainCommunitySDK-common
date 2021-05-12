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
    Handles all Competitor data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_CompetitorQuery_hpp
#define ACDB_CompetitorQuery_hpp

#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class CompetitorQuery {
 public:
  // functions
  CompetitorQuery(SQLite::Database& aDatabase);

  bool Delete(const ACDB_marker_idx_type aId);

  bool Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd);

  bool Get(const ACDB_marker_idx_type aId, std::vector<CompetitorTableDataType>& aResultOut);

  bool GetCanTarget(const ACDB_marker_idx_type aTargetId, const ACDB_marker_idx_type aAdvertiserId,
                    bool& aResultOut);

  bool GetPotentialAdvertisers(const ACDB_marker_idx_type aId,
                               std::vector<ACDB_marker_idx_type>& aResultOut);

  bool Write(const ACDB_marker_idx_type aId, CompetitorTableDataType&& aCompetitorTableData);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mDelete;

  std::unique_ptr<SQLite::Statement> mDeleteGeohash;

  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mReadAdTarget;

  std::unique_ptr<SQLite::Statement> mReadAdvertisers;

  std::unique_ptr<SQLite::Statement> mWrite;
};  // end of class CompetitorQuery
}  // end of namespace Acdb

#endif  // end of ACDB_CompetitorQuery_hpp
