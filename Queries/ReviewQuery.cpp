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

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReviewQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM reviews WHERE reviewId = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM reviews WHERE markerId IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string DeleteMarkerSql{"DELETE FROM reviews WHERE markerId = ?;"};
static const std::string ReadSql{
    "SELECT reviewId, markerId, lastUpdate, title, rating, date, captain, review, votes, response FROM reviews WHERE markerId = ? ORDER BY votes DESC, date DESC LIMIT 1;"};
static const std::string ReadLastUpdateSql{"SELECT MAX(lastUpdate) FROM reviews"};
static const std::string ReadListSql{
    "SELECT reviewId, markerId, lastUpdate, title, rating, date, captain, review, votes, response FROM reviews "
    "WHERE markerId = ? "
    "ORDER BY captain = ? DESC, votes DESC, date DESC "
    "LIMIT ? OFFSET ?;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO reviews (reviewId, markerId, rating, title, date, captain, review, lastUpdate, votes, response) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Review query object.
//!
//----------------------------------------------------------------
ReviewQuery::ReviewQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mDeleteMarker.reset(new SQLite::Statement{aDatabase, DeleteMarkerSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadLastUpdate.reset(new SQLite::Statement{aDatabase, ReadLastUpdateSql});
    mReadList.reset(new SQLite::Statement{aDatabase, ReadListSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mDeleteMarker.reset();
    mRead.reset();
    mReadLastUpdate.reset();
    mReadList.reset();
    mWrite.reset();
  }
}  // End of ReviewQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Delete review from database
//!
//----------------------------------------------------------------
bool ReviewQuery::Delete(const ACDB_review_idx_type aId) {
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
//!   @brief Delete reviews from database by geohash
//!
//----------------------------------------------------------------
bool ReviewQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
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
//!   @detail Delete reviews for the specified marker.
//!
//----------------------------------------------------------------
bool ReviewQuery::DeleteMarker(const ACDB_marker_idx_type aMarkerId) {
  enum Parameters { MarkerId = 1 };

  if (!mDeleteMarker) {
    return false;
  }

  bool success = false;

  try {
    mDeleteMarker->bind(Parameters::MarkerId, static_cast<int64_t>(aMarkerId));

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
//!   @detail Get featured review for the specified marker.
//!
//----------------------------------------------------------------
bool ReviewQuery::Get(const ACDB_marker_idx_type aMarkerId, ReviewTableDataType& aResultOut) {
  enum Parameters { MarkerId = 1 };
  enum Columns {
    ReviewId = 0,
    ColMarkerId,
    LastUpdate,
    Title,
    Rating,
    Date,
    Captain,
    Review,
    Votes,
    Response
  };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::MarkerId, static_cast<int64_t>(aMarkerId));

    success = mRead->executeStep();
    if (success) {
      aResultOut.mId = mRead->getColumn(Columns::ReviewId).getInt64();
      aResultOut.mMarkerId = mRead->getColumn(Columns::ColMarkerId).getInt64();
      aResultOut.mLastUpdated = mRead->getColumn(Columns::LastUpdate).getInt64();
      aResultOut.mTitle = mRead->getColumn(Columns::Title).getText();
      aResultOut.mRating = mRead->getColumn(Columns::Rating).getInt();
      aResultOut.mDate = mRead->getColumn(Columns::Date).getText();
      aResultOut.mCaptain = mRead->getColumn(Columns::Captain).getText();
      aResultOut.mReview = mRead->getColumn(Columns::Review).getText();
      aResultOut.mVotes = mRead->getColumn(Columns::Votes).getInt();
      aResultOut.mResponse = mRead->getColumn(Columns::Response).getText();
      aResultOut.mIsDeleted = false;
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
//!   @detail Get the last update time for reviews.
//!
//----------------------------------------------------------------
bool ReviewQuery::GetLastUpdate(uint64_t& aLastUpdateOut) {
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
//!   @detail Get paginated list of reviews.  Captain's review first
//!           if it exists, then order by votes and date.  PageNumber
//!           starts at 1.
//!
//----------------------------------------------------------------
bool ReviewQuery::GetList(const ACDB_marker_idx_type aMarkerId, const std::string& aCaptain,
                          uint32_t aPageNumber, uint32_t aPageSize,
                          std::vector<ReviewTableDataType>& aResultOut) {
  enum Parameters { MarkerId = 1, Captain, Limit, Offset };
  enum Columns {
    ReviewId = 0,
    ColMarkerId,
    LastUpdate,
    Title,
    Rating,
    Date,
    ColCaptain,
    Review,
    Votes,
    Response
  };

  if (!mReadList) {
    return false;
  }

  bool success = false;

  try {
    mReadList->bind(Parameters::MarkerId, static_cast<int64_t>(aMarkerId));
    mReadList->bind(Parameters::Captain, aCaptain);
    mReadList->bind(Parameters::Limit, aPageSize);
    mReadList->bind(Parameters::Offset, (aPageNumber - 1) * aPageSize);

    while (mReadList->executeStep()) {
      ReviewTableDataType result;
      result.mId = mReadList->getColumn(Columns::ReviewId).getInt64();
      result.mMarkerId = mReadList->getColumn(Columns::ColMarkerId).getInt64();
      result.mLastUpdated = mReadList->getColumn(Columns::LastUpdate).getInt64();
      result.mTitle = mReadList->getColumn(Columns::Title).getText();
      result.mRating = mReadList->getColumn(Columns::Rating).getInt();
      result.mDate = mReadList->getColumn(Columns::Date).getText();
      result.mCaptain = mReadList->getColumn(Columns::ColCaptain).getText();
      result.mReview = mReadList->getColumn(Columns::Review).getText();
      result.mVotes = mReadList->getColumn(Columns::Votes).getInt();
      result.mResponse = mReadList->getColumn(Columns::Response).getText();
      result.mIsDeleted = false;

      aResultOut.push_back(std::move(result));
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
//!   @brief Write review to database
//!
//----------------------------------------------------------------
bool ReviewQuery::Write(const ACDB_review_idx_type aId, ReviewTableDataType&& aReviewTableData) {
  enum Parameters {
    ReviewId = 1,
    MarkerId,
    Rating,
    Title,
    Date,
    Captain,
    Review,
    LastUpdate,
    Votes,
    Response
  };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::ReviewId, static_cast<int64_t>(aId));
    mWrite->bind(Parameters::MarkerId, static_cast<int64_t>(aReviewTableData.mMarkerId));
    mWrite->bind(Parameters::Rating, aReviewTableData.mRating);
    mWrite->bind(Parameters::Title, aReviewTableData.mTitle);
    mWrite->bind(Parameters::Date, aReviewTableData.mDate);
    mWrite->bind(Parameters::Captain, aReviewTableData.mCaptain);
    mWrite->bind(Parameters::Review, aReviewTableData.mReview);
    mWrite->bind(Parameters::LastUpdate, static_cast<int64_t>(aReviewTableData.mLastUpdated));
    mWrite->bind(Parameters::Votes, aReviewTableData.mVotes);
    mWrite->bind(Parameters::Response, aReviewTableData.mResponse);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
