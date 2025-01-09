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
static const std::string ReadBasicFilteredSql{
    "SELECT m.id, m.poi_type, m.lastUpdate, m.name, m.searchFilter, ri.minLon, ri.minLat, COALESCE(bp.programTier, -1) programTier "
    "FROM markers m "
    "    INNER JOIN rIndex ri ON m.Id = ri.Id LEFT JOIN businessProgram bp ON m.Id = bp.Id "
    "WHERE minLon > ? AND maxLon < ? "
    "    AND minLat > ? AND maxLat < ? "
    "    AND m.poi_type & ? "
    "    AND m.searchFilter & ? "
    "    AND m.name LIKE ? "
    "LIMIT ?;"};
static const std::string ReadExtendedFilteredSql{
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
SearchMarkerQuery::SearchMarkerQuery(SQLite::Database& aDatabase)
    : mDatabase{aDatabase} {}  // End of SearchMarkerQuery

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

  bool success = false;

  try {
    SQLite::Statement read{mDatabase, ReadSql};
    read.bind(Parameters::Id, static_cast<int64_t>(aId));

    success = read.executeStep();
    if (success) {
      aResultOut.mId = read.getColumn(Columns::ColId).getInt64();
      aResultOut.mType = read.getColumn(Columns::PoiType).getInt();
      aResultOut.mLastUpdated = read.getColumn(Columns::LastUpdate).getInt64();
      aResultOut.mName = read.getColumn(Columns::Name).getText();
      aResultOut.mPosn.lon = read.getColumn(Columns::MinLon).getUInt();
      aResultOut.mPosn.lat = read.getColumn(Columns::MinLat).getUInt();
      aResultOut.mBusinessProgramTier = read.getColumn(Columns::ProgramTier).getInt();

      if (!read.isColumnNull(Columns::AvgRating)) {
        aResultOut.mReviewStatsData.mAverageRating =
            static_cast<float>(read.getColumn(Columns::AvgRating).getDouble());
      }

      if (!read.isColumnNull(Columns::ReviewCount)) {
        aResultOut.mReviewStatsData.mNumberOfReviews =
            read.getColumn(Columns::ReviewCount).getInt();
      }

      if (!read.isColumnNull(Columns::Phone)) {
        aResultOut.mContactData.mPhoneNumber = read.getColumn(Columns::Phone).getText();
      }

      if (!read.isColumnNull(Columns::VhfChannel)) {
        aResultOut.mContactData.mVhfChannel = read.getColumn(Columns::VhfChannel).getText();
      }

      if (!read.isColumnNull(Columns::Currency) && !read.isColumnNull(Columns::VolumeUnit)) {
        if (!read.isColumnNull(Columns::GasPrice)) {
          aResultOut.mFuelData.mGasPrice =
              static_cast<float>(read.getColumn(Columns::GasPrice).getDouble());
        }

        if (!read.isColumnNull(Columns::DieselPrice)) {
          aResultOut.mFuelData.mDieselPrice =
              static_cast<float>(read.getColumn(Columns::DieselPrice).getDouble());
        }

        aResultOut.mFuelData.mFuelPriceCurrency = read.getColumn(Columns::Currency).getText();
        aResultOut.mFuelData.mFuelPriceUnit = read.getColumn(Columns::VolumeUnit).getUInt();
      }
    }
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
//!   filter.
//!
//----------------------------------------------------------------
bool SearchMarkerQuery::GetBasicFiltered(const SearchMarkerFilter& aFilter,
                                         std::vector<MarkerTableDataType>& aResultOut) {
  enum Parameters { MinLon = 1, MaxLon, MinLat, MaxLat, PoiType, SearchFilter, Name, Limit };
  enum Columns {
    ColId = 0,
    ColPoiType,
    LastUpdate,
    ColName,
    ColSearchFilter,
    ColMinLon,
    ColMinLat,
    ProgramTier
  };

  bool success = false;

  try {
    SQLite::Statement readBasicFiltered{mDatabase, ReadBasicFilteredSql};
    readBasicFiltered.bind(Parameters::MinLon, aFilter.GetBbox().swc.lon);
    readBasicFiltered.bind(Parameters::MaxLon, aFilter.GetBbox().nec.lon);
    readBasicFiltered.bind(Parameters::MinLat, aFilter.GetBbox().swc.lat);
    readBasicFiltered.bind(Parameters::MaxLat, aFilter.GetBbox().nec.lat);
    readBasicFiltered.bind(Parameters::PoiType, aFilter.GetAllowedTypes());
    readBasicFiltered.bind(Parameters::SearchFilter,
                           static_cast<int64_t>(aFilter.GetAllowedCategories()));

    const std::string WILDCARD{"%"};
    std::string searchExpression;

    if (aFilter.GetSearchString().empty()) {
      searchExpression = WILDCARD;
    } else if (aFilter.GetStringMatchMode() == SearchMarkerFilter::MatchBeginningOfWord) {
      searchExpression = aFilter.GetSearchString() + WILDCARD;
    } else {
      searchExpression = WILDCARD + aFilter.GetSearchString() + WILDCARD;
    }

    readBasicFiltered.bind(Parameters::Name, searchExpression);
    readBasicFiltered.bind(Parameters::Limit, aFilter.GetMaxResults());

    while (readBasicFiltered.executeStep()) {
      MarkerTableDataType result;
      result.mId = readBasicFiltered.getColumn(Columns::ColId).getInt64();
      result.mType = readBasicFiltered.getColumn(Columns::ColPoiType).getInt();
      result.mLastUpdated = readBasicFiltered.getColumn(Columns::LastUpdate).getInt64();
      result.mName = readBasicFiltered.getColumn(Columns::ColName).getText();
      result.mSearchFilter = readBasicFiltered.getColumn(Columns::ColSearchFilter).getInt64();
      result.mPosn.lon = readBasicFiltered.getColumn(Columns::ColMinLon).getUInt();
      result.mPosn.lat = readBasicFiltered.getColumn(Columns::ColMinLat).getUInt();
      result.mBusinessProgramTier = readBasicFiltered.getColumn(Columns::ProgramTier).getInt();

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();

    readBasicFiltered.reset();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetBasicFiltered

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Get a list of item records based on the specified
//!   filter.
//!
//----------------------------------------------------------------
bool SearchMarkerQuery::GetExtendedFiltered(const SearchMarkerFilter& aFilter,
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
  bool success = false;

  try {
    SQLite::Statement readExtendedFiltered{mDatabase, ReadExtendedFilteredSql};
    readExtendedFiltered.bind(Parameters::MinLon, aFilter.GetBbox().swc.lon);
    readExtendedFiltered.bind(Parameters::MaxLon, aFilter.GetBbox().nec.lon);
    readExtendedFiltered.bind(Parameters::MinLat, aFilter.GetBbox().swc.lat);
    readExtendedFiltered.bind(Parameters::MaxLat, aFilter.GetBbox().nec.lat);
    readExtendedFiltered.bind(Parameters::PoiType, aFilter.GetAllowedTypes());
    readExtendedFiltered.bind(Parameters::SearchFilter,
                              static_cast<int64_t>(aFilter.GetAllowedCategories()));

    const std::string WILDCARD{"%"};
    std::string searchExpression;

    if (aFilter.GetSearchString().empty()) {
      searchExpression = WILDCARD;
    } else if (aFilter.GetStringMatchMode() == SearchMarkerFilter::MatchBeginningOfWord) {
      searchExpression = aFilter.GetSearchString() + WILDCARD;
    } else {
      searchExpression = WILDCARD + aFilter.GetSearchString() + WILDCARD;
    }

    readExtendedFiltered.bind(Parameters::Name, searchExpression);
    readExtendedFiltered.bind(Parameters::Limit, aFilter.GetMaxResults());

    while (readExtendedFiltered.executeStep()) {
      ExtendedMarkerDataType result;
      result.mId = readExtendedFiltered.getColumn(Columns::ColId).getInt64();
      result.mType = readExtendedFiltered.getColumn(Columns::ColPoiType).getInt();
      result.mLastUpdated = readExtendedFiltered.getColumn(Columns::LastUpdate).getInt64();
      result.mName = readExtendedFiltered.getColumn(Columns::ColName).getText();
      result.mPosn.lon = readExtendedFiltered.getColumn(Columns::ColMinLon).getUInt();
      result.mPosn.lat = readExtendedFiltered.getColumn(Columns::ColMinLat).getUInt();
      result.mBusinessProgramTier = readExtendedFiltered.getColumn(Columns::ProgramTier).getInt();

      if (!readExtendedFiltered.isColumnNull(Columns::AvgRating)) {
        result.mReviewStatsData.mAverageRating =
            static_cast<float>(readExtendedFiltered.getColumn(Columns::AvgRating).getDouble());
      }

      if (!readExtendedFiltered.isColumnNull(Columns::ReviewCount)) {
        result.mReviewStatsData.mNumberOfReviews =
            readExtendedFiltered.getColumn(Columns::ReviewCount).getInt();
      }

      if (!readExtendedFiltered.isColumnNull(Columns::Phone)) {
        result.mContactData.mPhoneNumber = readExtendedFiltered.getColumn(Columns::Phone).getText();
      }

      if (!readExtendedFiltered.isColumnNull(Columns::VhfChannel)) {
        result.mContactData.mVhfChannel =
            readExtendedFiltered.getColumn(Columns::VhfChannel).getText();
      }

      if (!readExtendedFiltered.isColumnNull(Columns::Currency) &&
          !readExtendedFiltered.isColumnNull(Columns::VolumeUnit)) {
        if (!readExtendedFiltered.isColumnNull(Columns::GasPrice)) {
          result.mFuelData.mGasPrice =
              static_cast<float>(readExtendedFiltered.getColumn(Columns::GasPrice).getDouble());
        }

        if (!readExtendedFiltered.isColumnNull(Columns::DieselPrice)) {
          result.mFuelData.mDieselPrice =
              static_cast<float>(readExtendedFiltered.getColumn(Columns::DieselPrice).getDouble());
        }

        result.mFuelData.mFuelPriceCurrency =
            readExtendedFiltered.getColumn(Columns::Currency).getText();
        result.mFuelData.mFuelPriceUnit =
            readExtendedFiltered.getColumn(Columns::VolumeUnit).getUInt();
      }

      aResultOut.push_back(std::move(result));
    }

    success = !aResultOut.empty();
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // End of GetExtendedFiltered
}  // end of namespace Acdb
