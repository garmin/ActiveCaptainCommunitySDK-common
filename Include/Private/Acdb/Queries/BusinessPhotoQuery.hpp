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
    Handles all BusinessPhoto data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_BusinessPhotoQuery_hpp
#define ACDB_BusinessPhotoQuery_hpp

#include <vector>
#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class BusinessPhotoQuery {
 public:
  // functions
  BusinessPhotoQuery(SQLite::Database& aDatabase);

  bool Delete(const ACDB_marker_idx_type aId);

  bool Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd);

  bool Get(const ACDB_marker_idx_type aId, std::vector<BusinessPhotoTableDataType>& aResultOut);

  bool Write(const ACDB_marker_idx_type aId, BusinessPhotoTableDataType&& aBusinessPhotoTableData);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mDelete;

  std::unique_ptr<SQLite::Statement> mDeleteGeohash;

  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mWrite;
};  // end of class BusinessPhotoQuery
}  // end of namespace Acdb

#endif  // end of ACDB_BusinessPhotoQuery_hpp
