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
#define DBG_TAG "MarkerQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/MarkerQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM markers WHERE id = ?;"};
static const std::string DeleteGeohashSql{"DELETE FROM markers WHERE geohash BETWEEN ? AND ?;"};
static const std::string ReadSql{
    "SELECT m.id, m.poi_type, m.lastUpdate, m.name, m.searchFilter, m.geohash, ri.minLon, ri.minLat, COALESCE(bp.programTier, -1) programTier "
    "FROM markers m INNER JOIN rIndex ri ON m.Id = ri.Id LEFT JOIN businessProgram bp ON m.Id = bp.Id "
    "WHERE m.id = ?;"};
static const std::string ReadFilteredSql{
    "SELECT m.id, m.poi_type, m.lastUpdate, m.name, m.searchFilter, m.geohash, ri.minLon, ri.minLat, COALESCE(bp.programTier, -1) programTier "
    "FROM markers m INNER JOIN rIndex ri ON m.Id = ri.Id LEFT JOIN businessProgram bp ON m.Id = bp.Id "
    "WHERE minLon > ? AND maxLon < ? "
    "AND minLat > ? AND maxLat < ? "
    "AND m.poi_type & ?;"};
static const std::string ReadIds{
    "SELECT id "
    "FROM markers "
    "ORDER BY lastUpdate ASC, id ASC "
    "LIMIT ? OFFSET ?;"};
