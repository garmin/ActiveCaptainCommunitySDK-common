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
    @brief contains access to ActiveCaptain database.

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "Repository"

#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <time.h>
#include <utility>
#include <vector>

#include "DBG_pub.h"
#include "Acdb/DatabaseConfig.hpp"
#include "Acdb/EventDispatcher.hpp"
#include "Acdb/FileUtil.hpp"
#include "Acdb/MapMarker.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/Repository.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
#include "Acdb/RwlLocker.hpp"
#include "Acdb/SqliteCppUtil.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Version.hpp"
#include "Acdb/TextTranslator.hpp"

#include "acdb_prv_config.h"

#if (acdb_MFD_DB_SHARING_SUPPORT)
#include "Acdb/Manifest.hpp"
#include "SYNC_pub.h"
#include "SYNC_pub_dataset.h"
#include "UFS_pub.h"
#include "UFS_pub_search.h"
#include "UFS_pub_types.h"
#include "VCM_pub_card.h"
#endif

#if (acdb_SIDELOAD_SERVER_SUPPORT)
#include "Acdb/SideloadServer.hpp"
#endif

namespace Acdb {
const char* ExternalDbPath = "/Garmin/acdb";
const std::string DbName("active_captain");
const std::string DbExt(".db");
const std::string JournalExt("-wal");
const std::string TmpExt(".tmp");
const std::string ZipExt(".gz");
const std::string SupportedSchemaVer("2.0.0.0");

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
Repository::Repository(const std::string& aDbPath)
    : mDbPath(aDbPath),
      mRwl(),
      mInfoAdapter(),
      mMarkerAdapter(),
      mPresentationAdapter(),
      mTranslationAdapter(),
      mUpdateAdapter() {}  // end of Repository

//----------------------------------------------------------------
//!
//!       @public
//!       @details Start a transaction.  The caller must hold the
//!                database write lock. Assumes the database is
//!                open.
//!
//----------------------------------------------------------------
bool Repository::BeginTransaction() const {
  DBG_ASSERT(mDatabase, "Database must be open.");

  try {
    mDatabase->exec("BEGIN TRANSACTION;");
    return true;
  } catch (...) {
    return false;
  }

}  // end of BeginTransaction

//----------------------------------------------------------------
//!
//!       @public
//!       @details Finalize the transaction, it will be committed
//!                if success is true or rolled back if it is false.
//!                Assumes the database is open.
//!
//----------------------------------------------------------------
void Repository::EndTransaction(const bool aSuccess) const {
  bool success = aSuccess;

  DBG_ASSERT(mDatabase, "Database must be open.");

  if (success) {
    try {
      mDatabase->exec("END TRANSACTION;");
    } catch (...) {
      success = false;
    }
  }

  if (!success) {
    DBG_D("Transaction failed, rolling back.");

    try {
      mDatabase->exec("ROLLBACK;");
    } catch (...) {
    }
  }
}  // end of EndTransaction

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Apply the marker update lists to database
//!
//!       Takes a list of item entries and performs one of the
//!       following actions based on the item's type:
//!        * Removes deleted item from the database.
//!        * Inserts new item to the database.
//!        * Updates existing items in the database.
//!
//!       @returns true on success, false otherwise.
//!
//----------------------------------------------------------------
bool Repository::ApplyMarkerUpdateToDb(std::vector<MarkerTableDataCollection>& aMarkerList,
                                       const TileXY* aTileXY) {
  if (aMarkerList.empty()) {
    return false;
  }

  RwlLocker locker{mRwl, true};

  if (!IsOpen()) {
    DBG_ASSERT_ALWAYS("Database is not open. Update applied in bad state.");
    return false;
  }

  bool success = BeginTransaction();

  // Process all marker entries
  if (success) {
    uint64_t lastUpdateMax = 0;

    success = mUpdateAdapter->UpdateMarkers(aMarkerList, lastUpdateMax);

    // If this update came from syncing a tile, update tileLastUpdate table.
    if (aTileXY != nullptr) {
      LastUpdateInfoType lastUpdateInfo;

      success = success && mInfoAdapter->GetTileLastUpdateInfo(*aTileXY, lastUpdateInfo);

      // Sanity check -- only write a new lastUpdate value if it's newer than what we already have.
      if (lastUpdateMax > lastUpdateInfo.mMarkerLastUpdate) {
        lastUpdateInfo.mMarkerLastUpdate = lastUpdateMax;
        success = success && mInfoAdapter->WriteTileLastUpdateInfo(*aTileXY, lastUpdateInfo);
      }
    }
  }

  EndTransaction(success);

  return success;

}  // end of ApplyMarkerUpdateToDb

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Apply the review update lists to database
//!
//!        Takes a list of review entries and performs one of the
//!        following actions:
//!         * Inserts new review to the database.
//!         * Updates an existing review in the database
//!
//!       @returns true on success, false otherwise.
//!
//----------------------------------------------------------------
bool Repository::ApplyReviewUpdateToDb(std::vector<ReviewTableDataCollection>& aReviewList,
                                       const TileXY* aTileXY) {
  if (aReviewList.empty()) {
    return false;
  }

  RwlLocker locker{mRwl, true};

  if (!IsOpen()) {
    DBG_ASSERT_ALWAYS("Database is not open. Update applied in bad state.");
    return false;
  }

  bool success = BeginTransaction();

  // Process all review entries
  if (success) {
    uint64_t lastUpdateMax = 0;

    success = mUpdateAdapter->UpdateReviews(aReviewList, lastUpdateMax);

    // If this update came from syncing a tile, update tileLastUpdate table.
    if (aTileXY != nullptr) {
      LastUpdateInfoType lastUpdateInfo;

      success = success && mInfoAdapter->GetTileLastUpdateInfo(*aTileXY, lastUpdateInfo);

      // Sanity check -- only write a new lastUpdate value if it's newer than what we already have.
      if (lastUpdateMax > lastUpdateInfo.mUserReviewLastUpdate) {
        lastUpdateInfo.mUserReviewLastUpdate = lastUpdateMax;
        success = success && mInfoAdapter->WriteTileLastUpdateInfo(*aTileXY, lastUpdateInfo);
      }
    }
  }

  EndTransaction(success);

  return success;
}  // end of ApplyReviewUpdateToDb

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Apply the updates to support tables to database
//!
//!        Takes a list of entries and performs one of the
//!        following actions:
//!         * Inserts new entry to the database.
//!         * Updates an existing entry in the database
//!
//!       @returns true on success, false otherwise.
//!
//----------------------------------------------------------------
bool Repository::ApplySupportTableUpdateToDb(
    std::vector<LanguageTableDataType>& aLanguageList,
    std::vector<MustacheTemplateTableDataType>& aMustacheTemplateList,
    std::vector<TranslationTableDataType>& aTranslations) {
  RwlLocker locker{mRwl, true};

  if (!IsOpen()) {
    DBG_ASSERT_ALWAYS("Database is not open. Update applied in bad state.");
    return false;
  }

  bool success = BeginTransaction();

  success = success && mUpdateAdapter->UpdateSupportTables(aLanguageList, aMustacheTemplateList,
                                                           aTranslations);

  EndTransaction(success);

  return success;
}  // end of ApplySupportTableUpdateToDb

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Deletes the Active Captain database and notifies listeners.
//!
//----------------------------------------------------------------
void Repository::Delete() {
  if (DeleteDatabaseFile()) {
    EventDispatcher::SendEvent(MessageId::StateNotInstalled);
  }
}  // end of Delete

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a list of business photos for a marker.  This
//!       returns all presentation data regarding the specified
//!       photos.
//!
//----------------------------------------------------------------
Presentation::BusinessPhotoListPtr Repository::GetBusinessPhotoList(
    const ACDB_marker_idx_type aIdx) {
  Presentation::BusinessPhotoListPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mPresentationAdapter->GetBusinessPhotoList(aIdx);
  }

