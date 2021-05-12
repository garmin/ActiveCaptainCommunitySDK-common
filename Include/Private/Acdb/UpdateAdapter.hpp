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
    Translation layer for updating/deleting marker
    data in the database.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_UpdateAdapter_hpp
#define ACDB_UpdateAdapter_hpp

#include <vector>

#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
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
#include "Acdb/Queries/MustacheTemplateQuery.hpp"
#include "Acdb/Queries/NavigationQuery.hpp"
#include "Acdb/Queries/PositionQuery.hpp"
#include "Acdb/Queries/RetailQuery.hpp"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "Acdb/Queries/ReviewPhotoQuery.hpp"
#include "Acdb/Queries/ServicesQuery.hpp"
#include "Acdb/Queries/TileLastUpdateQuery.hpp"
#include "Acdb/Queries/TilesQuery.hpp"
#include "Acdb/Queries/TranslatorQuery.hpp"

namespace Acdb {
class UpdateAdapter {
 public:
  UpdateAdapter(SQLite::Database& aDatabase);

  bool DeleteTile(const TileXY& aTileXY);

  bool DeleteTileReviews(const TileXY& aTileXY);

  bool UpdateMarkers(std::vector<MarkerTableDataCollection>& aMarkers,
                     uint64_t& aLastUpdateMax_out);

  bool UpdateReviews(std::vector<ReviewTableDataCollection>& aReviews,
                     uint64_t& aLastUpdateMax_out);

  bool UpdateSupportTables(std::vector<LanguageTableDataType>& aLanguages,
                           std::vector<MustacheTemplateTableDataType>& aMustacheTemplates,
                           std::vector<TranslationTableDataType>& aTranslations);

 private:
  AddressQuery mAddress;
  AmenitiesQuery mAmenities;
  BusinessQuery mBusiness;
  BusinessPhotoQuery mBusinessPhoto;
  BusinessProgramQuery mBusinessProgram;
  CompetitorQuery mCompetitor;
  ContactQuery mContact;
  DockageQuery mDockage;
  FuelQuery mFuel;
  LanguageQuery mLanguage;
  MarkerQuery mMarker;
  MarkerMetaQuery mMarkerMeta;
  MooringsQuery mMoorings;
  MustacheTemplateQuery mMustacheTemplate;
  NavigationQuery mNavigation;
  PositionQuery mPosition;
  RetailQuery mRetail;
  ReviewQuery mReview;
  ReviewPhotoQuery mReviewPhoto;
  ServicesQuery mServices;
  TileLastUpdateQuery mTileLastUpdate;
  TilesQuery mTiles;
  TranslatorQuery mTranslator;

};  // end of class UpdateAdapter
}  // end of namespace Acdb

#endif  // end of ACDB_UpdateAdapter_hpp
