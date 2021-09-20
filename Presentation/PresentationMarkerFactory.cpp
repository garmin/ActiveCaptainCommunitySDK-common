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

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "PresentationMarkerFactory"

#include <inttypes.h>
#include <memory>
#include <vector>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/MarkerFactory.hpp"
#include "Acdb/Presentation/BusinessPhotoList.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/Presentation/PresentationMarkerFactory.hpp"
#include "Acdb/Presentation/ReviewList.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/SectionType.hpp"
#include "Acdb/StringFormatter.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/TextTranslator.hpp"
#include "NavDateTimeExtensions.hpp"
#include "rapidjson/document.h"

namespace Acdb {
namespace Presentation {

static AttributeField GetAttributeField(const rapidjson::Value& aDocument,
                                        const bool aIsMultiValue = false);

static std::unique_ptr<AttributeField> GetAttributeFieldOptional(const std::string& aJson);

static std::vector<AttributeField> GetAttributeFields(const std::string& aJson,
                                                      const bool aIsMultiValue = false);

static AttributePriceField GetAttributePriceField(const rapidjson::Value& aDocument);

static std::vector<AttributePriceField> GetAttributePriceFields(const std::string& aJson);

static BusinessPromotionField GetBusinessPromotionField(const rapidjson::Value& aDocument);

static BusinessPromotionListField GetBusinessPromotionListField(const std::string& aJson);

static CompetitorAdField GetCompetitorAdField(AdvertiserTableDataCollection&& aAdvertiserTableData);

static LinkField GetLinkField(const std::string& aJson);

static LinkField GetLinkFieldEdit(const ACDB_marker_idx_type aIdx, const SectionType aSectionType,
                                  std::string&& aLinkText);

static LinkField GetLinkFieldPhotos(const ACDB_marker_idx_type aIdx, std::string&& aLinkText);

static LinkField GetLinkFieldReport(const ACDB_marker_idx_type aIdx,
                                    const ACDB_review_idx_type aReviewIdx, std::string&& aLinkText);

static LinkField GetLinkFieldSeeAll(const ACDB_marker_idx_type aIdx,
                                    const SectionType aSectionType);

static LinkField GetLinkFieldSeeAllReviews(const ACDB_marker_idx_type aIdx, int aPageNumber,
                                           std::string&& aLinkText);

static LinkField GetLinkFieldSummary(const ACDB_marker_idx_type aIdx);

static LinkField GetLinkFieldVote(const ACDB_marker_idx_type aIdx,
                                  const ACDB_review_idx_type aReviewIdx);

static ReviewField GetReviewField(ReviewTableDataType& aReviewTableData, bool aIncludeStars,
                                  std::unique_ptr<LinkField> aVoteField, LinkField&& aLinkField,
                                  std::vector<ReviewPhotoTableDataType>&& aReviewPhotos);

static StringField GetStringField(const rapidjson::Value& aDocument);

static std::vector<StringField> GetStringFields(const std::string& aJson);

static std::vector<YesNoMultiValueField> GetYesNoMultiValueFields(const std::string& aJson);

static YesNoPriceField GetYesNoPriceField(const rapidjson::Value& aDocument);

static std::vector<YesNoPriceField> GetYesNoPriceFields(const std::string& aJson);

static YesNoUnknownNearbyField GetYesNoUnknownNearbyField(const rapidjson::Value& aDocument);

static std::vector<YesNoUnknownNearbyField> GetYesNoUnknownNearbyFields(const std::string& aJson);

static std::vector<YesNoUnknownNearbyFieldPair> GetYesNoUnknownNearbyFieldPairs(
    const std::vector<YesNoUnknownNearbyField>& aYesNoUnknownNearbyFields);

static bool IsCommentsSectionType(ACDB_type_type aType);

static const ACDB_type_type COMMENTS_SECTION_TYPES =
    ACDB_BOAT_RAMP | ACDB_BRIDGE | ACDB_DAM | ACDB_FERRY | ACDB_HAZARD | ACDB_INLET | ACDB_LOCK;

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the address data object.
//!
//----------------------------------------------------------------
AddressPtr GetAddress(const ACDB_marker_idx_type aIdx,
                      const AddressTableDataType& aAddressTableData) {
  auto title = TextTranslator::GetInstance().Find(aAddressTableData.mSectionTitle);

  auto stringFields = GetStringFields(aAddressTableData.mStringFieldsJson);
  auto attributeFields = GetAttributeFields(aAddressTableData.mAttributeFieldsJson);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Address, std::string());

  auto address = AddressPtr(new Address(std::move(title), std::move(stringFields),
                                        std::move(attributeFields), std::move(editField)));
  return address;
}  // end of GetAddress

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the amenities data object.
//!
//----------------------------------------------------------------
AmenitiesPtr GetAmenities(const ACDB_marker_idx_type aIdx,
                          const AmenitiesTableDataType& aAmenitiesTableData) {
  auto title = TextTranslator::GetInstance().Find(aAmenitiesTableData.mSectionTitle);

  auto sectionNote = GetAttributeFieldOptional(aAmenitiesTableData.mSectionNoteJson);
  auto yesNoUnknownNearbyFields = GetYesNoUnknownNearbyFields(aAmenitiesTableData.mYesNoJson);
  auto yesNoUnknownNearbyFieldPairs = GetYesNoUnknownNearbyFieldPairs(yesNoUnknownNearbyFields);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Amenities, std::string());
  auto seeAllField = GetLinkFieldSeeAll(aIdx, SectionType::Amenities);

