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
    Handles all ReviewSummary data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReviewSummaryQuery_hpp
#define ACDB_ReviewSummaryQuery_hpp

#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class ReviewSummaryQuery {
 public:
  // functions
  ReviewSummaryQuery(SQLite::Database& aDatabase);

  bool Get(const ACDB_marker_idx_type aId, ReviewSummaryTableDataType& aResultOut);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mRead;
};  // end of class ServicesQuery

}  // end of namespace Acdb

#endif  // end of ACDB_ReviewSummaryQuery_hpp