  return result;
}  // end of GetBusinessPhotoList

//----------------------------------------------------------------
//!
//!       @public
//!       @details
//!       Returns the size of the database on disk.
//!
//----------------------------------------------------------------
void Repository::GetDatabaseSize(uint64_t& aUsedSpace) {
  FileUtil::GetSize(GetDbPath(), aUsedSpace);
}  // end of GetDatabaseSize

//----------------------------------------------------------------
//!
//!       @public
//!       @details Retrieve version and last update info for the
//!                specified database
//!
//!       @returns true on success and outputs the version and
//!                most recent marker and user review timestamps.
//!                Returns false on failure.
//!
//----------------------------------------------------------------
bool Repository::GetDbFileVersionInfo(const std::string& aPath, Version& aVersionOut,
                                      LastUpdateInfoType& aUpdateInfoOut) const {
  bool success(true);
  Version retVersion;

  // Check encryption:
  // Previous versions of the module used an encrypted database. In case of an update, this
  // encrypted file might still be in place. We would not be able to open it.
  success = IsValidDatabaseFile(aPath);
  DBG_I_IF(
      !success,
      "Invalid DB file found, possibly an encrypted file from a previously installed app version.");

  // Open database
  std::string expandedPath = DatabaseConfig::GetExpandedPath(aPath);
  auto tempDatabase = OpenDatabaseFile(expandedPath);
  success = success && tempDatabase;
  if (success) {
    InfoAdapter infoAdapter{*tempDatabase};

    // Setup to access the database.
    success = ReadyDbAccess(*tempDatabase);
    DBG_W_IF(!success, "Failed to access database.");

    // Retrieve last update info
    success = success && infoAdapter.GetLastUpdateInfo(aUpdateInfoOut);

    // Retrieve version info
    if (success) {
      infoAdapter.GetVersion(aVersionOut);
    }
  }

  return success;
}  // end of GetDbFileVersionInfo

