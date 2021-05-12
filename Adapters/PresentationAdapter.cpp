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
    @brief Encapsulates all presentation operations

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "PresentationAdapter"

#include <vector>

#include "DBG_pub.h"
#include "Acdb/PresentationAdapter.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/Presentation/PresentationMarkerFactory.hpp"
#include "Acdb/SectionType.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
PresentationAdapter::PresentationAdapter(SQLite::Database& aDatabase)
    : mAddress{aDatabase},
      mAmenities{aDatabase},
      mBusiness{aDatabase},
      mBusinessPhoto{aDatabase},
      mBusinessProgram{aDatabase},
      mCompetitor{aDatabase},
      mContact{aDatabase},
      mDockage{aDatabase},
      mFuel{aDatabase},
      mMarker{aDatabase},
      mMarkerMeta{aDatabase},
      mMoorings{aDatabase},
      mMustacheTemplate{aDatabase},
      mNavigation{aDatabase},
      mPosition{aDatabase},
      mRetail{aDatabase},
      mReview{aDatabase},
      mReviewPhoto{aDatabase},
      mReviewSummary{aDatabase},
      mServices{aDatabase} {}  // End of PresentationAdapter

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to address object
//!
//----------------------------------------------------------------
Presentation::AddressPtr PresentationAdapter::GetAddress(const ACDB_marker_idx_type aIdx,
                                                         const bool aIsRequired) {
  Presentation::AddressPtr address = nullptr;
  AddressTableDataType addressTableData;
  if (mAddress.Get(aIdx, addressTableData)) {
    address = Acdb::Presentation::GetAddress(aIdx, addressTableData);
  } else if (aIsRequired) {
    addressTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::AddressTitle);
    address = Acdb::Presentation::GetAddress(aIdx, addressTableData);
  }

  return address;
}  // end of GetAddress

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to amenities object
//!
//----------------------------------------------------------------
Presentation::AmenitiesPtr PresentationAdapter::GetAmenities(const ACDB_marker_idx_type aIdx,
                                                             const bool aIsRequired) {
  Presentation::AmenitiesPtr amenities = nullptr;
  AmenitiesTableDataType amenitiesTableData;
  if (mAmenities.Get(aIdx, amenitiesTableData)) {
    amenities = Acdb::Presentation::GetAmenities(aIdx, amenitiesTableData);
  } else if (aIsRequired) {
    amenitiesTableData.mSectionTitle =
        static_cast<ACDB_text_handle_type>(TextHandle::AmenitiesTitle);
    amenities = Acdb::Presentation::GetAmenities(aIdx, amenitiesTableData);
  }

  return amenities;
}  // end of GetAmenities

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to business object
//!
//----------------------------------------------------------------
Presentation::BusinessPtr PresentationAdapter::GetBusiness(const ACDB_marker_idx_type aIdx,
                                                           const bool aIsRequired) {
  Presentation::BusinessPtr business = nullptr;
  BusinessTableDataType businessTableData;
  if (mBusiness.Get(aIdx, businessTableData)) {
    business = Acdb::Presentation::GetBusiness(aIdx, businessTableData);
  } else if (aIsRequired) {
    businessTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::BusinessTitle);
    business = Acdb::Presentation::GetBusiness(aIdx, businessTableData);
  }

  return business;
}  // end of GetBusiness

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns pointer to business object
//!
//----------------------------------------------------------------
Presentation::BusinessPhotoListPtr PresentationAdapter::GetBusinessPhotoList(
    const ACDB_marker_idx_type aIdx) {
  Presentation::BusinessPhotoListPtr businessPhotoList = nullptr;

  std::vector<BusinessPhotoTableDataType> businessPhotoTableData;

  if (mBusinessPhoto.Get(aIdx, businessPhotoTableData)) {
    businessPhotoList = Presentation::GetBusinessPhotoList(aIdx, std::move(businessPhotoTableData));
  }

  return businessPhotoList;
}  // end of GetBusinessPhotoList

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to competitor ad object
//!
//----------------------------------------------------------------
Presentation::CompetitorAdPtr PresentationAdapter::GetCompetitorAd(
    const ACDB_marker_idx_type aIdx,
    const BusinessProgramTableDataType& aBusinessProgramTableData) {
  Presentation::CompetitorAdPtr competitorAd = nullptr;

  // If this marker is a premier participant, it cannot be advertised on.
  if (aBusinessProgramTableData.mProgramTier < PremierProgramTier) {
    std::vector<ACDB_marker_idx_type> potentialAdvertisers;
    std::vector<AdvertiserTableDataCollection> advertisers;

    // Determine if other businesses have selected this marker as a competitor.
    // This call intentionally returns results in a random order.
    if (mCompetitor.GetPotentialAdvertisers(aIdx, potentialAdvertisers)) {
      // Determine which advertisers are eligible to advertise on this marker.
      // Only eligible if this marker is in the advertiser's top 5 non-premier
      // competitors.
      for (auto it = potentialAdvertisers.begin();
           advertisers.size() < MaxCompetitorAds && it != potentialAdvertisers.end(); ++it) {
        bool canTarget(false);

        if (mCompetitor.GetCanTarget(aIdx, *it, canTarget) && canTarget) {
          AdvertiserTableDataCollection advertiserTableData;

          if (mBusinessProgram.Get(*it, advertiserTableData.mBusinessProgram) &&
              mMarker.Get(*it, advertiserTableData.mMarker) &&
              mReviewSummary.Get(*it, advertiserTableData.mReviewSummary)) {
            advertisers.push_back(advertiserTableData);
          }
        }
      }
    }

    if (!advertisers.empty()) {
      competitorAd = Acdb::Presentation::GetCompetitorAd(aIdx, std::move(advertisers));
    }
  }

  return competitorAd;
}  // end of GetCompetitorAd

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to contact object
//!
//----------------------------------------------------------------
Presentation::ContactPtr PresentationAdapter::GetContact(const ACDB_marker_idx_type aIdx,
                                                         const bool aIsRequired) {
  Presentation::ContactPtr contact = nullptr;
  ContactTableDataType contactTableData;
  if (mContact.Get(aIdx, contactTableData)) {
    contact = Acdb::Presentation::GetContact(aIdx, contactTableData);
  } else if (aIsRequired) {
    contactTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::ContactTitle);
    contact = Acdb::Presentation::GetContact(aIdx, contactTableData);
  }

  return contact;
}  // end of GetContact

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to dockage object
//!
//----------------------------------------------------------------
Presentation::DockagePtr PresentationAdapter::GetDockage(const ACDB_marker_idx_type aIdx,
                                                         const bool aIsRequired) {
  Presentation::DockagePtr dockage = nullptr;
  DockageTableDataType dockageTableData;
  if (mDockage.Get(aIdx, dockageTableData)) {
    dockage = Acdb::Presentation::GetDockage(aIdx, dockageTableData);
  } else if (aIsRequired) {
    dockageTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::DockageTitle);
    dockage = Acdb::Presentation::GetDockage(aIdx, dockageTableData);
  }

  return dockage;
}  // end of GetDockage

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to fuel object
//!
//----------------------------------------------------------------
Presentation::FuelPtr PresentationAdapter::GetFuel(const ACDB_marker_idx_type aIdx,
                                                   const bool aIsRequired) {
  Presentation::FuelPtr fuel = nullptr;
  FuelTableDataType fuelTableData;
  if (mFuel.Get(aIdx, fuelTableData)) {
    fuel = Acdb::Presentation::GetFuel(aIdx, fuelTableData);
  } else if (aIsRequired) {
    fuelTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::FuelTitle);
    fuel = Acdb::Presentation::GetFuel(aIdx, fuelTableData);
  }

  return fuel;
}  // end of GetFuel

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Marker.  This contains all of the
//!       presentation data for the marker.
//!
//----------------------------------------------------------------
Presentation::PresentationMarkerPtr PresentationAdapter::GetMarker(
    const ACDB_marker_idx_type aIdx, const std::string& aCaptainName) {
  Presentation::PresentationMarkerPtr presentationMarker = nullptr;

  MarkerTableDataType markerTableData;
  MarkerMetaTableDataType markerMetaTableData;
  ReviewSummaryTableDataType reviewSummaryTableData;
  if (mMarker.Get(aIdx, markerTableData) && mMarkerMeta.Get(aIdx, markerMetaTableData) &&
      mReviewSummary.Get(aIdx, reviewSummaryTableData)) {
    SectionType requiredSections = SectionType::GetRequiredSections(markerTableData.mType);

    std::vector<BusinessPhotoTableDataType> businessPhotoTableData;
    mBusinessPhoto.Get(aIdx, businessPhotoTableData);  // can be empty

    BusinessProgramTableDataType businessProgramTableData;
    if (!mBusinessProgram.Get(aIdx, businessProgramTableData)) {
      businessProgramTableData =
          BusinessProgramTableDataType();  // Ensure we are using default values.
    }

    presentationMarker.reset(new Presentation::PresentationMarker(
        aIdx,
        Acdb::Presentation::GetMarkerDetail(aIdx, markerTableData, markerMetaTableData,
                                            reviewSummaryTableData, businessPhotoTableData),
        GetAddress(aIdx, IsSectionRequired(requiredSections, SectionType::Address)),
        GetAmenities(aIdx, IsSectionRequired(requiredSections, SectionType::Amenities)),
        GetBusiness(aIdx, IsSectionRequired(requiredSections, SectionType::Business)),
        GetCompetitorAd(aIdx, businessProgramTableData),
        GetContact(aIdx, IsSectionRequired(requiredSections, SectionType::Contact)),
        GetDockage(aIdx, IsSectionRequired(requiredSections, SectionType::Dockage)),
        GetFuel(aIdx, IsSectionRequired(requiredSections, SectionType::Fuel)),
        GetMoorings(aIdx, IsSectionRequired(requiredSections, SectionType::Moorings)),
        GetNavigation(aIdx, IsSectionRequired(requiredSections, SectionType::Navigation)),
        GetRetail(aIdx, IsSectionRequired(requiredSections, SectionType::Retail)),
        GetReviewDetail(aIdx, markerTableData.mType, reviewSummaryTableData,
                        IsSectionRequired(requiredSections, SectionType::ReviewDetail),
                        aCaptainName),
        GetServices(aIdx, IsSectionRequired(requiredSections, SectionType::Services))));
  }

  return presentationMarker;
}  // end of GetMarker

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to moorings object
//!
//----------------------------------------------------------------
Presentation::MooringsPtr PresentationAdapter::GetMoorings(const ACDB_marker_idx_type aIdx,
                                                           const bool aIsRequired) {
  Presentation::MooringsPtr moorings = nullptr;
  MooringsTableDataType mooringsTableData;
  if (mMoorings.Get(aIdx, mooringsTableData)) {
    moorings = Acdb::Presentation::GetMoorings(aIdx, mooringsTableData);
  } else if (aIsRequired) {
    mooringsTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::MooringsTitle);
    moorings = Acdb::Presentation::GetMoorings(aIdx, mooringsTableData);
  }

  return moorings;
}

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to navigation object
//!
//----------------------------------------------------------------
Presentation::NavigationPtr PresentationAdapter::GetNavigation(const ACDB_marker_idx_type aIdx,
                                                               const bool aIsRequired) {
  Presentation::NavigationPtr navigation = nullptr;
  NavigationTableDataType navigationTableData;
  if (mNavigation.Get(aIdx, navigationTableData)) {
    navigation = Acdb::Presentation::GetNavigation(aIdx, navigationTableData);
  } else if (aIsRequired) {
    navigationTableData.mSectionTitle =
        static_cast<ACDB_text_handle_type>(TextHandle::NavigationTitle);
    navigation = Acdb::Presentation::GetNavigation(aIdx, navigationTableData);
  }

  return navigation;
}  // end of GetNavigation

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to retail object
//!
//----------------------------------------------------------------
Presentation::RetailPtr PresentationAdapter::GetRetail(const ACDB_marker_idx_type aIdx,
                                                       const bool aIsRequired) {
  Presentation::RetailPtr retail = nullptr;
  RetailTableDataType retailTableData;
  if (mRetail.Get(aIdx, retailTableData)) {
    retail = Acdb::Presentation::GetRetail(aIdx, retailTableData);
  } else if (aIsRequired) {
    retailTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::RetailTitle);
    retail = Acdb::Presentation::GetRetail(aIdx, retailTableData);
  }

  return retail;
}  // end of GetRetail

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to review detail object
//!
//----------------------------------------------------------------
Presentation::ReviewDetailPtr PresentationAdapter::GetReviewDetail(
    const ACDB_marker_idx_type aIdx, const ACDB_type_type aType,
    const ReviewSummaryTableDataType& aReviewSummaryTableData, const bool aIsRequired,
    const std::string& aCaptainName) {
  Presentation::ReviewDetailPtr reviews = nullptr;

  std::unique_ptr<ReviewTableDataType> reviewTableDataPtr;
  std::vector<ReviewPhotoTableDataType> featuredReviewPhotoTableData;
  reviewTableDataPtr.reset(new ReviewTableDataType());

  if (!mReview.Get(aIdx, *reviewTableDataPtr)) {
    reviewTableDataPtr.reset(nullptr);
  } else {
    mReviewPhoto.Get(reviewTableDataPtr->mId, featuredReviewPhotoTableData);  // can be empty
  }

  if (reviewTableDataPtr || aIsRequired) {
    reviews = Acdb::Presentation::GetReviewDetail(aIdx, std::move(reviewTableDataPtr),
                                                  std::move(featuredReviewPhotoTableData), aType,
                                                  aReviewSummaryTableData, aCaptainName);
  }

  return reviews;
}  // end of GetReviewDetail

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns review list for a specific Marker.  This contains
//!       all of the presentation data for the review list.
//!
//----------------------------------------------------------------
Presentation::ReviewListPtr PresentationAdapter::GetReviewList(const ACDB_marker_idx_type aIdx,
                                                               const int aPageNumber,
                                                               const int aPageSize,
                                                               const std::string& aCaptainName) {
  Presentation::ReviewListPtr reviewList = nullptr;

  std::vector<ReviewTableDataType> reviewTableData;
  MarkerTableDataType markerTableData;
  ReviewSummaryTableDataType reviewSummaryTableData;

  if (mReview.GetList(aIdx, aCaptainName, aPageNumber, aPageSize, reviewTableData) &&
      mMarker.Get(aIdx, markerTableData) && mReviewSummary.Get(aIdx, reviewSummaryTableData)) {
    std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>> reviewPhotoTableDataMap;
    mReviewPhoto.GetListByMarkerId(aIdx, aCaptainName, aPageNumber, aPageSize,
                                   reviewPhotoTableDataMap);

    reviewList = Presentation::GetReviewList(
        aIdx, markerTableData.mType, std::move(reviewTableData), std::move(reviewPhotoTableDataMap),
        std::move(reviewSummaryTableData), aCaptainName, aPageNumber, aPageSize);
  }

  return reviewList;
}  // end of GetReviewList

