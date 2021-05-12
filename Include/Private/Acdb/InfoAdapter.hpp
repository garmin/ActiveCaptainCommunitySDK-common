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
    Encapsulates all database information operations.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_InfoAdapter_hpp
#define ACDB_InfoAdapter_hpp

#include <map>
#include "Acdb/Queries/MarkerQuery.hpp"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "Acdb/Queries/TileLastUpdateQuery.hpp"
#include "Acdb/Queries/VersionQuery.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
class Version;

class InfoAdapter {
 public:
  InfoAdapter(SQLite::Database& aDatabase);

  bool GetLastUpdateInfo(LastUpdateInfoType& aUpdateInfoOut);

  bool GetTileLastUpdateInfo(const TileXY& aTileXY, LastUpdateInfoType& aUpdateInfoOut);

  bool GetTileLastUpdateInfoBbox(const bbox_type& aBbox,
                                 std::map<TileXY, LastUpdateInfoType>& aUpdateInfoOut);

  void GetVersion(Version& aResult);

  bool WriteTileLastUpdateInfo(const TileXY& aTileXY, const LastUpdateInfoType& aUpdateInfoOut);

 private:
  // Variables
  VersionQuery mVersion;
  ReviewQuery mReview;
  MarkerQuery mMarker;
  TileLastUpdateQuery mTileLastUpdate;

};  // end of class InfoAdapter
}  // end of namespace Acdb

#endif  // end of ACDB_QueryInterface_hpp
