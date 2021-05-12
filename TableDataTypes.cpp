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
@brief Member function definitions for table data types

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "TableDataTypes"

#include "Acdb/TableDataTypes.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MarkerTableDataType::MarkerTableDataType(ACDB_marker_idx_type aId, ACDB_type_type aType,
                                         uint64_t aLastUpdated, std::string&& aName,
                                         scposn_type aPosn, uint64_t aGeohash,
                                         uint64_t aSearchFilter, int aBusinessProgramTier)
    : mId(aId),
      mType(aType),
      mLastUpdated(aLastUpdated),
      mName(std::move(aName)),
      mPosn(aPosn),
      mGeohash(aGeohash),
      mSearchFilter(aSearchFilter),
      mBusinessProgramTier(aBusinessProgramTier) {}  // end of MarkerTableDataType()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool MarkerTableDataType::operator==(const MarkerTableDataType& aRhs) const {
  return mId == aRhs.mId && mType == aRhs.mType && mLastUpdated == aRhs.mLastUpdated &&
         mName == aRhs.mName && mPosn.lat == aRhs.mPosn.lat && mPosn.lon == aRhs.mPosn.lon &&
         mGeohash == aRhs.mGeohash && mSearchFilter == aRhs.mSearchFilter &&
         mBusinessProgramTier == aRhs.mBusinessProgramTier;
}  // end of MarkerTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewStatsData::ReviewStatsData(float aAverageRating, uint32_t aNumberOfReviews)
    : mAverageRating(aAverageRating),
      mNumberOfReviews(aNumberOfReviews) {}  // end of ReviewStatsData()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewStatsData::operator==(const ReviewStatsData& aRhs) const {
  return mAverageRating == aRhs.mAverageRating && mNumberOfReviews == aRhs.mNumberOfReviews;
}  // end of ReviewStatsData::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ContactData::ContactData(const std::string&& aPhoneNumber, const std::string&& aVhfChannel)
    : mPhoneNumber(std::move(aPhoneNumber)),
      mVhfChannel(std::move(aVhfChannel)) {}  // end of ContactData()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ContactData::operator==(const ContactData& aRhs) const {
  return mPhoneNumber == aRhs.mPhoneNumber && mVhfChannel == aRhs.mVhfChannel;
}  // end of ContactData::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
FuelData::FuelData(float aGasPrice, float aDieselPrice, const std::string&& aFuelPriceCurrency,
                   ACDB_unit_type aFuelPriceUnit)
    : mGasPrice(aGasPrice),
      mDieselPrice(aDieselPrice),
      mFuelPriceCurrency(std::move(aFuelPriceCurrency)),
      mFuelPriceUnit(aFuelPriceUnit) {}  // end of FuelData()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool FuelData::operator==(const FuelData& aRhs) const {
  return mGasPrice == aRhs.mGasPrice && mDieselPrice == aRhs.mDieselPrice &&
         mFuelPriceCurrency == aRhs.mFuelPriceCurrency && mFuelPriceUnit == aRhs.mFuelPriceUnit;
}  // end of FuelData::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ExtendedMarkerDataType::ExtendedMarkerDataType(ACDB_marker_idx_type aId, ACDB_type_type aType,
                                               uint64_t aLastUpdated, std::string&& aName,
                                               scposn_type aPosn, int aBusinessProgramTier,
                                               ContactData&& aContactData, FuelData&& aFuelData,
                                               ReviewStatsData&& aReviewStatsData)
    : mId(aId),
      mType(aType),
      mLastUpdated(aLastUpdated),
      mName(std::move(aName)),
      mPosn(aPosn),
      mBusinessProgramTier(aBusinessProgramTier),
      mContactData(std::move(aContactData)),
      mFuelData(std::move(aFuelData)),
      mReviewStatsData(std::move(aReviewStatsData)) {}  // end of ExtendedMarkerDataType()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ExtendedMarkerDataType::operator==(const ExtendedMarkerDataType& aRhs) const {
  return mId == aRhs.mId && mType == aRhs.mType && mLastUpdated == aRhs.mLastUpdated &&
         mName == aRhs.mName && mPosn.lat == aRhs.mPosn.lat && mPosn.lon == aRhs.mPosn.lon &&
         mBusinessProgramTier == aRhs.mBusinessProgramTier && mContactData == aRhs.mContactData &&
         mFuelData == aRhs.mFuelData && mReviewStatsData == aRhs.mReviewStatsData;
}  // end of ExtendedMarkerDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
AddressTableDataType::AddressTableDataType(ACDB_text_handle_type aSectionTitle,
                                           std::string&& aStringFieldsJson,
                                           std::string&& aAttributeFieldsJson)
    : mSectionTitle(aSectionTitle),
      mStringFieldsJson(std::move(aStringFieldsJson)),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)) {}  // end of AddressTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool AddressTableDataType::operator==(const AddressTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mStringFieldsJson == aRhs.mStringFieldsJson &&
         mAttributeFieldsJson == aRhs.mAttributeFieldsJson;
}  // end of AddressTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
AmenitiesTableDataType::AmenitiesTableDataType(ACDB_text_handle_type aSectionTitle,
                                               std::string&& aSectionNoteJson,
                                               std::string&& aYesNoJson)
    : mSectionTitle(aSectionTitle),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mYesNoJson(std::move(aYesNoJson)) {}  // end of AmenitiesTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool AmenitiesTableDataType::operator==(const AmenitiesTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mSectionNoteJson == aRhs.mSectionNoteJson &&
         mYesNoJson == aRhs.mYesNoJson;
}  // end of AmenitiesTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessTableDataType::BusinessTableDataType(ACDB_text_handle_type aSectionTitle,
                                             std::string&& aAttributeFieldsJson,
                                             std::string&& aAttributeMultiValueFieldsJson,
                                             std::string&& aBusinessPromotionsJson,
                                             std::string&& aCallToActionJson)
    : mSectionTitle(aSectionTitle),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mAttributeMultiValueFieldsJson(std::move(aAttributeMultiValueFieldsJson)),
      mBusinessPromotionsJson(std::move(aBusinessPromotionsJson)),
      mCallToActionJson(std::move(aCallToActionJson)) {}  // end of BusinessTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessTableDataType::operator==(const BusinessTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mAttributeFieldsJson == aRhs.mAttributeFieldsJson &&
         mAttributeMultiValueFieldsJson == aRhs.mAttributeMultiValueFieldsJson &&
         mBusinessPromotionsJson == aRhs.mBusinessPromotionsJson &&
         mCallToActionJson == aRhs.mCallToActionJson;
}  // end of BusinessTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessPhotoTableDataType::BusinessPhotoTableDataType(ACDB_marker_idx_type aId, int aOrdinal,
                                                       std::string&& aDownloadUrl)
    : mId(aId),
      mOrdinal(aOrdinal),
      mDownloadUrl(std::move(aDownloadUrl)) {}  // end of BusinessPhotoTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessPhotoTableDataType::operator==(const BusinessPhotoTableDataType& aRhs) const {
  return mId == aRhs.mId && mOrdinal == aRhs.mOrdinal && mDownloadUrl == aRhs.mDownloadUrl;
}  // end of BusinessPhotoTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewPhotoTableDataType::ReviewPhotoTableDataType(ACDB_review_idx_type aId, int aOrdinal,
                                                   std::string&& aDownloadUrl)
    : mId(aId),
      mOrdinal(aOrdinal),
      mDownloadUrl(std::move(aDownloadUrl)) {}  // end of ReviewPhotoTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewPhotoTableDataType::operator==(const ReviewPhotoTableDataType& aRhs) const {
  return mId == aRhs.mId && mOrdinal == aRhs.mOrdinal && mDownloadUrl == aRhs.mDownloadUrl;
}  // end of ReviewPhotoTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Default constructor
//!
//----------------------------------------------------------------
BusinessProgramTableDataType::BusinessProgramTableDataType()
    : mId(ACDB_INVALID_MARKER_IDX),
      mCompetitorAdJson(),
      mProgramTier(ACDB_INVALID_BUSINESS_PROGRAM_TIER) {}  // end of BusinessProgramTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessProgramTableDataType::BusinessProgramTableDataType(ACDB_marker_idx_type aId,
                                                           std::string&& aCompetitorAdJson,
                                                           int aProgramTier)
    : mId(aId),
      mCompetitorAdJson(std::move(aCompetitorAdJson)),
      mProgramTier(aProgramTier) {}  // end of BusinessProgramTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessProgramTableDataType::operator==(const BusinessProgramTableDataType& aRhs) const {
  return mId == aRhs.mId && mCompetitorAdJson == aRhs.mCompetitorAdJson &&
         mProgramTier == aRhs.mProgramTier;
}  // end of BusinessPromotionTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
CompetitorTableDataType::CompetitorTableDataType(ACDB_marker_idx_type aId,
                                                 ACDB_marker_idx_type aCompetitorId, int aOrdinal)
    : mId(aId),
      mCompetitorId(aCompetitorId),
      mOrdinal(aOrdinal) {}  // end of CompetitorTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool CompetitorTableDataType::operator==(const CompetitorTableDataType& aRhs) const {
  return mId == aRhs.mId && mCompetitorId == aRhs.mCompetitorId && mOrdinal == aRhs.mOrdinal;
}  // end of CompetitorTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ContactTableDataType::ContactTableDataType(ACDB_text_handle_type aSectionTitle,
                                           std::string&& aAttributeFieldsJson, std::string&& aPhone,
                                           std::string&& aVhfChannel)
    : mSectionTitle(aSectionTitle),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mPhone(std::move(aPhone)),
      mVhfChannel(std::move(aVhfChannel)) {}  // end of ContactTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ContactTableDataType::operator==(const ContactTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mAttributeFieldsJson == aRhs.mAttributeFieldsJson &&
         mPhone == aRhs.mPhone && mVhfChannel == aRhs.mVhfChannel;
}  // end of ContactTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
DockageTableDataType::DockageTableDataType(ACDB_text_handle_type aSectionTitle,
                                           std::string&& aYesNoMultiValueJson,
                                           std::string&& aAttributePriceJson,
                                           std::string&& aAttributeFieldsJson,
                                           std::string&& aSectionNoteJson, std::string&& aYesNoJson,
                                           int aDistanceUnit)
    : mSectionTitle(aSectionTitle),
      mYesNoMultiValueJson(std::move(aYesNoMultiValueJson)),
      mAttributePriceJson(std::move(aAttributePriceJson)),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mYesNoJson(std::move(aYesNoJson)),
      mDistanceUnit(aDistanceUnit) {}  // end of DockageTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool DockageTableDataType::operator==(const DockageTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mYesNoMultiValueJson == aRhs.mYesNoMultiValueJson &&
         mAttributePriceJson == aRhs.mAttributePriceJson &&
         mAttributeFieldsJson == aRhs.mAttributeFieldsJson &&
         mSectionNoteJson == aRhs.mSectionNoteJson && mYesNoJson == aRhs.mYesNoJson &&
         mDistanceUnit == aRhs.mDistanceUnit;
}  // end of DockageTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Default constructor
//!
//----------------------------------------------------------------
FuelTableDataType::FuelTableDataType()
    : mSectionTitle(),
      mYesNoPriceJson(),
      mYesNoJson(),
      mAttributeFieldsJson(),
      mSectionNoteJson(),
      mDistanceUnit(ACDB_METER),
      mCurrency(),
      mDieselPrice(0),
      mGasPrice(0),
      mVolumeUnit(ACDB_LITER) {}  // end of FuelTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
