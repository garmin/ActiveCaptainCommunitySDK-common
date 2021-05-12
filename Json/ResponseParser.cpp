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
Definition of the ResponseParser.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ResponseParser"

#include <string>

#include "DBG_pub.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/Json/JsonParser.hpp"
#include "Acdb/Json/MarkerParser.hpp"
#include "Acdb/Json/ResponseParser.hpp"
#include "Acdb/Json/ReviewParser.hpp"
#include "rapidjson/document.h"

namespace Acdb {
namespace Json {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse the export response json
//!
//----------------------------------------------------------------
bool ParseExportResponse(const char* aJson, size_t aLength,
                         std::vector<ExportFile>& aExportFiles_out) {
  static const char tileXNode[] = "tileX";
  static const char tileYNode[] = "tileY";
  static const char fileSizeNode[] = "fileSize";
  static const char md5HashNode[] = "md5Hash";
  static const char urlNode[] = "url";

#if (acdb_COMPRESSION_TYPE == acdb_COMPRESSION_TYPE_GZIP)
  static const char compressedFileNode[] = "gzip";
#elif (acdb_COMPRESSION_TYPE == acdb_COMPRESSION_TYPE_ZIP)
  static const char compressedFileNode[] = "zip";
#endif

  rapidjson::Document document;
  document.Parse(aJson, aLength);
  if (!document.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = document.GetArray().begin(); success && it != document.GetArray().end(); it++) {
    ExportFile exportFile;

    success = success && it->IsObject();
    rapidjson::Value value = it->GetObject();
    success = success && GetSint32(value, tileXNode, exportFile.mTileXY.mX);
    success = success && GetSint32(value, tileYNode, exportFile.mTileXY.mY);

    auto compressedFileIt = value.FindMember(compressedFileNode);
    success = success && (compressedFileIt != value.MemberEnd());

    success = success && GetString(compressedFileIt->value, md5HashNode, exportFile.mMd5);
    success = success && GetUint64(compressedFileIt->value, fileSizeNode, exportFile.mSize);
    success = success && GetString(compressedFileIt->value, urlNode, exportFile.mUrl);

    aExportFiles_out.push_back(exportFile);
  }

  return success;
}  // end of ParseExportResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse the sync status response json
//!
//----------------------------------------------------------------
bool ParseSyncStatusResponse(const char* aJson, size_t aLength,
                             std::map<TileXY, TileUpdateOperation>& aTileUpdateOperations_out) {
  static const char tileXNode[] = "tileX";
  static const char tileYNode[] = "tileY";
  static const char poiUpdateTypeNode[] = "poiUpdateType";
  static const char reviewUpdateTypeNode[] = "reviewUpdateType";

  const std::map<std::string, TileUpdateType> tileUpdateTypeMap{
      {"None", TileUpdateType::None},
      {"Export", TileUpdateType::Download},
      {"Sync", TileUpdateType::Sync},
      {"Delete", TileUpdateType::Delete}};

  rapidjson::Document document;
  document.Parse(aJson, aLength);
  if (!document.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = document.GetArray().begin(); success && it != document.GetArray().end(); it++) {
    TileXY tileXY;
    std::string poiOperation;
    std::string reviewOperation;

    success = success && it->IsObject();
    rapidjson::Value value = it->GetObject();
    success = success && GetSint32(value, tileXNode, tileXY.mX);
    success = success && GetSint32(value, tileYNode, tileXY.mY);
    success = success && GetString(value, poiUpdateTypeNode, poiOperation);
    success = success && GetString(value, reviewUpdateTypeNode, reviewOperation);

    TileUpdateOperation tileUpdateOperation;

    if (!success) {
      DBG_ASSERT_ALWAYS("Invalid sync status response");
    } else {
      auto tileUpdateTypeMapIt = tileUpdateTypeMap.find(poiOperation);
      if (tileUpdateTypeMapIt == tileUpdateTypeMap.end()) {
        DBG_ASSERT_ALWAYS("Sync status unknown Marker updateType");
        success = false;
      } else {
        tileUpdateOperation.mMarkerUpdateType = tileUpdateTypeMapIt->second;
      }

      tileUpdateTypeMapIt = tileUpdateTypeMap.find(reviewOperation);
      if (tileUpdateTypeMapIt == tileUpdateTypeMap.end()) {
        DBG_ASSERT_ALWAYS("Sync status unknown Review updateType");
        success = false;
      } else {
        tileUpdateOperation.mReviewUpdateType = tileUpdateTypeMapIt->second;
      }

      aTileUpdateOperations_out[tileXY] = tileUpdateOperation;
    }
  }

  return success;
}  // end of ParseSyncStatusResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse the tiles by bounding box response json
//!
//----------------------------------------------------------------
bool ParseTilesByBoundingBoxesResponse(const char* aJson, size_t aLength,
                                       std::set<TileXY>& aTileList_out) {
  static const char tileXNode[] = "tileX";
  static const char tileYNode[] = "tileY";

  aTileList_out.clear();

  rapidjson::Document document;
  document.Parse(aJson, aLength);
  if (!document.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = document.GetArray().begin(); success && it != document.GetArray().end(); it++) {
    TileXY tileXY;

    success = success && it->IsObject();
    rapidjson::Value value = it->GetObject();

    success = success && GetSint32(value, tileXNode, tileXY.mX);
    success = success && GetSint32(value, tileYNode, tileXY.mY);

    aTileList_out.insert(tileXY);
  }

  return success;
}  // end of ParseTilesByBoundingBoxesResponse

}  // end of namespace Json
}  // end of namespace Acdb
