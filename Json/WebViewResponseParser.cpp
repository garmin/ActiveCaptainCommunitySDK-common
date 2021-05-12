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
Definition of the WebViewResponseParser.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "WebViewResponseParser"

#include <map>
#include <string>
#include <vector>

#include "DBG_pub.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/Json/JsonParser.hpp"
#include "Acdb/Json/WebViewResponseParser.hpp"
#include "Acdb/Json/MarkerParser.hpp"
#include "Acdb/Json/ReviewParser.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "rapidjson/document.h"

namespace Acdb {
namespace Json {

WebViewResponseParser::ResultType WebViewResponseParser::ParseWebViewResponse(
    const char* aJson, size_t aLength, MarkerTableDataCollection& aMarker_out,
    ReviewTableDataCollection& aReview_out) {
  rapidjson::Document document;
  document.Parse(aJson, aLength);
  if (!document.IsObject()) {
    return ResultType::Error;
  }

  std::string resultTypeStr;
  auto resultTypeIt = document.FindMember("resultType");
  if (resultTypeIt == document.MemberEnd()) {
    return ResultType::Error;
  }

  resultTypeStr = String::ToUpper(resultTypeIt->value.GetString());

  if (resultTypeStr == "SUCCESS" || resultTypeStr == "DELETE") {
    if (Json::ParseMarkerWebViewResponse(document, aMarker_out)) {
      return ResultType::MarkerUpdate;
    } else {
      return ResultType::Error;
    }
  }

  if (resultTypeStr == "REVIEWSUCCESS" || resultTypeStr == "REVIEWDELETE" ||
      resultTypeStr == "REVIEWFLAGGED") {
    if (Json::ParseReviewWebViewResponse(document, aReview_out)) {
      return ResultType::ReviewUpdate;
    } else {
      return ResultType::Error;
    }
  }

  if (resultTypeStr == "ERROR") {
    return ResultType::Error;
  }

  return ResultType::Unknown;
}  // end of ApplyWebViewResponse

}  // end of namespace Json
}  // end of namespace Acdb