//----------------------------------------------------------------
//!
//!       @public
//!       @details Retrieve last update info for the open database
//!
//!       @returns true on success and outputs the most recent marker and
//!       user review timestamps in the database.  Returns false
//!       on failure.
//!
//----------------------------------------------------------------
bool Repository::GetLastUpdateInfo(LastUpdateInfoType& aUpdateInfoOut) {
  bool result(false);

  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = BeginTransaction();
    result = result && mInfoAdapter->GetLastUpdateInfo(aUpdateInfoOut);
    EndTransaction(result);
  }

  return result;
}  // end of GetLastUpdateInfo

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Marker of the respository.  This
//!       returns all data regarding the specified marker.
//!
//----------------------------------------------------------------
IMapMarkerPtr Repository::GetMapMarker(const ACDB_marker_idx_type aIdx) {
  IMapMarkerPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mMarkerAdapter->GetMapMarker(aIdx);
  }

  return result;
}  // end of GetMapMarker

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Marker of the respository.  This
//!       returns all data regarding the specified marker.
//!
//----------------------------------------------------------------
ISearchMarkerPtr Repository::GetSearchMarker(const ACDB_marker_idx_type aIdx) {
  ISearchMarkerPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mMarkerAdapter->GetSearchMarker(aIdx);
  }

  return result;
}  // end of GetSearchMarker

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find points in the provided bounding box.
//!
//----------------------------------------------------------------
void Repository::GetMapMarkersByFilter(const MapMarkerFilter& aFilter,
                                       std::vector<IMapMarkerPtr>& aResults) {
  RwlLocker locker{mRwl, false};
  if (!mDatabase) {
    return;
  }

  bbox_type leftBbox;
  bbox_type rightBbox;
  if (MakeSplitBoundingBoxForCrossMeridianSearch(aFilter.GetBbox(), leftBbox, rightBbox)) {
    MapMarkerFilter adaptedFilter = aFilter;

    std::vector<IMapMarkerPtr> leftResults;
    adaptedFilter.SetBbox(leftBbox);
    mMarkerAdapter->GetMapMarkersByFilter(adaptedFilter, leftResults);

    std::vector<IMapMarkerPtr> rightResults;
    adaptedFilter.SetBbox(rightBbox);
    mMarkerAdapter->GetMapMarkersByFilter(adaptedFilter, rightResults);

    std::move(leftResults.begin(), leftResults.end(), std::back_inserter(aResults));
    std::move(rightResults.begin(), rightResults.end(), std::back_inserter(aResults));
  } else {
    mMarkerAdapter->GetMapMarkersByFilter(aFilter, aResults);
  }
}  // end of GetMapMarkersByFilter

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get data for merging
//!
//----------------------------------------------------------------
bool Repository::GetMergePageData(const int aPageNumber, const int aPageSize,
                                  std::vector<MarkerTableDataCollection>& aMarkers_out,
                                  std::vector<ReviewTableDataCollection>& aReviews_out) {
  bool success = false;

  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    aMarkers_out.clear();
    aReviews_out.clear();

    aMarkers_out.reserve(MergePageSize);

    // Create a transaction while retrieving data from the database.  This avoids having to acquire
    // a lock for every SELECT statement.
    success = BeginTransaction();

    std::vector<ACDB_marker_idx_type> markerIds;
    markerIds.reserve(MergePageSize);
    GetMergeMarkerIds(aPageNumber, aPageSize, markerIds);

    for (auto markerId : markerIds) {
      MarkerTableDataCollection marker;
      success = success && GetMergeMarker(markerId, marker);
      if (success) {
        aMarkers_out.push_back(std::move(marker));

        std::vector<ReviewTableDataCollection> markerReviews;
        success = success && GetMergeReviews(markerId, markerReviews);
        if (!markerReviews.empty()) {
          aReviews_out.insert(aReviews_out.end(), markerReviews.begin(), markerReviews.end());
        }
      }
    }

    EndTransaction(success);
  }

  return success;

}  // end of GetMergePageData

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get marker for merging
//!
//----------------------------------------------------------------
bool Repository::GetMergeMarker(const ACDB_marker_idx_type aIdx,
                                MarkerTableDataCollection& aMarker) {
  Presentation::PresentationMarkerPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    aMarker = mMergeAdapter->GetMarker(aIdx);
    return true;
  }

  return false;

}  // end of GetMergeMarker

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find marker IDs for merging
//!
//----------------------------------------------------------------
void Repository::GetMergeMarkerIds(const uint32_t aPageNumber, const uint32_t aPageSize,
                                   std::vector<ACDB_marker_idx_type>& aResults) {
  mMergeAdapter->GetMarkerIds(aPageNumber, aPageSize, aResults);
}  // end of GetMergeMarkerIds

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get reviews for merging
//!
//----------------------------------------------------------------
bool Repository::GetMergeReviews(const ACDB_marker_idx_type aIdx,
                                 std::vector<ReviewTableDataCollection>& aReviews) {
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    aReviews = mMergeAdapter->GetReviews(aIdx);
    return true;
  }

  return false;
}  // end of GetMergeReviews

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find points matching the provided filter
//!
//----------------------------------------------------------------
void Repository::GetBasicSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                               std::vector<ISearchMarkerPtr>& aResults) {
  RwlLocker locker{mRwl, false};
  if (!mDatabase) {
    return;
  }

  bbox_type leftBbox;
  bbox_type rightBbox;
  if (MakeSplitBoundingBoxForCrossMeridianSearch(aFilter.GetBbox(), leftBbox, rightBbox)) {
    SearchMarkerFilter adaptedFilter = aFilter;

    std::vector<ISearchMarkerPtr> leftResults;
    adaptedFilter.SetBbox(leftBbox);
    mMarkerAdapter->GetBasicSearchMarkersByFilter(adaptedFilter, leftResults);

    std::vector<ISearchMarkerPtr> rightResults;
    adaptedFilter.SetBbox(rightBbox);
    mMarkerAdapter->GetBasicSearchMarkersByFilter(adaptedFilter, rightResults);

    std::move(leftResults.begin(), leftResults.end(), std::back_inserter(aResults));
    std::move(rightResults.begin(), rightResults.end(), std::back_inserter(aResults));
  } else {
    mMarkerAdapter->GetBasicSearchMarkersByFilter(aFilter, aResults);
  }
}  // end of GetBasicSearchMarkersByFilter

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find points matching the provided filter
//!
//----------------------------------------------------------------
void Repository::GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                          std::vector<ISearchMarkerPtr>& aResults) {
  RwlLocker locker{mRwl, false};
  if (!mDatabase) {
    return;
  }

  bbox_type leftBbox;
  bbox_type rightBbox;
  if (MakeSplitBoundingBoxForCrossMeridianSearch(aFilter.GetBbox(), leftBbox, rightBbox)) {
    SearchMarkerFilter adaptedFilter = aFilter;

    std::vector<ISearchMarkerPtr> leftResults;
    adaptedFilter.SetBbox(leftBbox);
    mMarkerAdapter->GetSearchMarkersByFilter(adaptedFilter, leftResults);

    std::vector<ISearchMarkerPtr> rightResults;
    adaptedFilter.SetBbox(rightBbox);
    mMarkerAdapter->GetSearchMarkersByFilter(adaptedFilter, rightResults);

    std::move(leftResults.begin(), leftResults.end(), std::back_inserter(aResults));
    std::move(rightResults.begin(), rightResults.end(), std::back_inserter(aResults));
  } else {
    mMarkerAdapter->GetSearchMarkersByFilter(aFilter, aResults);
  }
}  // end of GetSearchMarkersByFilter

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns Mustache template with specified name.
//!
//----------------------------------------------------------------
std::string Repository::GetMustacheTemplate(const std::string& aName) {
  std::string result;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mPresentationAdapter->GetTemplate(aName);
  }

  return result;
}  // end of GetMustacheTemplate

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Marker of the respository.  This
//!       returns all presentation data regarding the specified
//!       marker.
//!
//----------------------------------------------------------------
Presentation::PresentationMarkerPtr Repository::GetPresentationMarker(
    const ACDB_marker_idx_type aIdx, const std::string& aCaptainName) {
  Presentation::PresentationMarkerPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mPresentationAdapter->GetMarker(aIdx, aCaptainName);
  }

  return result;
}  // end of GetPresentationMarker

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a list of reviews for a marker.  This returns
//!       all presentation data regarding the specified reviews.
//!
//----------------------------------------------------------------
Presentation::ReviewListPtr Repository::GetReviewList(const ACDB_marker_idx_type aIdx,
                                                      const int aPageNumber, const int aPageSize,
                                                      const std::string& aCaptainName) {
  Presentation::ReviewListPtr result = nullptr;
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mPresentationAdapter->GetReviewList(aIdx, aPageNumber, aPageSize, aCaptainName);
  }

  return result;
}  // end of GetReviewList

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns support table data for merging
//!
//----------------------------------------------------------------
bool Repository::GetSupportTableData(std::vector<LanguageTableDataType>& aLanguages,
                                     std::vector<MustacheTemplateTableDataType>& aMustacheTemplates,
                                     std::vector<TranslationTableDataType>& aTranslations) {
  bool success = false;

  if (mDatabase) {
    success = BeginTransaction();
    success = success &&
              mMergeAdapter->GetSupportTableData(aLanguages, aMustacheTemplates, aTranslations);
    EndTransaction(success);
  }

  return success;
}  // end of GetSupportTableData