  auto amenities =
      AmenitiesPtr(new Amenities(std::move(title), std::move(yesNoUnknownNearbyFields),
                                 std::move(yesNoUnknownNearbyFieldPairs), std::move(sectionNote),
                                 std::move(editField), std::move(seeAllField)));
  return amenities;
}  // end of GetAmenities

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the business data object.
//!
//----------------------------------------------------------------
BusinessPtr GetBusiness(const ACDB_marker_idx_type aIdx,
                        const BusinessTableDataType& aBusinessTableData) {
  auto title = TextTranslator::GetInstance().Find(aBusinessTableData.mSectionTitle);

  auto attributeFields = GetAttributeFields(aBusinessTableData.mAttributeFieldsJson);
  auto attributeMultiValueFields =
      GetAttributeFields(aBusinessTableData.mAttributeMultiValueFieldsJson, true);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Business, std::string());

  std::unique_ptr<BusinessPromotionListField> businessPromotionListField = nullptr;
  if (!aBusinessTableData.mBusinessPromotionsJson.empty()) {
    businessPromotionListField.reset(new BusinessPromotionListField(
        GetBusinessPromotionListField(aBusinessTableData.mBusinessPromotionsJson)));
  }

  std::unique_ptr<LinkField> callToActionField = nullptr;

  if (!aBusinessTableData.mCallToActionJson.empty()) {
    callToActionField.reset(new LinkField(GetLinkField(aBusinessTableData.mCallToActionJson)));
  }

  auto business = BusinessPtr(new Business(
      std::move(title), std::move(attributeFields), std::move(attributeMultiValueFields),
      std::move(editField), std::move(businessPromotionListField), std::move(callToActionField)));
  return business;
}  // end of GetBusiness

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the review list data object.
//!
//----------------------------------------------------------------
BusinessPhotoListPtr GetBusinessPhotoList(
    const ACDB_marker_idx_type aIdx,
    std::vector<BusinessPhotoTableDataType>&& aBusinessPhotoTableData) {
  std::string title = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PhotosTitle));

  std::vector<BusinessPhotoField> businessPhotoFields;

  for (auto businessPhotoTableData : aBusinessPhotoTableData) {
    businessPhotoFields.push_back(
        BusinessPhotoField(std::move(businessPhotoTableData.mDownloadUrl)));
  }

  LinkField backButtonField = GetLinkFieldSummary(aIdx);

  return BusinessPhotoListPtr(new BusinessPhotoList(
      std::move(title), std::move(businessPhotoFields), std::move(backButtonField)));
}  // end of GetBusinessPhotoList

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the competitor ad data object.
//!
//----------------------------------------------------------------
CompetitorAdPtr GetCompetitorAd(const ACDB_marker_idx_type aIdx,
                                std::vector<AdvertiserTableDataCollection>&& aAdvertiserTableData) {
  std::string title = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AdsTitle));

  std::vector<CompetitorAdField> competitorAdFields;

  for (auto advertiserTableData : aAdvertiserTableData) {
    competitorAdFields.push_back(GetCompetitorAdField(std::move(advertiserTableData)));
  }

  return CompetitorAdPtr(new CompetitorAd(std::move(title), std::move(competitorAdFields)));

}  // end of GetCompetitorAd

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create the competitor ad field data object.
//!
//----------------------------------------------------------------
static CompetitorAdField GetCompetitorAdField(
    AdvertiserTableDataCollection&& aAdvertiserTableData) {
  rapidjson::Document document;
  document.Parse(aAdvertiserTableData.mBusinessProgram.mCompetitorAdJson.c_str());

  std::string text;
  std::string photoUrl;

  if (document.IsObject()) {
    auto textIterator = document.FindMember("text");
    if (textIterator != document.MemberEnd()) {
      text = textIterator->value.GetString();
    }

    auto photoUrlIterator = document.FindMember("photoUrl");
    if (photoUrlIterator != document.MemberEnd()) {
      photoUrl = photoUrlIterator->value.GetString();
    }
  }

  return CompetitorAdField(
      aAdvertiserTableData.mBusinessProgram.mId, std::move(aAdvertiserTableData.mMarker.mName),
      std::move(text), std::move(photoUrl),
      GetReviewSummary(aAdvertiserTableData.mReviewSummary, aAdvertiserTableData.mMarker.mType),
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AdLabel)));
}  // end of GetCompetitorAdField

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the contact data object.
//!
//----------------------------------------------------------------
ContactPtr GetContact(const ACDB_marker_idx_type aIdx,
                      const ContactTableDataType& aContactTableData) {
  auto title = TextTranslator::GetInstance().Find(aContactTableData.mSectionTitle);

  auto attributeFields = GetAttributeFields(aContactTableData.mAttributeFieldsJson);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Contact, std::string());

  auto contact =
      ContactPtr(new Contact(std::move(title), std::move(attributeFields), std::move(editField)));
  return contact;
}  // end of GetContact

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the dockage data object.
//!
//----------------------------------------------------------------
DockagePtr GetDockage(const ACDB_marker_idx_type aIdx,
                      const DockageTableDataType& aDockageTableData) {
  auto title = TextTranslator::GetInstance().Find(aDockageTableData.mSectionTitle);

  auto yesNoMultiValueFields = GetYesNoMultiValueFields(aDockageTableData.mYesNoMultiValueJson);
  auto attributePriceFields = GetAttributePriceFields(aDockageTableData.mAttributePriceJson);
  auto attributeFields = GetAttributeFields(aDockageTableData.mAttributeFieldsJson);
  auto sectionNote = GetAttributeFieldOptional(aDockageTableData.mSectionNoteJson);
  auto ynubFields = GetYesNoUnknownNearbyFields(aDockageTableData.mYesNoJson);
  auto ynubFieldPairs = GetYesNoUnknownNearbyFieldPairs(ynubFields);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Dockage, std::string());
  auto seeAllField = GetLinkFieldSeeAll(aIdx, SectionType::Dockage);

  auto dockage = DockagePtr(new Dockage(
      std::move(title), std::move(yesNoMultiValueFields), std::move(attributePriceFields),
      std::move(attributeFields), std::move(sectionNote), std::move(ynubFields),
      std::move(ynubFieldPairs), std::move(editField), std::move(seeAllField)));
  return dockage;
}  // end of GetDockage

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the fuel data object.
//!
//----------------------------------------------------------------
FuelPtr GetFuel(const ACDB_marker_idx_type aIdx, const FuelTableDataType& aFuelTableData) {
  auto title = TextTranslator::GetInstance().Find(aFuelTableData.mSectionTitle);

  auto yesNoPriceFields = GetYesNoPriceFields(aFuelTableData.mYesNoPriceJson);
  auto attributeFields = GetAttributeFields(aFuelTableData.mAttributeFieldsJson);
  auto ynubFields = GetYesNoUnknownNearbyFields(aFuelTableData.mYesNoJson);
  auto ynubFieldPairs = GetYesNoUnknownNearbyFieldPairs(ynubFields);
  auto sectionNote = GetAttributeFieldOptional(aFuelTableData.mSectionNoteJson);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Fuel, std::string());

  auto fuel =
      FuelPtr(new Fuel(std::move(title), std::move(yesNoPriceFields), std::move(attributeFields),
                       std::move(ynubFields), std::move(ynubFieldPairs), std::move(sectionNote),
                       std::move(editField)));
  return fuel;
}  // end of GetFuel

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the marker detail data object.
//!
//----------------------------------------------------------------
MarkerDetail GetMarkerDetail(const ACDB_marker_idx_type aIdx,
                             const MarkerTableDataType& aMarkerTableData,
                             const MarkerMetaTableDataType& aMarkerMetaTableData,
                             const ReviewSummaryTableDataType& aReviewSummaryTableData,
                             std::vector<BusinessPhotoTableDataType>& aBusinessPhotoTableData) {
  std::string name(aMarkerTableData.mName);

  std::string lastModifiedDateStr;
  if (aMarkerTableData.mLastUpdated != 0) {
    lastModifiedDateStr = StringFormatter::GetInstance().FormatDate(aMarkerTableData.mLastUpdated);
  }

  AttributeField lastModifiedAttributeField(
      TextTranslator::GetInstance().Find((ACDB_text_handle_type)TextHandle::DateLastModifiedLabel),
      std::move(lastModifiedDateStr), std::string(), std::string());

  std::string locationStr = StringFormatter::GetInstance().FormatPosition(aMarkerTableData.mPosn);
  StringField locationField(std::move(locationStr));

  TextHandle markerTypeTextHandle = GetMarkerTypeTextHandle(aMarkerTableData.mType);
  auto markerTypeAttributeField = AttributeField(
      std::string(),
      TextTranslator::GetInstance().Find((ACDB_text_handle_type)markerTypeTextHandle),
      std::string(), std::string());

  auto sectionNoteAttributeField = GetAttributeFieldOptional(aMarkerMetaTableData.mSectionNoteJson);

  auto title = TextTranslator::GetInstance().Find(aMarkerMetaTableData.mSectionTitle);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::PointOfInterest, std::string());

  auto reviewSummaryField = IsCommentsSectionType(aMarkerTableData.mType)
                                ? nullptr
                                : GetReviewSummary(aReviewSummaryTableData, aMarkerTableData.mType);

  std::unique_ptr<BusinessPhotoField> businessPhotoPtr = nullptr;
  if (aBusinessPhotoTableData.size() > 0) {
    businessPhotoPtr.reset(
        new BusinessPhotoField(std::move(aBusinessPhotoTableData[0].mDownloadUrl)));
  }

  std::unique_ptr<LinkField> seeAllPhotosFieldPtr = nullptr;
  if (aBusinessPhotoTableData.size() > 1) {
    seeAllPhotosFieldPtr.reset(new LinkField(GetLinkFieldPhotos(
        aIdx, TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAllPhotos)))));
  }

  MarkerDetail markerDetail(std::move(name), std::move(reviewSummaryField),
                            std::move(lastModifiedAttributeField), std::move(locationField),
                            std::move(title), std::move(markerTypeAttributeField),
                            std::move(sectionNoteAttributeField), std::move(editField),
                            std::move(businessPhotoPtr), std::move(seeAllPhotosFieldPtr));
  return markerDetail;
}  // end of GetMarkerDetail

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the moorings data object.
//!
//----------------------------------------------------------------
MooringsPtr GetMoorings(const ACDB_marker_idx_type aIdx,
                        const MooringsTableDataType& aMooringsTableData) {
  auto title = TextTranslator::GetInstance().Find(aMooringsTableData.mSectionTitle);

  auto attributeFields = GetAttributeFields(aMooringsTableData.mAttributeFieldsJson);
  auto sectionNote = GetAttributeFieldOptional(aMooringsTableData.mSectionNoteJson);
  auto yesNoPriceFields = GetYesNoPriceFields(aMooringsTableData.mYesNoPriceJson);
  auto ynubFields = GetYesNoUnknownNearbyFields(aMooringsTableData.mYesNoJson);
  auto ynubFieldPairs = GetYesNoUnknownNearbyFieldPairs(ynubFields);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Moorings, std::string());
  auto seeAllField = GetLinkFieldSeeAll(aIdx, SectionType::Moorings);

  auto moorings = MooringsPtr(new Moorings(std::move(title), std::move(yesNoPriceFields),
                                           std::move(attributeFields), std::move(sectionNote),
                                           std::move(ynubFields), std::move(ynubFieldPairs),
                                           std::move(editField), std::move(seeAllField)));
  return moorings;
}  // end of GetMoorings

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the navigation data object.
//!
//----------------------------------------------------------------
NavigationPtr GetNavigation(const ACDB_marker_idx_type aIdx,
                            const NavigationTableDataType& aNavigationTableData) {
  auto title = TextTranslator::GetInstance().Find(aNavigationTableData.mSectionTitle);

  auto attributeFields = GetAttributeFields(aNavigationTableData.mAttributeFieldsJson);
  auto sectionNote = GetAttributeFieldOptional(aNavigationTableData.mSectionNoteJson);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Navigation, std::string());

  auto navigation = NavigationPtr(new Navigation(std::move(title), std::move(attributeFields),
                                                 std::move(sectionNote), std::move(editField)));
  return navigation;
}  // end of GetNavigation

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the retail data object.
//!
//----------------------------------------------------------------
RetailPtr GetRetail(const ACDB_marker_idx_type aIdx, const RetailTableDataType& aRetailTableData) {
  auto title = TextTranslator::GetInstance().Find(aRetailTableData.mSectionTitle);

  auto sectionNote = GetAttributeFieldOptional(aRetailTableData.mSectionNoteJson);
  auto yesNoUnknownNearbyFields = GetYesNoUnknownNearbyFields(aRetailTableData.mYesNoJson);
  auto yesNoUnknownNearbyFieldPairs = GetYesNoUnknownNearbyFieldPairs(yesNoUnknownNearbyFields);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Retail, std::string());
  auto seeAllField = GetLinkFieldSeeAll(aIdx, SectionType::Retail);

  auto retail =
      RetailPtr(new Retail(std::move(title), std::move(yesNoUnknownNearbyFields),
                           std::move(yesNoUnknownNearbyFieldPairs), std::move(sectionNote),
                           std::move(editField), std::move(seeAllField)));
  return retail;
}  // end of GetRetail

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the reviews data object.
//!
//----------------------------------------------------------------
ReviewDetailPtr GetReviewDetail(
    const ACDB_marker_idx_type aIdx, std::unique_ptr<ReviewTableDataType> aFeaturedReviewTableData,
    std::vector<ReviewPhotoTableDataType>&& aFeaturedReviewPhotoTableData,
    const ACDB_type_type aType, const ReviewSummaryTableDataType& aReviewSummaryTableData,
    const std::string& aCaptainName) {
  TextHandle titleTextHandle;
  TextHandle editTextHandle;
  bool includeStars;
  TextHandle userReviewEditTextHandle;

  if (IsCommentsSectionType(aType)) {
    titleTextHandle = TextHandle::CommentsTitle;
    editTextHandle = TextHandle::WriteComment;
    includeStars = false;
    userReviewEditTextHandle = TextHandle::EditComment;
  } else {
    titleTextHandle = TextHandle::ReviewsTitle;
    editTextHandle = TextHandle::WriteReview;
    includeStars = true;
    userReviewEditTextHandle = TextHandle::EditReview;
  }

  auto title = TextTranslator::GetInstance().Find(static_cast<int>(titleTextHandle));
  std::unique_ptr<ReviewField> featuredReviewField = nullptr;

  if (aFeaturedReviewTableData != nullptr) {
    if (!aCaptainName.empty() && aFeaturedReviewTableData->mCaptain == aCaptainName) {
      LinkField linkField = GetLinkFieldEdit(
          aIdx, SectionType::ReviewDetail,
          TextTranslator::GetInstance().Find(static_cast<int>(userReviewEditTextHandle)));
      featuredReviewField.reset(new ReviewField(
          GetReviewField(*aFeaturedReviewTableData, includeStars, nullptr, std::move(linkField),
                         std::move(aFeaturedReviewPhotoTableData))));
    } else {
      std::unique_ptr<LinkField> voteField = std::unique_ptr<LinkField>(
          new LinkField(GetLinkFieldVote(aIdx, aFeaturedReviewTableData->mId)));
      LinkField linkField = GetLinkFieldReport(
          aIdx, aFeaturedReviewTableData->mId,
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel)));
      featuredReviewField.reset(new ReviewField(
          GetReviewField(*aFeaturedReviewTableData, includeStars, std::move(voteField),
                         std::move(linkField), std::move(aFeaturedReviewPhotoTableData))));
    }
  }

  std::string editText = TextTranslator::GetInstance().Find(static_cast<int>(editTextHandle));
  auto editField = GetLinkFieldEdit(aIdx, SectionType::ReviewDetail, std::move(editText));

  std::string linkText = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll));
  auto seeAllField = GetLinkFieldSeeAllReviews(aIdx, 1, std::move(linkText));

  auto reviewSummary = GetReviewSummary(aReviewSummaryTableData, aType);

  auto reviewDetail = ReviewDetailPtr(
      new ReviewDetail(std::move(title), std::move(featuredReviewField), std::move(editField),
                       std::move(seeAllField), std::move(reviewSummary)));
  return reviewDetail;
}  // end of GetReviewDetail

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the review list data object.
//!
//----------------------------------------------------------------
ReviewListPtr GetReviewList(const ACDB_marker_idx_type aIdx, const ACDB_type_type aType,
                            std::vector<ReviewTableDataType>&& aReviewTableData,
                            std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>>&&
                                aReviewPhotoTableDataMap,
                            ReviewSummaryTableDataType&& aReviewSummaryTableData,

                            const std::string& aCaptainName, const int aPageNumber,
                            const int aPageSize) {
  TextHandle titleTextHandle;
  bool includeStars;
  TextHandle userReviewEditTextHandle;

  if (IsCommentsSectionType(aType)) {
    titleTextHandle = TextHandle::CommentsTitle;
    includeStars = false;
    userReviewEditTextHandle = TextHandle::EditComment;
  } else {
    titleTextHandle = TextHandle::ReviewsTitle;
    includeStars = true;
    userReviewEditTextHandle = TextHandle::EditReview;
  }

  std::unique_ptr<ReviewField> userReviewField = nullptr;
  std::vector<ReviewField> reviewFields;

  for (auto reviewTableData : aReviewTableData) {
    std::vector<ReviewPhotoTableDataType> photosForThisReview;
    std::map<ACDB_review_idx_type, std::vector<ReviewPhotoTableDataType>>::iterator it;
    it = aReviewPhotoTableDataMap.find(reviewTableData.mId);

    if (it != aReviewPhotoTableDataMap.end()) {
      photosForThisReview = it->second;
    }

    if (!aCaptainName.empty() && reviewTableData.mCaptain == aCaptainName) {
      LinkField linkField = GetLinkFieldEdit(
          aIdx, SectionType::ReviewDetail,
          TextTranslator::GetInstance().Find(static_cast<int>(userReviewEditTextHandle)));

      userReviewField.reset(
          new ReviewField(GetReviewField(reviewTableData, includeStars, nullptr,
                                         std::move(linkField), std::move(photosForThisReview))));
    } else {
      std::unique_ptr<LinkField> voteField(
          new LinkField(GetLinkFieldVote(aIdx, reviewTableData.mId)));
      LinkField linkField = GetLinkFieldReport(
          aIdx, reviewTableData.mId,
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel)));

      reviewFields.push_back(GetReviewField(reviewTableData, includeStars, std::move(voteField),
                                            std::move(linkField), std::move(photosForThisReview)));
    }
  }

  auto reviewSummary = GetReviewSummary(aReviewSummaryTableData, aType);

  LinkField backButtonField = GetLinkFieldSummary(aIdx);
  LinkField editField = GetLinkFieldEdit(aIdx, SectionType::ReviewDetail, std::string());

  std::unique_ptr<LinkField> prevField = nullptr;
  std::unique_ptr<LinkField> nextField = nullptr;

  if (aPageSize > 1) {
    if (aPageNumber > 1) {
      prevField.reset(new LinkField(GetLinkFieldSeeAllReviews(
          aIdx, aPageNumber - 1,
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PrevLabel)))));
    }

    if (reviewSummary != nullptr && reviewSummary->GetReviewCount() > aPageNumber * aPageSize) {
      nextField.reset(new LinkField(GetLinkFieldSeeAllReviews(
          aIdx, aPageNumber + 1,
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NextLabel)))));
    }
  }

  return ReviewListPtr(
      new ReviewList(TextTranslator::GetInstance().Find(static_cast<int>(titleTextHandle)),
                     std::move(reviewSummary), std::move(userReviewField), std::move(reviewFields),
                     std::move(backButtonField), std::move(editField), std::move(prevField),
                     std::move(nextField)));
}  // end of GetReviewList

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the review summary data object.
//!
//----------------------------------------------------------------
ReviewSummaryPtr GetReviewSummary(const ReviewSummaryTableDataType& aReviewSummaryData,
                                  const ACDB_type_type aType) {
  bool includeStars = !IsCommentsSectionType(aType);

  auto reviewSummary = ReviewSummaryPtr(new ReviewSummary(
      aReviewSummaryData.mAverageStars, aReviewSummaryData.mReviewCount, includeStars));
  return reviewSummary;
}  // end of GetReviewSummary

