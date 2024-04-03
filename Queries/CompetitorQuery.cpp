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
#define DBG_TAG "CompetitorQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/CompetitorQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM competitor WHERE poiId = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM competitor WHERE poiId IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string ReadSql{
    "SELECT poiId, competitorPoiId, ordinal FROM competitor WHERE poiId = ?;"};
static const std::string ReadAdTargetSql{
    "SELECT CASE WHEN ? IN "
    "( "
    "    SELECT c.competitorPoiId "
    "    FROM competitor c "
    "        LEFT JOIN businessProgram bp ON c.competitorPoiId = bp.Id "
    "    WHERE c.poiId = ? AND (bp.programTier IS NULL OR bp.ProgramTier != 3) "
    "    ORDER BY c.ordinal "
    "    LIMIT 5 "
    ") THEN 1 ELSE 0 END;"};
static const std::string ReadAdvertisersSql{
    "SELECT c.poiId "
    "FROM competitor c "
    "INNER JOIN businessProgram bp ON c.PoiId = bp.Id "
    "WHERE c.competitorPoiId = ? "
    "    AND bp.competitorAd IS NOT NULL "
    "    AND bp.competitorAd != '' "
    "ORDER BY random();"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO competitor (poiId, competitorPoiId, ordinal) VALUES (?, ?, ?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Competitor query object.
//!
//----------------------------------------------------------------
CompetitorQuery::CompetitorQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadAdTarget.reset(new SQLite::Statement{aDatabase, ReadAdTargetSql});
    mReadAdvertisers.reset(new SQLite::Statement{aDatabase, ReadAdvertisersSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mRead.reset();
    mReadAdTarget.reset();
    mReadAdvertisers.reset();
    mWrite.reset();
  }
}  // End of CompetitorQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete competitor from database
//!
//----------------------------------------------------------------
bool CompetitorQuery::Delete(const ACDB_marker_idx_type aId) {
  enum Parameters { Id = 1 };

  if (!mDelete) {
    return false;
  }

  bool success = false;

  try {
    mDelete->bind(Parameters::Id, static_cast<int64_t>(aId));

    mDelete->exec();
    success = mDelete->isDone();

    mDelete->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete competitors from database by geohash
//!
//----------------------------------------------------------------
bool CompetitorQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
  enum Parameters { GeohashStart = 1, GeohashEnd };

  if (!mDeleteGeohash) {
    return false;
  }

  bool success = false;

  try {
    mDeleteGeohash->bind(Parameters::GeohashStart, static_cast<int64_t>(aGeohashStart));
    mDeleteGeohash->bind(Parameters::GeohashEnd, static_cast<int64_t>(aGeohashEnd));

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
//!   @detail Determine if advertiser can advertise on marker.
//!
//----------------------------------------------------------------
bool CompetitorQuery::GetCanTarget(const ACDB_marker_idx_type aTargetId,
                                   const ACDB_marker_idx_type aAdvertiserId, bool& aResultOut) {
  enum Parameters { TargetId = 1, AdvertiserId };
  enum Columns { Value = 0 };

  if (!mReadAdTarget) {
    return false;
  }

  bool success = false;

  try {
    mReadAdTarget->bind(Parameters::TargetId, static_cast<int64_t>(aTargetId));
    mReadAdTarget->bind(Parameters::AdvertiserId, static_cast<int64_t>(aAdvertiserId));

    success = mReadAdTarget->executeStep();
    if (success) {
      aResultOut = (mReadAdTarget->getColumn(Columns::Value).getInt() == 1);
    }

    mReadAdTarget->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetCanTarget

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool CompetitorQuery::Get(const ACDB_marker_idx_type aId,
                          std::vector<CompetitorTableDataType>& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns { PoiId = 0, CompetitorPoiId, Ordinal };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<int64_t>(aId));

    while (mRead->executeStep()) {
      CompetitorTableDataType result;

      result.mId = mRead->getColumn(Columns::PoiId).getInt64();
      result.mCompetitorId = mRead->getColumn(Columns::CompetitorPoiId).getInt64();
      result.mOrdinal = mRead->getColumn(Columns::Ordinal).getInt();

      aResultOut.push_back(std::move(result));
    }
    success = !aResultOut.empty();

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
//!   @detail Get IDs of businesses which have selected this
//!   marker as a competitor.
//!
//----------------------------------------------------------------
bool CompetitorQuery::GetPotentialAdvertisers(const ACDB_marker_idx_type aId,
                                              std::vector<ACDB_marker_idx_type>& aResultOut) {
  enum Parameters { TargetId = 1 };
  enum Columns { AdvertiserId = 0 };

  if (!mReadAdvertisers) {
    return false;
  }

  bool success = false;

  try {
    mReadAdvertisers->bind(Parameters::TargetId, static_cast<int64_t>(aId));

    while (mReadAdvertisers->executeStep()) {
      aResultOut.push_back(mReadAdvertisers->getColumn(Columns::AdvertiserId).getInt64());
    }

    success = !aResultOut.empty();

    mReadAdvertisers->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetPotentialAdvertisers

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write competitor to database
//!
//----------------------------------------------------------------
bool CompetitorQuery::Write(const ACDB_marker_idx_type aId,
                            CompetitorTableDataType&& aCompetitorTableData) {
  enum Parameters { PoiId = 1, CompetitorPoiId, Ordinal };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::PoiId, static_cast<int64_t>(aId));
    mWrite->bind(Parameters::CompetitorPoiId,
                 static_cast<int64_t>(aCompetitorTableData.mCompetitorId));
    mWrite->bind(Parameters::Ordinal, aCompetitorTableData.mOrdinal);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