//----------------------------------------------------------------
//!
//!       @public
//!       @details Retrieve last update info for the open database
//!
//!       @returns true on success and outputs the most recent marker and
//!       user review timestamps in the database.  Returns false
//!       on failure.
//!
//----------------------------------------------------------------
bool Repository::GetTileLastUpdateInfo(const TileXY& aTileXY, LastUpdateInfoType& aUpdateInfoOut) {
  bool result(false);

  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    result = mInfoAdapter->GetTileLastUpdateInfo(aTileXY, aUpdateInfoOut);
  }

  return result;
}  // end of GetTileLastUpdateInfo

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get tiles, last update info overlapped by the given
//!    bounding boxes.
//!
//----------------------------------------------------------------
void Repository::GetTilesLastUpdateInfoByBoundingBoxes(
    const std::vector<bbox_type>& aBboxes, std::map<TileXY, LastUpdateInfoType>& aTiles) {
  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    for (auto bbox : aBboxes) {
      bbox_type leftBbox;
      bbox_type rightBbox;
      if (MakeSplitBoundingBoxForCrossMeridianSearch(bbox, leftBbox, rightBbox)) {
        mInfoAdapter->GetTileLastUpdateInfoBbox(leftBbox, aTiles);
        mInfoAdapter->GetTileLastUpdateInfoBbox(rightBbox, aTiles);
      } else {
        mInfoAdapter->GetTileLastUpdateInfoBbox(bbox, aTiles);
      }
    }
  }
}  // end of GetTilesLastUpdateInfoByBoundingBoxes

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get the average star rating for the specified marker.
//!
//----------------------------------------------------------------
float Repository::GetUserReviewAverageStars(const ACDB_marker_idx_type aIdx) {
  float result = 0;
  RwlLocker locker{mRwl, false};

  if (mDatabase) {
    result = mMarkerAdapter->GetAverageStars(aIdx);
  }

  return result;
}  // end of GetUserReviewAverageStars

//----------------------------------------------------------------
//!
//!       @public
//!       @details Informs the caller if the repository is
//!                 open and usable
//!
//----------------------------------------------------------------
bool Repository::IsOpen() { return (mDatabase != nullptr); }  // end of IsOpen

