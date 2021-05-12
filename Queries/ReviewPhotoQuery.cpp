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

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewPhotoQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/ReviewPhotoQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM reviewPhotos WHERE id = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM reviewPhotos WHERE id IN (SELECT reviewId FROM reviews WHERE markerId IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?));"};
static const std::string DeleteMarkerSql{
    "DELETE FROM reviewPhotos WHERE id IN (SELECT reviewId FROM reviews WHERE markerId = ?);"};
static const std::string ReadSql{
    "SELECT id, ordinal, downloadUrl FROM reviewPhotos WHERE id = ? ORDER BY ordinal ASC;"};
static const std::string ReadListSql{
    "SELECT id, ordinal, downloadUrl FROM reviewPhotos WHERE id IN "
    "(SELECT reviewId FROM reviews WHERE markerId = ? ORDER BY captain = ? DESC, votes DESC, date DESC LIMIT ? OFFSET ?) ORDER BY id ASC, ordinal ASC;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO reviewPhotos (id, ordinal, downloadUrl) VALUES (?, ?, ?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create ReviewPhoto query object.
//!
//----------------------------------------------------------------
ReviewPhotoQuery::ReviewPhotoQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mDeleteMarker.reset(new SQLite::Statement{aDatabase, DeleteMarkerSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadList.reset(new SQLite::Statement{aDatabase, ReadListSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mDeleteMarker.reset();
    mRead.reset();
    mReadList.reset();
    mWrite.reset();
  }
}  // End of ReviewPhotoQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete review photo from database
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::Delete(const ACDB_review_idx_type aId) {
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
}  // End of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete ReviewPhoto from database by geohash
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
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
//!   @brief Delete review photo by marker id from database
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::DeleteMarker(const ACDB_marker_idx_type aMarkerId) {
  enum Parameters { MarkerId = 1 };

  if (!mDeleteMarker) {
    return false;
  }

  bool success = false;

  try {
    mDeleteMarker->bind(Parameters::MarkerId, static_cast<long long>(aMarkerId));

    mDeleteMarker->exec();
    success = mDeleteMarker->isDone();

    mDeleteMarker->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }
  return success;
}  // End of DeleteMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::Get(const ACDB_review_idx_type aId,
                           std::vector<ReviewPhotoTableDataType>& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns { ColId = 0, Ordinal, DownloadUrl };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<long long>(aId));

    while (mRead->executeStep()) {
      ReviewPhotoTableDataType result;
      result.mId = mRead->getColumn(Columns::ColId).getInt64();
      result.mOrdinal = mRead->getColumn(Columns::Ordinal).getInt();
      result.mDownloadUrl = mRead->getColumn(Columns::DownloadUrl).getText();
      aResultOut.emplace_back(std::move(result));
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
//!   @detail Get the review photos for the specified marker id.
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::GetListByMarkerId(
    const ACDB_marker_idx_type aMarkerId, const std::string& aCaptain, uint32_t aPageNumber,
    uint32_t aPageSize,
    std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>>& aResultOut) {
  enum Parameters { MarkerId = 1, Captain, Limit, Offset };
  enum Columns { ColId = 0, Ordinal, DownloadUrl };

  if (!mReadList) {
    return false;
  }

  bool success = false;

  try {
    mReadList->bind(Parameters::MarkerId, static_cast<long long>(aMarkerId));
    mReadList->bind(Parameters::Captain, aCaptain);
    mReadList->bind(Parameters::Limit, aPageSize);
    mReadList->bind(Parameters::Offset, (aPageNumber - 1) * aPageSize);

    while (mReadList->executeStep()) {
      ReviewPhotoTableDataType result;
      result.mId = mReadList->getColumn(Columns::ColId).getInt64();
      result.mOrdinal = mReadList->getColumn(Columns::Ordinal).getInt();
      result.mDownloadUrl = mReadList->getColumn(Columns::DownloadUrl).getText();

      std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>>::iterator it =
          aResultOut.find(result.mId);

      if (it != aResultOut.end()) {
        it->second.emplace_back(result);
      } else {
        std::vector<ReviewPhotoTableDataType> newData{result};
        aResultOut.insert(std::make_pair(result.mId, newData));
      }
    }

    success = !aResultOut.empty();

    mReadList->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetList

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write review photo to database
//!
//----------------------------------------------------------------
bool ReviewPhotoQuery::Write(const ACDB_review_idx_type aId,
                             ReviewPhotoTableDataType&& aReviewPhotoTableData) {
  enum Parameters { Id = 1, Ordinal, DownloadUrl };
  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<long long>(aId));
    mWrite->bind(Parameters::Ordinal, aReviewPhotoTableData.mOrdinal);
    mWrite->bind(Parameters::DownloadUrl, aReviewPhotoTableData.mDownloadUrl);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