static const std::string ReadLastUpdateSql{"SELECT MAX(lastUpdate) FROM markers;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO markers (id, poi_type, lastUpdate, name, searchFilter, geohash) VALUES (?, ?, ?, ?, ?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Marker query object.
//!
//----------------------------------------------------------------
MarkerQuery::MarkerQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadFiltered.reset(new SQLite::Statement{aDatabase, ReadFilteredSql});
    mReadIds.reset(new SQLite::Statement{aDatabase, ReadIds});
    mReadLastUpdate.reset(new SQLite::Statement{aDatabase, ReadLastUpdateSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mRead.reset();
    mReadFiltered.reset();
    mReadIds.reset();
    mReadLastUpdate.reset();
    mWrite.reset();
  }
}  // End of MarkerQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete marker from database
//!
//----------------------------------------------------------------
bool MarkerQuery::Delete(const ACDB_marker_idx_type aId) {
  enum Parameters { Id = 1 };

  if (!mDelete) {
    return false;
  }

  bool success = false;

  try {
    mDelete->bind(Parameters::Id, static_cast<long long>(aId));

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
//!   @brief Delete markers from database by geohash
//!
//----------------------------------------------------------------
bool MarkerQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
  enum Parameters { GeohashStart = 1, GeohashEnd };

  if (!mDeleteGeohash) {
    return false;
  }

  bool success = false;

  try {
    mDeleteGeohash->bind(Parameters::GeohashStart, static_cast<long long>(aGeohashStart));
    mDeleteGeohash->bind(Parameters::GeohashEnd, static_cast<long long>(aGeohashEnd));

    mDeleteGeohash->exec();
    success = mDeleteGeohash->isDone();

    mDeleteGeohash->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the info for the specified object.
//!
//----------------------------------------------------------------
bool MarkerQuery::Get(const ACDB_marker_idx_type aId, MarkerTableDataType& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns {
    ColId = 0,
    PoiType,
    LastUpdate,
    Name,
    SearchFilter,
    Geohash,
    Lon,
    Lat,
    ProgramTier
  };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<long long>(aId));

    success = mRead->executeStep();
    if (success) {
      aResultOut.mId = mRead->getColumn(Columns::ColId).getInt64();
      aResultOut.mType = mRead->getColumn(Columns::PoiType).getInt();
      aResultOut.mLastUpdated = mRead->getColumn(Columns::LastUpdate).getInt64();
      aResultOut.mName = mRead->getColumn(Columns::Name).getText();
      aResultOut.mSearchFilter = mRead->getColumn(Columns::SearchFilter).getInt64();
      aResultOut.mGeohash = mRead->getColumn(Columns::Geohash).getInt64();
      aResultOut.mPosn.lat = mRead->getColumn(Columns::Lat).getUInt();
      aResultOut.mPosn.lon = mRead->getColumn(Columns::Lon).getUInt();
      aResultOut.mBusinessProgramTier = mRead->getColumn(Columns::ProgramTier).getInt();
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
//!   @detail Get a list of item records based on the specified
//!   filer.
//!
//----------------------------------------------------------------
bool MarkerQuery::GetFiltered(const MapMarkerFilter& aFilter,
                              std::vector<MarkerTableDataType>& aResultOut) {
  enum Parameters { MinLon = 1, MaxLon, MinLat, MaxLat, PoiTypeMask };
  enum Columns {
    ColId = 0,
    PoiType,
    LastUpdate,
    Name,
    SearchFilter,
    Geohash,
    Lon,
    Lat,
    ProgramTier
  };

  if (!mReadFiltered) {
    return false;
  }

  bool success = false;

  try {
    mReadFiltered->bind(Parameters::MinLon, aFilter.GetBbox().swc.lon);
    mReadFiltered->bind(Parameters::MaxLon, aFilter.GetBbox().nec.lon);
    mReadFiltered->bind(Parameters::MinLat, aFilter.GetBbox().swc.lat);
    mReadFiltered->bind(Parameters::MaxLat, aFilter.GetBbox().nec.lat);
    mReadFiltered->bind(Parameters::PoiTypeMask, aFilter.GetAllowedTypes());

    while (mReadFiltered->executeStep()) {
      MarkerTableDataType result;
      result.mId = mReadFiltered->getColumn(Columns::ColId).getInt64();
      result.mType = mReadFiltered->getColumn(Columns::PoiType).getInt();
      result.mLastUpdated = mReadFiltered->getColumn(Columns::LastUpdate).getInt64();
      result.mName = mReadFiltered->getColumn(Columns::Name).getText();
      result.mSearchFilter = mReadFiltered->getColumn(Columns::SearchFilter).getInt64();
      result.mGeohash = mReadFiltered->getColumn(Columns::Geohash).getInt64();
      result.mPosn.lat = mReadFiltered->getColumn(Columns::Lat).getUInt();
      result.mPosn.lon = mReadFiltered->getColumn(Columns::Lon).getUInt();
      result.mBusinessProgramTier = mReadFiltered->getColumn(Columns::ProgramTier).getInt();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    mReadFiltered->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetFiltered

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the last update time for markers.
//!
//----------------------------------------------------------------
bool MarkerQuery::GetLastUpdate(uint64_t& aLastUpdateOut) {
  enum Columns { LastUpdate = 0 };

  if (!mReadLastUpdate) {
    return false;
  }

  bool success = false;

  try {
    success = mReadLastUpdate->executeStep();
    if (success) {
      aLastUpdateOut =
          static_cast<uint64_t>(mReadLastUpdate->getColumn(Columns::LastUpdate).getInt64());
    }

    mReadLastUpdate->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetLastUpdate

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Get paginated list of marker IDs in database.  PageNumber starts at 0.
//!
//----------------------------------------------------------------
bool MarkerQuery::GetIds(const uint32_t aPageNumber, const uint32_t aPageSize,
                         std::vector<ACDB_marker_idx_type>& aResultOut) {
  enum Parameters { Limit = 1, Offset };
  enum Columns { Id = 0 };

  if (!mReadIds) {
    return false;
  }

  bool success = false;

  try {
    mReadIds->bind(Parameters::Limit, aPageSize);
    mReadIds->bind(Parameters::Offset, aPageNumber * aPageSize);

    while (mReadIds->executeStep()) {
      aResultOut.push_back(mReadIds->getColumn(Columns::Id).getInt64());
    }

    success = !aResultOut.empty();

    mReadIds->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of GetIds

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write marker to database
//!
//----------------------------------------------------------------
bool MarkerQuery::Write(const ACDB_marker_idx_type aId, MarkerTableDataType&& aMarkerTableData) {
  enum Parameters { Id = 1, PoiType, LastUpdate, Name, SearchFilter, Geohash };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<long long>(aId));
    mWrite->bind(Parameters::PoiType, aMarkerTableData.mType);
    mWrite->bind(Parameters::LastUpdate, static_cast<long long>(aMarkerTableData.mLastUpdated));
    mWrite->bind(Parameters::Name, aMarkerTableData.mName);
    mWrite->bind(Parameters::SearchFilter, static_cast<long long>(aMarkerTableData.mSearchFilter));
    mWrite->bind(Parameters::Geohash, static_cast<long long>(aMarkerTableData.mGeohash));

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of Write

}  // end of namespace Acdb