FuelTableDataType::FuelTableDataType(ACDB_text_handle_type aSectionTitle,
                                     std::string&& aYesNoPriceJson, std::string&& aYesNoJson,
                                     std::string&& aAttributeFieldsJson,
                                     std::string&& aSectionNoteJson, int aDistanceUnit,
                                     std::string&& aCurrency, double aDieselPrice, double aGasPrice,
                                     int aVolumeUnit)
    : mSectionTitle(aSectionTitle),
      mYesNoPriceJson(std::move(aYesNoPriceJson)),
      mYesNoJson(std::move(aYesNoJson)),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mDistanceUnit(aDistanceUnit),
      mCurrency(std::move(aCurrency)),
      mDieselPrice(aDieselPrice),
      mGasPrice(aGasPrice),
      mVolumeUnit(aVolumeUnit) {}  // end of FuelTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool FuelTableDataType::operator==(const FuelTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mYesNoPriceJson == aRhs.mYesNoPriceJson &&
         mYesNoJson == aRhs.mYesNoJson && mAttributeFieldsJson == aRhs.mAttributeFieldsJson &&
         mSectionNoteJson == aRhs.mSectionNoteJson && mDistanceUnit == aRhs.mDistanceUnit &&
         mCurrency == aRhs.mCurrency && mDieselPrice == aRhs.mDieselPrice &&
         mGasPrice == aRhs.mGasPrice && mVolumeUnit == aRhs.mVolumeUnit;
}  // end of FuelTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
LanguageTableDataType::LanguageTableDataType(int aId, std::string&& aIsoCode)
    : mId{aId}, mIsoCode{std::move(aIsoCode)} {}  // end of LanguageTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool LanguageTableDataType::operator==(const LanguageTableDataType& aRhs) const {
  return mId == aRhs.mId && mIsoCode == aRhs.mIsoCode;
}  // end of LanguageTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MarkerMetaTableDataType::MarkerMetaTableDataType(std::string&& aSectionNoteJson,
                                                 ACDB_text_handle_type aSectionTitle)
    : mSectionNoteJson(std::move(aSectionNoteJson)),
      mSectionTitle(aSectionTitle) {}  // end of MarkerMetaTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool MarkerMetaTableDataType::operator==(const MarkerMetaTableDataType& aRhs) const {
  return mSectionNoteJson == aRhs.mSectionNoteJson && mSectionTitle == aRhs.mSectionTitle;
}  // end of MarkerMetaTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MooringsTableDataType::MooringsTableDataType(ACDB_text_handle_type aSectionTitle,
                                             std::string&& aSectionNoteJson,
                                             std::string&& aYesNoPriceJson,
                                             std::string&& aAttributeFieldsJson,
                                             std::string&& aYesNoJson)
    : mSectionTitle(aSectionTitle),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mYesNoPriceJson(std::move(aYesNoPriceJson)),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mYesNoJson(std::move(aYesNoJson)) {}  // end of MooringsTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool MooringsTableDataType::operator==(const MooringsTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mSectionNoteJson == aRhs.mSectionNoteJson &&
         mYesNoPriceJson == aRhs.mYesNoPriceJson &&
         mAttributeFieldsJson == aRhs.mAttributeFieldsJson && mYesNoJson == aRhs.mYesNoJson;
}  // end of MooringsTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MustacheTemplateTableDataType::MustacheTemplateTableDataType(std::string&& aName,
                                                             std::string&& aTemplate)
    : mName{std::move(aName)},
      mTemplate{std::move(aTemplate)} {}  // end of MustacheTemplateTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool MustacheTemplateTableDataType::operator==(const MustacheTemplateTableDataType& aRhs) const {
  return mName == aRhs.mName && mTemplate == aRhs.mTemplate;
}  // end of MustacheTemplateTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
NavigationTableDataType::NavigationTableDataType(ACDB_text_handle_type aSectionTitle,
                                                 std::string&& aSectionNoteJson,
                                                 std::string&& aAttributeFieldsJson,
                                                 int aDistanceUnit)
    : mSectionTitle(aSectionTitle),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mAttributeFieldsJson(std::move(aAttributeFieldsJson)),
      mDistanceUnit(aDistanceUnit) {}  // end of NavigationTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool NavigationTableDataType::operator==(const NavigationTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mSectionNoteJson == aRhs.mSectionNoteJson &&
         mAttributeFieldsJson == aRhs.mAttributeFieldsJson && mDistanceUnit == aRhs.mDistanceUnit;
}  // end of NavigationTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
RetailTableDataType::RetailTableDataType(ACDB_text_handle_type aSectionTitle,
                                         std::string&& aSectionNoteJson, std::string&& aYesNoJson)
    : mSectionTitle(aSectionTitle),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mYesNoJson(std::move(aYesNoJson)) {}  // end of RetailTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool RetailTableDataType::operator==(const RetailTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mSectionNoteJson == aRhs.mSectionNoteJson &&
         mYesNoJson == aRhs.mYesNoJson;
}  // end of RetailTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewSummaryTableDataType::ReviewSummaryTableDataType(float aAverageStars, int aReviewCount)
    : mAverageStars(aAverageStars),
      mReviewCount(aReviewCount) {}  // end of ReviewSummaryTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewSummaryTableDataType::operator==(const ReviewSummaryTableDataType& aRhs) const {
  return mAverageStars == aRhs.mAverageStars && mReviewCount == aRhs.mReviewCount;
}  // end of ReviewSummaryTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReviewTableDataType::ReviewTableDataType(ACDB_review_idx_type aId, ACDB_marker_idx_type aMarkerId,
                                         uint64_t aLastUpdated, int aRating, std::string&& aTitle,
                                         std::string&& aDate, std::string&& aCaptain,
                                         std::string&& aReview, int aVotes, bool aIsDeleted,
                                         std::string&& aResponse)
    : mId(aId),
      mMarkerId(aMarkerId),
      mLastUpdated(aLastUpdated),
      mRating(aRating),
      mTitle(std::move(aTitle)),
      mDate(std::move(aDate)),
      mCaptain(std::move(aCaptain)),
      mReview(std::move(aReview)),
      mVotes(aVotes),
      mIsDeleted(aIsDeleted),
      mResponse(std::move(aResponse)) {}  // end of ReviewTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ReviewTableDataType::operator==(const ReviewTableDataType& aRhs) const {
  return mId == aRhs.mId && mMarkerId == aRhs.mMarkerId && mLastUpdated == aRhs.mLastUpdated &&
         mRating == aRhs.mRating && mTitle == aRhs.mTitle && mDate == aRhs.mDate &&
         mCaptain == aRhs.mCaptain && mReview == aRhs.mReview && mVotes == aRhs.mVotes &&
         mIsDeleted == aRhs.mIsDeleted && mResponse == aRhs.mResponse;
}  // end of ReviewTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ServicesTableDataType::ServicesTableDataType(ACDB_text_handle_type aSectionTitle,
                                             std::string&& aSectionNoteJson,
                                             std::string&& aYesNoJson)
    : mSectionTitle(aSectionTitle),
      mSectionNoteJson(std::move(aSectionNoteJson)),
      mYesNoJson(std::move(aYesNoJson)) {}  // end of ServicesTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool ServicesTableDataType::operator==(const ServicesTableDataType& aRhs) const {
  return mSectionTitle == aRhs.mSectionTitle && mSectionNoteJson == aRhs.mSectionNoteJson &&
         mYesNoJson == aRhs.mYesNoJson;
}  // end of ServicesTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
TileTableDataType::TileTableDataType(int aTileX, int aTileY, uint64_t aGeohashStart,
                                     uint64_t aGeohashEnd)
    : mTileX(aTileX),
      mTileY(aTileY),
      mGeohashStart(aGeohashStart),
      mGeohashEnd(aGeohashEnd) {}  // end of TileTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool TileTableDataType::operator==(const TileTableDataType& aRhs) const {
  return mTileX == aRhs.mTileX && mTileY == aRhs.mTileY && mGeohashStart == aRhs.mGeohashStart &&
         mGeohashEnd == aRhs.mGeohashEnd;
}  // end of TileTableDataType::operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Less operator
//!
//----------------------------------------------------------------
bool TileTableDataType::operator<(const TileTableDataType& aRhs) const {
  return (mTileX < aRhs.mTileX) || ((mTileX == aRhs.mTileX) && (mTileY < aRhs.mTileY));
}  // end of TileTableDataType::operator<

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
TranslationTableDataType::TranslationTableDataType(int aId, int aLangId, std::string&& aTranslation)
    : mId{aId},
      mLangId{aLangId},
      mTranslation{std::move(aTranslation)} {}  // end of TranslationTableDataType

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool TranslationTableDataType::operator==(const TranslationTableDataType& aRhs) const {
  return mId == aRhs.mId && mLangId == aRhs.mLangId && mTranslation == aRhs.mTranslation;
}  // end of TranslationTableDataType::operator==

}  // end of namespace Acdb
