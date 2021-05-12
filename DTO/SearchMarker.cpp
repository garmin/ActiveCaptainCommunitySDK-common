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
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "SearchMarker"

#include <map>

#include "DBG_pub.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/SearchMarker.hpp"
#include "Acdb/StringUtil.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
SearchMarker::SearchMarker(const ACDB_marker_idx_type aId, const ACDB_type_type aType,
                           const uint64_t aLastUpdated, std::string&& aName, const int32_t aLat,
                           const int32_t aLon, const MapIconType aMapIcon,
                           ContactData&& aContactData, FuelData&& aFuelData,
                           ReviewStatsData&& aReviewStatsData, std::string&& aLocalizedType)
    : mBaseMarker(aId, aType, aLastUpdated, std::move(aName), aLat, aLon, aMapIcon),
      mContactData(std::move(aContactData)),
      mFuelData(std::move(aFuelData)),
      mReviewStatsData(std::move(aReviewStatsData)),
      mLocalizedType(std::move(aLocalizedType)) {}  // end of SearchMarker::SearchMarker

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
ACDB_marker_idx_type SearchMarker::GetId() const { return mBaseMarker.GetId(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
uint64_t SearchMarker::GetLastUpdated() const { return mBaseMarker.GetLastUpdated(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
MapIconType SearchMarker::GetMapIcon() const { return mBaseMarker.GetMapIcon(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
std::string SearchMarker::GetName() const { return mBaseMarker.GetName(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
scposn_type SearchMarker::GetPosition() const { return mBaseMarker.GetPosition(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
ACDB_type_type SearchMarker::GetType() const { return mBaseMarker.GetType(); }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the info as provided by the base marker
//!
//----------------------------------------------------------------
std::string SearchMarker::ToString() const {
  return String::Format(
      "%s, Phone: %s , VHF: %s , GasPrice: %.1f , DieselPrice: %.1f , Rating: %.1f",
      mBaseMarker.ToString().c_str(), mContactData.mPhoneNumber.c_str(),
      mContactData.mVhfChannel.c_str(), mFuelData.mGasPrice, mFuelData.mDieselPrice,
      mReviewStatsData.mAverageRating);
}

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's type as localized string
//!
//----------------------------------------------------------------
std::string SearchMarker::GetLocalizedType() const { return mLocalizedType; }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's average rating
//!  returns a negative value, if no rating is available
//!
//----------------------------------------------------------------
float SearchMarker::GetAverageRating() const { return mReviewStatsData.mAverageRating; }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's total number of ratings
//!
//----------------------------------------------------------------
uint32_t SearchMarker::GetNumberOfReviews() const { return mReviewStatsData.mNumberOfReviews; }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's phone number
//!
//----------------------------------------------------------------
std::string SearchMarker::GetPhoneNumber() const { return mContactData.mPhoneNumber; }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's VHF channel
//!
//----------------------------------------------------------------
std::string SearchMarker::GetVhfChannel() const { return mContactData.mVhfChannel; }

//----------------------------------------------------------------
//!
//!  @public
//!  @brief Accessor
//!
//!  @return the marker's price for a certain fuel type
//!
//----------------------------------------------------------------
bool SearchMarker::GetFuelPriceInfo(FuelType aFuelType, float& aPrice, std::string& aCurrency,
                                    FuelPriceUnit& aPriceUnit) const {
  aCurrency = mFuelData.mFuelPriceCurrency;

  // convert fuel price unit from raw value to enum
  switch (mFuelData.mFuelPriceUnit) {
    case ACDB_GALLON:
      aPriceUnit = ISearchMarker::FuelPriceUnit::PerGallon;
      break;

    default:
      aPriceUnit = ISearchMarker::FuelPriceUnit::PerLiter;
  }

  switch (aFuelType) {
    case ISearchMarker::FuelType::Gas:
      aPrice = mFuelData.mGasPrice;
      return (aPrice > 0);

    case ISearchMarker::FuelType::Diesel:
      aPrice = mFuelData.mDieselPrice;
      return (aPrice > 0);

    default:
      return false;
  }
}

}  // end of namespace Acdb
