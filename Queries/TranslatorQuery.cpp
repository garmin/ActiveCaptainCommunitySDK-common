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
#define DBG_TAG "TranslatorQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/TranslatorQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string ReadSql{
    "SELECT t1.id, COALESCE(t2.translation, t1.translation) AS translation "
    "FROM translations t1 "
    "    LEFT JOIN translations t2 ON t1.id = t2.id AND t2.langId = "
    "    ( "
    "        SELECT id FROM languageType WHERE isoCode = ? "
    "    ) "
    "WHERE t1.langId = 1;"};
static const std::string ReadAllSql{"SELECT id, langId, translation FROM translations;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO translations (id, langId, translation) VALUES (?, ?, ?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Translation query object.
//!
//----------------------------------------------------------------
TranslatorQuery::TranslatorQuery(SQLite::Database& aDatabase) {
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
}  // End of TranslatorQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool TranslatorQuery::Get(const std::string& aLanguage,
                          std::vector<TranslationDataType>& aResultOut) {
  enum Parameters { IsoCode = 1 };
  enum Columns { Id = 0, Translation };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::IsoCode, aLanguage);

    while (mRead->executeStep()) {
      TranslationDataType result;
      result.first = mRead->getColumn(Columns::Id).getInt();
      result.second = mRead->getColumn(Columns::Translation).getText();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

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
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool TranslatorQuery::GetAll(std::vector<TranslationTableDataType>& aResultOut) {
  enum Columns { Id = 0, LangId, Translation };

  if (!mReadAll) {
    return false;
  }

  bool success = false;

  try {
    while (mReadAll->executeStep()) {
      TranslationTableDataType result;
      result.mId = mReadAll->getColumn(Columns::Id).getInt();
      result.mLangId = mReadAll->getColumn(Columns::LangId).getInt();
      result.mTranslation = mReadAll->getColumn(Columns::Translation).getText();

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
//!   @detail Write translation to database.
//!
//----------------------------------------------------------------
bool TranslatorQuery::Write(TranslationTableDataType&& aTranslationTableData) {
  enum Parameters { Id = 1, LangId, Translation };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, aTranslationTableData.mId);
    mWrite->bind(Parameters::LangId, aTranslationTableData.mLangId);
    mWrite->bind(Parameters::Translation, aTranslationTableData.mTranslation);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of Write

}  // end of namespace Acdb