//----------------------------------------------------------------
//!
//!       @public
//!       @detail
//!       Open an ActiveCaptain database.  It is acceptable if
//!       the database is not present in the expected location.
//!
//!       @returns if the open was successful
//----------------------------------------------------------------
bool Repository::Open() {
  bool success = OpenDatabase(true /*updateStateOnFailure*/);

  if (success) {
    EventDispatcher::SendEvent(MessageId::StateInstalled);
  } else {
    EventDispatcher::SendEvent(MessageId::StateNotInstalled);
  }

  return success;
}  // end of Open

//----------------------------------------------------------------
//!
//!       @private
//!       @detail
//!       Private implementation of Open(), allowing the caller to
//!       determine if a status update should be send automatically at
//!       the end.
//!
//!       @returns if the open was successful
//----------------------------------------------------------------
bool Repository::OpenDatabase(bool updateStateOnFailure) {
  bool success{true};
  bool notCompatible{false};

  // Find paths to database
  std::string filePath = GetDbPath();
  std::string expandedPath = DatabaseConfig::GetExpandedPath(filePath);

  success = (!expandedPath.empty() && !filePath.empty());

  // Database may not be present, so tolerate this failure.
  success = success && FileUtil::Exists(filePath);

  // Check encryption:
  // Previous versions of the module used an encrypted database. In case of an update, this
  // encrypted file might still be in place. We would not be able to open it.
  bool invalidFile = false;
  if (success) {
    success = IsValidDatabaseFile(filePath);
    if (!success) {
      DBG_I(
          "Invalid DB file found, possibly an encrypted file from a previously installed app version.");
      invalidFile = true;
    }
  }

  RwlLocker locker{mRwl, true};

  if (mDatabase) {
    // the database is already open. Nothing to do.
    return true;
  }

  // Open database file
  if (success) {
    mDatabase = OpenDatabaseFile(expandedPath);
    success = (mDatabase != nullptr);
  }

  // Setup to access the database.  If anything goes wrong here, delete the database so we can get a
  // new copy.
  if (success) {
    success = ReadyDbAccess(*mDatabase);
    if (!success) {
      DBG_W("Failed access setup, removing from system.");
      invalidFile = true;
    }
  }

  if (success) {
    mInfoAdapter.reset(new InfoAdapter{*mDatabase});
    mMarkerAdapter.reset(new MarkerAdapter{*mDatabase});
    mMergeAdapter.reset(new MergeAdapter{*mDatabase});
    mPresentationAdapter.reset(new PresentationAdapter{*mDatabase});
    mTranslationAdapter.reset(new TranslationAdapter{*mDatabase});
    mUpdateAdapter.reset(new UpdateAdapter{*mDatabase});

    // check if compatible
    Version newVersion;
    mInfoAdapter->GetVersion(newVersion);
    if (!newVersion.SchemaCompatible()) {
      success = false;
      notCompatible = true;
      Close();
    }
  }

  if (notCompatible || invalidFile) {
    if (updateStateOnFailure) {
      Delete();  // this updates the module state after deletion
    } else {
      DeleteDatabaseFile();
    }
  }

  return success;
}  // end of OpenDatabase

//----------------------------------------------------------------
//!
//!       @public
//!       @details Releases prepared queries and closes the
//!                underlying .db file
//!
//!       @returns if the close was successful
//!
//----------------------------------------------------------------
void Repository::Close() {
  RwlLocker locker{mRwl, true};

  DBG_D_IF(!mDatabase, "DB already closed");

  if (mDatabase) {
    mUpdateAdapter.reset();
    mInfoAdapter.reset();
    mMarkerAdapter.reset();
    mMergeAdapter.reset();
    mPresentationAdapter.reset();
    mTranslationAdapter.reset();
    mDatabase.reset();
  }
}  // end of Close

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//----------------------------------------------------------------
Version Repository::GetVersion() {
  Version retVersion;

  RwlLocker locker{mRwl, false};
  if (mDatabase) {
    mInfoAdapter->GetVersion(retVersion);
  }

  return retVersion;
}  // end of GetVersion

//----------------------------------------------------------------
//!
//!       @public
//!       @details
//!       Gets the local path to the Active Captain database.
//!
//!       @returns the path or an empty string on failure.
//!
//----------------------------------------------------------------
std::string Repository::GetDbPath() {
  if (!mDbPath.empty()) {
    return mDbPath;
  }

  std::string ret;

#if (acdb_MFD_DB_SHARING_SUPPORT)
  char* cardPath = NULL;

  VCM_card_path_get(&cardPath);
  if (NULL != cardPath) {
    std::string searchPath = String::Format("%s%s/", cardPath, ExternalDbPath);
    FindNewestDbFile(searchPath, ret);
  }
  free(cardPath);
#else
  std::string dbBasePath = DatabaseConfig::GetBasePath();
  ret = String::Format("%s/%s%s", dbBasePath.c_str(), DbName.c_str(), DbExt.c_str());
#endif

  DBG_D_IF(ret.empty(), "Found no DB");

  return ret;
}  // end of GetDbPath

//----------------------------------------------------------------
//!
//!       @public
//!       @details
//!       Gets the local path to the Active Captain database's journal.
//!
//!       @returns the path or an empty string on failure.
//!
//----------------------------------------------------------------
std::string Repository::GetDbJournalPath() {
  std::string dbPath = GetDbPath();
  if (dbPath.empty()) {
    return dbPath;
  }

  return dbPath + JournalExt;
}  // end of GetDbJournalPath

