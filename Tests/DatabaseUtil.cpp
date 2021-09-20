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
    @brief Database utility functions for tests

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "DatabaseUtil"

#include "Acdb/Queries/AddressQuery.hpp"
#include "Acdb/Queries/AmenitiesQuery.hpp"
#include "Acdb/Queries/BusinessQuery.hpp"
#include "Acdb/Queries/BusinessPhotoQuery.hpp"
#include "Acdb/Queries/BusinessProgramQuery.hpp"
#include "Acdb/Queries/CompetitorQuery.hpp"
#include "Acdb/Queries/ContactQuery.hpp"
#include "Acdb/Queries/DockageQuery.hpp"
#include "Acdb/Queries/FuelQuery.hpp"
#include "Acdb/Queries/LanguageQuery.hpp"
#include "Acdb/Queries/MarkerQuery.hpp"
#include "Acdb/Queries/MarkerMetaQuery.hpp"
#include "Acdb/Queries/MooringsQuery.hpp"
#include "Acdb/Queries/NavigationQuery.hpp"
#include "Acdb/Queries/PositionQuery.hpp"
#include "Acdb/Queries/RetailQuery.hpp"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "Acdb/Queries/ReviewPhotoQuery.hpp"
#include "Acdb/Queries/ServicesQuery.hpp"
#include "Acdb/Queries/TranslatorQuery.hpp"
#include "Acdb/SearchMarkerFilter.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "DBG_pub.h"
#include "sqlite3.h"

