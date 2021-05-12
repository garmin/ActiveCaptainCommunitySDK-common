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
    generating ActiveCaptain objects

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MarkerFactory"

#include <memory>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/MapIconType.hpp"
#include "Acdb/MarkerFactory.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/TextTranslator.hpp"

namespace Acdb {
static MapIconType GetMapIcon(ACDB_type_type aMarkerType, int aBusinessProgramTier);

static constexpr int MinBusinessProgramIconTier =
    2;  // Program tiers 2 and above receive special icons.

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Get text handle for specified marker type
//!
//!   @return text handle
//!
//----------------------------------------------------------------
TextHandle GetMarkerTypeTextHandle(ACDB_type_type aMarkerType) {
  static std::unordered_map<ACDB_type_type, TextHandle> markerTypeTextHandles(
      {{ACDB_ANCHORAGE, TextHandle::AnchorageValue},
       {ACDB_HAZARD, TextHandle::HazardValue},
       {ACDB_MARINA, TextHandle::MarinaValue},
       {ACDB_BOAT_RAMP, TextHandle::BoatRampValue},
       {ACDB_BUSINESS, TextHandle::BusinessValue},
       {ACDB_INLET, TextHandle::InletValue},
       {ACDB_BRIDGE, TextHandle::BridgeValue},
       {ACDB_LOCK, TextHandle::LockValue},
       {ACDB_DAM, TextHandle::DamValue},
       {ACDB_FERRY, TextHandle::FerryValue},
       {ACDB_AIRPORT, TextHandle::AirportValue}});

  TextHandle markerTypeTextHandle = TextHandle::UnknownValue;
  auto it = markerTypeTextHandles.find(aMarkerType);

  if (it != markerTypeTextHandles.end()) {
    markerTypeTextHandle = it->second;
  }

  return markerTypeTextHandle;
}  // end of GetMarkerTypeTextHandle

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Creates the correct specific Marker child
//!         class based on the passed in information.
//!         The marker returned by this function is not
//!         capable of generating a full content view.
//!   @return pointer to a MapMarker object
//!
//----------------------------------------------------------------
MapMarkerPtr GetMapMarker(MarkerTableDataType& aMarkerData) {
  MapIconType mapIcon = GetMapIcon(aMarkerData.mType, aMarkerData.mBusinessProgramTier);
  auto marker = MapMarkerPtr(new MapMarker(aMarkerData.mId, aMarkerData.mType,
                                           aMarkerData.mLastUpdated, std::move(aMarkerData.mName),
                                           aMarkerData.mPosn.lat, aMarkerData.mPosn.lon, mapIcon));

  return marker;
}  // End of GetMapMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Creates the correct specific Marker child
//!         class based on the passed in information.
//!         The marker returned by this function is not
//!         capable of generating a full content view.
//!   @return pointer to a SearchMarker object
//!
//----------------------------------------------------------------
SearchMarkerPtr GetSearchMarker(ExtendedMarkerDataType& aMarkerData) {
  MapIconType mapIcon = GetMapIcon(aMarkerData.mType, aMarkerData.mBusinessProgramTier);

  TextHandle markerTypeTextHandle = GetMarkerTypeTextHandle(aMarkerData.mType);
  std::string localizedType =
      TextTranslator::GetInstance().Find(static_cast<ACDB_text_handle_type>(markerTypeTextHandle));

  auto marker = SearchMarkerPtr(new SearchMarker(
      aMarkerData.mId, aMarkerData.mType, aMarkerData.mLastUpdated, std::move(aMarkerData.mName),
      aMarkerData.mPosn.lat, aMarkerData.mPosn.lon, mapIcon, std::move(aMarkerData.mContactData),
      std::move(aMarkerData.mFuelData), std::move(aMarkerData.mReviewStatsData),
      std::move(localizedType)));

  return marker;
}  // End of GetSearchMarker

//----------------------------------------------------------------
//!
//!   @private
//!   @detail
//!         Determine map icon for the given marker object.
//!   @return MapIconType object
//!
//----------------------------------------------------------------
static MapIconType GetMapIcon(ACDB_type_type aMarkerType, int aBusinessProgramTier) {
  static const std::map<ACDB_type_type, MapIconType> markerTypeDefaultMapIcons = {
      {ACDB_UNKNOWN_TYPE, MapIconType::Unknown},
      {ACDB_ANCHORAGE, MapIconType::Anchorage},
      {ACDB_HAZARD, MapIconType::Hazard},
      {ACDB_MARINA, MapIconType::Marina},
      {ACDB_BOAT_RAMP, MapIconType::BoatRamp},
      {ACDB_BUSINESS, MapIconType::Business},
      {ACDB_INLET, MapIconType::Inlet},
      {ACDB_BRIDGE, MapIconType::Bridge},
      {ACDB_LOCK, MapIconType::Lock},
      {ACDB_DAM, MapIconType::Dam},
      {ACDB_FERRY, MapIconType::Ferry},
      {ACDB_AIRPORT, MapIconType::Airport}};

  MapIconType mapIconType = MapIconType::Unknown;

  auto defaultMapIconIter = markerTypeDefaultMapIcons.find(aMarkerType);
  if (defaultMapIconIter != markerTypeDefaultMapIcons.end()) {
    mapIconType = defaultMapIconIter->second;
  }

  static const std::map<ACDB_type_type, MapIconType> markerTypeBusinessProgramMapIcons = {
      {ACDB_ANCHORAGE, MapIconType::AnchorageSponsor},
      {ACDB_BUSINESS, MapIconType::BusinessSponsor},
      {ACDB_MARINA, MapIconType::MarinaSponsor}};

  if (aBusinessProgramTier >= MinBusinessProgramIconTier) {
    auto businessProgramMapIconIter = markerTypeBusinessProgramMapIcons.find(aMarkerType);
    if (businessProgramMapIconIter != markerTypeBusinessProgramMapIcons.end()) {
      mapIconType = businessProgramMapIconIter->second;
    }
  }

  return mapIconType;

}  // End of GetMapIcon

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Get text handle for specified yes/no/unknown/nearby value
//!
//!   @return text handle
//!
//----------------------------------------------------------------
TextHandle GetYesNoUnknownNearbyTextHandle(const std::string& aYnubValue) {
  static std::unordered_map<std::string, TextHandle> yesNoUnknownNearbyTextHandles = {
      {"yes", TextHandle::YesValue},
      {"no", TextHandle::NoValue},
      {"nearby", TextHandle::NearbyValue}};

  TextHandle textHandle = TextHandle::UnknownValue;

  auto it = yesNoUnknownNearbyTextHandles.find(String::ToLower(aYnubValue));

  if (it != yesNoUnknownNearbyTextHandles.end()) {
    textHandle = it->second;
  }

  return textHandle;
}  // end of GetYesNoUnknownNearbyTextHandle
}  // end of namespace Acdb