//----------------------------------------------------------------
//!
//!       @public
//!       @details Begin the sideload process by locking the repository.
//!
//!       @return if the lock was successful.
//!
//----------------------------------------------------------------
bool Repository::BeginSideload() {
  if (!mDatabase) {
    return false;
  }

  // Flush WAL file to the database file after each update to
  // make sure the database is prepared for a sideload
  SqliteCppUtil::FlushWalFile(*mDatabase);
  mRwl.LockShared();
  return true;
}  // end of BeginSideload

//----------------------------------------------------------------
//!
//!       @public
//!       @details End the sideload process by unlocking the repository.
//!
//----------------------------------------------------------------
void Repository::EndSideload() { mRwl.Unlock(); }  // end of EndSideload

//----------------------------------------------------------------
//!
//!       @public
//!       @details
//!       Turn off the WAL journaling mode of the specified database
//!       file so that it can be opened in read-only mode by
//!       multiple chartplotters. WAL mode requires R/W access, and
//!       this would preclude multiple openers.
//!
//!       ToDo: move this function out of Repository by refactoring
//!        SQL interface.
//!
//!       @returns true on success.
//----------------------------------------------------------------
bool Repository::PrepareSharedDb(const std::string& aPath) const {
  bool success{false};

#if (acdb_MFD_DB_SHARING_SUPPORT)
  success = (!aPath.empty()) && IsValidDatabaseFile(aPath);
  DBG_D_IF(!success, "Shared DB %s is not a valid database", aPath.c_str());

  if (success) {
    DBG_D("Preparing DB %s for shared access", aPath.c_str());

    try {
      // Open in R/W mode, as we'll be modifying the database
      // incoming path is already absolute; no need to expand (GetExpandedPath() is an empty dummy
      // anyway in case of acdb_MFD_DB_SHARING_SUPPORT)
      std::unique_ptr<SQLite::Database> tempDatabase = SqliteCppUtil::OpenDatabaseFileExt(
          aPath, SQLite::OPEN_READWRITE | SQLite::OPEN_FULLMUTEX);
      success = (tempDatabase != nullptr);
      DBG_E_IF(!success, "Failed to open database.");

      // Database has WAL enabled by default. A bit of a catch-22 that
      // we have to support WAL during this function in order to write
      // to the DB to disable it (below). Some VFS implementations do
      // not support shared-memory primitives and the only way to use
      // WAL with them is to set the locking mode to EXCLUSIVE.
      success = success &&
                SqliteCppUtil::SetLockingMode(*tempDatabase, SqliteCppUtil::LockingMode::Exclusive);
      DBG_E_IF(!success, "Failed to set locking mode.");

      if (success) {
        // Set journal mode to 'delete', which is a non-WAL mode. Note that this
        // will change the version DB file format version back to 1 (legacy).
        SqliteCppUtil::SetJournalMode(*tempDatabase, SqliteCppUtil::JournalMode::Delete);
        DBG_E_IF(!success, "Failed to set journal mode.");
      }
    } catch (const SQLite::Exception& e) {
      DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
      success = false;
    }
  }
#endif

  return success;
}  // end of PrepareSharedDb

//----------------------------------------------------------------
//!
//!       @public
//!       @details Loads a language's strings.
//!
//----------------------------------------------------------------
void Repository::SetLanguage(const std::string& aLanguage) {
  RwlLocker locker{mRwl, true};
  if (mDatabase) {
    mTranslationAdapter->InitTextTranslator(aLanguage);
  }
}  // end of SetLanguage

//----------------------------------------------------------------
//!
//!       @private
//!       @details Releases prepared queries and deletes the
//!                underlying .db file
//!
//----------------------------------------------------------------
bool Repository::DeleteDatabaseFile() {
  bool success = false;
  RwlLocker locker{mRwl, true};

  Close();

  std::string path = DatabaseConfig::GetExpandedPath(GetDbPath());
  bool dropDb = true;

#if (acdb_MFD_DB_SHARING_SUPPORT)
  boolean isMaster;

  // Only the master should delete the database
  dropDb = ((SYNC_OK == SYNC_dataset_master_get(SYNC_DS_AC_COMMUNITY, &isMaster)) && isMaster);
#endif

  if (dropDb) {
    success = SqliteCppUtil::DropDatabaseFileExt(path);
  }

#if (acdb_SIDELOAD_SERVER_SUPPORT)
  // Delete manifest file, so sideload can provide a new one.
  if (success) {
    SideloadServer::GetInstance().DeleteManifestFile();
  }
#endif

  // special case: SQL might completely fail to handle the current DB file
  // (e.g. when we hit an old encrypted database file)
  // then SqliteCppUtil::DropDatabaseExt() will delete the DB file itself, but not the journal
  // the old journal may cause trouble after a newer DB file was loaded
  // => must delete that manually, if necessary.
  std::string journalPath = GetDbJournalPath();
  if (!journalPath.empty()) {
    FileUtil::Delete(journalPath);
  }

  DBG_D_IF(!success, "Failed to delete database");
  return success;
}  // end of DeleteDatabaseFile

