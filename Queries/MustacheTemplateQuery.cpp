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
#define DBG_TAG "MustacheTemplateQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/MustacheTemplateQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string ReadSql{"SELECT template FROM mustacheTemplates WHERE name = ?;"};
static const std::string ReadAllSql{"SELECT name, template FROM mustacheTemplates;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO mustacheTemplates (name, template) VALUES (?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create MustacheTemplate query object.
//!
//----------------------------------------------------------------
MustacheTemplateQuery::MustacheTemplateQuery(SQLite::Database& aDatabase) {
  try {
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadAll.reset(new SQLite::Statement{aDatabase, ReadAllSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mRead.reset();
    mReadAll.reset();
    mWrite.reset();
  }
}  // End of MustacheTemplateQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool MustacheTemplateQuery::Get(const std::string& aName, std::string& aResultOut) {
  enum Parameters { Name = 1 };
  enum Columns { Template = 0 };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Name, aName);

    success = mRead->executeStep();
    if (success) {
      aResultOut = mRead->getColumn(Columns::Template).getText();
    }

    mRead->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Get

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get all objects from database
//!
//----------------------------------------------------------------
bool MustacheTemplateQuery::GetAll(std::vector<MustacheTemplateTableDataType>& aResultOut) {
  enum Columns { Name = 0, Template };

  if (!mReadAll) {
    return false;
  }

  bool success = false;

  try {
    while (mReadAll->executeStep()) {
      MustacheTemplateTableDataType result;
      result.mName = mReadAll->getColumn(Columns::Name).getText();
      result.mTemplate = mReadAll->getColumn(Columns::Template).getText();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    mReadAll->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetAll

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Write Mustache template to database
//!
//----------------------------------------------------------------
bool MustacheTemplateQuery::Write(MustacheTemplateTableDataType&& aMustacheTemplateTableData) {
  enum Parameters { Name = 1, Template };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Name, aMustacheTemplateTableData.mName);
    mWrite->bind(Parameters::Template, aMustacheTemplateTableData.mTemplate);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