//----------------------------------------------------------------
//!
//!       @private
//!       @brief accessor
//!
//!       @returns pointer to services object
//!
//----------------------------------------------------------------
Presentation::ServicesPtr PresentationAdapter::GetServices(const ACDB_marker_idx_type aIdx,
                                                           const bool aIsRequired) {
  Presentation::ServicesPtr services = nullptr;
  ServicesTableDataType servicesTableData;
  if (mServices.Get(aIdx, servicesTableData)) {
    services = Acdb::Presentation::GetServices(aIdx, servicesTableData);
  } else if (aIsRequired) {
    servicesTableData.mSectionTitle = static_cast<ACDB_text_handle_type>(TextHandle::ServicesTitle);
    services = Acdb::Presentation::GetServices(aIdx, servicesTableData);
  }

  return services;
}  // end of GetServices

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns a specific Mustache template.
//!
//----------------------------------------------------------------
std::string PresentationAdapter::GetTemplate(const std::string& aName) {
  std::string result;

  mMustacheTemplate.Get(aName, result);

  return result;
}  // end of GetTemplate

//----------------------------------------------------------------
//!
//!       @private
//!       @brief determine if section is required
//!
//----------------------------------------------------------------
inline bool PresentationAdapter::IsSectionRequired(SectionType aRequiredSections,
                                                   SectionType aSectionType) {
  return (aRequiredSections & aSectionType) != SectionType::None;
}  // end of IsSectionRequired
}  // end of namespace Acdb