//----------------------------------------------------------------
//!
//!       @public
//!       @detail Create the services data object.
//!
//----------------------------------------------------------------
ServicesPtr GetServices(const ACDB_marker_idx_type aIdx,
                        const ServicesTableDataType& aServicesTableData) {
  auto title = TextTranslator::GetInstance().Find(aServicesTableData.mSectionTitle);

  auto sectionNote = GetAttributeFieldOptional(aServicesTableData.mSectionNoteJson);
  auto yesNoUnknownNearbyFields = GetYesNoUnknownNearbyFields(aServicesTableData.mYesNoJson);
  auto yesNoUnknownNearbyFieldPairs = GetYesNoUnknownNearbyFieldPairs(yesNoUnknownNearbyFields);

  auto editField = GetLinkFieldEdit(aIdx, SectionType::Services, std::string());
  auto seeAllField = GetLinkFieldSeeAll(aIdx, SectionType::Services);

  auto services =
      ServicesPtr(new Services(std::move(title), std::move(yesNoUnknownNearbyFields),
                               std::move(yesNoUnknownNearbyFieldPairs), std::move(sectionNote),
                               std::move(editField), std::move(seeAllField)));
  return services;
}  // end of GetServices

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create AttributeField data object.
//!
//----------------------------------------------------------------
static AttributeField GetAttributeField(const rapidjson::Value& aDocument,
                                        const bool aIsMultiValue) {
  std::string label;
  std::string value;
  std::string hyperLink;
  std::string note;

  auto fieldTextHandleIterator = aDocument.FindMember("fieldTextHandle");
  if (fieldTextHandleIterator != aDocument.MemberEnd()) {
    auto fieldTextHandle = fieldTextHandleIterator->value.GetInt();
    label = TextTranslator::GetInstance().Find(fieldTextHandle);
  } else {
    auto fieldIterator = aDocument.FindMember("field");
    if (fieldIterator != aDocument.MemberEnd()) {
      label = fieldIterator->value.GetString();
    }
  }

  if (aIsMultiValue) {
    std::vector<std::string> values;

    auto valuesIterator = aDocument.FindMember("valueTextHandles");
    if (valuesIterator != aDocument.MemberEnd()) {
      for (auto it = valuesIterator->value.GetArray().begin();
           it != valuesIterator->value.GetArray().end(); it++) {
        auto valueTextHandle = it->GetInt();
        auto valueStr = TextTranslator::GetInstance().Find(valueTextHandle);
        values.push_back(valueStr);
      }
    }

    value = String::Join(values, ", ");
  } else {
    auto valueTextHandleIterator = aDocument.FindMember("valueTextHandle");
    if (valueTextHandleIterator != aDocument.MemberEnd()) {
      auto valueTextHandle = valueTextHandleIterator->value.GetInt();
      value = TextTranslator::GetInstance().Find(valueTextHandle);
    } else {
      auto valueIterator = aDocument.FindMember("value");
      if (valueIterator != aDocument.MemberEnd()) {
        auto isDistanceIterator = aDocument.FindMember("isDistance");
        if (isDistanceIterator != aDocument.MemberEnd() && isDistanceIterator->value.GetBool()) {
          auto valueDouble = atof(valueIterator->value.GetString());
          value = StringFormatter::GetInstance().FormatDepthValue(valueDouble);
        } else {
          value = valueIterator->value.GetString();
        }
      }
    }
  }

  auto hyperLinkIterator = aDocument.FindMember("hyperlink");
  if (hyperLinkIterator != aDocument.MemberEnd()) {
    hyperLink = hyperLinkIterator->value.GetString();
  }

  auto noteIterator = aDocument.FindMember("note");
  if (noteIterator != aDocument.MemberEnd()) {
    note = noteIterator->value.GetString();
  }

  return AttributeField(std::move(label), std::move(value), std::move(note), std::move(hyperLink));
}  // end of GetAttributeField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail If JSON is non-empty, creates AttributeField
//!               data object.  Used for fields which are not
//!               mandatory, such as section notes.
//!
//----------------------------------------------------------------
static std::unique_ptr<AttributeField> GetAttributeFieldOptional(const std::string& aJson) {
  std::unique_ptr<AttributeField> attributeField = nullptr;

  rapidjson::Document document;
  document.Parse(aJson.c_str());

  if (document.IsObject()) {
    attributeField.reset(new AttributeField(GetAttributeField(document)));
  }

  return attributeField;
}  // end of GetAttributeFieldOptional

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of AttributeField data objects.
//!
//----------------------------------------------------------------
static std::vector<AttributeField> GetAttributeFields(const std::string& aJson,
                                                      const bool aIsMultiValue) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<AttributeField> attributeFields;

  if (document.IsArray()) {
    for (auto& attributeFieldDocument : document.GetArray()) {
      if (attributeFieldDocument.IsObject()) {
        auto attributeField = GetAttributeField(attributeFieldDocument, aIsMultiValue);
        attributeFields.push_back(attributeField);
      }
    }
  }

  return attributeFields;
}  // end of GetAttributeFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create AttributePriceField data object.
//!
//----------------------------------------------------------------
static AttributePriceField GetAttributePriceField(const rapidjson::Value& aDocument) {
  std::string price;
  std::string pricingUnit;
  std::string priceDate;

  auto attributeField = GetAttributeField(aDocument);

  auto priceIterator = aDocument.FindMember("price");
  if (priceIterator != aDocument.MemberEnd()) {
    price = priceIterator->value.GetString();
  }

  auto pricingUnitTextHandleIterator = aDocument.FindMember("pricingUnitTextHandle");
  if (pricingUnitTextHandleIterator != aDocument.MemberEnd()) {
    auto pricingUnitTextHandle = pricingUnitTextHandleIterator->value.GetInt();
    pricingUnit = TextTranslator::GetInstance().Find(pricingUnitTextHandle);
  }

  auto priceDateIterator = aDocument.FindMember("priceDate");
  if (priceDateIterator != aDocument.MemberEnd()) {
    priceDate = StringFormatter::GetInstance().FormatDate(priceDateIterator->value.GetString());
  }

  return AttributePriceField(std::move(attributeField), std::move(price), std::move(pricingUnit),
                             std::move(priceDate));
}  // end of GetAttributePriceField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of AttributePriceField data objects.
//!
//----------------------------------------------------------------
static std::vector<AttributePriceField> GetAttributePriceFields(const std::string& aJson) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<AttributePriceField> attributePriceFields;

  if (document.IsArray()) {
    for (auto& attributePriceDocument : document.GetArray()) {
      if (attributePriceDocument.IsObject()) {
        auto attributePriceField = GetAttributePriceField(attributePriceDocument);
        attributePriceFields.push_back(attributePriceField);
      }
    }
  }

  return attributePriceFields;
}  // end of GetAttributePriceFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create BusinessPromotionField data object
//!
//----------------------------------------------------------------
static BusinessPromotionField GetBusinessPromotionField(const rapidjson::Value& aDocument) {
  std::string title;
  std::string details;
  std::string startDate;
  std::string endDate;

  auto titleIterator = aDocument.FindMember("title");
  if (titleIterator != aDocument.MemberEnd()) {
    title = titleIterator->value.GetString();
  }

  auto detailsIterator = aDocument.FindMember("details");
  if (detailsIterator != aDocument.MemberEnd()) {
    details = detailsIterator->value.GetString();
  }

  auto startDateIterator = aDocument.FindMember("startDate");
  if (startDateIterator != aDocument.MemberEnd()) {
    startDate = startDateIterator->value.GetString();
  }

  auto endDateIterator = aDocument.FindMember("endDate");
  if (endDateIterator != aDocument.MemberEnd()) {
    endDate = endDateIterator->value.GetString();
  }

  return BusinessPromotionField(std::move(title), std::move(details), std::move(startDate),
                                std::move(endDate));
}  // end of GetBusinessPromotionField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create BusinessPromotionListField data object
//!
//----------------------------------------------------------------
static BusinessPromotionListField GetBusinessPromotionListField(const std::string& aJson) {
  rapidjson::Document document;

  document.Parse(aJson.c_str());

  std::string label;
  std::vector<BusinessPromotionField> businessPromotionFields;

  auto fieldTextHandleIterator = document.FindMember("fieldTextHandle");
  if (fieldTextHandleIterator != document.MemberEnd()) {
    auto fieldTextHandle = fieldTextHandleIterator->value.GetInt();
    label = TextTranslator::GetInstance().Find(fieldTextHandle);
  }

  auto businessPromotionsIterator = document.FindMember("businessPromotions");

  if (businessPromotionsIterator != document.MemberEnd()) {
    if (businessPromotionsIterator->value.IsArray()) {
      NavDateTime now = NavDateTimeExtensions::GetCurrentDateTime();

      for (auto& businessPromotionFieldDocument : businessPromotionsIterator->value.GetArray()) {
        auto businessPromotionField = GetBusinessPromotionField(businessPromotionFieldDocument);

        // Only add the promotion if it is currently active.

        Navionics::NavDateTime start;
        Navionics::NavDateTime end;

        // Only add the promotion if it is currently active.
        if (start.FromString(businessPromotionField.GetStartDate(), YYYYMMDDTHHMMSSZ_FORMAT) &&
            end.FromString(businessPromotionField.GetEndDate(), YYYYMMDDTHHMMSSZ_FORMAT) &&
            now >= start && now < end) {
          businessPromotionFields.push_back(businessPromotionField);
        }
      }
    }
  }

  return BusinessPromotionListField(std::move(label), std::move(businessPromotionFields));
}  // end of GetBusinessPromotionListField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object from JSON
//!
//----------------------------------------------------------------
static LinkField GetLinkField(const std::string& aJson) {
  rapidjson::Document document;

  document.Parse(aJson.c_str());

  std::string linkUrl;
  std::string linkText;

  auto linkUrlIterator = document.FindMember("linkUrl");
  if (linkUrlIterator != document.MemberEnd()) {
    linkUrl = linkUrlIterator->value.GetString();
  }

  auto linkTextIterator = document.FindMember("linkText");
  if (linkTextIterator != document.MemberEnd()) {
    linkText = linkTextIterator->value.GetString();
  }

  return LinkField(std::move(linkUrl), std::move(linkText));
}  // end of GetLinkField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for edit action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldEdit(const ACDB_marker_idx_type aIdx, const SectionType aSectionType,
                                  std::string&& aLinkText) {
  std::string linkUrl =
      String::Format("edit/%" PRIu64 "/%s", aIdx, aSectionType.ToString().c_str());

  return LinkField(std::move(linkUrl), std::move(aLinkText));
}  // end of GetLinkFieldEdit

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for photos action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldPhotos(const ACDB_marker_idx_type aIdx, std::string&& aLinkText) {
  std::string linkUrl = String::Format("photos/%" PRIu64, aIdx);

  return LinkField(std::move(linkUrl), std::move(aLinkText));
}  // end of GetLinkFieldPhotos

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for report action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldReport(const ACDB_marker_idx_type aIdx,
                                    const ACDB_review_idx_type aReviewIdx,
                                    std::string&& aLinkText) {
  std::string linkUrl = String::Format("report/%" PRIu64 "/%" PRIu64, aIdx, aReviewIdx);

  return LinkField(std::move(linkUrl), std::move(aLinkText));
}  // end of GetLinkFieldReport

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for seeAll action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldSeeAll(const ACDB_marker_idx_type aIdx,
                                    const SectionType aSectionType) {
  std::string linkUrl =
      String::Format("seeAll/%" PRIu64 "/%s", aIdx, aSectionType.ToString().c_str());

  std::string linkText = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll));

  return LinkField(std::move(linkUrl), std::move(linkText));
}  // end of GetLinkFieldSeeAll

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for seeAll reviews action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldSeeAllReviews(const ACDB_marker_idx_type aIdx, int aPageNumber,
                                           std::string&& aLinkText) {
  std::string linkUrl = String::Format("seeAll/%" PRIu64 "/Reviews/%i", aIdx, aPageNumber);

  return LinkField(std::move(linkUrl), std::move(aLinkText));
}  // end of GetLinkFieldSeeAllReviews

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for summary action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldSummary(const ACDB_marker_idx_type aIdx) {
  std::string linkUrl = String::Format("summary/%" PRIu64, aIdx);

  return LinkField(std::move(linkUrl), std::string());
}  // end of GetLinkFieldSummary

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create LinkField data object for vote action.
//!
//----------------------------------------------------------------
static LinkField GetLinkFieldVote(const ACDB_marker_idx_type aIdx,
                                  const ACDB_review_idx_type aReviewIdx) {
  std::string linkUrl = String::Format("vote/%" PRIu64 "/%" PRIu64, aIdx, aReviewIdx);

  return LinkField(std::move(linkUrl), std::string());
}  // end of GetLinkFieldVote

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create ReviewField data object.
//!
//----------------------------------------------------------------
static ResponseField GetResponseField(ReviewTableDataType& aReviewTableData) {
  return ResponseField(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ResponseLabel)),
      std::move(aReviewTableData.mResponse));
}  // end of GetReviewField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create ReviewField data object.
//!
//----------------------------------------------------------------
static ReviewField GetReviewField(ReviewTableDataType& aReviewTableData, bool aIncludeStars,
                                  std::unique_ptr<LinkField> aVoteField, LinkField&& aLinkField,
                                  std::vector<ReviewPhotoTableDataType>&& aReviewPhotos) {
  std::string reviewDate = StringFormatter::GetInstance().FormatDate(aReviewTableData.mDate);

  std::unique_ptr<ResponseField> responsePtr = nullptr;
  if (!aReviewTableData.mResponse.empty()) {
    responsePtr.reset(new ResponseField(GetResponseField(aReviewTableData)));
  }

  std::vector<ReviewPhotoField> reviewPhotoFields;
  for (auto reviewPhotoTableData : aReviewPhotos) {
    reviewPhotoFields.push_back(ReviewPhotoField(std::move(reviewPhotoTableData.mDownloadUrl)));
  }

  return ReviewField(std::move(aReviewTableData.mTitle), aReviewTableData.mRating,
                     std::move(reviewDate), std::move(aReviewTableData.mCaptain),
                     std::move(aReviewTableData.mReview), aIncludeStars, std::move(aVoteField),
                     aReviewTableData.mVotes, std::move(aLinkField), std::move(responsePtr),
                     std::move(reviewPhotoFields));
}  // end of GetReviewField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create StringField data object.
//!
//----------------------------------------------------------------
static StringField GetStringField(const rapidjson::Value& aDocument) {
  std::string value;

  auto valueIterator = aDocument.FindMember("value");
  if (valueIterator != aDocument.MemberEnd()) {
    value = valueIterator->value.GetString();
  }

  return StringField(std::move(value));
}  // end of GetStringField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of StringField data objects.
//!
//----------------------------------------------------------------
static std::vector<StringField> GetStringFields(const std::string& aJson) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<StringField> stringFields;

  if (document.IsArray()) {
    for (auto& stringFieldDocument : document.GetArray()) {
      if (stringFieldDocument.IsObject()) {
        auto stringField = GetStringField(stringFieldDocument);
        stringFields.push_back(stringField);
      }
    }
  }

  return stringFields;
}  // end of GetStringFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create YesNoMultiValueField data object.
//!
//----------------------------------------------------------------
static YesNoMultiValueField GetYesNoMultiValueField(const rapidjson::Value& aDocument) {
  auto yesNoUnknownNearbyField = GetYesNoUnknownNearbyField(aDocument);

  std::vector<std::string> values;

  auto valuesIterator = aDocument.FindMember("valueTextHandles");
  if (valuesIterator != aDocument.MemberEnd()) {
    for (auto it = valuesIterator->value.GetArray().begin();
         it != valuesIterator->value.GetArray().end(); it++) {
      auto valueTextHandle = it->GetInt();
      auto valueStr = TextTranslator::GetInstance().Find(valueTextHandle);
      values.push_back(valueStr);
    }
  }

  std::string csvString = String::Join(values, ", ");

  return YesNoMultiValueField(std::move(yesNoUnknownNearbyField), std::move(csvString));
}  // end of GetYesNoMultiValueField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of YesNoMultiValueField data object.
//!
//----------------------------------------------------------------
static std::vector<YesNoMultiValueField> GetYesNoMultiValueFields(const std::string& aJson) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<YesNoMultiValueField> yesNoMultiValueFields;
  std::vector<std::string> values;

  if (document.IsArray()) {
    for (auto& yesNoMultiValueDocument : document.GetArray()) {
      if (yesNoMultiValueDocument.IsObject()) {
        auto yesNoMultiValueField = GetYesNoMultiValueField(yesNoMultiValueDocument);
        yesNoMultiValueFields.push_back(yesNoMultiValueField);
      }
    }
  }

  return yesNoMultiValueFields;
}  // end of GetYesNoMultiValueFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create YesNoPriceField data object.
//!
//----------------------------------------------------------------
static YesNoPriceField GetYesNoPriceField(const rapidjson::Value& aDocument) {
  std::string price;
  std::string pricingUnit;
  std::string priceDate;

  auto yesNoUnknownNearbyField = GetYesNoUnknownNearbyField(aDocument);

  auto priceIterator = aDocument.FindMember("price");
  if (priceIterator != aDocument.MemberEnd()) {
    price = priceIterator->value.GetString();
  }

  auto pricingUnitTextHandleIterator = aDocument.FindMember("pricingUnitTextHandle");
  if (pricingUnitTextHandleIterator != aDocument.MemberEnd()) {
    auto pricingUnitTextHandle = pricingUnitTextHandleIterator->value.GetInt();
    pricingUnit = TextTranslator::GetInstance().Find(pricingUnitTextHandle);
  }

  auto priceDateIterator = aDocument.FindMember("priceDate");
  if (priceDateIterator != aDocument.MemberEnd()) {
    priceDate = StringFormatter::GetInstance().FormatDate(priceDateIterator->value.GetString());
  }

  return YesNoPriceField(std::move(yesNoUnknownNearbyField), std::move(price),
                         std::move(pricingUnit), std::move(priceDate));
}  // end of GetYesNoPriceField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of YesNoPriceField data objects.
//!
//----------------------------------------------------------------
static std::vector<YesNoPriceField> GetYesNoPriceFields(const std::string& aJson) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<YesNoPriceField> yesNoPriceFields;

  if (document.IsArray()) {
    for (auto& yesNoPriceDocument : document.GetArray()) {
      if (yesNoPriceDocument.IsObject()) {
        auto yesNoPriceField = GetYesNoPriceField(yesNoPriceDocument);
        yesNoPriceFields.push_back(yesNoPriceField);
      }
    }
  }

  return yesNoPriceFields;
}  // end of GetYesNoPriceFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create YesNoUnknownNearbyField data object.
//!
//----------------------------------------------------------------
static YesNoUnknownNearbyField GetYesNoUnknownNearbyField(const rapidjson::Value& aDocument) {
  std::string label;
  std::string value;
  std::string note;
  std::string altText;

  auto fieldTextHandleIterator = aDocument.FindMember("fieldTextHandle");
  if (fieldTextHandleIterator != aDocument.MemberEnd()) {
    auto fieldTextHandle = fieldTextHandleIterator->value.GetInt();
    label = TextTranslator::GetInstance().Find(fieldTextHandle);
  }

  auto valueIterator = aDocument.FindMember("value");
  if (valueIterator != aDocument.MemberEnd()) {
    value = valueIterator->value.GetString();
  }

  auto noteIterator = aDocument.FindMember("note");
  if (noteIterator != aDocument.MemberEnd()) {
    note = noteIterator->value.GetString();
  }

  auto altTextHandle = GetYesNoUnknownNearbyTextHandle(value);
  altText = TextTranslator::GetInstance().Find(static_cast<int>(altTextHandle));

  return YesNoUnknownNearbyField(std::move(label), std::move(value), std::move(note),
                                 std::move(altText));
}  // end of GetYesNoUnknownNearbyField

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of YesNoUnknownNearbyField data objects.
//!
//----------------------------------------------------------------
static std::vector<YesNoUnknownNearbyField> GetYesNoUnknownNearbyFields(const std::string& aJson) {
  rapidjson::Document document;
  document.Parse(aJson.c_str());

  std::vector<YesNoUnknownNearbyField> yesNoUnknownNearbyFields;

  if (document.IsArray()) {
    for (auto& yesNoUnknownNearbyDocument : document.GetArray()) {
      if (yesNoUnknownNearbyDocument.IsObject()) {
        auto yesNoUnknownNearbyField = GetYesNoUnknownNearbyField(yesNoUnknownNearbyDocument);
        yesNoUnknownNearbyFields.push_back(yesNoUnknownNearbyField);
      }
    }
  }

  return yesNoUnknownNearbyFields;
}  // end of GetYesNoUnknownNearbyFields

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Create vector of YesNoUnknownNearbyFieldPair data objects.
//!
//----------------------------------------------------------------
static std::vector<YesNoUnknownNearbyFieldPair> GetYesNoUnknownNearbyFieldPairs(
    const std::vector<YesNoUnknownNearbyField>& aYesNoUnknownNearbyFields) {
  std::vector<YesNoUnknownNearbyFieldPair> yesNoUnknownNearbyFieldPairs;
  bool useLastPair = false;

  for (const auto& yesNoUnknownNearbyField : aYesNoUnknownNearbyFields) {
    if (String::ToLower(yesNoUnknownNearbyField.GetValue()) == "yes" ||
        String::ToLower(yesNoUnknownNearbyField.GetValue()) == "nearby") {
      if (useLastPair) {
        yesNoUnknownNearbyFieldPairs.back().mRightItem.reset(
            new YesNoUnknownNearbyField(yesNoUnknownNearbyField));
        useLastPair = false;
      } else {
        yesNoUnknownNearbyFieldPairs.push_back(YesNoUnknownNearbyFieldPair());
        yesNoUnknownNearbyFieldPairs.back().mLeftItem.reset(
            new YesNoUnknownNearbyField(yesNoUnknownNearbyField));
        useLastPair = true;
      }
    }
  }

  return yesNoUnknownNearbyFieldPairs;
}  // end of GetYesNoUnknownNearbyFieldPairs

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Return true if marker type uses a Comments (instead of Reviews) section
//!
//----------------------------------------------------------------
static inline bool IsCommentsSectionType(ACDB_type_type aType) {
  return (aType & COMMENTS_SECTION_TYPES) != 0;
}  // end of IsCommentsSectionType

}  // end of namespace Presentation
}  // end of namespace Acdb
