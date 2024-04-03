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
#define DBG_TAG "TilesQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/TilesQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

static const std::string ReadSql{
    "SELECT tileX, tileY, geohashStart, geohashEnd FROM tiles WHERE tileX = ? AND tileY = ?;"};
static const std::string ReadBboxSql{
    "SELECT t.tileX, t.tileY, t.geohashStart, t.geohashEnd "
    "FROM tiles t "
    "	INNER JOIN tileRIndex tr ON ( ( t.tileY * 16 ) + t.tileX ) = tr.id "
    "WHERE tr.maxLon >= ? AND tr.minLon <= ? AND tr.maxLat >= ? AND tr.minLat <= ? "
    "ORDER BY t.tileY ASC, t.tileX ASC"};

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Tiles query object.
//!
//----------------------------------------------------------------
TilesQuery::TilesQuery(SQLite::Database& aDatabase) {
  try {
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadBbox.reset(new SQLite::Statement{aDatabase, ReadBboxSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mRead.reset();
    mReadBbox.reset();
  }
}  // End of TilesQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the info for the specified object.
//!
//----------------------------------------------------------------
bool TilesQuery::Get(const int aTileX, const int aTileY, TileTableDataType& aResultOut) {
  enum Parameters { TileX = 1, TileY };
  enum Columns { ColTileX = 0, ColTileY, GeohashStart, GeohashEnd };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::TileX, aTileX);
    mRead->bind(Parameters::TileY, aTileY);

    success = mRead->executeStep();
    if (success) {
      aResultOut.mTileX = mRead->getColumn(Columns::ColTileX).getInt();
      aResultOut.mTileY = mRead->getColumn(Columns::ColTileY).getInt();
      aResultOut.mGeohashStart = mRead->getColumn(Columns::GeohashStart).getInt64();
      aResultOut.mGeohashEnd = mRead->getColumn(Columns::GeohashEnd).getInt64();
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
//!   @detail Get tiles overlapped by given bounding box.
//!
//----------------------------------------------------------------
bool TilesQuery::GetBbox(const bbox_type& aBbox, std::vector<TileTableDataType>& aResultOut) {
  enum Parameters { MaxLon = 1, MinLon, MaxLat, MinLat };
  enum Columns { TileX = 0, TileY, GeohashStart, GeohashEnd };

  if (!mReadBbox) {
    return false;
  }

  bool success = false;

  try {
    mReadBbox->bind(Parameters::MaxLon, static_cast<int64_t>(aBbox.swc.lon));
    mReadBbox->bind(Parameters::MinLon, static_cast<int64_t>(aBbox.nec.lon));
    mReadBbox->bind(Parameters::MaxLat, static_cast<int64_t>(aBbox.swc.lat));
    mReadBbox->bind(Parameters::MinLat, static_cast<int64_t>(aBbox.nec.lat));

    while (mReadBbox->executeStep()) {
      TileTableDataType result;

      result.mTileX = mReadBbox->getColumn(Columns::TileX).getInt();
      result.mTileY = mReadBbox->getColumn(Columns::TileY).getInt();
      result.mGeohashStart = mReadBbox->getColumn(Columns::GeohashStart).getInt64();
      result.mGeohashEnd = mReadBbox->getColumn(Columns::GeohashEnd).getInt64();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    mReadBbox->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetBbox

}  // end of namespace Acdb
