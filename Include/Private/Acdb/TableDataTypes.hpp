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
    @file Table data struct declarations
    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_TableDataTypes_hpp
#define ACDB_TableDataTypes_hpp

#include <stdint.h>
#include <string>
#include "ACDB_pub_types.h"
#include "Acdb/IMapMarker.hpp"
#include "Acdb/ISearchMarker.hpp"

namespace Acdb {
struct MarkerTableDataType {
  ACDB_marker_idx_type mId;
  ACDB_type_type mType;
  uint64_t mLastUpdated;
  std::string mName;
  scposn_type mPosn;
  uint64_t mGeohash;
  uint64_t mSearchFilter;
  int mBusinessProgramTier;

  MarkerTableDataType() = default;

  MarkerTableDataType(ACDB_marker_idx_type aId, ACDB_type_type aType, uint64_t aLastUpdated,
                      std::string&& aName, scposn_type aPosn, uint64_t aGeohash,
                      uint64_t aSearchFilter, int aBusinessProgramTier);

  bool operator==(const MarkerTableDataType& aRhs) const;
};

struct ReviewStatsData {
  const float InvalidStarRating = -1.0f;

  float mAverageRating = InvalidStarRating;
  uint32_t mNumberOfReviews = 0;

  ReviewStatsData() = default;

  ReviewStatsData(float aAverageRating, uint32_t aNumberOfReviews);

  bool operator==(const ReviewStatsData& aRhs) const;
};

struct ContactData {
  std::string mPhoneNumber;
  std::string mVhfChannel;

  ContactData() = default;

  ContactData(const std::string&& aPhoneNumber, const std::string&& aVhfChannel);

  bool operator==(const ContactData& aRhs) const;
};

struct FuelData {
  const float InvalidFuelPrice = -1.0f;

  float mGasPrice = InvalidFuelPrice;
  float mDieselPrice = InvalidFuelPrice;
  std::string mFuelPriceCurrency;
  ACDB_unit_type mFuelPriceUnit = ACDB_UNKNOWN_UNIT;

  FuelData() = default;

  FuelData(float aGasPrice, float aDieselPrice, const std::string&& aFuelPriceCurrency,
           ACDB_unit_type aFuelPriceUnit);

  bool operator==(const FuelData& aRhs) const;
};

struct ExtendedMarkerDataType {
  ACDB_marker_idx_type mId;
  ACDB_type_type mType;
  uint64_t mLastUpdated;
  std::string mName;
  scposn_type mPosn;
  int mBusinessProgramTier;

  ContactData mContactData;
  FuelData mFuelData;
  ReviewStatsData mReviewStatsData;

  ExtendedMarkerDataType() = default;

  ExtendedMarkerDataType(ACDB_marker_idx_type aId, ACDB_type_type aType, uint64_t aLastUpdated,
                         std::string&& aName, scposn_type aPosn, int aBusinessProgramTier,
                         ContactData&& aContactData, FuelData&& aFuelData,
                         ReviewStatsData&& aReviewStatsData);

  bool operator==(const ExtendedMarkerDataType& aRhs) const;
};

struct AddressTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mStringFieldsJson;
  std::string mAttributeFieldsJson;

  AddressTableDataType() = default;

  AddressTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aStringFieldsJson,
                       std::string&& aAttributeFieldsJson);

  bool operator==(const AddressTableDataType& aRhs) const;
};

struct AmenitiesTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mSectionNoteJson;
  std::string mYesNoJson;

  AmenitiesTableDataType() = default;

  AmenitiesTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aSectionNoteJson,
                         std::string&& aYesNoJson);

  bool operator==(const AmenitiesTableDataType& aRhs) const;
};

struct BusinessTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mAttributeFieldsJson;
  std::string mAttributeMultiValueFieldsJson;
  std::string mBusinessPromotionsJson;
  std::string mCallToActionJson;

  BusinessTableDataType() = default;

  BusinessTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aAttributeFieldsJson,
                        std::string&& aAttributeMultiValueFieldsJson,
                        std::string&& aBusinessPromotionsJson, std::string&& aCallToActionJson);

  bool operator==(const BusinessTableDataType& aRhs) const;
};

