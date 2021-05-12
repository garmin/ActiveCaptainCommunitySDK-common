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

#ifndef ACDB_ISearchMarker_hpp
#define ACDB_ISearchMarker_hpp

#include "Acdb/IMarker.hpp"

namespace Acdb {
//! SearchMarker represents a result from an search,
//! adding a few search related extra details to the normal IMarker
class ISearchMarker : public IMarker {
 public:
  // types
  enum class FuelType { Gas, Diesel };

  // types
  enum class FuelPriceUnit { PerLiter, PerGallon };

  // public functions
  virtual std::string GetLocalizedType() const = 0;

  virtual float GetAverageRating() const = 0;

  virtual uint32_t GetNumberOfReviews() const = 0;

  virtual std::string GetPhoneNumber() const = 0;

  virtual std::string GetVhfChannel() const = 0;

  virtual bool GetFuelPriceInfo(FuelType aFuelType, float& aPrice, std::string& aCurrency,
                                FuelPriceUnit& aPriceUnit) const = 0;

  virtual ~ISearchMarker() = default;

};  // end of class ISearchMarker
}  // end of namespace Acdb

#endif  // end of ACDB_ISearchMarker_hpp
