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
    Translation layer for retrieving/storing marker
    data from/to the database.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_PresentationAdapter_hpp
#define ACDB_PresentationAdapter_hpp

#include <vector>

#include "Acdb/Presentation/BusinessPhotoList.hpp"
#include "Acdb/Presentation/ReviewList.hpp"
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
#include "Acdb/Queries/MarkerQuery.hpp"
#include "Acdb/Queries/MarkerMetaQuery.hpp"
#include "Acdb/Queries/MooringsQuery.hpp"
#include "Acdb/Queries/MustacheTemplateQuery.hpp"
#include "Acdb/Queries/NavigationQuery.hpp"
#include "Acdb/Queries/PositionQuery.hpp"
#include "Acdb/Queries/RetailQuery.hpp"
#include "Acdb/Queries/ReviewQuery.hpp"
#include "Acdb/Queries/ReviewPhotoQuery.hpp"
#include "Acdb/Queries/ReviewSummaryQuery.hpp"
#include "Acdb/Queries/ServicesQuery.hpp"
#include "Acdb/SectionType.hpp"

namespace Acdb {
class PresentationAdapter {
 public:
  PresentationAdapter(SQLite::Database& aDatabase);

  Presentation::BusinessPhotoListPtr GetBusinessPhotoList(const ACDB_marker_idx_type aIdx);

  Presentation::PresentationMarkerPtr GetMarker(const ACDB_marker_idx_type aIdx,
                                                const std::string& aCaptainName = std::string());

  Presentation::ReviewListPtr GetReviewList(const ACDB_marker_idx_type aIdx, const int aPageNumber,
                                            const int aPageSize,
                                            const std::string& aCaptainName = std::string());

  std::string GetTemplate(const std::string& aName);

 private:
  static constexpr int PremierProgramTier = 3;
  static constexpr int MaxCompetitorAds = 2;

  Presentation::AddressPtr GetAddress(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::AmenitiesPtr GetAmenities(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::BusinessPtr GetBusiness(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::CompetitorAdPtr GetCompetitorAd(
      const ACDB_marker_idx_type aIdx,
      const BusinessProgramTableDataType& aBusinessProgramTableData);

  Presentation::ContactPtr GetContact(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::DockagePtr GetDockage(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::FuelPtr GetFuel(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::MooringsPtr GetMoorings(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::NavigationPtr GetNavigation(const ACDB_marker_idx_type aIdx,
                                            const bool aIsRequired);

  Presentation::RetailPtr GetRetail(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  Presentation::ReviewDetailPtr GetReviewDetail(
      const ACDB_marker_idx_type aIdx, const ACDB_type_type aType,
      const ReviewSummaryTableDataType& aReviewSummaryTableData, const bool aIsRequired,
      const std::string& aCaptainName);

  Presentation::ServicesPtr GetServices(const ACDB_marker_idx_type aIdx, const bool aIsRequired);

  bool IsSectionRequired(SectionType aRequiredSections, SectionType aSectionType);

  AddressQuery mAddress;
  AmenitiesQuery mAmenities;
  BusinessQuery mBusiness;
  BusinessPhotoQuery mBusinessPhoto;
  BusinessProgramQuery mBusinessProgram;
  CompetitorQuery mCompetitor;
  ContactQuery mContact;
  DockageQuery mDockage;
  FuelQuery mFuel;
  MarkerQuery mMarker;
  MarkerMetaQuery mMarkerMeta;
  MooringsQuery mMoorings;
  MustacheTemplateQuery mMustacheTemplate;
  NavigationQuery mNavigation;
  PositionQuery mPosition;
  RetailQuery mRetail;
  ReviewQuery mReview;
  ReviewPhotoQuery mReviewPhoto;
  ReviewSummaryQuery mReviewSummary;
  ServicesQuery mServices;

};  // end of class PresentationAdapter
}  // end of namespace Acdb

#endif  // end of ACDB_PresentationAdapter_hpp
