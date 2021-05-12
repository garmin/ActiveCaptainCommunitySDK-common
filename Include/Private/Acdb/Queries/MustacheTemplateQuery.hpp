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
    Handles all MustacheTemplate data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MustacheTemplateQuery_hpp
#define ACDB_MustacheTemplateQuery_hpp

#include <vector>
#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class MustacheTemplateQuery {
 public:
  // functions
  MustacheTemplateQuery(SQLite::Database& aDatabase);

  bool Get(const std::string& aName, std::string& aResultOut);

  bool GetAll(std::vector<MustacheTemplateTableDataType>& aResultOut);

  bool Write(MustacheTemplateTableDataType&& aMustacheTemplateTableData);

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mRead;

  std::unique_ptr<SQLite::Statement> mReadAll;

  std::unique_ptr<SQLite::Statement> mWrite;
};  // end of class MustacheTemplateQuery

}  // end of namespace Acdb

#endif  // end of ACDB_MustacheTemplateQuery_hpp
