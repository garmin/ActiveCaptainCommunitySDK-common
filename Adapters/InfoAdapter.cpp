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
    @brief Encapsulates all database info operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "InfoAdapter"

#include <vector>

#include "DBG_pub.h"
#include "Acdb/InfoAdapter.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
#include "Acdb/Version.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
InfoAdapter::InfoAdapter(SQLite::Database& aDatabase)
    : mVersion(aDatabase),
      mReview(aDatabase),
      mMarker(aDatabase),
      mTileLastUpdate{aDatabase} {}  // end of InfoAdapter

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns true on success and outputs the most recent marker and
//!       review timestamps in the database.  Returns  false on failure.
//!
//----------------------------------------------------------------
bool InfoAdapter::GetLastUpdateInfo(LastUpdateInfoType& aUpdateInfoOut) {
  return (mMarker.GetLastUpdate(aUpdateInfoOut.mMarkerLastUpdate) &&
          mReview.GetLastUpdate(aUpdateInfoOut.mUserReviewLastUpdate));
}  // end of GetLastUpdateInfo

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns true on success and outputs the most recent marker and
//!       review timestamps in the database.  Returns  false on failure.
//!
//----------------------------------------------------------------
bool InfoAdapter::GetTileLastUpdateInfo(const TileXY& aTileXY, LastUpdateInfoType& aUpdateInfoOut) {
  return mTileLastUpdate.Get(aTileXY, aUpdateInfoOut);
}  // end of GetTileLastUpdateInfo

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns true on success and outputs map of tiles
//!       overlapped by bbox with their most recent marker and
//!       review timestamps.  Returns false on failure.
//!
//----------------------------------------------------------------
bool InfoAdapter::GetTileLastUpdateInfoBbox(const bbox_type& aBbox,
                                            std::map<TileXY, LastUpdateInfoType>& aUpdateInfoOut) {
  return mTileLastUpdate.GetBbox(aBbox, aUpdateInfoOut);
}  // end of GetTileLastUpdateInfoBbox

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//----------------------------------------------------------------
void InfoAdapter::GetVersion(Version& aResult) {
  std::string versionString;

  if (mVersion.Get(versionString)) {
    aResult.Set(versionString);
  } else {
    DBG_W("Failed to read version from the database.");
  }
}  // end of GetVersion

//----------------------------------------------------------------
//!
//!       @public
//!       @brief setter
//!
//!       @returns writes the marker and review timestamps to the
//!       database.  Returns true on success, false on failure.
//!
//----------------------------------------------------------------
bool InfoAdapter::WriteTileLastUpdateInfo(const TileXY& aTileXY,
                                          const LastUpdateInfoType& aUpdateInfoOut) {
  return mTileLastUpdate.Write(aTileXY, aUpdateInfoOut);
}  // end of WriteTileLastUpdateInfo
}  // end of namespace Acdb
