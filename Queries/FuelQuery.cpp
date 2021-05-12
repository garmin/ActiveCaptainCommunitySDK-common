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
#define DBG_TAG "FuelQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/FuelQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string DeleteSql{"DELETE FROM fuel WHERE id = ?;"};
static const std::string DeleteGeohashSql{
    "DELETE FROM fuel WHERE id IN (SELECT id FROM markers WHERE geohash BETWEEN ? AND ?);"};
static const std::string ReadSql{
    "SELECT sectionTitle, priceList, yesNo, labeled, sectionNote, distanceUnit, currency, dieselPrice, gasPrice, volumeUnit FROM fuel WHERE fuel.id = ?;"};
static const std::string WriteSql{
    "INSERT OR REPLACE INTO fuel (id, sectionTitle, priceList, yesNo, labeled, sectionNote, distanceUnit, currency, dieselPrice, gasPrice, volumeUnit) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create Fuel query object.
//!
//----------------------------------------------------------------
FuelQuery::FuelQuery(SQLite::Database& aDatabase) {
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
}  // End of FuelQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Delete fuel from database
//!
//----------------------------------------------------------------
bool FuelQuery::Delete(const ACDB_marker_idx_type aId) {
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
//!   @brief Delete fuel from database by geohash
//!
//----------------------------------------------------------------
bool FuelQuery::Delete(const uint64_t aGeohashStart, const uint64_t aGeohashEnd) {
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
bool FuelQuery::Get(const ACDB_marker_idx_type aId, FuelTableDataType& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns {
    SectionTitle = 0,
    PriceList,
    YesNo,
    Labeled,
    SectionNote,
    DistanceUnit,
    Currency,
    DieselPrice,
    GasPrice,
    VolumeUnit
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
      aResultOut.mYesNoPriceJson = mRead->getColumn(Columns::PriceList).getText();
      aResultOut.mYesNoJson = mRead->getColumn(Columns::YesNo).getText();
      aResultOut.mAttributeFieldsJson = mRead->getColumn(Columns::Labeled).getText();
      aResultOut.mSectionNoteJson = mRead->getColumn(Columns::SectionNote).getText();
      aResultOut.mDistanceUnit = mRead->getColumn(Columns::DistanceUnit).getUInt();
      aResultOut.mCurrency = mRead->getColumn(Columns::Currency).getText();
      aResultOut.mDieselPrice = mRead->getColumn(Columns::DieselPrice).getDouble();
      aResultOut.mGasPrice = mRead->getColumn(Columns::GasPrice).getDouble();
      aResultOut.mVolumeUnit = mRead->getColumn(Columns::VolumeUnit).getUInt();
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
//!   @brief Write fuel to database
//!
//----------------------------------------------------------------
bool FuelQuery::Write(const ACDB_marker_idx_type aId, FuelTableDataType&& aFuelTableData) {
  enum Parameters {
    Id = 1,
    SectionTitle,
    PriceList,
    YesNo,
    Labeled,
    SectionNote,
    DistanceUnit,
    Currency,
    DieselPrice,
    GasPrice,
    VolumeUnit
  };

  if (!mWrite) {
    return false;
  }

  bool success = false;

  try {
    mWrite->bind(Parameters::Id, static_cast<long long>(aId));
    mWrite->bind(Parameters::SectionTitle, aFuelTableData.mSectionTitle);
    mWrite->bind(Parameters::PriceList, aFuelTableData.mYesNoPriceJson);
    mWrite->bind(Parameters::YesNo, aFuelTableData.mYesNoJson);
    mWrite->bind(Parameters::Labeled, aFuelTableData.mAttributeFieldsJson);
    mWrite->bind(Parameters::SectionNote, aFuelTableData.mSectionNoteJson);
    mWrite->bind(Parameters::DistanceUnit, aFuelTableData.mDistanceUnit);
    mWrite->bind(Parameters::Currency, aFuelTableData.mCurrency);
    mWrite->bind(Parameters::DieselPrice, aFuelTableData.mDieselPrice);
    mWrite->bind(Parameters::GasPrice, aFuelTableData.mGasPrice);
    mWrite->bind(Parameters::VolumeUnit, aFuelTableData.mVolumeUnit);

    success = mWrite->exec();

    mWrite->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of Write

}  // end of namespace Acdb
