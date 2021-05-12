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
    Class to represent a specific set of queries

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewSummaryQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/ReviewSummaryQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string ReadSql{
    "SELECT AVG(rating) AS averageStars, COUNT(reviewId) AS reviewCount FROM reviews WHERE markerId = ?;"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create ReviewSummary query object.
//!
//----------------------------------------------------------------
ReviewSummaryQuery::ReviewSummaryQuery(SQLite::Database& aDatabase) {
  try {
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mRead.reset();
  }
}  // End of ReviewSummaryQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool ReviewSummaryQuery::Get(const ACDB_marker_idx_type aId,
                             ReviewSummaryTableDataType& aResultOut) {
  enum Parameters { MarkerId = 1 };
  enum Columns { AverageStars = 0, ReviewCount };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::MarkerId, static_cast<long long>(aId));

    success = mRead->executeStep();
    if (success) {
      aResultOut.mAverageStars =
          static_cast<float>(mRead->getColumn(Columns::AverageStars).getDouble());
      aResultOut.mReviewCount = mRead->getColumn(Columns::ReviewCount).getInt();
    }

    mRead->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Get

}  // end of namespace Acdb
