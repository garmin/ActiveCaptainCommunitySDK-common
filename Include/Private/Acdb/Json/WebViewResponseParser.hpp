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

#ifndef ACDB_WebViewResponseParser_hpp
#define ACDB_WebViewResponseParser_hpp

#include <memory>
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Json {

class WebViewResponseParser {
 public:
  enum class ResultType : uint32_t { MarkerUpdate, ReviewUpdate, Error, Unknown };

  static ResultType ParseWebViewResponse(const char* aJson, size_t aLength,
                                         MarkerTableDataCollection& aMarker_out,
                                         ReviewTableDataCollection& aReview_out);
};

}  // end of namespace Json
}  // end of namespace Acdb

#endif  // end of ACDB_WebViewResponseParser_hpp
