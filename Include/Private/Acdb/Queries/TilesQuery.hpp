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
    Handles all Tiles data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_TilesQuery_hpp
#define ACDB_TilesQuery_hpp

#include <vector>
#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class TilesQuery {
 public:
  // functions
  TilesQuery(SQLite::Database& aDatabase);

  bool Get(const int aTileX, const int aTileY, TileTableDataType& aResultOut);

  bool GetBbox(const bbox_type& aBbox, std::vector<TileTableDataType>& aResultOut);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mReadBbox;
};  // end of class TilesQuery

}  // end of namespace Acdb

#endif  // end of ACDB_TilesQuery_hpp
