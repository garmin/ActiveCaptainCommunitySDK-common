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
Definition of the ReviewParser.

Copyright 2018-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewParser_hpp
#define ACDB_ReviewParser_hpp

#include <memory>
#include "Acdb/PrvTypes.hpp"
#include "rapidjson/document.h"

namespace Acdb {
namespace Json {

bool ParseReviewWebViewResponse(rapidjson::Document& aJsonDocument,
                                ReviewTableDataCollection& aReview_out);

bool ParseReviewSyncResponse(const char* aJson, size_t aLength,
                             std::vector<ReviewTableDataCollection>& aReviews_out);

bool ParseVoteForReviewResponse(const char* aJson, size_t aLength,
                                ReviewTableDataCollection& aReview_out);

}  // end of namespace Json
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewParser_hpp
