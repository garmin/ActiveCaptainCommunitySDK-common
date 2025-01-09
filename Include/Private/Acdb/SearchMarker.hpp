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

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_SearchMarker_hpp
#define ACDB_SearchMarker_hpp

#include "Acdb/ISearchMarker.hpp"
#include "Acdb/MapMarker.hpp"

namespace Acdb {
//! SearchMarker represents a point from the ActiveCaptain database
class SearchMarker : public ISearchMarker {
 public:
  SearchMarker(const ACDB_marker_idx_type aId, const ACDB_type_type aType,
               const uint64_t aLastUpdated, std::string&& aName, const int32_t aLat,
               const int32_t aLon, const MapIconType aMapIcon, std::string&& aLocalizedType);

  SearchMarker(const ACDB_marker_idx_type aId, const ACDB_type_type aType,
               const uint64_t aLastUpdated, std::string&& aName, const int32_t aLat,
               const int32_t aLon, const MapIconType aMapIcon, ContactData&& aContactData,
               FuelData&& aFuelData, ReviewStatsData&& aReviewStatsData,
               std::string&& aLocalizedType);

  ACDB_marker_idx_type GetId() const override;

  uint64_t GetLastUpdated() const override;

  MapIconType GetMapIcon() const override;

  std::string GetName() const override;

  scposn_type GetPosition() const override;

  ACDB_type_type GetType() const override;

  std::string ToString() const override;

  std::string GetLocalizedType() const override;

  float GetAverageRating() const override;

  uint32_t GetNumberOfReviews() const override;

  std::string GetPhoneNumber() const override;

  std::string GetVhfChannel() const override;

  bool GetFuelPriceInfo(FuelType aFuelType, float& aPrice, std::string& aCurrency,
                        FuelPriceUnit& aPriceUnit) const override;

  virtual ~SearchMarker() = default;

 private:
  // private variables
  MapMarker mBaseMarker;
  ContactData mContactData;
  FuelData mFuelData;
  ReviewStatsData mReviewStatsData;
  std::string mLocalizedType;
};  // end of class SearchMarker
}  // end of namespace Acdb

#endif  // end of ACDB_SearchMarker_hpp
