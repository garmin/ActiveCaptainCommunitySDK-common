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
#define DBG_TAG "BusinessPhotoQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/BusinessPhotoQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM businessPhotos WHERE id = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM businessPhotos WHERE id IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string ReadSql{
    "SELECT id, ordinal, downloadUrl FROM businessPhotos WHERE id = ? ORDER BY ordinal ASC;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO businessPhotos (id, ordinal, downloadUrl) VALUES (?, ?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create BusinessPhoto query object.
//!
//----------------------------------------------------------------
BusinessPhotoQuery::BusinessPhotoQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mRead.reset();
    mWrite.reset();
  }
}  // End of BusinessPhotoQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete business photo from database
//!
//----------------------------------------------------------------
bool BusinessPhotoQuery::Delete(const ACDB_marker_idx_type aId) {
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
//!   @brief Delete BusinessPhoto from database by geohash
//!
//----------------------------------------------------------------
bool BusinessPhotoQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
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
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool BusinessPhotoQuery::Get(const ACDB_marker_idx_type aId,
                             std::vector<BusinessPhotoTableDataType>& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns { ColId = 0, Ordinal, DownloadUrl };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<int64_t>(aId));

    while (mRead->executeStep()) {
      BusinessPhotoTableDataType result;
      result.mId = mRead->getColumn(Columns::ColId).getInt64();
      result.mOrdinal = mRead->getColumn(Columns::Ordinal).getInt();
      result.mDownloadUrl = mRead->getColumn(Columns::DownloadUrl).getText();
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
//!   @brief Write business photo to database
//!
//----------------------------------------------------------------
bool BusinessPhotoQuery::Write(const ACDB_marker_idx_type aId,
                               BusinessPhotoTableDataType&& aBusinessPhotoTableData) {
  enum Parameters { Id = 1, Ordinal, DownloadUrl };
  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<int64_t>(aId));
    mWrite->bind(Parameters::Ordinal, aBusinessPhotoTableData.mOrdinal);
    mWrite->bind(Parameters::DownloadUrl, aBusinessPhotoTableData.mDownloadUrl);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
