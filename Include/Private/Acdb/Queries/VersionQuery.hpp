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
    Handles all Version data operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_VersionQuery_hpp
#define ACDB_VersionQuery_hpp

#include <memory>
#include <string>
#include "SQLiteCpp/Statement.h"

namespace Acdb {
class VersionQuery {
 public:
  // functions

  VersionQuery(SQLite::Database& aDatabase);

  bool Put(const std::string& aVersion);

  bool Get(std::string& aResultOut);

  bool Delete();

 private:
  // Variables
  std::unique_ptr<SQLite::Statement> mWrite;

  std::unique_ptr<SQLite::Statement> mDelete;

  std::unique_ptr<SQLite::Statement> mRead;
};  // end of class VersionQuery
}  // end of namespace Acdb

#endif  // end of ACDB_VersionQuery_hpp
