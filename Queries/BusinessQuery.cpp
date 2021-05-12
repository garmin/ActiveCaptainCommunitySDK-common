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
#define DBG_TAG "BusinessQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/BusinessQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM business WHERE id = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM business WHERE id IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string ReadSql{
    "SELECT sectionTitle, labeled, commaSeparatedList, businessPromotions, callToAction FROM business WHERE id = ?;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO business (id, sectionTitle, labeled, commaSeparatedList, businessPromotions, callToAction) VALUES (?, ?, ?, ?, ?, ?);"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Business query object.
//!
//----------------------------------------------------------------
BusinessQuery::BusinessQuery(SQLite::Database& aDatabase) {
  try {
    mDelete.reset(new SQLite::Statement{aDatabase, DeleteSql});
    mDeleteGeohash.reset(new SQLite::Statement{aDatabase, DeleteGeohashSql});
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mWrite.reset(new SQLite::Statement{aDatabase, WriteSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mDelete.reset();
    mDeleteGeohash.reset();
    mRead.reset();
    mWrite.reset();
  }
}  // End of BusinessQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete business from database
//!
//----------------------------------------------------------------
bool BusinessQuery::Delete(const ACDB_marker_idx_type aId) {
  enum Parameters { Id = 1 };

  if (!mDelete) {
    return false;
  }

  bool success = false;

  try {
    mDelete->bind(Parameters::Id, static_cast<long long>(aId));

    mDelete->exec();
    success = mDelete->isDone();

    mDelete->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete businesses from database by geohash
//!
//----------------------------------------------------------------
bool BusinessQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
  enum Parameters { GeohashStart = 1, GeohashEnd };

  if (!mDeleteGeohash) {
    return false;
  }

  bool success = false;

  try {
    mDeleteGeohash->bind(Parameters::GeohashStart, static_cast<long long>(aGeohashStart));
    mDeleteGeohash->bind(Parameters::GeohashEnd, static_cast<long long>(aGeohashEnd));

    mDeleteGeohash->exec();
    success = mDeleteGeohash->isDone();

    mDeleteGeohash->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Delete

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the detailed info for the specified object.
//!
//----------------------------------------------------------------
bool BusinessQuery::Get(const ACDB_marker_idx_type aId, BusinessTableDataType& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns { SectionTitle = 0, Labeled, CommaSeparatedList, BusinessPromotions, CallToAction };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<long long>(aId));

    success = mRead->executeStep();
    if (success) {
      aResultOut.mSectionTitle = mRead->getColumn(Columns::SectionTitle).getInt();
      aResultOut.mAttributeFieldsJson = mRead->getColumn(Columns::Labeled).getText();
      aResultOut.mAttributeMultiValueFieldsJson =
          mRead->getColumn(Columns::CommaSeparatedList).getText();
      aResultOut.mBusinessPromotionsJson = mRead->getColumn(Columns::BusinessPromotions).getText();
      aResultOut.mCallToActionJson = mRead->getColumn(Columns::CallToAction).getText();
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
//!   @brief Write business to database
//!
//----------------------------------------------------------------
bool BusinessQuery::Write(const ACDB_marker_idx_type aId,
                          BusinessTableDataType&& aBusinessTableData) {
  enum Parameters {
    Id = 1,
    SectionTitle,
    Labeled,
    CommaSeparatedList,
    BusinessPromotions,
    CallToAction
  };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<long long>(aId));
    mWrite->bind(Parameters::SectionTitle, aBusinessTableData.mSectionTitle);
    mWrite->bind(Parameters::Labeled, aBusinessTableData.mAttributeFieldsJson);
    mWrite->bind(Parameters::CommaSeparatedList, aBusinessTableData.mAttributeMultiValueFieldsJson);
    mWrite->bind(Parameters::BusinessPromotions, aBusinessTableData.mBusinessPromotionsJson);
    mWrite->bind(Parameters::CallToAction, aBusinessTableData.mCallToActionJson);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
