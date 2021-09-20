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
Definition of the MarkerParser.

Copyright 2018-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MarkerParser"

#include <map>
#include <string>
#include <vector>

#include "DBG_pub.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/Json/JsonParser.hpp"
#include "Acdb/Json/MarkerParser.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "rapidjson/document.h"
#include "UTL_pub_lib_cnvt.h"

namespace Acdb {
namespace Json {

static bool GetMapLocation(const rapidjson::Value& aDocument, const char* aNodeName,
                           scposn_type& aOutput);

static bool ParseSingleMarkerFromApiResponse(const char* aJson, size_t aLength,
                                             MarkerTableDataCollection& aMarker_out);

static bool CheckMarkerDeleted(const rapidjson::Value& aDocument, const char* aNodeName,
                               bool& aOutput);

static bool GetMarkerType(const rapidjson::Value& aDocument, const char* aNodeName,
                          ACDB_type_type& aOutput);

static bool GetUnitType(const rapidjson::Value& aDocument, const char* aNodeName,
                        ACDB_type_type& aOutput);

static void ParseAddress(const rapidjson::Value& aDocument, AddressTableDataType* aOutput);

static void ParseAmenities(const rapidjson::Value& aDocument, AmenitiesTableDataType* aOutput);

static void ParseBusiness(const rapidjson::Value& aDocument, BusinessTableDataType* aOutput);

static void ParseBusinessPhotos(const rapidjson::Value& aDocument,
                                std::vector<BusinessPhotoTableDataType>& aOutput);

static void ParseBusinessProgram(const rapidjson::Value& aDocument,
                                 BusinessProgramTableDataType* aOutput);

static void ParseCompetitors(const rapidjson::Value& aDocument,
                             std::vector<CompetitorTableDataType>& aOutput);

static void ParseContact(const rapidjson::Value& aDocument, ContactTableDataType* aOutput);

static void ParseDockage(const rapidjson::Value& aDocument, DockageTableDataType* aOutput);

static void ParseFuel(const rapidjson::Value& aDocument, FuelTableDataType* aOutput);

static bool ParseMarker(const rapidjson::Value& aDocument, MarkerTableDataCollection& aOutput);

static void ParseMoorings(const rapidjson::Value& aDocument, MooringsTableDataType* aOutput);

static void ParseNavigation(const rapidjson::Value& aDocument, NavigationTableDataType* aOutput);

static bool ParsePointOfInterest(const rapidjson::Value& aDocument,
                                 MarkerTableDataType* aOutputMarker,
                                 MarkerMetaTableDataType* aOutputMarkerMeta);

static void ParseRetail(const rapidjson::Value& aDocument, RetailTableDataType* aOutput);

static void ParseServices(const rapidjson::Value& aDocument, ServicesTableDataType* aOutput);

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse the create marker response json
//!
//----------------------------------------------------------------
bool ParseCreateMarkerResponse(const char* aJson, size_t aLength,
                               MarkerTableDataCollection& aMarker_out) {
  return ParseSingleMarkerFromApiResponse(aJson, aLength, aMarker_out);
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse marker sync response
//!
//----------------------------------------------------------------
bool ParseMarkerSyncResponse(const char* aJson, size_t aLength,
                             std::vector<MarkerTableDataCollection>& aMarkers_out) {
  rapidjson::Document document;
  document.Parse(aJson, aLength);

  if (!document.IsArray()) {
    return false;
  }

  bool success = true;

  for (auto it = document.GetArray().begin(); success && it != document.GetArray().end(); it++) {
    MarkerTableDataCollection markerTableDataCollection;
    success = success && it->IsObject();
    success = success && ParseMarker(*it, markerTableDataCollection);
    if (success) {
      aMarkers_out.push_back(std::move(markerTableDataCollection));
    }
  }

  return success;
}  // end of ParseMarkerSyncResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse webview response
//!
//----------------------------------------------------------------
bool ParseMarkerWebViewResponse(rapidjson::Document& aJsonDocument,
                                MarkerTableDataCollection& aMarker_out) {
  std::string resultTypeStr;
  auto resultTypeIt = aJsonDocument.FindMember("resultType");
  if (resultTypeIt == aJsonDocument.MemberEnd()) {
    return false;
  }

  resultTypeStr = resultTypeIt->value.GetString();
  resultTypeStr = String::ToUpper(resultTypeStr);

  auto dataIt = aJsonDocument.FindMember("data");
  bool success = (dataIt != aJsonDocument.MemberEnd());

  if (resultTypeStr == "SUCCESS") {
    success = success && ParseMarker(dataIt->value, aMarker_out);
  } else if (resultTypeStr == "DELETE") {
    success = GetUint64(dataIt->value, aMarker_out.mMarker.mId);
    aMarker_out.mIsDeleted = true;
  } else if (resultTypeStr == "ERROR") {
    success = false;
  } else {
    DBG_ASSERT_ALWAYS("Unknown marker webview resultType");
    success = false;
  }

  return success;

}  // end of ParseMarkerWebViewResponse

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Parse the move marker response json
//!
//----------------------------------------------------------------
bool ParseMoveMarkerResponse(const char* aJson, size_t aLength,
                             MarkerTableDataCollection& aMarker_out) {
  return ParseSingleMarkerFromApiResponse(aJson, aLength, aMarker_out);
}

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse a single marker from an API response
//!
//----------------------------------------------------------------
static bool ParseSingleMarkerFromApiResponse(const char* aJson, size_t aLength,
                                             MarkerTableDataCollection& aMarker_out) {
  rapidjson::Document document;
  document.Parse(aJson, aLength);

  if (!document.IsObject()) {
    return false;
  }

  return ParseMarker(document.GetObject(), aMarker_out);
}

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse a marker object
//!
//----------------------------------------------------------------
static bool ParseMarker(const rapidjson::Value& aDocument, MarkerTableDataCollection& aOutput) {
  static const char dateLastModifiedNode[] = "dateLastModified";
  static const char geohashNode[] = "geohashStr";
  static const char idNode[] = "idStr";
  static const char mapLocationNode[] = "mapLocation";
  static const char poiTypeNode[] = "poiType";
  static const char searchFilterNode[] = "searchFilterStr";
  static const char statusNode[] = "status";

  static const char addressNode[] = "address";
  static const char amenitiesNode[] = "amenity";
  static const char businessNode[] = "business";
  static const char businessPhotosNode[] = "businessPhotos";
  static const char businessProgramNode[] = "businessProgram";
  static const char competitorsNode[] = "competitors";
  static const char contactNode[] = "contact";
  static const char dockageNode[] = "dockage";
  static const char fuelNode[] = "fuel";
  static const char mooringsNode[] = "mooring";
  static const char navigationNode[] = "navigation";
  static const char retailNode[] = "retail";
  static const char servicesNode[] = "services";
  static const char pointOfInterestNode[] = "pointOfInterest";

  aOutput.mAddress = nullptr;
  aOutput.mAmenities = nullptr;
  aOutput.mBusiness = nullptr;
  aOutput.mBusinessPhotos.clear();
  aOutput.mBusinessProgram = nullptr;
  aOutput.mCompetitors.clear();
  aOutput.mContact = nullptr;
  aOutput.mDockage = nullptr;
  aOutput.mFuel = nullptr;
  aOutput.mMoorings = nullptr;
  aOutput.mNavigation = nullptr;
  aOutput.mRetail = nullptr;
  aOutput.mServices = nullptr;

  bool success = GetUint64(aDocument, idNode, aOutput.mMarker.mId);
  success =
      success && GetDateTimeEpoch(aDocument, dateLastModifiedNode, aOutput.mMarker.mLastUpdated);
  success = success && GetMarkerType(aDocument, poiTypeNode, aOutput.mMarker.mType);
  success = success && CheckMarkerDeleted(aDocument, statusNode, aOutput.mIsDeleted);

  // Short circuit -- if parsing failed before this point, or if the marker was deleted, skip
  // further processing.
  if (!success || aOutput.mIsDeleted) {
    return success;
  }

  success = success && GetMapLocation(aDocument, mapLocationNode, aOutput.mMarker.mPosn);
  success = success && GetUint64(aDocument, geohashNode, aOutput.mMarker.mGeohash);

  auto pointOfInterestIt = aDocument.FindMember(pointOfInterestNode);
  if (pointOfInterestIt == aDocument.MemberEnd() || !pointOfInterestIt->value.IsObject()) {
    // pointOfInterest node not present in JSON or contains invalid value.
    success = false;
  }

  success = success && ParsePointOfInterest(pointOfInterestIt->value, &(aOutput.mMarker),
                                            &(aOutput.mMarkerMeta));

  // Short circuit -- if parsing failed before this point, skip further processing.
  if (!success) {
    return success;
  }

  if (!GetUint64(aDocument, searchFilterNode, aOutput.mMarker.mSearchFilter)) {
    aOutput.mMarker.mSearchFilter = 0;
  }

  auto addressIt = aDocument.FindMember(addressNode);
  if (addressIt != aDocument.MemberEnd()) {
    aOutput.mAddress.reset(new AddressTableDataType());
    ParseAddress(addressIt->value, aOutput.mAddress.get());
  }

  auto amenitiesIt = aDocument.FindMember(amenitiesNode);
  if (amenitiesIt != aDocument.MemberEnd()) {
    aOutput.mAmenities.reset(new AmenitiesTableDataType());
    ParseAmenities(amenitiesIt->value, aOutput.mAmenities.get());
  }

  auto businessIt = aDocument.FindMember(businessNode);
  if (businessIt != aDocument.MemberEnd()) {
    aOutput.mBusiness.reset(new BusinessTableDataType());
    ParseBusiness(businessIt->value, aOutput.mBusiness.get());
  }

  auto businessPhotosIt = aDocument.FindMember(businessPhotosNode);
  if (businessPhotosIt != aDocument.MemberEnd()) {
    ParseBusinessPhotos(businessPhotosIt->value, aOutput.mBusinessPhotos);
  }

  auto businessProgramIt = aDocument.FindMember(businessProgramNode);
  if (businessProgramIt != aDocument.MemberEnd()) {
    aOutput.mBusinessProgram.reset(new BusinessProgramTableDataType());
    ParseBusinessProgram(businessProgramIt->value, aOutput.mBusinessProgram.get());
  }

  auto competitorIt = aDocument.FindMember(competitorsNode);
  if (competitorIt != aDocument.MemberEnd()) {
    ParseCompetitors(competitorIt->value, aOutput.mCompetitors);
  }

  auto contactIt = aDocument.FindMember(contactNode);
  if (contactIt != aDocument.MemberEnd()) {
    aOutput.mContact.reset(new ContactTableDataType());
    ParseContact(contactIt->value, aOutput.mContact.get());
  }

  auto dockageIt = aDocument.FindMember(dockageNode);
  if (dockageIt != aDocument.MemberEnd()) {
    aOutput.mDockage.reset(new DockageTableDataType());
    ParseDockage(dockageIt->value, aOutput.mDockage.get());
  }

  auto fuelIt = aDocument.FindMember(fuelNode);
  if (fuelIt != aDocument.MemberEnd()) {
    aOutput.mFuel.reset(new FuelTableDataType());
    ParseFuel(fuelIt->value, aOutput.mFuel.get());
  }

  auto mooringsIt = aDocument.FindMember(mooringsNode);
  if (mooringsIt != aDocument.MemberEnd()) {
    aOutput.mMoorings.reset(new MooringsTableDataType());
    ParseMoorings(mooringsIt->value, aOutput.mMoorings.get());
  }

  auto navigationIt = aDocument.FindMember(navigationNode);
  if (navigationIt != aDocument.MemberEnd()) {
    aOutput.mNavigation.reset(new NavigationTableDataType());
    ParseNavigation(navigationIt->value, aOutput.mNavigation.get());
  }

  auto retailIt = aDocument.FindMember(retailNode);
  if (retailIt != aDocument.MemberEnd()) {
    aOutput.mRetail.reset(new RetailTableDataType());
    ParseRetail(retailIt->value, aOutput.mRetail.get());
  }

  auto servicesIt = aDocument.FindMember(servicesNode);
  if (servicesIt != aDocument.MemberEnd()) {
    aOutput.mServices.reset(new ServicesTableDataType());
    ParseServices(servicesIt->value, aOutput.mServices.get());
  }

  return success;
}  // end of ParseMarker

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Get JSON node value as an scposn_type.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
static bool GetMapLocation(const rapidjson::Value& aDocument, const char* aNodeName,
                           scposn_type& aOutput) {
  static const char latitudeNode[] = "latitude";
  static const char longitudeNode[] = "longitude";

  auto it = aDocument.FindMember(aNodeName);
  if (it == aDocument.MemberEnd() || !it->value.IsObject()) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  double lat;
  double lon;

  bool success;

  success = GetDouble(it->value, latitudeNode, lat);
  success = success && GetDouble(it->value, longitudeNode, lon);

  if (!success) {
    // Lat or lon could not be parsed from JSON.
    return false;
  }

  aOutput.lat = (int32_t)(lat * (double)UTL_DEG_TO_SEMI);
  aOutput.lon = (int32_t)(lon * (double)UTL_DEG_TO_SEMI);

  return true;
}  // end of GetMapLocation

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Check if marker is deleted.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.  aOutput is set accordingly
//!
//----------------------------------------------------------------
static bool CheckMarkerDeleted(const rapidjson::Value& aDocument, const char* aNodeName,
                               bool& aOutput) {
  const std::string deletedStatus = "Deleted";

  std::string status;
  if (!GetString(aDocument, aNodeName, status)) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  aOutput = (status == deletedStatus);

  return true;
}  // end of CheckMarkerDeleted

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Get marker type.
//!   @returns
//!       True if node exists, and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
static bool GetMarkerType(const rapidjson::Value& aDocument, const char* aNodeName,
                          ACDB_type_type& aOutput) {
  static const std::map<std::string, ACDB_type_type> typeMap = {
      {"Unknown", ACDB_UNKNOWN_TYPE},
      {"Anchorage", ACDB_ANCHORAGE},
      {"Hazard", ACDB_HAZARD},
      {"Marina", ACDB_MARINA},
      {"BoatRamp", ACDB_BOAT_RAMP},
      {"Business", ACDB_BUSINESS},
      {"Inlet", ACDB_INLET},
      {"Bridge", ACDB_BRIDGE},
      {"Lock", ACDB_LOCK},
      {"Dam", ACDB_DAM},
      {"Ferry", ACDB_FERRY},
      {"Airport", ACDB_UNKNOWN_TYPE}  // Deprecated type
  };

  std::string typeStr;
  if (!GetString(aDocument, aNodeName, typeStr)) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  auto typeMapIt = typeMap.find(typeStr);
  if (typeMapIt == typeMap.end()) {
    // Unrecognized type.
    aOutput = ACDB_UNKNOWN_TYPE;
    return false;
  }

  aOutput = typeMapIt->second;
  return true;
}  // end of GetMarkerType

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Get unit type.
//!   @returns
//!       True if node exists and contains a valid value, false
//!       otherwise.
//!
//----------------------------------------------------------------
static bool GetUnitType(const rapidjson::Value& aDocument, const char* aNodeName,
                        ACDB_type_type& aOutput) {
  static const std::map<std::string, ACDB_unit_type> unitMap = {{"Unknown", ACDB_UNKNOWN_UNIT},
                                                                {"Feet", ACDB_FEET},
                                                                {"Meter", ACDB_METER},
                                                                {"Gallon", ACDB_GALLON},
                                                                {"Liter", ACDB_LITER}};

  std::string unitStr;
  if (!GetString(aDocument, aNodeName, unitStr)) {
    // Node not present in JSON or contains invalid value.
    return false;
  }

  auto unitMapIt = unitMap.find(unitStr);
  if (unitMapIt == unitMap.end()) {
    // Unrecognized unit.
    aOutput = ACDB_UNKNOWN_UNIT;
    return false;
  }

  aOutput = unitMapIt->second;
  return true;
}  // end of GetMarkerType

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Address section.
//!
//----------------------------------------------------------------
static void ParseAddress(const rapidjson::Value& aDocument, AddressTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char stringFieldsNode[] = "stringFields";
  const char attributeFieldsNode[] = "attributeFields";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, stringFieldsNode, aOutput->mStringFieldsJson);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
}  // end of ParseAddress

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Amenities section.
//!
//----------------------------------------------------------------
static void ParseAmenities(const rapidjson::Value& aDocument, AmenitiesTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";
  const char sectionNoteNode[] = "sectionNote";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
}  // end of ParseAmenities

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Business section.
//!
//----------------------------------------------------------------
static void ParseBusiness(const rapidjson::Value& aDocument, BusinessTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char attributeFieldsNode[] = "attributeFields";
  const char attributeMultiValueFieldsNode[] = "attributeMultiValueFields";
  const char businessPromotionListFieldNode[] = "businessPromotionListField";
  const char callToActionField[] = "callToActionField";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
  GetJsonString(aDocument, attributeMultiValueFieldsNode, aOutput->mAttributeMultiValueFieldsJson);
  GetJsonString(aDocument, businessPromotionListFieldNode, aOutput->mBusinessPromotionsJson);
  GetJsonString(aDocument, callToActionField, aOutput->mCallToActionJson);
}  // end of ParseBusiness

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse BusinessPhotos section.
//!
//----------------------------------------------------------------
static void ParseBusinessPhotos(const rapidjson::Value& aDocument,
                                std::vector<BusinessPhotoTableDataType>& aOutput) {
  const char ordinalNode[] = "ordinal";
  const char downloadUrlNode[] = "downloadUrl";

  if (aDocument.IsArray()) {
    for (auto it = aDocument.GetArray().begin(); it != aDocument.GetArray().end(); it++) {
      BusinessPhotoTableDataType businessPhotoTableData;

      if (it->IsObject()) {
        bool success = true;
        success = success && Json::GetSint32(*it, ordinalNode, businessPhotoTableData.mOrdinal);
        success =
            success && Json::GetString(*it, downloadUrlNode, businessPhotoTableData.mDownloadUrl);
        if (success) {
          aOutput.push_back(std::move(businessPhotoTableData));
        }
      }
    }
  }
}  // end of ParseBusinessPhotos

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse BusinessProgram section.
//!
//----------------------------------------------------------------
static void ParseBusinessProgram(const rapidjson::Value& aDocument,
                                 BusinessProgramTableDataType* aOutput) {
  const char programTierNode[] = "programTier";
  const char competitorAdNode[] = "competitorAd";

  Json::GetSint32(aDocument, programTierNode, aOutput->mProgramTier);
  Json::GetJsonString(aDocument, competitorAdNode, aOutput->mCompetitorAdJson);
}  // end of ParseBusinessProgram

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Competitors section.
//!
//----------------------------------------------------------------
static void ParseCompetitors(const rapidjson::Value& aDocument,
                             std::vector<CompetitorTableDataType>& aOutput) {
  const char ordinalNode[] = "ordinal";
  const char competitorPoiIdNode[] = "competitorPoiIdStr";

  if (aDocument.IsArray()) {
    for (auto it = aDocument.GetArray().begin(); it != aDocument.GetArray().end(); it++) {
      CompetitorTableDataType competitorTableData;

      if (it->IsObject()) {
        bool success = true;
        success = success && Json::GetSint32(*it, ordinalNode, competitorTableData.mOrdinal);
        success =
            success && Json::GetUint64(*it, competitorPoiIdNode, competitorTableData.mCompetitorId);
        if (success) {
          aOutput.push_back(std::move(competitorTableData));
        }
      }
    }
  }
}  // end of ParseCompetitors

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Contact section.
//!
//----------------------------------------------------------------
static void ParseContact(const rapidjson::Value& aDocument, ContactTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char attributeFieldsNode[] = "attributeFields";
  const char fieldTextHandleNode[] = "fieldTextHandle";
  const char valueTextHandle[] = "value";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);