struct BusinessPhotoTableDataType {
  ACDB_marker_idx_type mId;
  int mOrdinal;
  std::string mDownloadUrl;

  BusinessPhotoTableDataType() = default;

  BusinessPhotoTableDataType(ACDB_marker_idx_type aId, int aOrdinal, std::string&& aDownloadUrl);

  bool operator==(const BusinessPhotoTableDataType& aRhs) const;
};

struct ReviewPhotoTableDataType {
  ACDB_review_idx_type mId;
  int mOrdinal;
  std::string mDownloadUrl;

  ReviewPhotoTableDataType() = default;

  ReviewPhotoTableDataType(ACDB_review_idx_type aId, int aOrdinal, std::string&& aDownloadUrl);

  bool operator==(const ReviewPhotoTableDataType& aRhs) const;
};

struct BusinessProgramTableDataType {
  ACDB_marker_idx_type mId;
  std::string mCompetitorAdJson;
  int mProgramTier;

  BusinessProgramTableDataType();

  BusinessProgramTableDataType(ACDB_marker_idx_type aId, std::string&& aCompetitorAdJson,
                               int aProgramTier);

  bool operator==(const BusinessProgramTableDataType& aRhs) const;
};

struct CompetitorTableDataType {
  ACDB_marker_idx_type mId;
  ACDB_marker_idx_type mCompetitorId;
  int mOrdinal;

  CompetitorTableDataType() = default;

  CompetitorTableDataType(ACDB_marker_idx_type aId, ACDB_marker_idx_type aCompetitorId,
                          int aOrdinal);

  bool operator==(const CompetitorTableDataType& aRhs) const;
};

struct ContactTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mAttributeFieldsJson;
  std::string mPhone;
  std::string mVhfChannel;

  ContactTableDataType() = default;

  ContactTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aAttributeFieldsJson,
                       std::string&& aPhone, std::string&& aVhfChannel);

  bool operator==(const ContactTableDataType& aRhs) const;
};

struct DockageTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mYesNoMultiValueJson;
  std::string mAttributePriceJson;
  std::string mAttributeFieldsJson;
  std::string mSectionNoteJson;
  std::string mYesNoJson;
  int mDistanceUnit;

  DockageTableDataType() = default;

  DockageTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aYesNoMultiValueJson,
                       std::string&& aAttributePriceJson, std::string&& aAttributeFieldsJson,
                       std::string&& aSectionNoteJson, std::string&& aYesNoJson, int aDistanceUnit);

  bool operator==(const DockageTableDataType& aRhs) const;
};

struct FuelTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mYesNoPriceJson;
  std::string mYesNoJson;
  std::string mAttributeFieldsJson;
  std::string mSectionNoteJson;
  int mDistanceUnit;
  std::string mCurrency;
  double mDieselPrice;
  double mGasPrice;
  int mVolumeUnit;

  FuelTableDataType();

  FuelTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aYesNoPriceJson,
                    std::string&& aYesNoJson, std::string&& aAttributeFieldsJson,
                    std::string&& aSectionNoteJson, int aDistanceUnit, std::string&& aCurrency,
                    double aDieselPrice, double aGasPrice, int aVolumeUnit);

  bool operator==(const FuelTableDataType& aRhs) const;
};

struct LanguageTableDataType {
  int mId;
  std::string mIsoCode;

  LanguageTableDataType() = default;

  LanguageTableDataType(int aId, std::string&& aIsoCode);

  bool operator==(const LanguageTableDataType& aRhs) const;
};

struct MarkerMetaTableDataType {
  std::string mSectionNoteJson;
  ACDB_text_handle_type mSectionTitle;

  MarkerMetaTableDataType() = default;