namespace Acdb {
namespace Test {

static void CreateTables(TF_state_type* aState, SQLite::Database& aDatabase);

static uint64_t GetGeohashStart(int tileX, int tileY);

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Create an in-RAM empty database.
//!
//----------------------------------------------------------------
SQLite::Database CreateDatabase(TF_state_type* aState) {
  SQLite::Database result{":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_FULLMUTEX};
  CreateTables(aState, result);

  return result;
}  // end of CreateDatabase

//----------------------------------------------------------------
//!
//!   @private
//!   @detail
//!         Add table schema to database
//!
//----------------------------------------------------------------
static void CreateTables(TF_state_type* aState, SQLite::Database& aDatabase) {
  const std::string createSqls[]{
      "CREATE TABLE markers( id INTEGER PRIMARY KEY NOT NULL, poi_type INTEGER, lastUpdate INTEGER, name TEXT, searchFilter INTEGER, geohash BIGINT );",
      "CREATE TABLE address( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, string TEXT, labeled TEXT );",
      "CREATE TABLE amenities( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, sectionNote TEXT, yesNo TEXT );",
      "CREATE TABLE business( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, labeled TEXT, commaSeparatedList TEXT, businessPromotions TEXT, callToAction TEXT );",
      "CREATE TABLE businessPhotos( id INTEGER NOT NULL, ordinal INTEGER NOT NULL, downloadUrl TEXT NOT NULL, PRIMARY KEY (id, ordinal) );",
      "CREATE TABLE businessProgram( id INTEGER PRIMARY KEY NOT NULL, competitorAd TEXT, programTier INTEGER NOT NULL );",
      "CREATE TABLE competitor( poiId INTEGER NOT NULL, competitorPoiId INTEGER NOT NULL, ordinal INTEGER NOT NULL, PRIMARY KEY (poiId, competitorPoiId) );",
      "CREATE TABLE contact( id INTEGER PRIMARY KEY NOT NULL, vhfChannel TEXT, phone TEXT, sectionTitle INTEGER, labeled TEXT );",
      "CREATE TABLE dockage( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, commaSeparatedList TEXT, price TEXT, labeled TEXT, sectionNote TEXT, yesNo TEXT, distanceUnit INTEGER );",
      "CREATE TABLE fuel( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, priceList TEXT, yesNo TEXT, labeled TEXT, sectionNote TEXT, distanceUnit INTEGER, currency TEXT, dieselPrice REAL, gasPrice REAL, volumeUnit INTEGER );",
      "CREATE TABLE languageType( id INTEGER PRIMARY KEY NOT NULL, isoCode TEXT );",
      "CREATE TABLE markerMeta( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, sectionNote TEXT );",
      "CREATE TABLE mooring( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, price TEXT, labeled TEXT, sectionNote TEXT, yesNo TEXT );",
      "CREATE TABLE mustacheTemplates( name TEXT PRIMARY KEY NOT NULL, template TEXT NOT NULL );",
      "CREATE TABLE navigation( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, labeled TEXT, sectionNote TEXT, distanceUnit INTEGER );",
      "CREATE TABLE retail( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, sectionNote TEXT, yesNo TEXT );",
      "CREATE TABLE reviews( markerId INTEGER, reviewId INTEGER PRIMARY KEY NOT NULL, rating INTEGER, title TEXT, date TEXT, captain TEXT, review TEXT, response TEXT, lastUpdate INTEGER, votes INTEGER );",
      "CREATE TABLE reviewPhotos( id INTEGER NOT NULL, ordinal INTEGER NOT NULL, downloadUrl TEXT NOT NULL, PRIMARY KEY (id, ordinal) );",
      "CREATE TABLE searchBitType( bit INTEGER PRIMARY KEY NOT NULL, description TEXT );",
      "CREATE TABLE services( id INTEGER PRIMARY KEY NOT NULL, sectionTitle INTEGER, sectionNote TEXT, yesNo TEXT );",
      "CREATE TABLE tileLastUpdate( tileX INTEGER NOT NULL, tileY INTEGER NOT NULL, markerLastUpdate INTEGER, reviewLastUpdate INTEGER, PRIMARY KEY (tileX, tileY) );",
      "CREATE TABLE tiles( tileX INTEGER NOT NULL, tileY INTEGER NOT NULL, geohashStart BIGINT NOT NULL, geohashEnd BIGINT NOT NULL, PRIMARY KEY (tileX, tileY) );",
      "CREATE TABLE translations( id INTEGER, langId INTEGER, translation TEXT, PRIMARY KEY (id, langId) );",
      "CREATE TABLE versions( value TEXT );",
      "CREATE VIRTUAL TABLE rIndex USING rtree_i32(id, minLon, maxLon, minLat, maxLat);",
      "CREATE VIRTUAL TABLE tileRIndex USING rtree_i32(id, minLon, maxLon, minLat, maxLat);"};

  for (const std::string& sql : createSqls) {
    SQLite::Statement stmt{aDatabase, sql};
    TF_assert(aState, SQLITE_DONE == stmt.tryExecuteStep());
  }
}  // End of CreateTables

//----------------------------------------------------------------
//!
//!   @private
//!   @detail
//!         Get test geohash start by TileXY
//!
//----------------------------------------------------------------
static uint64_t GetGeohashStart(int aTileX, int aTileY) {
  return static_cast<uint64_t>((aTileY * 16) + aTileX) * 1000;
}  // end of GetGeohashStart

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Get test MarkerTableDataCollection
//!
//----------------------------------------------------------------
MarkerTableDataCollection GetMarkerTableDataCollection() {
  uint64_t minimalCategory = SearchMarkerFilter::Any;
  uint64_t marinaCategory = minimalCategory | SearchMarkerFilter::MarinasAndMoorings;

  ACDB_marker_idx_type markerId = 1;

  MarkerTableDataCollection markerTableDataCollection;

  markerTableDataCollection.mMarker =
      MarkerTableDataType{markerId,   ACDB_MARINA,           1527084000,     "Test Marina 1",
                          {100, 100}, GetGeohashStart(1, 1), marinaCategory, 3};
  markerTableDataCollection.mMarkerMeta = MarkerMetaTableDataType{
      "{ \"value\": \"Marker note here.\", \"isDistance\": false }",  // SectionNote
      static_cast<ACDB_text_handle_type>(TextHandle::SummaryTitle)    // SectionTitle
  };

  markerTableDataCollection.mAddress.reset(new AddressTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::AddressTitle),  // SectionTitle
      "[ { \"value\": \"1200 W 151st St\" }, { \"value\": \"Olathe, KS 66062\" }, { \"value\": \"US\" } ]",  // StringFields
      "[ { \"value\": \"www.garmin.com\", \"hyperlink\" : \"http://www.garmin.com\", \"isDistance\" : false } ]",  // AttributeFields
  });

