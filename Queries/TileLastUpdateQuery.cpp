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
#define DBG_TAG "TileLastUpdateQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/TileLastUpdateQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM tileLastUpdate WHERE tileX = ? AND tileY = ?;"};
static const std::string ReadSql{
    "SELECT markerLastUpdate, reviewLastUpdate FROM tileLastUpdate WHERE tileX = ? AND tileY = ?;"};
static const std::string ReadBboxSql{
    "SELECT t.tileX, t.tileY, tlu.markerLastUpdate, tlu.reviewLastUpdate "
    "FROM tiles T "
    "INNER JOIN tileRIndex tr ON ((t.tileY * 16) + t.tileX) = tr.id "
    "LEFT JOIN tileLastUpdate tlu ON (t.tileX = tlu.tileX AND t.tileY = tlu.tileY) "
    "WHERE tr.maxLon >= ? AND tr.minLon <= ? AND tr.maxLat >= ? AND tr.minLat <= ? "
    "ORDER BY t.tileY ASC, t.tileX ASC;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO tileLastUpdate (tileX, tileY, markerLastUpdate, reviewLastUpdate) VALUES (?, ?, ?, ?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create TileLastUpdate query object.
//!
//----------------------------------------------------------------
TileLastUpdateQuery::TileLastUpdateQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadBbox.reset(new SQLite::Statement{aDatabase, ReadBboxSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mRead.reset();
    mReadBbox.reset();
    mWrite.reset();
  }
}  // End of TileLastUpdateQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete TileLastUpdate row from database
//!
//----------------------------------------------------------------
bool TileLastUpdateQuery::Delete(const TileXY& aTileXY) {
  enum Parameters { TileX = 1, TileY };

  if (!mDelete) {
    return false;
  }

  bool success = false;

  try {
    mDelete->bind(Parameters::TileX, aTileXY.mX);
    mDelete->bind(Parameters::TileY, aTileXY.mY);

    mDelete->exec();
    success = mDelete->isDone();

    mDelete->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the info for the specified object.
//!
//----------------------------------------------------------------
bool TileLastUpdateQuery::Get(const TileXY& aTileXY, LastUpdateInfoType& aResultOut) {
  enum Parameters { TileX = 1, TileY };
  enum Columns { MarkerLastUpdate = 0, ReviewLastUpdate };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::TileX, aTileXY.mX);
    mRead->bind(Parameters::TileY, aTileXY.mY);

    success = mRead->executeStep();
    if (success) {
      aResultOut.mMarkerLastUpdate =
          static_cast<uint64_t>(mRead->getColumn(Columns::MarkerLastUpdate).getInt64());
      aResultOut.mUserReviewLastUpdate =
          static_cast<uint64_t>(mRead->getColumn(Columns::ReviewLastUpdate).getInt64());
    }

    mRead->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Get

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get last update info for tiles overlapped by bounding box.
//!
//----------------------------------------------------------------
bool TileLastUpdateQuery::GetBbox(const bbox_type& aBbox,
                                  std::map<TileXY, LastUpdateInfoType>& aResultOut) {
  enum Parameters { MaxLon = 1, MinLon, MaxLat, MinLat };
  enum Columns { TileX = 0, TileY, MarkerLastUpdate, ReviewLastUpdate };

  if (!mReadBbox) {
    return false;
  }

  bool success = false;

  try {
    mReadBbox->bind(Parameters::MaxLon, static_cast<long long>(aBbox.swc.lon));
    mReadBbox->bind(Parameters::MinLon, static_cast<long long>(aBbox.nec.lon));
    mReadBbox->bind(Parameters::MaxLat, static_cast<long long>(aBbox.swc.lat));
    mReadBbox->bind(Parameters::MinLat, static_cast<long long>(aBbox.nec.lat));

    while (mReadBbox->executeStep()) {
      TileXY tileXY;
      LastUpdateInfoType lastUpdateInfoType;

      tileXY.mX = mReadBbox->getColumn(Columns::TileX).getInt();
      tileXY.mY = mReadBbox->getColumn(Columns::TileY).getInt();
      lastUpdateInfoType.mMarkerLastUpdate =
          mReadBbox->getColumn(Columns::MarkerLastUpdate).getInt64();
      lastUpdateInfoType.mUserReviewLastUpdate =
          mReadBbox->getColumn(Columns::ReviewLastUpdate).getInt64();

      aResultOut[tileXY] = lastUpdateInfoType;
    }

    success = !aResultOut.empty();

    mReadBbox->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetBbox

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write TileLastUpdate row to database
//!
//----------------------------------------------------------------
bool TileLastUpdateQuery::Write(const TileXY& aTileXY, const LastUpdateInfoType& aLastUpdateInfo) {
  enum Parameters { TileX = 1, TileY, MarkerLastUpdate, ReviewLastUpdate };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::TileX, aTileXY.mX);
    mWrite->bind(Parameters::TileY, aTileXY.mY);
    mWrite->bind(Parameters::MarkerLastUpdate,
                 static_cast<long long>(aLastUpdateInfo.mMarkerLastUpdate));
    mWrite->bind(Parameters::ReviewLastUpdate,
                 static_cast<long long>(aLastUpdateInfo.mUserReviewLastUpdate));

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of Write

}  // end of namespace Acdb
