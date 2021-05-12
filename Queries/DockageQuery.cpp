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
#define DBG_TAG "DockageQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/DockageQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM dockage WHERE id = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM dockage WHERE id IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string ReadSql{
    "SELECT sectionTitle, commaSeparatedList, price, labeled, sectionNote, yesNo, distanceUnit FROM dockage WHERE id = ?;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO dockage (id, sectionTitle, commaSeparatedList, price, labeled, sectionNote, yesNo, distanceUnit) VALUES "
    "(?, ?, ?, ?, ?, ?, ?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Dockage query object.
//!
//----------------------------------------------------------------
DockageQuery::DockageQuery(SQLite::Database& aDatabase) {
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
}  // End of DockageQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete dockage from database
//!
//----------------------------------------------------------------
bool DockageQuery::Delete(const ACDB_marker_idx_type aId) {
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
//!   @brief Delete dockage from database by geohash
//!
//----------------------------------------------------------------
bool DockageQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
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
bool DockageQuery::Get(const ACDB_marker_idx_type aId, DockageTableDataType& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns {
    SectionTitle = 0,
    CommaSeparatedList,
    Price,
    Labeled,
    SectionNote,
    YesNo,
    DistanceUnit
  };

  if (!mRead) {
    return false;
  }

  bool success = false;

  try {
    mRead->bind(Parameters::Id, static_cast<long long>(aId));

    success = mRead->executeStep();
    if (success) {
      aResultOut.mSectionTitle = mRead->getColumn(Columns::SectionTitle).getInt();
      aResultOut.mYesNoMultiValueJson = mRead->getColumn(Columns::CommaSeparatedList).getText();
      aResultOut.mAttributePriceJson = mRead->getColumn(Columns::Price).getText();
      aResultOut.mAttributeFieldsJson = mRead->getColumn(Columns::Labeled).getText();
      aResultOut.mSectionNoteJson = mRead->getColumn(Columns::SectionNote).getText();
      aResultOut.mYesNoJson = mRead->getColumn(Columns::YesNo).getText();
      aResultOut.mDistanceUnit = mRead->getColumn(Columns::DistanceUnit).getUInt();
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
//!   @brief Write Dockage to database
//!
//----------------------------------------------------------------
bool DockageQuery::Write(const ACDB_marker_idx_type aId, DockageTableDataType&& aDockageTableData) {
  enum Parameters {
    Id = 1,
    SectionTitle,
    CommaSeparatedList,
    Price,
    Labeled,
    SectionNote,
    YesNo,
    DistanceUnit
  };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<long long>(aId));
    mWrite->bind(Parameters::SectionTitle, aDockageTableData.mSectionTitle);
    mWrite->bind(Parameters::CommaSeparatedList, aDockageTableData.mYesNoMultiValueJson);
    mWrite->bind(Parameters::Price, aDockageTableData.mAttributePriceJson);
    mWrite->bind(Parameters::Labeled, aDockageTableData.mAttributeFieldsJson);
    mWrite->bind(Parameters::SectionNote, aDockageTableData.mSectionNoteJson);
    mWrite->bind(Parameters::YesNo, aDockageTableData.mYesNoJson);
    mWrite->bind(Parameters::DistanceUnit, aDockageTableData.mDistanceUnit);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
