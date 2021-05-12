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
    @brief Contains functionality related to
    ActiveCaptain markers.

    Copyright 2017-2018 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_PresentationMarker_hpp
#define ACDB_PresentationMarker_hpp

#include <vector>

#include "ACDB_pub_types.h"
#include "Acdb/Presentation/IPresentationMarker.hpp"
#include "Acdb/Presentation/Section/Address.hpp"
#include "Acdb/Presentation/Section/Amenities.hpp"
#include "Acdb/Presentation/Section/Business.hpp"
#include "Acdb/Presentation/Section/CompetitorAd.hpp"
#include "Acdb/Presentation/Section/Contact.hpp"
#include "Acdb/Presentation/Section/Dockage.hpp"
#include "Acdb/Presentation/Section/Fuel.hpp"
#include "Acdb/Presentation/Section/MarkerDetail.hpp"
#include "Acdb/Presentation/Section/Moorings.hpp"
#include "Acdb/Presentation/Section/Navigation.hpp"
#include "Acdb/Presentation/Section/Retail.hpp"
#include "Acdb/Presentation/Section/ReviewDetail.hpp"
#include "Acdb/Presentation/Section/Services.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

//! Marker represents a point from the ActiveCaptain database
class PresentationMarker : public IPresentationMarker {
 public:
  // public functions
  PresentationMarker(const ACDB_marker_idx_type aId, MarkerDetail&& aMarkerDetail,
                     AddressPtr aAddress, AmenitiesPtr aAmenities, BusinessPtr aBusiness,
                     CompetitorAdPtr aCompetitorAd, ContactPtr aContact, DockagePtr aDockage,
                     FuelPtr aFuel, MooringsPtr aMoorings, NavigationPtr aNavigation,
                     RetailPtr aRetail, ReviewDetailPtr aReviewDetail, ServicesPtr aServices);

  ACDB_marker_idx_type GetId() const;

  const Address* GetAddress() const;

  const Amenities* GetAmenities() const;

  const Business* GetBusiness() const;

  const CompetitorAd* GetCompetitorAd() const;

  const Contact* GetContact() const;

  const Dockage* GetDockage() const;

  const Fuel* GetFuel() const;

  const MarkerDetail& GetMarkerDetail() const;

  const Moorings* GetMoorings() const;

  const Navigation* GetNavigation() const;

  const Retail* GetRetail() const;

  const ReviewDetail* GetReviewDetail() const;

  const Services* GetServices() const;

 private:
  ACDB_marker_idx_type mId;
  MarkerDetail mMarkerDetail;
  AddressPtr mAddress;
  AmenitiesPtr mAmenities;
  BusinessPtr mBusiness;
  CompetitorAdPtr mCompetitorAd;
  ContactPtr mContact;
  DockagePtr mDockage;
  FuelPtr mFuel;
  MooringsPtr mMoorings;
  NavigationPtr mNavigation;
  RetailPtr mRetail;
  ReviewDetailPtr mReviewDetail;
  ServicesPtr mServices;

};  // end of class PresentationMarker

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_PresentationMarker_hpp
