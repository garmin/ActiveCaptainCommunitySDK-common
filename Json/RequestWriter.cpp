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
Definition of the RequestWriter.

Copyright 2018-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "RequestWriter"

#include <map>
#include "DBG_pub.h"
#include "Acdb/Json/RequestWriter.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "NavDateTimeExtensions.hpp"
#include "UTL_pub_lib_cnvt.h"

namespace Acdb {
namespace Json {
static std::string WriteValueToString(const rapidjson::Value& jsonValue);

static std::string GetMarkerTypeName(ACDB_type_type aType);

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Get the name of a marker type.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
static std::string GetMarkerTypeName(ACDB_type_type aType) {
  static const std::map<ACDB_type_type, std::string> TypeNameMap = {
      {ACDB_ANCHORAGE, "Anchorage"}, {ACDB_HAZARD, "Hazard"},     {ACDB_MARINA, "Marina"},
      {ACDB_BOAT_RAMP, "BoatRamp"},  {ACDB_BUSINESS, "Business"}, {ACDB_INLET, "Inlet"},
      {ACDB_BRIDGE, "Bridge"},       {ACDB_LOCK, "Lock"},         {ACDB_DAM, "Dam"},
      {ACDB_FERRY, "Ferry"}};

  auto it = TypeNameMap.find(aType);
  if (it != TypeNameMap.end()) {
    return it->second;
  } else {
    DBG_ASSERT_ALWAYS("Unsupported Marker Type");
    return "Unknown";
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Write the body for create marker request to a string
//!
//----------------------------------------------------------------
std::string WriteCreateMarkerRequest(const std::string& aName, ACDB_type_type aType,
                                     const scposn_type& aPosn) {
  rapidjson::Document document;
  document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  rapidjson::Value createRequestValue(rapidjson::kObjectType);

  // add name
  createRequestValue.AddMember("name", rapidjson::Value().SetString(aName.c_str(), allocator),
                               allocator);

  // add type
  std::string typeName = GetMarkerTypeName(aType);
  createRequestValue.AddMember("poiType", rapidjson::Value().SetString(typeName.c_str(), allocator),
                               allocator);

  // add position
  rapidjson::Value locationValue(rapidjson::kObjectType);
  locationValue.AddMember("latitude", (aPosn.lat * UTL_SEMI_TO_DEG), allocator);
  locationValue.AddMember("longitude", (aPosn.lon * UTL_SEMI_TO_DEG), allocator);
  createRequestValue.AddMember("mapLocation", locationValue, allocator);

  return WriteValueToString(createRequestValue);
}  // end of WriteCreateMarkerRequest

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Write the body for an export API request to a string
//!
//----------------------------------------------------------------
std::string WriteExportRequest(const std::set<TileXY>& aTile) {
  rapidjson::Document document;
  document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  rapidjson::Value jsonTilesArray(rapidjson::kArrayType);
  for (auto tile : aTile) {
    rapidjson::Value tileValue(rapidjson::kObjectType);
    tileValue.AddMember("tileX", rapidjson::Value().SetInt(tile.mX), allocator);
    tileValue.AddMember("tileY", rapidjson::Value().SetInt(tile.mY), allocator);

    jsonTilesArray.PushBack(tileValue, allocator);
  }

  return WriteValueToString(jsonTilesArray);
}  // end of WriteExportRequest

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Write the body for move marker request to a string
//!
//----------------------------------------------------------------
std::string WriteMoveMarkerRequest(const scposn_type& aPosn) {
  rapidjson::Document document;
  document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  rapidjson::Value locationValue(rapidjson::kObjectType);
  locationValue.AddMember("latitude", (aPosn.lat * UTL_SEMI_TO_DEG), allocator);
  locationValue.AddMember("longitude", (aPosn.lon * UTL_SEMI_TO_DEG), allocator);

  return WriteValueToString(locationValue);
}  // end of WriteMoveMarkerRequest

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Write the body for a sync status API request to a string
//!
//----------------------------------------------------------------
std::string WriteSyncStatusRequest(const std::map<TileXY, LastUpdateInfoType>& aTileLastUpdate) {
  rapidjson::Document document;
  document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  rapidjson::Value jsonTilesArray(rapidjson::kArrayType);
  for (auto tile : aTileLastUpdate) {
    rapidjson::Value tileValue(rapidjson::kObjectType);
    tileValue.AddMember("tileX", rapidjson::Value().SetInt(tile.first.mX), allocator);
    tileValue.AddMember("tileY", rapidjson::Value().SetInt(tile.first.mY), allocator);

    if (tile.second.mMarkerLastUpdate > 0) {
      Navionics::NavDateTime markerDate =
          NavDateTimeExtensions::EpochToNavDateTime(UNIX_EPOCH, tile.second.mMarkerLastUpdate);
      Navionics::NavString markerDateStr;
      markerDate.ToString(markerDateStr, DateStringType::YYYYMMDDTHHMMSSZ_FORMAT);
      tileValue.AddMember("poiDateLastModified",
                          rapidjson::Value().SetString(markerDateStr.c_str(), allocator),
                          allocator);
    }

    if (tile.second.mUserReviewLastUpdate > 0) {
      Navionics::NavDateTime reviewDate =
          NavDateTimeExtensions::EpochToNavDateTime(UNIX_EPOCH, tile.second.mUserReviewLastUpdate);
      Navionics::NavString reviewDateStr;
      reviewDate.ToString(reviewDateStr, DateStringType::YYYYMMDDTHHMMSSZ_FORMAT);
      tileValue.AddMember("reviewDateLastModified",
                          rapidjson::Value().SetString(reviewDateStr.c_str(), allocator),
                          allocator);
    }

    jsonTilesArray.PushBack(tileValue, allocator);
  }

  return WriteValueToString(jsonTilesArray);
}  // end of WriteSyncStatusRequest

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Write a rapidjson::value object to a string
//!
//----------------------------------------------------------------
static std::string WriteValueToString(const rapidjson::Value& jsonValue) {
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  jsonValue.Accept(writer);
  return buffer.GetString();
}  // end of WriteValueToString

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Write the body for a tiles by bounding box API request to a string
//!
//----------------------------------------------------------------
std::string WriteTilesByBoundingBoxesRequest(const std::vector<bbox_type>& aBboxList) {
  rapidjson::Document document;
  document.SetObject();
  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  rapidjson::Value bboxArray(rapidjson::kArrayType);
  for (auto bbox : aBboxList) {
    rapidjson::Value bboxValue(rapidjson::kObjectType);

    rapidjson::Value swcValue(rapidjson::kObjectType);
    swcValue.AddMember("latitude",
                       rapidjson::Value().SetDouble((double)bbox.swc.lat * UTL_SEMI_TO_DEG),
                       allocator);
    swcValue.AddMember("longitude",
                       rapidjson::Value().SetDouble((double)bbox.swc.lon * UTL_SEMI_TO_DEG),
                       allocator);

    rapidjson::Value necValue(rapidjson::kObjectType);
    necValue.AddMember("latitude",
                       rapidjson::Value().SetDouble((double)bbox.nec.lat * UTL_SEMI_TO_DEG),
                       allocator);
    necValue.AddMember("longitude",
                       rapidjson::Value().SetDouble((double)bbox.nec.lon * UTL_SEMI_TO_DEG),
                       allocator);

    bboxValue.AddMember("southwestCorner", swcValue, allocator);
    bboxValue.AddMember("northeastCorner", necValue, allocator);

    bboxArray.PushBack(bboxValue, allocator);
  }

  return WriteValueToString(bboxArray);
}  // end of WriteTilesByBoundingBoxesRequest

}  // end of namespace Json
}  // end of namespace Acdb
