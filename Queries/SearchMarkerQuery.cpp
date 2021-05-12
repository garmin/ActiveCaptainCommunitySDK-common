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

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "SearchMarkerQuery"

#include "ACDB_pub_types.h"
#include "Acdb/Queries/SearchMarkerQuery.hpp"
#include "DBG_pub.h"
#include "SQLiteCpp/Column.h"

namespace Acdb {

static const std::string ReadSql{
    "SELECT m.id, m.poi_type, m.lastUpdate, m.name, ri.minLon, ri.minLat, COALESCE(bp.programTier, -1) programTier, "
    "       AVG(rv.rating), COUNT(rv.markerId), "
    "       c.phone, c.vhfChannel, "
    "       f.gasPrice, f.dieselPrice, f.currency, f.volumeUnit "
    "FROM markers m "
    "    INNER JOIN rIndex ri ON m.Id = ri.Id "
    "    LEFT OUTER JOIN businessProgram bp ON m.id = bp.id "
    "    LEFT OUTER JOIN contact c ON m.id = c.id "
    "    LEFT OUTER JOIN fuel f ON m.id = f.id "
    "    LEFT OUTER JOIN reviews rv ON m.id = rv.markerId "
    "WHERE m.id = ? "
    "GROUP BY m.id;"};
static const std::string ReadFilteredSql{
    "SELECT m.id, m.poi_type, m.lastUpdate, m.name, ri.minLon, ri.minLat, COALESCE(bp.programTier, -1) programTier, "
    "       AVG(rv.rating), COUNT(rv.markerId), "
    "       c.phone, c.vhfChannel, "
    "       f.gasPrice, f.dieselPrice, f.currency, f.volumeUnit "
    "FROM markers m "
    "    INNER JOIN rIndex ri ON m.Id = ri.Id "
    "    LEFT OUTER JOIN businessProgram bp ON m.id = bp.id "
    "    LEFT OUTER JOIN contact c ON m.id = c.id "
    "    LEFT OUTER JOIN fuel f ON m.id = f.id "
    "    LEFT OUTER JOIN reviews rv ON m.id = rv.markerId "
    "WHERE minLon > ? AND maxLon < ? "
    "    AND minLat > ? AND maxLat < ? "
    "    AND m.poi_type & ? "
    "    AND m.searchFilter & ? "
    "    AND m.name LIKE ? "
    "GROUP BY m.id "
    "LIMIT ?;"};

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Create SearchMarker query object.
//!
//----------------------------------------------------------------
SearchMarkerQuery::SearchMarkerQuery(SQLite::Database& aDatabase) {
  try {
    mRead.reset(new SQLite::Statement{aDatabase, ReadSql});
    mReadFiltered.reset(new SQLite::Statement{aDatabase, ReadFilteredSql});
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    mRead.reset();
    mReadFiltered.reset();
  }
}  // End of SearchMarkerQuery

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get the info for the specified object.
//!
//----------------------------------------------------------------
bool SearchMarkerQuery::Get(const ACDB_marker_idx_type aId, ExtendedMarkerDataType& aResultOut) {
  enum Parameters { Id = 1 };
  enum Columns {
    ColId = 0,
    PoiType,
    LastUpdate,
    Name,
    MinLon,
    MinLat,
    ProgramTier,
    AvgRating,
    ReviewCount,
    Phone,
    VhfChannel,
    GasPrice,
    DieselPrice,
    Currency,
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
      aResultOut.mId = mRead->getColumn(Columns::ColId).getInt64();
      aResultOut.mType = mRead->getColumn(Columns::PoiType).getInt();
      aResultOut.mLastUpdated = mRead->getColumn(Columns::LastUpdate).getInt64();
      aResultOut.mName = mRead->getColumn(Columns::Name).getText();
      aResultOut.mPosn.lon = mRead->getColumn(Columns::MinLon).getUInt();
      aResultOut.mPosn.lat = mRead->getColumn(Columns::MinLat).getUInt();
      aResultOut.mBusinessProgramTier = mRead->getColumn(Columns::ProgramTier).getInt();

      if (!mRead->isColumnNull(Columns::AvgRating)) {
        aResultOut.mReviewStatsData.mAverageRating =
            static_cast<float>(mRead->getColumn(Columns::AvgRating).getDouble());
      }

      if (!mRead->isColumnNull(Columns::ReviewCount)) {
        aResultOut.mReviewStatsData.mNumberOfReviews =
            mRead->getColumn(Columns::ReviewCount).getInt();
      }

      if (!mRead->isColumnNull(Columns::Phone)) {
        aResultOut.mContactData.mPhoneNumber = mRead->getColumn(Columns::Phone).getText();
      }

      if (!mRead->isColumnNull(Columns::VhfChannel)) {
        aResultOut.mContactData.mVhfChannel = mRead->getColumn(Columns::VhfChannel).getText();
      }

      if (!mRead->isColumnNull(Columns::Currency) && !mRead->isColumnNull(Columns::VolumeUnit)) {
        if (!mRead->isColumnNull(Columns::GasPrice)) {
          aResultOut.mFuelData.mGasPrice =
              static_cast<float>(mRead->getColumn(Columns::GasPrice).getDouble());
        }

        if (!mRead->isColumnNull(Columns::DieselPrice)) {
          aResultOut.mFuelData.mDieselPrice =
              static_cast<float>(mRead->getColumn(Columns::DieselPrice).getDouble());
        }

        aResultOut.mFuelData.mFuelPriceCurrency = mRead->getColumn(Columns::Currency).getText();
        aResultOut.mFuelData.mFuelPriceUnit = mRead->getColumn(Columns::VolumeUnit).getUInt();
      }
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
//!   @detail Get a list of item records based on the specified
//!   filer.
//!
//----------------------------------------------------------------
bool SearchMarkerQuery::GetFiltered(const SearchMarkerFilter& aFilter,
                                    std::vector<ExtendedMarkerDataType>& aResultOut) {
  enum Parameters { MinLon = 1, MaxLon, MinLat, MaxLat, PoiType, SearchFilter, Name, Limit };
  enum Columns {
    ColId = 0,
    ColPoiType,
    LastUpdate,
    ColName,
    ColMinLon,
    ColMinLat,
    ProgramTier,
    AvgRating,
    ReviewCount,
    Phone,
    VhfChannel,
    GasPrice,
    DieselPrice,
    Currency,
    VolumeUnit
  };

  if (!mReadFiltered) {
    return false;
  }

  bool success = false;

  try {
    mReadFiltered->bind(Parameters::MinLon, aFilter.GetBbox().swc.lon);
    mReadFiltered->bind(Parameters::MaxLon, aFilter.GetBbox().nec.lon);
    mReadFiltered->bind(Parameters::MinLat, aFilter.GetBbox().swc.lat);
    mReadFiltered->bind(Parameters::MaxLat, aFilter.GetBbox().nec.lat);
    mReadFiltered->bind(Parameters::PoiType, aFilter.GetAllowedTypes());
    mReadFiltered->bind(Parameters::SearchFilter,
                        static_cast<long long>(aFilter.GetAllowedCategories()));

    const std::string WILDCARD{"%"};
    std::string searchExpression;

    if (aFilter.GetSearchString().empty()) {
      searchExpression = WILDCARD;
    } else if (aFilter.GetStringMatchMode() == SearchMarkerFilter::MatchBeginningOfWord) {
      searchExpression = aFilter.GetSearchString() + WILDCARD;
    } else {
      searchExpression = WILDCARD + aFilter.GetSearchString() + WILDCARD;
    }

    mReadFiltered->bind(Parameters::Name, searchExpression);
    mReadFiltered->bind(Parameters::Limit, aFilter.GetMaxResults());

    while (mReadFiltered->executeStep()) {
      ExtendedMarkerDataType result;
      result.mId = mReadFiltered->getColumn(Columns::ColId).getInt64();
      result.mType = mReadFiltered->getColumn(Columns::ColPoiType).getInt();
      result.mLastUpdated = mReadFiltered->getColumn(Columns::LastUpdate).getInt64();
      result.mName = mReadFiltered->getColumn(Columns::ColName).getText();
      result.mPosn.lon = mReadFiltered->getColumn(Columns::ColMinLon).getUInt();
      result.mPosn.lat = mReadFiltered->getColumn(Columns::ColMinLat).getUInt();
      result.mBusinessProgramTier = mReadFiltered->getColumn(Columns::ProgramTier).getInt();

      if (!mReadFiltered->isColumnNull(Columns::AvgRating)) {
        result.mReviewStatsData.mAverageRating =
            static_cast<float>(mReadFiltered->getColumn(Columns::AvgRating).getDouble());
      }

      if (!mReadFiltered->isColumnNull(Columns::ReviewCount)) {
        result.mReviewStatsData.mNumberOfReviews =
            mReadFiltered->getColumn(Columns::ReviewCount).getInt();
      }

      if (!mReadFiltered->isColumnNull(Columns::Phone)) {
        result.mContactData.mPhoneNumber = mReadFiltered->getColumn(Columns::Phone).getText();
      }

      if (!mReadFiltered->isColumnNull(Columns::VhfChannel)) {
        result.mContactData.mVhfChannel = mReadFiltered->getColumn(Columns::VhfChannel).getText();
      }

      if (!mReadFiltered->isColumnNull(Columns::Currency) &&
          !mReadFiltered->isColumnNull(Columns::VolumeUnit)) {
        if (!mReadFiltered->isColumnNull(Columns::GasPrice)) {
          result.mFuelData.mGasPrice =
              static_cast<float>(mReadFiltered->getColumn(Columns::GasPrice).getDouble());
        }

        if (!mReadFiltered->isColumnNull(Columns::DieselPrice)) {
          result.mFuelData.mDieselPrice =
              static_cast<float>(mReadFiltered->getColumn(Columns::DieselPrice).getDouble());
        }

        result.mFuelData.mFuelPriceCurrency = mReadFiltered->getColumn(Columns::Currency).getText();
        result.mFuelData.mFuelPriceUnit = mReadFiltered->getColumn(Columns::VolumeUnit).getUInt();
      }

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    mReadFiltered->reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetSearchMarkersByFilter
}  // end of namespace Acdb
