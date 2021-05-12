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
Definition of the JsonParser.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "JsonParser"

#include "DBG_pub.h"
#include "Acdb/Json/JsonParser.hpp"
#include "Acdb/StringUtil.hpp"
#include "NavDateTimeExtensions.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

namespace Acdb {
namespace Json {

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Get JSON node value as an epoch value.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
bool GetDateTimeEpoch(const rapidjson::Value& aDocument, const char* aNodeName, uint64_t& aOutput) {
  std::string dateTimeStr;
  if (!GetString(aDocument, aNodeName, dateTimeStr)) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  Navionics::NavDateTime dateTime;
  if (!dateTime.FromString(dateTimeStr, YYYYMMDDTHHMMSSZ_FORMAT)) {
    // Invalid date format.
    return false;
  }

  aOutput = NavDateTimeExtensions::NavDateTimeToEpoch(dateTime, EpochType::UNIX_EPOCH);

  return true;
}  // end of GetDateTimeEpoch

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get JSON node value as a double.
//!   @returns
//!       True if node exists and is correct type, false otherwise.
//!
//----------------------------------------------------------------
bool GetDouble(const rapidjson::Value& aDocument, const char* aNodeName, double& aOutput) {
  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd() || !it->value.IsDouble()) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  aOutput = it->value.GetDouble();
  return true;
}  // end of GetDouble

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get JSON node value as a JSON string.
//!   @returns
//!       True if node exists, false otherwise.
//!
//----------------------------------------------------------------
bool GetJsonString(const rapidjson::Value& aDocument, const char* aNodeName, std::string& aOutput) {
  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd()) {
    // Node not present in JSON.
    return false;
  }

  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  it->value.Accept(writer);
  aOutput = buffer.GetString();

  return true;
}  // end of GetJsonString

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get JSON node value as a sint32.
//!   @returns
//!       True if node exists and is correct type, false otherwise.
//!
//----------------------------------------------------------------
bool GetSint32(const rapidjson::Value& aDocument, const char* aNodeName, int32_t& aOutput) {
  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd() || !it->value.IsInt()) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  aOutput = it->value.GetInt();
  return true;
}  // end of GetSint32

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get JSON node value as a std::string.
//!   @returns
//!       True if node exists and is correct type, false otherwise.
//!
//----------------------------------------------------------------
bool GetString(const rapidjson::Value& aDocument, const char* aNodeName, std::string& aOutput) {
  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd() || !it->value.IsString()) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  aOutput = it->value.GetString();
  return true;
}  // end of GetString

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get a UInt64 value from a type value
//!       Can be a uint64 type value, or a uint64 encoded as string
//!   @returns
//!       True if value could be read, false otherwise.
//!
//----------------------------------------------------------------
bool GetUint64(const rapidjson::Value& aValue, uint64_t& aOutput) {
  if (aValue.IsUint64()) {
    aOutput = aValue.GetUint64();
    return true;
  }

  if (aValue.IsString()) {
    std::string valueString = aValue.GetString();
    if (!valueString.empty()) {
      aOutput = String::ToUInt64(valueString);
      return true;
    }
  }

  return false;
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Get JSON node value as a uint64.
//!   @returns
//!       True if node exists and is correct type, false otherwise.
//!
//----------------------------------------------------------------
bool GetUint64(const rapidjson::Value& aDocument, const char* aNodeName, uint64_t& aOutput) {
  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd()) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  return GetUint64(it->value, aOutput);
}  // end of GetUint64

}  // end of namespace Json
}  // end of namespace Acdb