  markerTableDataCollection.mAmenities.reset(new AmenitiesTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::AmenitiesTitle),  // SectionTitle
      "{ \"fieldTextHandle\": 29, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote
      "[ { \"fieldTextHandle\": 31, \"value\": \"Nearby\", \"note\": \"Great stuff nearby.\" }, { \"fieldTextHandle\": 32, \"value\": \"No\" } ]"  // YesNoFields
  });

  markerTableDataCollection.mBusiness.reset(new BusinessTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::BusinessTitle),  // SectionTitle
      "[ { \"fieldTextHandle\": 45, \"valueTextHandle\": 0, \"isDistance\": false}, {\"fieldTextHandle\": 46, \"valueTextHandle\": 48, \"isDistance\": false } ]",  // AttributeFields
      "[ { \"valueTextHandles\": [50, 52], \"fieldTextHandle\": 49, \"isDistance\": false } ]",  // AttributeMultiValueFields
      "{ \"fieldTextHandle\": 159, \"businessPromotions\": [ { \"ordinal\": 1, \"startDate\": \"1970-01-01T00:00:00Z\", \"endDate\": \"2100-12-31T23:59:59Z\", \"title\": \"Title here\", \"details\": \"Details here\" } ] }",  // BusinessPromotions
      "{ \"linkText\": \"Click Here!\", \"linkUrl\": \"http://www.garmin.com\" }"  // CallToAction
  });

  markerTableDataCollection.mBusinessPhotos = std::vector<BusinessPhotoTableDataType>{
      {markerId, 1, "https://activecaptain.garmin.com/photos/999.jpg"},
      {markerId, 2, "https://activecaptain.garmin.com/photos/1000.jpg"}};

  markerTableDataCollection.mBusinessProgram.reset(new BusinessProgramTableDataType{
      markerId,
      "{ \"text\": \"Stay with us instead!\", \"photoUrl\": \"https://activecaptain.garmin.com/photos/999.jpg\" }",
      3});

  markerTableDataCollection.mCompetitors = std::vector<CompetitorTableDataType>{
      {1, 2, 1}, {1, 3, 2}, {1, 4, 3}, {1, 5, 4}, {1, 6, 5},
      {1, 7, 6}, {1, 8, 7}, {1, 9, 8}

      // Won't advertise on the following POIs:
      //     3 -- competitor but also a tier 3
      //     8 and 9 -- only the first 5 eligible competitors can be advertised on
      //     10 -- not a competitor
  };

  markerTableDataCollection.mContact.reset(new ContactTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::ContactTitle),
      std::string(),  // aAttributeFieldsJson,
      "555-1234",     // aPhone,
      "789"           // aVhfChannel
  });

  markerTableDataCollection.mDockage.reset(new DockageTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::DockageTitle),  // SectionTitle
      "[ { \"valueTextHandles\": [ 83, 84, 86 ], \"fieldTextHandle\": 76, \"value\": \"Yes\" }, { \"fieldTextHandle\": 97, \"value\": \"No\" } ]",  // YesNoMultiValue
      "[ { \"price\": \"1.25 USD\", \"pricingUnitTextHandle\": 4, \"priceDate\": \"2018-05-23T00:00:00Z\", \"fieldTextHandle\": 78, \"isDistance\": false } ]",  // AttributePrice
      "[ { \"fieldTextHandle\": 79, \"value\": \"500\", \"isDistance\": false }, { \"fieldTextHandle\": 89, \"value\": \"45.72\", \"isDistance\": true } ]",  // AttributeFields,
      "{ \"fieldTextHandle\": 75, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote,
      "[ { \"fieldTextHandle\": 91, \"value\": \"Unknown\" }, { \"fieldTextHandle\": 92, \"value\": \"Nearby\" } ]",  // YesNo
      ACDB_METER  // DistanceUnit
  });

  markerTableDataCollection.mFuel.reset(new FuelTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::FuelTitle),
      std::string(),  // aYesNoPriceJson
      std::string(),  // aYesNoJson
      std::string(),  // aAttributeFieldsJson
      std::string(),  // aSectionNoteJson
      ACDB_METER,     // aDistanceUnit,
      "USD",          // aCurrency,
      1.2,            // aDieselPrice,
      1.6,            // aGasPrice,
      ACDB_LITER      // aVolumeUnit
  });

  markerTableDataCollection.mMoorings.reset(new MooringsTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::MooringsTitle),  // SectionTitle
      "{ \"fieldTextHandle\": 106, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote
      "[ { \"price\": \"1.00 USD\", \"pricingUnitTextHandle\": 4, \"priceDate\": \"2018-04-17T00:00:00Z\", \"fieldTextHandle\": 109, \"value\": \"Yes\" } ]",  // YesNoPrice
      "[ { \"fieldTextHandle\": 79, \"value\": \"772\", \"isDistance\": false }, { \"fieldTextHandle\": 80, \"value\": \"100\", \"isDistance\": false } ]",  // AttributeFields
      "[ { \"fieldTextHandle\": 107, \"value\": \"No\" }, { \"fieldTextHandle\": 108, \"value\": \"Yes\" }, { \"fieldTextHandle\": 93, \"value\": \"Unknown\" } ]"  // YesNoFields
  });

  markerTableDataCollection.mNavigation.reset(new NavigationTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::NavigationTitle),  // SectionTitle
      "{ \"fieldTextHandle\": 65, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote
      "[ { \"fieldTextHandle\": 55, \"value\": \"0.3048\", \"isDistance\": true }, { \"fieldTextHandle\": 56, \"valueTextHandle\": 58, \"isDistance\": false } ]",  // AttributeFields
      ACDB_METER  // DistanceUnit
  });

  markerTableDataCollection.mRetail.reset(new RetailTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::RetailTitle),  // SectionTitle
      "{ \"fieldTextHandle\": 111, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote
      "[ { \"fieldTextHandle\": 112, \"value\": \"No\" }, { \"fieldTextHandle\": 113, \"value\": \"Yes\" }, { \"fieldTextHandle\": 114, \"value\": \"Unknown\" } ]"  // YesNoFields
  });

  markerTableDataCollection.mServices.reset(new ServicesTableDataType{
      static_cast<ACDB_text_handle_type>(TextHandle::ServicesTitle),  // SectionTitle
      "{ \"fieldTextHandle\": 118, \"value\": \"Section note here.\", \"isDistance\": false }",  // SectionNote
      "[ { \"fieldTextHandle\": 122, \"value\": \"No\" }, { \"fieldTextHandle\": 123, \"value\": \"Yes\", \"note\": \"Local\" }, { \"fieldTextHandle\": 124, \"value\": \"Unknown\" } ]"  // YesNoFields
  });

  return markerTableDataCollection;
}  // end of GetMarkerTableDataCollection

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Get test reviews
//!
//----------------------------------------------------------------
std::vector<ReviewTableDataCollection> GetReviewsTableDataCollection() {
  return std::vector<ReviewTableDataCollection>{
      ReviewTableDataCollection{
          ReviewTableDataType(1, 1, 1527084005, 1, "Review Title 1", "2018-05-23T00:00:00Z",
                              "Test Captain 1", "Review Text 1", 0, false, std::string()),
          std::vector<ReviewPhotoTableDataType>{
              ReviewPhotoTableDataType{1, 1, "https://activecaptain.garmin.com/photos/10101.jpg"}}},
      ReviewTableDataCollection{
          ReviewTableDataType(2, 1, 1527084006, 2, "Review Title 2", "2018-05-23T00:00:00Z",
                              "Test Captain 2", "Review Text 2", 0, false, std::string()),
          std::vector<ReviewPhotoTableDataType>{
              ReviewPhotoTableDataType{2, 1, "https://activecaptain.garmin.com/photos/10102.jpg"},
              ReviewPhotoTableDataType{2, 2,
                                       "https://activecaptain.garmin.com/photos/10102-2.jpg"}}},
      ReviewTableDataCollection{ReviewTableDataType(3, 1, 1527084007, 3, "Review Title 3",
                                                    "2018-05-23T00:00:00Z", "Test Captain 3",
                                                    "Review Text 3", 0, false, std::string())},
      ReviewTableDataCollection{ReviewTableDataType(4, 1, 1527084008, 3, "Review Title 4",
                                                    "2018-05-23T00:00:00Z", "Test Captain 4",
                                                    "Review Text 4", 0, false, std::string())}};
}  // end of GetReviewsTableDataCollection

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Add test data to the database.  Values are used by
//!         multiple tests.  Modify with caution.
//!
//----------------------------------------------------------------
void PopulateDatabase(TF_state_type* aState, SQLite::Database& aDatabase) {
  AddressQuery addressQuery{aDatabase};
  AmenitiesQuery amenitiesQuery{aDatabase};
  BusinessQuery businessQuery{aDatabase};
  BusinessPhotoQuery businessPhotoQuery{aDatabase};
  BusinessProgramQuery businessProgramQuery{aDatabase};
  CompetitorQuery competitorQuery{aDatabase};
  ContactQuery contactQuery{aDatabase};
  DockageQuery dockageQuery{aDatabase};
  FuelQuery fuelQuery{aDatabase};
  MarkerQuery markerQuery{aDatabase};
  MarkerMetaQuery markerMetaQuery{aDatabase};
  MooringsQuery mooringsQuery{aDatabase};
  NavigationQuery navigationQuery{aDatabase};
  PositionQuery positionQuery{aDatabase};
  RetailQuery retailQuery{aDatabase};
  ReviewQuery reviewQuery{aDatabase};
  ReviewPhotoQuery reviewPhotoQuery{aDatabase};
  ServicesQuery servicesQuery{aDatabase};

  PopulateTilesTable(aState, aDatabase);
  PopulateTranslationsTable(aState, aDatabase);

  // Write data for a marker.
  MarkerTableDataCollection markerTableDataCollection = GetMarkerTableDataCollection();
  ACDB_marker_idx_type markerId = markerTableDataCollection.mMarker.mId;
  scposn_type posn = markerTableDataCollection.mMarker.mPosn;
  TF_assert(aState, markerQuery.Write(markerId, std::move(markerTableDataCollection.mMarker)));
  TF_assert(aState, positionQuery.Write(markerId, posn));
  TF_assert(aState, addressQuery.Write(markerId, std::move(*(markerTableDataCollection.mAddress))));
  TF_assert(aState,
            amenitiesQuery.Write(markerId, std::move(*(markerTableDataCollection.mAmenities))));
  TF_assert(aState,
            businessQuery.Write(markerId, std::move(*(markerTableDataCollection.mBusiness))));
  for (auto businessPhoto : markerTableDataCollection.mBusinessPhotos) {
    TF_assert(aState, businessPhotoQuery.Write(markerId, std::move(businessPhoto)));
  }
  TF_assert(aState, businessProgramQuery.Write(
                        markerId, std::move(*(markerTableDataCollection.mBusinessProgram))));
  for (auto competitor : markerTableDataCollection.mCompetitors) {
    TF_assert(aState, competitorQuery.Write(markerId, std::move(competitor)));
  }
  TF_assert(aState, contactQuery.Write(markerId, std::move(*(markerTableDataCollection.mContact))));
  TF_assert(aState, dockageQuery.Write(markerId, std::move(*(markerTableDataCollection.mDockage))));
  TF_assert(aState, fuelQuery.Write(markerId, std::move(*(markerTableDataCollection.mFuel))));
  TF_assert(aState,
            markerMetaQuery.Write(markerId, std::move(markerTableDataCollection.mMarkerMeta)));
  TF_assert(aState,
            mooringsQuery.Write(markerId, std::move(*(markerTableDataCollection.mMoorings))));
  TF_assert(aState,
            navigationQuery.Write(markerId, std::move(*(markerTableDataCollection.mNavigation))));
  TF_assert(aState, retailQuery.Write(markerId, std::move(*(markerTableDataCollection.mRetail))));
  TF_assert(aState,
            servicesQuery.Write(markerId, std::move(*(markerTableDataCollection.mServices))));

  // Write reviews.
  std::vector<ReviewTableDataCollection> reviewTableDataCollection =
      GetReviewsTableDataCollection();
  for (auto review : reviewTableDataCollection) {
    ACDB_review_idx_type reviewId = review.mReview.mId;
    TF_assert(aState, reviewQuery.Write(reviewId, std::move(review.mReview)));

    // Write review photos.
    for (auto reviewPhoto : review.mReviewPhotos) {
      TF_assert(aState, reviewPhotoQuery.Write(reviewId, std::move(reviewPhoto)));
    }
  }

  // Write some additional markers.
  uint64_t minimalCategory = SearchMarkerFilter::Any;
  uint64_t marinaCategory = minimalCategory | SearchMarkerFilter::MarinasAndMoorings;
  std::vector<MarkerTableDataType> markerTableData{
      MarkerTableDataType{2,
                          ACDB_MARINA,
                          1527084001,
                          "Test Marina 2",
                          {200, 200},
                          GetGeohashStart(2, 2),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{3,
                          ACDB_MARINA,
                          1527084002,
                          "Test Marina 3",
                          {300, 300},
                          GetGeohashStart(3, 3),
                          minimalCategory,
                          3},
      MarkerTableDataType{4,
                          ACDB_MARINA,
                          1527084003,
                          "Test Marina 4",
                          {400, 400},
                          GetGeohashStart(4, 4),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{5,
                          ACDB_MARINA,
                          1527084004,
                          "Test Marina 5",
                          {500, 500},
                          GetGeohashStart(5, 5),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{6,
                          ACDB_MARINA,
                          1527084005,
                          "Test Marina 6",
                          {600, 600},
                          GetGeohashStart(6, 6),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{7,
                          ACDB_MARINA,
                          1527084006,
                          "Test Marina 7",
                          {700, 700},
                          GetGeohashStart(7, 7),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{8,
                          ACDB_MARINA,
                          1527084007,
                          "Test Marina 8",
                          {800, 800},
                          GetGeohashStart(8, 8),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{9,
                          ACDB_MARINA,
                          1527084008,
                          "Test Marina 9",
                          {900, 900},
                          GetGeohashStart(9, 9),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{10,
                          ACDB_MARINA,
                          1527084009,
                          "Test Marina 10",
                          {1000, 1000},
                          GetGeohashStart(10, 10),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{11,
                          ACDB_HAZARD,
                          1527084004,
                          "Test Hazard 1",
                          {325, 325},
                          GetGeohashStart(3, 3),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},

      MarkerTableDataType{21,
                          ACDB_MARINA,
                          1527084001,
                          "Yet Another Test Marina 1",
                          {200, 200},
                          GetGeohashStart(2, 2),
                          marinaCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
      MarkerTableDataType{22,
                          ACDB_MARINA,
                          1527084001,
                          "Yet Another Test Marina 2",
                          {200, 200},
                          GetGeohashStart(2, 2),
                          minimalCategory,
                          ACDB_INVALID_BUSINESS_PROGRAM_TIER},
  };

  // Add a review for marker 2.
  TF_assert(aState,
            reviewQuery.Write(999, ReviewTableDataType{5, 2, 1527084008, 3, "Review Title 5",
                                                       "2018-05-23T00:00:00Z", "Test Captain 5",
                                                       "Review Text 5", 0, false, std::string()}));

  for (auto marker : markerTableData) {
    markerId = marker.mId;
    posn = marker.mPosn;

    MarkerMetaTableDataType markerMetaTableData;

    TF_assert(aState, markerQuery.Write(markerId, std::move(marker)));
    TF_assert(aState, positionQuery.Write(markerId, posn));
    TF_assert(aState, markerMetaQuery.Write(markerId, std::move(std::move(markerMetaTableData))));
  }

  // Add Business Program entry for POI 3
  BusinessProgramTableDataType businessProgramTableData{3, "", 3};
  TF_assert(aState, businessProgramQuery.Write(3, std::move(businessProgramTableData)));

  PopulateTileLastUpdateTable(aState, aDatabase);
}  // end of PopulateDatabase

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Add test data to the database.  Values are used by
//!         multiple tests.  Modify with caution.
//!
//----------------------------------------------------------------
void PopulateTilesTable(TF_state_type* aState, SQLite::Database& aDatabase) {
  SQLite::Statement insertTile{
      aDatabase, "INSERT INTO tiles (tileX, tileY, geohashStart, geohashEnd) VALUES (?, ?, ?, ?)"};
  SQLite::Statement insertTileRIndex{
      aDatabase,
      "INSERT INTO tileRIndex (id, minLon, maxLon, minLat, maxLat) VALUES (?, ?, ?, ?, ?)"};

  enum TileParameters { TileX = 1, TileY, GeohashStart, GeohashEnd };
  enum TileRIndexParameters { Id = 1, MinLon, MaxLon, MinLat, MaxLat };

  for (int y = 0; y < 16; y++) {
    for (int x = 0; x < 16; x++) {
      insertTile.bind(TileParameters::TileX, x);
      insertTile.bind(TileParameters::TileY, y);
      insertTile.bind(TileParameters::GeohashStart, static_cast<long long>(GetGeohashStart(x, y)));
      insertTile.bind(TileParameters::GeohashEnd,
                      static_cast<long long>(GetGeohashStart(x, y) + 999));

      insertTileRIndex.bind(TileRIndexParameters::Id, (y * 16) + x);
      insertTileRIndex.bind(TileRIndexParameters::MinLon, x * 100);
      insertTileRIndex.bind(TileRIndexParameters::MaxLon, (x * 100) + 99);
      insertTileRIndex.bind(TileRIndexParameters::MinLat, y * 100);
      insertTileRIndex.bind(TileRIndexParameters::MaxLat, (y * 100) + 99);

      TF_assert(aState, SQLITE_DONE == insertTile.tryExecuteStep());
      TF_assert(aState, SQLITE_DONE == insertTileRIndex.tryExecuteStep());

      insertTile.reset();
      insertTileRIndex.reset();
    }
  }
}  // end of PopulateTilesTable

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Add test data to the database.  Values are used by
//!         multiple tests.  Modify with caution.
//!
//----------------------------------------------------------------
void PopulateTileLastUpdateTable(TF_state_type* aState, SQLite::Database& aDatabase) {
  SQLite::Statement insertTileLastUpdate{
      aDatabase,
      "INSERT OR REPLACE INTO tileLastUpdate (tileX, tileY, markerLastUpdate, reviewLastUpdate) "
      "SELECT t.tileX, t.tileY, COALESCE(MAX(m.lastUpdate), 0) AS markerLastUpdate, "
      "COALESCE(MAX(r.lastUpdate), 0) AS reviewLastUpdate "
      "FROM tiles t "
      "LEFT JOIN markers m ON m.geohash BETWEEN t.geohashStart AND t.geohashEnd "
      "LEFT JOIN reviews r ON r.markerId = m.Id GROUP BY t.tileX, t.tileY "
      "HAVING markerLastUpdate != 0 OR reviewLastUpdate != 0"};

  TF_assert(aState, SQLITE_DONE == insertTileLastUpdate.tryExecuteStep());
  insertTileLastUpdate.reset();
}  // end of PopulateTileLastUpdateTable

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Add test data to the database.  Values are used by
//!         multiple tests.  Modify with caution.
//!
//----------------------------------------------------------------
void PopulateTranslationsTable(TF_state_type* aState, SQLite::Database& aDatabase) {
  LanguageQuery languageQuery{aDatabase};
  TranslatorQuery translatorQuery{aDatabase};

  std::vector<LanguageTableDataType> languageTableData{LanguageTableDataType{1, "en_US"},
                                                       LanguageTableDataType{2, "pt_BR"}};

  for (auto language : languageTableData) {
    TF_assert(aState, languageQuery.Write(std::move(language)));
  }

  std::vector<TranslationTableDataType> translationTableData{
      TranslationTableDataType{1, 1, "en_US [1]"}, TranslationTableDataType{2, 1, "en_US [2]"},
      TranslationTableDataType{3, 1, "en_US [3]"}, TranslationTableDataType{4, 1, "en_US [4]"},
      TranslationTableDataType{1, 2, "pt_BR [1]"}, TranslationTableDataType{2, 2, "pt_BR [2]"},
  };

  for (auto translation : translationTableData) {
    TF_assert(aState, translatorQuery.Write(std::move(translation)));
  }
}  // end of PopulateTranslationsTable

}  // end of namespace Test
}  // end of namespace Acdb
