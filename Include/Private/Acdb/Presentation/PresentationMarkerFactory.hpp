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
    Generates Marker objects.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_PresentationMarkerFactory_hpp
#define ACDB_PresentationMarkerFactory_hpp

#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"

namespace Acdb {
struct AdvertiserTableDataCollection {
  BusinessProgramTableDataType mBusinessProgram;
  MarkerTableDataType mMarker;
  ReviewSummaryTableDataType mReviewSummary;
};

namespace Presentation {
AddressPtr GetAddress(const ACDB_marker_idx_type aIdx,
                      const AddressTableDataType& aAddressTableData);

AmenitiesPtr GetAmenities(const ACDB_marker_idx_type aIdx,
                          const AmenitiesTableDataType& aAmenitiesTableData);

BusinessPtr GetBusiness(const ACDB_marker_idx_type aIdx,
                        const BusinessTableDataType& aBusinessTableData);

BusinessPhotoListPtr GetBusinessPhotoList(
    const ACDB_marker_idx_type aIdx,
    std::vector<BusinessPhotoTableDataType>&& aBusinessPhotoTableData);

CompetitorAdPtr GetCompetitorAd(
    const ACDB_marker_idx_type aIdx,
    std::vector<AdvertiserTableDataCollection>&& aBusinessProgramTableData);

ContactPtr GetContact(const ACDB_marker_idx_type aIdx,
                      const ContactTableDataType& aContactTableData);

DockagePtr GetDockage(const ACDB_marker_idx_type aIdx,
                      const DockageTableDataType& aDockageTableData);

FuelPtr GetFuel(const ACDB_marker_idx_type aIdx, const FuelTableDataType& aFuelTableData);

MarkerDetail GetMarkerDetail(const ACDB_marker_idx_type aIdx,
                             const MarkerTableDataType& aMarkerTableData,
                             const MarkerMetaTableDataType& aMarkerMetaTableData,
                             const ReviewSummaryTableDataType& aReviewSummaryTableData,
                             std::vector<BusinessPhotoTableDataType>& aBusinessPhotoTableData);

MooringsPtr GetMoorings(const ACDB_marker_idx_type aIdx,
                        const MooringsTableDataType& aMooringsTableData);

NavigationPtr GetNavigation(const ACDB_marker_idx_type aIdx,
                            const NavigationTableDataType& aNavigationTableData);

RetailPtr GetRetail(const ACDB_marker_idx_type aIdx, const RetailTableDataType& aRetailTableData);

ReviewDetailPtr GetReviewDetail(const ACDB_marker_idx_type aIdx,
                                std::unique_ptr<ReviewTableDataType> aFeaturedReviewTableData,
                                std::vector<ReviewPhotoTableDataType>&& aReviewPhotoTableData,
                                const ACDB_type_type aType,
                                const ReviewSummaryTableDataType& aReviewSummaryTableData,
                                const std::string& aCaptainName = std::string());

ReviewListPtr GetReviewList(const ACDB_marker_idx_type aIdx, const ACDB_type_type aType,
                            std::vector<ReviewTableDataType>&& aReviewTableData,
                            std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>>&&
                                aReviewPhotoTableDataMap,
                            ReviewSummaryTableDataType&& aReviewSummaryTableData,
                            const std::string& aCaptainName, const int aPageNumber,
                            const int aPageSize);

ReviewSummaryPtr GetReviewSummary(const ReviewSummaryTableDataType& aReviewSummaryData,
                                  const ACDB_type_type aType);

ServicesPtr GetServices(const ACDB_marker_idx_type aIdx,
                        const ServicesTableDataType& aServicesTableData);

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_PresentationMarkerFactory_hpp