//----------------------------------------------------------------
//!
//!       @private
//!       @details Finds the newest Active Captain DB file at the
//!                path specified. Under normal conditions, there
//!                should only be one eligible file found. A
//!                failure during the cleanup after a side-load,
//!                however, could cause there to be more than one.
//!                In this situation, find the most up-to-date
//!                valid database, until the old one(s) can be
//!                cleansed.
//!
//!                ToDo: move this function out of Repository by
//!                refactoring SQL interface.
//!
//!       @returns true on success and outputs the full path to
//!                the chosen DB. Returns false on failure.
//!
//----------------------------------------------------------------
bool Repository::FindNewestDbFile(const std::string aPath, std::string& aFilenameOut) const {
  bool success(false);

#if (acdb_MFD_DB_SHARING_SUPPORT)
  UFS_dir_hndl_type dirHndl;
  std::string newestDbName;

  // Open folder to scan
  dirHndl = UFS_opendir(aPath.c_str());
  success = UFS_dh_is_valid(dirHndl);
  if (success) {
    UFS_dirent_type dirEntry;
    Manifest newestDb;

    std::string dbPattern = String::Format("%s%s", "*", DbExt.c_str());

    // Check each database file (ignoring temp files, manifest, etc.)
    while (UFS_find_next_file(dirHndl, dbPattern.c_str(), &dirEntry) == UFS_RESULT_OK) {
      Version version;
      LastUpdateInfoType updateInfo;
      bool result(true);

      std::string dbPath = String::Format("%s%s", aPath.c_str(), dirEntry.name);

      // Get version and update times of this database
      result = GetDbFileVersionInfo(dbPath, version, updateInfo);

      // Check version of this database against previous best
      if (result && newestDb.CheckForDbUpdate(version, updateInfo)) {
        newestDb.Set(version, updateInfo);
        newestDbName = dbPath;
      }
    }
  }

  if (newestDbName.empty()) {
    success = false;
  }

  if (success) {
    aFilenameOut = newestDbName;
  }

  if (dirHndl != UFS_INV_DIR_HNDL) {
    UFS_closedir(dirHndl);
  }
#endif

  return success;
}  // end of FindNewestDbFile

//----------------------------------------------------------------
//!
//!       @private
//!       @detail
//!       Open the file at the given path and check if it's a possibly
//!       valid DB file.
//!       Mainly needed for avoiding trouble opening old DB files which
//!       are encrypted.
//!
//!       @returns true if the file exists and appears to be a valid SQLite 3 file
//----------------------------------------------------------------
bool Repository::IsValidDatabaseFile(const std::string& aFilePath) const {
  char readBuffer[32];
  uint32_t bytesToRead = sizeof(readBuffer);

  bool success = FileUtil::Read(aFilePath, readBuffer, bytesToRead);
  if (success) {
    const char* compareString = "SQLite format 3";
    size_t compareLength = strlen(compareString);
    if (0 == memcmp(compareString, readBuffer, compareLength)) {
      // file starts with plain SQLite format declaration => valid file
      success = true;
    } else {
      // Not a plain SQLite3 file.  Possibly a non-SQLite file or an encrypted file.
      success = false;
    }
  }

  return success;
}  // end of IsValidDatabaseFile

//----------------------------------------------------------------
//!
//!       @private
//!       @detail
//!       Open the specified active captain repository. Will open
//!       in shared (R/O) mode if acdb_MFD_DB_SHARING_SUPPORT
//!       enabled, otherwise will be opened R/W.
//!
//!       @returns shared_ptr to the opened database, nullptr if the
//!       open failed.
//----------------------------------------------------------------
std::unique_ptr<SQLite::Database> Repository::OpenDatabaseFile(const std::string& aPath) const {
  int openMode;

#if (acdb_MFD_DB_SHARING_SUPPORT)
  // Open in read-only mode, in order to permit sharing
  openMode = SQLite::OPEN_READONLY;
#else
  // Open database in R/W mode
  openMode = SQLite::OPEN_READWRITE | SQLite::OPEN_FULLMUTEX;
#endif

  // Open database in the chosen mode
  return SqliteCppUtil::OpenDatabaseFileExt(aPath, openMode);
}  // end of OpenDatabaseFile

//----------------------------------------------------------------
//!
//!       @private
//!       @details
//!       Do any steps necessary to use the open database whose
//!       handle is specified.
//!
//!       Assumes caller already has locked mRwl
//!
//!       @returns true if successful, or false for a bad DB
//----------------------------------------------------------------
bool Repository::ReadyDbAccess(SQLite::Database& aDatabase) const {
  bool success(true);
  SqliteCppUtil::LockingMode lockingMode;
  SqliteCppUtil::JournalMode journalMode;

#if (acdb_MFD_DB_SHARING_SUPPORT)
  // To be able to share database in read-only mode, we must
  // not use WAL journaling or exclusive locking mode. For
  // this to work here, database must have been previously
  // prepared using PrepareSharedDb when installed.
  lockingMode = SqliteCppUtil::LockingMode::Normal;
  journalMode = SqliteCppUtil::JournalMode::Delete;
#else
  // We must set the locking mode to EXCLUSIVE so VFS
  // implementations that do not support shared-memory primitives
  // can still use WAL.
  lockingMode = SqliteCppUtil::LockingMode::Exclusive;
  journalMode = SqliteCppUtil::JournalMode::Wal;
#endif

  // Set desired file locking
  success = SqliteCppUtil::SetLockingMode(aDatabase, lockingMode);
  DBG_E_IF(!success, "Failed to set locking mode.");

  // Set desired journal mode
  if (success) {
    success = SqliteCppUtil::SetJournalMode(aDatabase, journalMode);
    DBG_E_IF(!success, "Failed to set journal mode.");
  }

  return success;
}  // end of ReadyDbAccess