  MarkerMetaTableDataType(std::string&& aSectionNoteJson, ACDB_text_handle_type aSectionTitle);

  bool operator==(const MarkerMetaTableDataType& aRhs) const;
};

struct MooringsTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mSectionNoteJson;
  std::string mYesNoPriceJson;
  std::string mAttributeFieldsJson;
  std::string mYesNoJson;

  MooringsTableDataType() = default;

  MooringsTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aSectionNoteJson,
                        std::string&& aYesNoPriceJson, std::string&& aAttributeFieldsJson,
                        std::string&& aYesNoJson);

  bool operator==(const MooringsTableDataType& aRhs) const;
};

struct MustacheTemplateTableDataType {
  std::string mName;
  std::string mTemplate;

  MustacheTemplateTableDataType() = default;

  MustacheTemplateTableDataType(std::string&& aName, std::string&& aIsoCode);

  bool operator==(const MustacheTemplateTableDataType& aRhs) const;
};

struct NavigationTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mSectionNoteJson;
  std::string mAttributeFieldsJson;
  int mDistanceUnit;

  NavigationTableDataType() = default;

  NavigationTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aSectionNoteJson,
                          std::string&& aAttributeFieldsJson, int aDistanceUnit);

  bool operator==(const NavigationTableDataType& aRhs) const;
};

struct RetailTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mSectionNoteJson;
  std::string mYesNoJson;

  RetailTableDataType() = default;

  RetailTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aSectionNoteJson,
                      std::string&& aYesNoJson);

  bool operator==(const RetailTableDataType& aRhs) const;
};

struct ReviewSummaryTableDataType {
  float mAverageStars;
  int mReviewCount;

  ReviewSummaryTableDataType() = default;

  ReviewSummaryTableDataType(float aAverageStars, int aReviewCount);

  bool operator==(const ReviewSummaryTableDataType& aRhs) const;
};

struct ReviewTableDataType {
  ACDB_review_idx_type mId;
  ACDB_marker_idx_type mMarkerId;
  uint64_t mLastUpdated;
  int mRating;
  std::string mTitle;
  std::string mDate;
  std::string mCaptain;
  std::string mReview;
  int mVotes;
  bool mIsDeleted;
  std::string mResponse;

  ReviewTableDataType() = default;

  ReviewTableDataType(ACDB_review_idx_type aId, ACDB_marker_idx_type aMarkerId,
                      uint64_t aLastUpdated, int aRating, std::string&& aTitle, std::string&& aDate,
                      std::string&& aCaptain, std::string&& aReview, int aVotes, bool aIsDeleted,
                      std::string&& aResponse);

  bool operator==(const ReviewTableDataType& aRhs) const;
};

struct ServicesTableDataType {
  ACDB_text_handle_type mSectionTitle;
  std::string mSectionNoteJson;
  std::string mYesNoJson;

  ServicesTableDataType() = default;

  ServicesTableDataType(ACDB_text_handle_type aSectionTitle, std::string&& aSectionNoteJson,
                        std::string&& aYesNoJson);

  bool operator==(const ServicesTableDataType& aRhs) const;
};

struct TileTableDataType {
  int mTileX;
  int mTileY;
  uint64_t mGeohashStart;
  uint64_t mGeohashEnd;

  TileTableDataType() = default;

  TileTableDataType(int aTileX, int aTileY, uint64_t aGeohashStart, uint64_t aGeohashEnd);

  bool operator==(const TileTableDataType& aRhs) const;

  bool operator<(const TileTableDataType& aRhs) const;
};

struct TranslationTableDataType {
  int mId;
  int mLangId;
  std::string mTranslation;

  TranslationTableDataType() = default;

  TranslationTableDataType(int aId, int aLangId, std::string&& aTranslation);

  bool operator==(const TranslationTableDataType& aRhs) const;
};
}  // namespace Acdb

#endif  // end of ACDB_TableDataTypes_hpp
