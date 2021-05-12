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
    Class to represent a specific set of queries

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "LanguageQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/LanguageQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string ReadSql{"SELECT id, isoCode FROM languageType;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO languageType (id, isoCode) VALUES (?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Language query object.
//!
//----------------------------------------------------------------
LanguageQuery::LanguageQuery(SQLite::Database& aDatabase) {
  try {
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mRead.reset();
    mWrite.reset();
  }
}  // End of LanguageQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get all objects from database
//!
//----------------------------------------------------------------
bool LanguageQuery::GetAll(std::vector<LanguageTableDataType>& aResultOut) {
  enum Columns { Id = 0, IsoCode };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    while (mRead->executeStep()) {
      LanguageTableDataType result;
      result.mId = mRead->getColumn(Columns::Id).getInt();
      result.mIsoCode = mRead->getColumn(Columns::IsoCode).getText();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    mRead->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetAll

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write language to database
//!
//----------------------------------------------------------------
bool LanguageQuery::Write(LanguageTableDataType&& aLanguageTableData) {
  enum Parameters { Id = 1, IsoCode };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, aLanguageTableData.mId);
    mWrite->bind(Parameters::IsoCode, aLanguageTableData.mIsoCode);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
