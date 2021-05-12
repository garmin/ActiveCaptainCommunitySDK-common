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
    Contains functionality to access ActiveCaptain database.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_Repository_hpp
#define ACDB_Repository_hpp

#include <memory>
#include <vector>
#include <set>
#include <string>

#include "Acdb/InfoAdapter.hpp"
#include "Acdb/MarkerAdapter.hpp"
#include "Acdb/MergeAdapter.hpp"
#include "Acdb/PresentationAdapter.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
#include "Acdb/ReadWriteLock.hpp"
#include "Acdb/TranslationAdapter.hpp"
#include "Acdb/UpdateAdapter.hpp"
#include "SQLiteCpp/Database.h"

namespace Acdb {
class MapMarkerFilter;
class SearchMarkerFilter;
class Version;

class Repository {
 public:
  Repository(const std::string& aDbPath = std::string{});

  bool ApplyMarkerUpdateToDb(std::vector<MarkerTableDataCollection>& aMarkerList,
                             const TileXY* aTileXY);

  bool ApplyReviewUpdateToDb(std::vector<ReviewTableDataCollection>& aReviewList,
                             const TileXY* aTileXY);

  bool ApplySupportTableUpdateToDb(
      std::vector<LanguageTableDataType>& aLanguageList,
      std::vector<MustacheTemplateTableDataType>& aMustacheTemplateList,
      std::vector<TranslationTableDataType>& aTranslations);

  void Delete();

  Presentation::BusinessPhotoListPtr GetBusinessPhotoList(const ACDB_marker_idx_type aIdx);

  void GetDatabaseSize(uint64_t& aUsedSpace);

  bool GetDbFileVersionInfo(const std::string& aPath, Version& aVersionOut,
                            LastUpdateInfoType& aUpdateInfoOut) const;

  bool GetLastUpdateInfo(LastUpdateInfoType& aUpdateInfoOut);

  std::string GetMustacheTemplate(const std::string& aName);

  IMapMarkerPtr GetMapMarker(const ACDB_marker_idx_type aIdx);

  ISearchMarkerPtr GetSearchMarker(const ACDB_marker_idx_type aIdx);

  void GetMapMarkersByFilter(const MapMarkerFilter& aFilter, std::vector<IMapMarkerPtr>& aResults);

  void GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                std::vector<ISearchMarkerPtr>& aResults);

  Presentation::PresentationMarkerPtr GetPresentationMarker(
      const ACDB_marker_idx_type aIdx, const std::string& aCaptainName = std::string{});

  Presentation::ReviewListPtr GetReviewList(const ACDB_marker_idx_type aIdx, const int aPageNumber,
                                            const int aPageSize,
                                            const std::string& aCaptainName = std::string{});

  bool GetSupportTableData(std::vector<LanguageTableDataType>& aLanguages,
                           std::vector<MustacheTemplateTableDataType>& aMustacheTemplates,
                           std::vector<TranslationTableDataType>& aTranslations);

  bool GetTileLastUpdateInfo(const TileXY& aTileXY, LastUpdateInfoType& aUpdateInfoOut);

  void GetTilesLastUpdateInfoByBoundingBoxes(const std::vector<bbox_type>& aBboxes,
                                             std::map<TileXY, LastUpdateInfoType>& aTiles);

  float GetUserReviewAverageStars(const ACDB_marker_idx_type aIdx);

  Version GetVersion();

  bool IsOpen();

  bool Open();

  void Close();

  std::string GetDbPath();

  std::string GetDbJournalPath();

  bool BeginSideload();

  void EndSideload();

  bool PrepareSharedDb(const std::string& aPath) const;

  void SetLanguage(const std::string& aLanguage);

  bool DeleteTile(const TileXY& aTileToDelete, const bool aCreateTransaction = true);

  bool DeleteTileReviews(const TileXY& aTileXY);

  bool InstallSingleTileDatabase(const std::string& aTileDatabaseFile, const TileXY& aTileXY);

 private:
  // Constants
  static const uint32_t MergePageSize = 50;

  // Functions

  bool BeginTransaction() const;

  void EndTransaction(const bool aSuccess) const;

  bool DeleteDatabaseFile();

  bool DeleteFile(const std::string& aPath);

  bool FindNewestDbFile(const std::string aPath, std::string& aFilenameOut) const;

  bool MergeSingleTileDatabase(const std::string& aTileDatabaseFile, const TileXY& aTileXY);

  bool IsValidDatabaseFile(const std::string& aFilePath) const;

  bool OpenDatabase(bool updateStateOnFailure);

  std::unique_ptr<SQLite::Database> OpenDatabaseFile(const std::string& aPath) const;

  bool ReadyDbAccess(SQLite::Database& aDatabase) const;

  bool MakeSplitBoundingBoxForCrossMeridianSearch(const bbox_type& aOriginalBbox,
                                                  bbox_type& aLeftBbox,
                                                  bbox_type& aRightBbox) const;

  bool GetMergePageData(const int aPageNumber, const int aPageSize,
                        std::vector<MarkerTableDataCollection>& aMarkers_out,
                        std::vector<ReviewTableDataCollection>& aReviews_out);

  bool GetMergeMarker(const ACDB_marker_idx_type aIdx, MarkerTableDataCollection& aMarker);

  void GetMergeMarkerIds(const uint32_t aPageNumber, const uint32_t aPageSize,
                         std::vector<ACDB_marker_idx_type>& aResults);

  bool GetMergeReviews(const ACDB_marker_idx_type aIdx,
                       std::vector<ReviewTableDataCollection>& aReviews);

  // Variables
  std::string mDbPath;  //!< path to the database
  ReadWriteLock mRwl;
  std::unique_ptr<SQLite::Database> mDatabase;
  std::unique_ptr<InfoAdapter> mInfoAdapter;
  std::unique_ptr<MarkerAdapter> mMarkerAdapter;
  std::unique_ptr<MergeAdapter> mMergeAdapter;
  std::unique_ptr<PresentationAdapter> mPresentationAdapter;
  std::unique_ptr<TranslationAdapter> mTranslationAdapter;
  std::unique_ptr<UpdateAdapter> mUpdateAdapter;
};  // end of class Repository
}  // end of namespace Acdb

#endif  // end of ACDB_Repository_hpp
