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
    Class to represent a specific repository query.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "VersionQuery"

#include "Acdb/Queries/VersionQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM versions;"};
static const std::string ReadSql{"SELECT value FROM versions;"};
static const std::string WriteSql{"INSERT INTO versions (value) VALUES (?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Version query object.
//!
//----------------------------------------------------------------
VersionQuery::VersionQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mRead.reset();
    mWrite.reset();
  }
}  // End of VersionQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Insert the database version string.  Note that we
//!   should only ever have a single version string, meaning that
//!   the version should be deleted within a transaction before
//!   this call is made.
//!
//----------------------------------------------------------------
bool VersionQuery::Put(const std::string& aVersion) {
  enum Parameters { Value = 1 };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Value, aVersion);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Put

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the database version string.
//!
//----------------------------------------------------------------
bool VersionQuery::Get(std::string& aResultOut) {
  enum Columns { Value = 0 };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    success = mRead->executeStep();
    if (success) {
      aResultOut = mRead->getColumn(Columns::Value).getText();
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
//!   @detail Remove the database version entry.
//!
//----------------------------------------------------------------
bool VersionQuery::Delete() {
  if (!mDelete) {
    return false;
  }

  bool success = false;

  try {
    mDelete->exec();
    success = mDelete->isDone();

    mDelete->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Delete
}  // namespace Acdb