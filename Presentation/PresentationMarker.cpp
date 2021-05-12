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
    @brief contains functionality related to
    Active Captain markers.

    Copyright 2017-2018 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "PresentationMarker"

#include <map>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "GRM_pub_lib.h"

#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
PresentationMarker::PresentationMarker(const ACDB_marker_idx_type aId, MarkerDetail&& aMarkerDetail,
                                       AddressPtr aAddress, AmenitiesPtr aAmenities,
                                       BusinessPtr aBusiness, CompetitorAdPtr aCompetitorAd,
                                       ContactPtr aContact, DockagePtr aDockage, FuelPtr aFuel,
                                       MooringsPtr aMoorings, NavigationPtr aNavigation,
                                       RetailPtr aRetail, ReviewDetailPtr aReviewDetail,
                                       ServicesPtr aServices)
    : mId(aId),
      mMarkerDetail(std::move(aMarkerDetail)),
      mAddress(std::move(aAddress)),
      mAmenities(std::move(aAmenities)),
      mBusiness(std::move(aBusiness)),
      mCompetitorAd(std::move(aCompetitorAd)),
      mContact(std::move(aContact)),
      mDockage(std::move(aDockage)),
      mFuel(std::move(aFuel)),
      mMoorings(std::move(aMoorings)),
      mNavigation(std::move(aNavigation)),
      mRetail(std::move(aRetail)),
      mReviewDetail(std::move(aReviewDetail)),
      mServices(std::move(aServices)) {}  // end of PresentationMarker::PresentationMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return copy of the marker id
//!
//----------------------------------------------------------------
ACDB_marker_idx_type PresentationMarker::GetId() const {
  return mId;
}  // end of PresentationMarker::GetId()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to address
//!
//----------------------------------------------------------------
const Address* PresentationMarker::GetAddress() const {
  return mAddress.get();
}  // end of PresentationMarker::GetAddress()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to amenities
//!
//----------------------------------------------------------------
const Amenities* PresentationMarker::GetAmenities() const {
  return mAmenities.get();
}  // end of PresentationMarker::GetAmenities()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to business
//!
//----------------------------------------------------------------
const Business* PresentationMarker::GetBusiness() const {
  return mBusiness.get();
}  // end of PresentationMarker::GetBusiness()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to competitorAd
//!
//----------------------------------------------------------------
const CompetitorAd* PresentationMarker::GetCompetitorAd() const {
  return mCompetitorAd.get();
}  // end of PresentationMarker::GetCompetitorAd()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to contact
//!
//----------------------------------------------------------------
const Contact* PresentationMarker::GetContact() const {
  return mContact.get();
}  // end of PresentationMarker::GetContact()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to dockage
//!
//----------------------------------------------------------------
const Dockage* PresentationMarker::GetDockage() const {
  return mDockage.get();
}  // end of PresentationMarker::GetDockage()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to fuel
//!
//----------------------------------------------------------------
const Fuel* PresentationMarker::GetFuel() const {
  return mFuel.get();
}  // end of PresentationMarker::GetFuel()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to marker detail
//!
//----------------------------------------------------------------
const MarkerDetail& PresentationMarker::GetMarkerDetail() const {
  return mMarkerDetail;
}  // end of PresentationMarker::GetMarkerDetail()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to moorings
//!
//----------------------------------------------------------------
const Moorings* PresentationMarker::GetMoorings() const {
  return mMoorings.get();
}  // end of PresentationMarker::GetMoorings()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to navigation
//!
//----------------------------------------------------------------
const Navigation* PresentationMarker::GetNavigation() const {
  return mNavigation.get();
}  // end of PresentationMarker::GetRetail()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to retail
//!
//----------------------------------------------------------------
const Retail* PresentationMarker::GetRetail() const {
  return mRetail.get();
}  // end of PresentationMarker::GetRetail()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to review detail
//!
//----------------------------------------------------------------
const ReviewDetail* PresentationMarker::GetReviewDetail() const {
  return mReviewDetail.get();
}  // end of PresentationMarker::GetReviewDetail()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const pointer to services
//!
//----------------------------------------------------------------
const Services* PresentationMarker::GetServices() const {
  return mServices.get();
}  // end of PresentationMarker::GetServices()

}  // end of namespace Presentation
}  // end of namespace Acdb