//----------------------------------------------------------------
//!
//!       @public
//!       @details Delete specified tiles.  If aCreateTransaction
//!                is false, the caller is responsible for
//!                creating and finalizing the transaction.
//!
//----------------------------------------------------------------
bool Repository::DeleteTile(const TileXY& aTileXY, const bool aCreateTransaction) {
  RwlLocker locker{mRwl, true};

  bool success{mDatabase};
  if (success) {
    if (aCreateTransaction) {
      success = success && BeginTransaction();
    }

    success = success && mUpdateAdapter->DeleteTile(aTileXY);

    if (aCreateTransaction) {
      EndTransaction(success);
    }
  }

  return success;
}  // end of DeleteTile

//----------------------------------------------------------------
//!
//!       @public
//!       @details Delete reviews for specified tiles.
//!
//----------------------------------------------------------------
bool Repository::DeleteTileReviews(const TileXY& aTileXY) {
  bool result{false};

  RwlLocker locker{mRwl, true};
  if (mDatabase) {
    result = BeginTransaction();
    result = result && mUpdateAdapter->DeleteTileReviews(aTileXY);
    EndTransaction(result);
  }

  return result;
}  // end of DeleteTileReviews

//----------------------------------------------------------------
//!
//!       @private
//!       @details Split cross-meridian bounding boxes
//!
//----------------------------------------------------------------
bool Repository::MakeSplitBoundingBoxForCrossMeridianSearch(const bbox_type& aOriginalBbox,
                                                            bbox_type& aLeftBbox,
                                                            bbox_type& aRightBbox) const {
  // Check if the search box crosses the antimeridian.
  if (aOriginalBbox.swc.lon <= aOriginalBbox.nec.lon) {
    // Box does not cross the antimeridian.
    return false;
  }

  // Box crosses the antimeridian.  Do a split search with two boxes:
  //     aLeftBbox covers everything West of the antimeridian.
  //     aRightBbox covers everything East of the antimeridian.
  aLeftBbox = aOriginalBbox;
  aLeftBbox.nec.lon = ACDB_MAX_LON;

  aRightBbox = aOriginalBbox;
  aRightBbox.swc.lon = ACDB_MIN_LON;

  return true;
}  // end of MakeSplitBoundingBoxForCrossMeridianSearch

//----------------------------------------------------------------
//!
//!       @public
//!       @details Installs a database file
//!
//----------------------------------------------------------------
bool Repository::InstallSingleTileDatabase(const std::string& aTileDatabaseFile,
                                           const TileXY& aTileXY) {
  // check the version
  Version currentlyInstalledVersion = GetVersion();
  Version downloadedVersion;
  LastUpdateInfoType updateInfo;
  bool success = GetDbFileVersionInfo(aTileDatabaseFile, downloadedVersion, updateInfo) &&
                 downloadedVersion.SchemaCompatible();
  if (success && downloadedVersion.IsNewerThan(currentlyInstalledVersion)) {
    // Version update. We must delete the local DB completely.
    if (mDatabase) {
      Close();
    }
    success = success && DeleteDatabaseFile();
  }

  if (!mDatabase) {
    RwlLocker locker{mRwl, true};

    // this is the 1st database installation at all. Just move the file over.
    success = success && FileUtil::Rename(aTileDatabaseFile, GetDbPath());
    if (success) {
      // open the DB instantly, but do not send a status.
      // the caller of this function will do this anyway.
      OpenDatabase(true /*updateStateOnFailure*/);
    }
  } else {
    // Start merging this DB into the existing one.
    success = success && MergeSingleTileDatabase(aTileDatabaseFile, aTileXY);

    // Delete the temporary DB file.
    FileUtil::Delete(aTileDatabaseFile);
  }

  return success;
}  // end of InstallSingleTileDatabase

//----------------------------------------------------------------
//!
//!       @public
//!       @details Merges the content of another database
//!
//----------------------------------------------------------------
bool Repository::MergeSingleTileDatabase(const std::string& aTileDatabaseFile,
                                         const TileXY& aTileXY) {
  bool success{true};

  Repository source{aTileDatabaseFile};
  if (!source.OpenDatabase(false /*updateStateOnFailure*/)) {
    return false;
  }

  // Merge support tables
  {
    std::vector<LanguageTableDataType> languages;
    std::vector<MustacheTemplateTableDataType> mustacheTemplates;
    std::vector<TranslationTableDataType> translations;

    success = success && source.GetSupportTableData(languages, mustacheTemplates, translations);
    success = success && ApplySupportTableUpdateToDb(languages, mustacheTemplates, translations);
  }

  // Merge markers and reviews
  {
    uint32_t pageNumber = 0;
    std::vector<MarkerTableDataCollection> markers;
    std::vector<ReviewTableDataCollection> reviews;

    // Write default LastUpdateInfoType for this tile.  Will be updated as data is merged.
    LastUpdateInfoType lastUpdateInfo;
    success = success && mInfoAdapter->WriteTileLastUpdateInfo(aTileXY, lastUpdateInfo);

    do {
      source.GetMergePageData(pageNumber, MergePageSize, markers, reviews);

      if (!markers.empty()) {
        success = success && ApplyMarkerUpdateToDb(markers, &aTileXY);
      }

      if (!reviews.empty()) {
        success = success && ApplyReviewUpdateToDb(reviews, &aTileXY);
      }

      pageNumber++;
    } while (success && !markers.empty());
  }

  source.Close();

  return success;
}  // end of MergeSingleTileDatabase

}  // end of namespace Acdb
