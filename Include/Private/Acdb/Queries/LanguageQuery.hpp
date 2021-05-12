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
    Handles all Language data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_LanguageQuery_hpp
#define ACDB_LanguageQuery_hpp

#include <vector>
#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class LanguageQuery {
 public:
  // functions
  LanguageQuery(SQLite::Database& aDatabase);

  bool GetAll(std::vector<LanguageTableDataType>& aResultOut);

  bool Write(LanguageTableDataType&& aLanguageTableData);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mWrite;
};  // end of class LanguageQuery

}  // end of namespace Acdb

#endif  // end of ACDB_LanguageQuery_hpp
