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

#ifndef ACDB_JsonParser_hpp
#define ACDB_JsonParser_hpp

#include <memory>
#include <string>
#include "rapidjson/document.h"

namespace Acdb {
namespace Json {
bool GetDateTimeEpoch(const rapidjson::Value& aDocument, const char* aNodeName, uint64_t& aOutput);

bool GetDouble(const rapidjson::Value& aDocument, const char* aNodeName, double& aOutput);

bool GetJsonString(const rapidjson::Value& aDocument, const char* aNodeName, std::string& aOutput);

bool GetSint32(const rapidjson::Value& aDocument, const char* aNodeName, int32_t& aOutput);

bool GetString(const rapidjson::Value& aDocument, const char* aNodeName, std::string& aOutput);

bool GetUint64(const rapidjson::Value& aDocument, const char* aNodeName, uint64_t& aOutput);

bool GetUint64(const rapidjson::Value& aValue, uint64_t& aOutput);

}  // end of namespace Json
}  // end of namespace Acdb

#endif  // end of ACDB_JsonParser_hpp
