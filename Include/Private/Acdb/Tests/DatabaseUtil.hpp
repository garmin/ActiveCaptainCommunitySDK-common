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
    @brief Database utility functions for tests

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_DatabaseUtil_hpp
#define ACDB_DatabaseUtil_hpp

#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Database.h"
#include "Acdb/StringFormatter.hpp"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

SQLite::Database CreateDatabase(TF_state_type* aState);

MarkerTableDataCollection GetMarkerTableDataCollection();

std::vector<ReviewTableDataCollection> GetReviewsTableDataCollection();

void PopulateDatabase(TF_state_type* aState, SQLite::Database& aDatabase);

void PopulateTilesTable(TF_state_type* aState, SQLite::Database& aDatabase);

void PopulateTileLastUpdateTable(TF_state_type* aState, SQLite::Database& aDatabase);

void PopulateTranslationsTable(TF_state_type* aState, SQLite::Database& aDatabase);

}  // end of namespace Test
}  // end of namespace Acdb

#endif  // ACDB_DatabaseUtil_hpp