  if (!aOutput->mAttributeFieldsJson.empty()) {
    rapidjson::Document attributeFieldsDocument;
    attributeFieldsDocument.Parse(aOutput->mAttributeFieldsJson.c_str());

    if (attributeFieldsDocument.IsArray()) {
      for (auto it = attributeFieldsDocument.GetArray().begin();
           it != attributeFieldsDocument.GetArray().end(); it++) {
        if (it->IsObject()) {
          rapidjson::Value attributeFieldDocument = it->GetObject();

          ACDB_text_handle_type textHandle;

          if (GetSint32(attributeFieldDocument, fieldTextHandleNode, textHandle)) {
            if (textHandle == static_cast<int>(TextHandle::PhoneNumberLabel)) {
              GRM_unused(GetString(attributeFieldDocument, valueTextHandle, aOutput->mPhone));
            } else if (textHandle == static_cast<int>(TextHandle::VhfChannelLabel)) {
              GRM_unused(GetString(attributeFieldDocument, valueTextHandle, aOutput->mVhfChannel));
            }
          }
        }
      }
    }
  }
}  // end of ParseContact

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Dockage section.
//!
//----------------------------------------------------------------
static void ParseDockage(const rapidjson::Value& aDocument, DockageTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoMultiValueFieldsNode[] = "yesNoMultiValueFields";
  const char attributePriceFieldsNode[] = "attributePriceFields";
  const char attributeFieldsNode[] = "attributeFields";
  const char sectionNoteNode[] = "sectionNote";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";
  const char distanceUnitNode[] = "distanceUnit";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoMultiValueFieldsNode, aOutput->mYesNoMultiValueJson);
  GetJsonString(aDocument, attributePriceFieldsNode, aOutput->mAttributePriceJson);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
  GetUnitType(aDocument, distanceUnitNode, aOutput->mDistanceUnit);
}  // end of ParseDockage

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Fuel section.
//!
//----------------------------------------------------------------
static void ParseFuel(const rapidjson::Value& aDocument, FuelTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoPriceFieldsNode[] = "yesNoPriceFields";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";
  const char attributeFieldsNode[] = "attributeFields";
  const char sectionNoteNode[] = "sectionNote";
  const char distanceUnitNode[] = "distanceUnit";
  const char currencyNode[] = "currency";
  const char dieselPriceNode[] = "dieselPrice";
  const char gasPriceNode[] = "gasPrice";
  const char volumeUnitsNode[] = "volumeUnits";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoPriceFieldsNode, aOutput->mYesNoPriceJson);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
  GetUnitType(aDocument, distanceUnitNode, aOutput->mDistanceUnit);
  GRM_unused(GetString(aDocument, currencyNode, aOutput->mCurrency));
  GetDouble(aDocument, dieselPriceNode, aOutput->mDieselPrice);
  GetDouble(aDocument, gasPriceNode, aOutput->mGasPrice);
  GetUnitType(aDocument, volumeUnitsNode, aOutput->mVolumeUnit);
}  // end of ParseFuel

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Moorings section.
//!
//----------------------------------------------------------------
static void ParseMoorings(const rapidjson::Value& aDocument, MooringsTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoPriceFieldsNode[] = "yesNoPriceFields";
  const char attributeFieldsNode[] = "attributeFields";
  const char sectionNoteNode[] = "sectionNote";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoPriceFieldsNode, aOutput->mYesNoPriceJson);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
}  // end of ParseMoorings

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Navigation section.
//!
//----------------------------------------------------------------
static void ParseNavigation(const rapidjson::Value& aDocument, NavigationTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char attributeFieldsNode[] = "attributeFields";
  const char sectionNoteNode[] = "sectionNote";
  const char distanceUnitNode[] = "distanceUnit";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, attributeFieldsNode, aOutput->mAttributeFieldsJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
  GetUnitType(aDocument, distanceUnitNode, aOutput->mDistanceUnit);
}  // end of ParseNavigation

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse PointOfInterest section.
//!
//----------------------------------------------------------------
static bool ParsePointOfInterest(const rapidjson::Value& aDocument,
                                 MarkerTableDataType* aOutputMarker,
                                 MarkerMetaTableDataType* aOutputMarkerMeta) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char nameNode[] = "name";
  const char sectionNoteNode[] = "sectionNote";

  bool success = GetSint32(aDocument, titleTextHandleNode, aOutputMarkerMeta->mSectionTitle);
  success = success && GetString(aDocument, nameNode, aOutputMarker->mName);
  GetJsonString(aDocument, sectionNoteNode, aOutputMarkerMeta->mSectionNoteJson);  // Optional

  return success;
}  // end of ParsePointOfInterest

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Retail section.
//!
//----------------------------------------------------------------
static void ParseRetail(const rapidjson::Value& aDocument, RetailTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";
  const char sectionNoteNode[] = "sectionNote";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
}  // end of ParseRetail

//----------------------------------------------------------------
//!
//!   @private
//!   @brief
//!       Parse Services section.
//!
//----------------------------------------------------------------
static void ParseServices(const rapidjson::Value& aDocument, ServicesTableDataType* aOutput) {
  const char titleTextHandleNode[] = "titleTextHandle";
  const char yesNoUnknownNearbyFieldsNode[] = "yesNoUnknownNearbyFields";
  const char sectionNoteNode[] = "sectionNote";

  GetSint32(aDocument, titleTextHandleNode, aOutput->mSectionTitle);
  GetJsonString(aDocument, yesNoUnknownNearbyFieldsNode, aOutput->mYesNoJson);
  GetJsonString(aDocument, sectionNoteNode, aOutput->mSectionNoteJson);
}  // end of ParseServices

}  // end of namespace Json
}  // end of namespace Acdb
