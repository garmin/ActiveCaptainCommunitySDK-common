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
    generating Active Captain objects

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MustacheViewFactory"

#include <inttypes.h>
#include <memory>
#include <string>
#include <vector>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/Presentation/BusinessPhotoList.hpp"
#include "Acdb/Presentation/MustacheContext.hpp"
#include "Acdb/Presentation/MustacheViewFactory.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/Presentation/ReviewList.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/SectionType.hpp"
#include "Acdb/StringUtil.hpp"
#include "mustache.hpp"

namespace Acdb {
namespace Presentation {
static std::string sHeadContent;
static std::string sImagePrefix;

static kainjow::mustache::data GetAttributeFieldData(const AttributeField& aAttributeField);

static kainjow::mustache::data GetAttributeFieldsData(
    const std::vector<AttributeField> aAttributeFields);

static kainjow::mustache::data GetAttributePriceFieldData(
    const AttributePriceField& aAttributePriceField);

static kainjow::mustache::data GetAttributePriceFieldsData(
    const std::vector<AttributePriceField>& aAttributePriceFields);

static kainjow::mustache::data GetPresentationMarkerData(
    const PresentationMarker& aPresentationMarker);

static kainjow::mustache::data GetAddressSectionData(const Address* aAddress);

static kainjow::mustache::data GetAmenitiesSectionData(const Amenities* aAmenities);

static kainjow::mustache::data GetBusinessPhotoFieldData(
    const BusinessPhotoField& aBusinessPhotoField);

static kainjow::mustache::data GetBusinessPhotoListData(
    const BusinessPhotoList& aBusinessPhotoList);

static kainjow::mustache::data GetBusinessPhotoListPageData(
    const BusinessPhotoList& aBusinessPhotoList);

static kainjow::mustache::data GetBusinessPromotionFieldData(
    const BusinessPromotionField& aBusinessPromotionField);

static kainjow::mustache::data GetBusinessPromotionFieldsData(
    const std::vector<BusinessPromotionField>& aBusinessPromotionFields);

static kainjow::mustache::data GetBusinessPromotionListFieldData(
    const BusinessPromotionListField& aBusinessPromotionListField);

static kainjow::mustache::data GetBusinessSectionData(const Business* aBusiness);

static kainjow::mustache::data GetCompetitorAdFieldData(
    const CompetitorAdField& aCompetitorAdField);

static kainjow::mustache::list GetCompetitorAdFieldsData(
    const std::vector<CompetitorAdField>& aCompetitorAdFields);

static kainjow::mustache::data GetCompetitorAdPhotoData(
    const CompetitorAdField& aCompetitorAdField);

static kainjow::mustache::data GetCompetitorAdSectionData(const CompetitorAd* aCompetitorAd);

static kainjow::mustache::data GetContactSectionData(const Contact* aContact);

static kainjow::mustache::data GetDockageSectionData(const Dockage* aDockage);

static kainjow::mustache::data GetLinkFieldData(const LinkField& aLinkField);

static kainjow::mustache::data GetFuelSectionData(const Fuel* aFuel);

static kainjow::mustache::data GetMooringsSectionData(const Moorings* aMoorings);

static kainjow::mustache::data GetNavigationSectionData(const Navigation* aNavigation);

static kainjow::mustache::data GetPointOfInterestSectionData(const MarkerDetail& aMarkerDetail);

static kainjow::mustache::data GetResponseFieldData(const ResponseField* aResponseField);

static kainjow::mustache::data GetRetailSectionData(const Retail* aRetail);

static kainjow::mustache::data GetReviewDetailSectionData(const ReviewDetail* aReviewDetail);

static kainjow::mustache::data GetReviewFieldData(const ReviewField& aReviewField);

static kainjow::mustache::data GetReviewListData(const ReviewList& aReviewList);

static kainjow::mustache::data GetReviewListPageData(const ReviewList& aReviewList);

static kainjow::mustache::data GetReviewPhotoFieldData(const ReviewPhotoField& aReviewPhotoField);

static kainjow::mustache::data GetReviewPhotoFieldListData(
    const std::vector<ReviewPhotoField>& aReviewPhotoField);

static kainjow::mustache::list GetReviewStarData(const std::vector<StringField>& aStarValues);

static kainjow::mustache::data GetReviewSummaryData(const ReviewSummary* aReviewSummary);

static kainjow::mustache::data GetServicesSectionData(const Services* aServices);

static kainjow::mustache::data GetStringFieldData(const StringField& aStringField);

static kainjow::mustache::data GetStringFieldsData(const std::vector<StringField>& aStringFields);

static kainjow::mustache::data GetSummarySectionData(const MarkerDetail& aMarkerDetail);

static kainjow::mustache::data GetYesNoMultiValueFieldData(
    const YesNoMultiValueField& aYesNoMultiValueField);

static kainjow::mustache::data GetYesNoMultiValueFieldsData(
    const std::vector<YesNoMultiValueField>& aYesNoMultiValueFields);

static kainjow::mustache::data GetYesNoPriceFieldData(const YesNoPriceField& aYesNoPriceField);

static kainjow::mustache::data GetYesNoPriceFieldsData(
    const std::vector<YesNoPriceField>& aYesNoPriceFields);

static kainjow::mustache::data GetYesNoUnknownNearbyCompactFieldListData(
    const std::vector<YesNoUnknownNearbyFieldPair>& aYesNoUnknownNearbyFieldPairs);

static kainjow::mustache::data GetYesNoUnknownNearbyFieldData(
    const YesNoUnknownNearbyField& aYesNoUnknownNearbyField);

static kainjow::mustache::data GetYesNoUnknownNearbyFieldListData(
    const std::vector<YesNoUnknownNearbyField>& aYesNoUnknownNearbyFields);

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for AttributeField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAttributeFieldData(const AttributeField& aAttributeField) {
  const std::string FIELD_TAG = "Field";
  const std::string HYPERLINK_TAG = "Hyperlink";
  const std::string NOTE_TAG = "Note";
  const std::string VALUE_TAG = "Value";

  kainjow::mustache::data data;

  if (!aAttributeField.GetLabel().empty()) {
    data[FIELD_TAG] = aAttributeField.GetLabel();
  }

  if (!aAttributeField.GetHyperLink().empty()) {
    data[HYPERLINK_TAG] = aAttributeField.GetHyperLink();
  }

  data[VALUE_TAG] = aAttributeField.GetValue();

  if (!aAttributeField.GetNote().empty()) {
    data[NOTE_TAG] = aAttributeField.GetNote();
  }

  return data;
}  // end of GetAttributeFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for AttributeFields
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAttributeFieldsData(
    const std::vector<AttributeField> aAttributeFields) {
  kainjow::mustache::list data;

  for (auto attributeField : aAttributeFields) {
    data.push_back(GetAttributeFieldData(attributeField));
  }

  return data;
}  // end of GetAttributeFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for AttributePriceField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAttributePriceFieldData(
    const AttributePriceField& aAttributePriceField) {
  const std::string PRICE_DATE_TAG = "PriceDate";
  const std::string PRICE_TAG = "Price";
  const std::string PRICING_UNIT_TAG = "PricingUnit";

  kainjow::mustache::data data = GetAttributeFieldData(aAttributePriceField);

  if (!aAttributePriceField.GetPrice().empty()) {
    data[PRICE_TAG] = aAttributePriceField.GetPrice();
    data[PRICING_UNIT_TAG] = aAttributePriceField.GetUnit();
    if (!aAttributePriceField.GetDate().empty()) {
      data[PRICE_DATE_TAG] = aAttributePriceField.GetDate();
    }
  }

  return data;
}  // end of GetAttributePriceFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for AttributePriceField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAttributePriceFieldsData(
    const std::vector<AttributePriceField>& aAttributePriceFields) {
  kainjow::mustache::list data;

  for (auto attributePriceField : aAttributePriceFields) {
    data.push_back(GetAttributePriceFieldData(attributePriceField));
  }

  return data;
}  // end of GetAttributePriceFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for BusinessPhotoField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPhotoFieldData(
    const BusinessPhotoField& aBusinessPhotoField) {
  const std::string DOWNLOAD_URL_TAG = "DownloadUrl";

  kainjow::mustache::data data;

  data[DOWNLOAD_URL_TAG] = aBusinessPhotoField.GetDownloadUrl();

  return data;
}  // end of GetBusinessPhotoFieldData

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Render BusinessPhotoList as HTML
//!   @return Rendered HTML string
//!
//----------------------------------------------------------------
std::string GetBusinessPhotoListHtml(const BusinessPhotoList& aBusinessPhotoList,
                                     const RepositoryPtr& aRepositoryPtr) {
  const std::string BUSINESS_PHOTO_LIST_PAGE = "{{> V2_BusinessPhotoListPage}}";

  kainjow::mustache::mustache mustache(BUSINESS_PHOTO_LIST_PAGE);

  kainjow::mustache::data data = GetBusinessPhotoListPageData(aBusinessPhotoList);
  MustacheContext context(aRepositoryPtr, &data);

  auto html = mustache.render(context);

  return html;
}  // end of GetBusinessPhotoListHtml

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review list
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPhotoListData(
    const BusinessPhotoList& aBusinessPhotoList) {
  const std::string BACK_BUTTON_FIELD_TAG = "BackButtonField";
  const std::string BUSINESS_PHOTOS_TAG = "BusinessPhotos";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aBusinessPhotoList.GetTitle();

  data[BACK_BUTTON_FIELD_TAG] = GetLinkFieldData(aBusinessPhotoList.GetBackButton());

  kainjow::mustache::list businessPhotosData;

  for (std::vector<BusinessPhotoField>::const_iterator it =
           aBusinessPhotoList.GetBusinessPhotos().begin();
       it != aBusinessPhotoList.GetBusinessPhotos().end(); ++it) {
    businessPhotosData.push_back(GetBusinessPhotoFieldData(*it));
  }

  data[BUSINESS_PHOTOS_TAG] = businessPhotosData;

  return data;
}  // end of GetBusinessPhotoListData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review list page
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPhotoListPageData(
    const BusinessPhotoList& aBusinessPhotoList) {
  const std::string HEAD_TAG = "Head";
  const std::string IMG_PREFIX_TAG = "ImgPrefix";
  const std::string REVIEW_LIST_TAG = "BusinessPhotoList";

  kainjow::mustache::data data;

  data[HEAD_TAG] = sHeadContent;
  data[IMG_PREFIX_TAG] = sImagePrefix;
  data[REVIEW_LIST_TAG] = GetBusinessPhotoListData(aBusinessPhotoList);

  return data;
}  // end of GetBusinessPhotoListPageData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for BusinessPromotionField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPromotionFieldData(
    const BusinessPromotionField& aBusinessPromotionField) {
  const std::string TITLE_TAG = "Title";
  const std::string DETAILS_TAG = "Details";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aBusinessPromotionField.GetTitle();
  data[DETAILS_TAG] = aBusinessPromotionField.GetDetails();

  return data;
}  // end of GetBusinessPromotionFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for BusinessPromotionField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPromotionFieldsData(
    const std::vector<BusinessPromotionField>& aBusinessPromotionFields) {
  kainjow::mustache::list data;

  for (auto businessPromotionField : aBusinessPromotionFields) {
    data.push_back(GetBusinessPromotionFieldData(businessPromotionField));
  }

  return data;
}  // end of GetBusinessPromotionFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for BusinessPromotionListField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessPromotionListFieldData(
    const BusinessPromotionListField& aBusinessPromotionListField) {
  const std::string FIELD_TAG = "Field";
  const std::string BUSINESS_PROMOTIONS_TAG = "BusinessPromotions";

  kainjow::mustache::data data;

  data[FIELD_TAG] = aBusinessPromotionListField.GetLabel();
  data[BUSINESS_PROMOTIONS_TAG] =
      GetBusinessPromotionFieldsData(aBusinessPromotionListField.GetBusinessPromotions());

  return data;
}  // end of GetBusinessPromotionListFieldData

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Render PresentationMarker in sections
//!   @return ContentViewMap with sections rendered as HTML
//!
//----------------------------------------------------------------
ContentViewMapPtr GetContentViewMap(const PresentationMarker& aPresentationMarker,
                                    const ReviewListPtr& aReviewListPtr,
                                    const RepositoryPtr& aRepositoryPtr) {
  kainjow::mustache::data markerData = GetPresentationMarkerData(aPresentationMarker);
  MustacheContext markerContext(aRepositoryPtr, &markerData);

  ContentViewMapPtr result = ContentViewMapPtr(new ContentViewMap());

  std::string generalViewTemplate;
  std::string navigationViewTemplate;
  std::string servicesViewTemplate;
  std::string fuelViewTemplate;
  std::string dockageViewTemplate;
  std::string reviewsViewTemplate;

  generalViewTemplate =
      "{{> GML_PointOfInterestSection}}<br><br>"
      "{{> GML_AddressSection}}<br><br>"
      "{{> GML_ContactSection}}<br><br>"
      "{{> GML_BusinessSection}}";

  kainjow::mustache::mustache mustacheGeneral(generalViewTemplate);
  result->insert(
      ContentViewPair(ContentViewGeneralInformation, mustacheGeneral.render(markerContext)));

  if (aPresentationMarker.GetNavigation()) {
    navigationViewTemplate = "{{> GML_NavigationSection}}";

    kainjow::mustache::mustache mustacheNavigation(navigationViewTemplate);
    result->insert(
        ContentViewPair(ContentViewNavigation, mustacheNavigation.render(markerContext)));
  }

  if (aPresentationMarker.GetAmenities() || aPresentationMarker.GetServices() ||
      aPresentationMarker.GetRetail()) {
    servicesViewTemplate =
        "{{> GML_AmenitiesSection}}<br><br>"
        "{{> GML_ServicesSection}}<br><br>"
        "{{> GML_RetailSection}}";

    kainjow::mustache::mustache mustacheServices(servicesViewTemplate);
    result->insert(ContentViewPair(ContentViewServices, mustacheServices.render(markerContext)));
  }

  if (aPresentationMarker.GetFuel()) {
    fuelViewTemplate = "{{> GML_FuelSection}}";

    kainjow::mustache::mustache mustacheFuel(fuelViewTemplate);
    result->insert(ContentViewPair(ContentViewFuel, mustacheFuel.render(markerContext)));
  }

  if (aPresentationMarker.GetDockage() || aPresentationMarker.GetMoorings()) {
    dockageViewTemplate =
        "{{> GML_DockageSection}}<br><br>"
        "{{> GML_MooringsSection}}";

    kainjow::mustache::mustache mustacheDockage(dockageViewTemplate);
    result->insert(ContentViewPair(ContentViewDockage, mustacheDockage.render(markerContext)));
  }

  if (aReviewListPtr && aReviewListPtr->GetReviews().size() != 0) {
    kainjow::mustache::data reviewData = GetReviewListPageData(*aReviewListPtr);
    MustacheContext reviewContext(aRepositoryPtr, &reviewData);

    reviewsViewTemplate = "{{> GML_ReviewsSection}}";

    kainjow::mustache::mustache mustacheReviews(reviewsViewTemplate);
    result->insert(ContentViewPair(ContentViewUserReview, mustacheReviews.render(reviewContext)));
  }

  return result;
}  // end of GetContentViewMap

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Render PresentationMarker as HTML
//!   @return Rendered HTML string
//!
//----------------------------------------------------------------
std::string GetPresentationMarkerHtml(const PresentationMarker& aPresentationMarker,
                                      const RepositoryPtr& aRepositoryPtr) {
  const std::string FULL_VIEW = "{{> V2_FullView}}";
  const std::string SUMMARY = "{{> V2_Summary}}";
  const std::string ENABLE_WEB_VIEWS_TAG = "EnableWebViews";

  kainjow::mustache::data data = GetPresentationMarkerData(aPresentationMarker);

#if (acdb_WEBVIEW_SUPPORT)
  data[ENABLE_WEB_VIEWS_TAG] = true;
#endif

  MustacheContext context(aRepositoryPtr, &data);

  kainjow::mustache::mustache summaryMustache(SUMMARY);
  auto html = summaryMustache.render(context);

  if (html.empty()) {
    // Summary template was not present -- the MustacheTemplates table may not be up-to-date.
    // Fall back to using the FullView template.
    kainjow::mustache::mustache fullViewMustache(FULL_VIEW);
    html = fullViewMustache.render(context);
  }

  return html;
}  // end of GetPresentationMarkerHtml

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for presentation marker
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetPresentationMarkerData(
    const PresentationMarker& aPresentationMarker) {
  const std::string ADDRESS_SECTION_TAG = "AddressSection";
  const std::string AMENITIES_SECTION_TAG = "AmenitiesSection";
  const std::string BUSINESS_SECTION_TAG = "BusinessSection";
  const std::string COMPETITOR_AD_SECTION_TAG = "CompetitorAdSection";
  const std::string CONTACT_SECTION_TAG = "ContactSection";
  const std::string DOCKAGE_SECTION_TAG = "DockageSection";
  const std::string FUEL_SECTION_TAG = "FuelSection";
  const std::string HEAD_TAG = "Head";
  const std::string IMG_PREFIX_TAG = "ImgPrefix";
  const std::string MOORINGS_SECTION_TAG = "MooringsSection";
  const std::string NAVIGATION_SECTION_TAG = "NavigationSection";
  const std::string POINT_OF_INTEREST_SECTION_TAG = "PointOfInterestSection";
  const std::string RETAIL_SECTION_TAG = "RetailSection";
  const std::string REVIEWS_SECTION_TAG = "ReviewsSection";
  const std::string SERVICES_SECTION_TAG = "ServicesSection";
  const std::string SUMMARY_SECTION_TAG = "SummarySection";

  kainjow::mustache::data data;

  data[HEAD_TAG] = sHeadContent;
  data[IMG_PREFIX_TAG] = sImagePrefix;
  data[POINT_OF_INTEREST_SECTION_TAG] =
      GetPointOfInterestSectionData(aPresentationMarker.GetMarkerDetail());
  data[SUMMARY_SECTION_TAG] = GetSummarySectionData(aPresentationMarker.GetMarkerDetail());

  if (aPresentationMarker.GetAddress()) {
    data[ADDRESS_SECTION_TAG] = GetAddressSectionData(aPresentationMarker.GetAddress());
  }

  if (aPresentationMarker.GetAmenities()) {
    data[AMENITIES_SECTION_TAG] = GetAmenitiesSectionData(aPresentationMarker.GetAmenities());
  }

  if (aPresentationMarker.GetBusiness()) {
    data[BUSINESS_SECTION_TAG] = GetBusinessSectionData(aPresentationMarker.GetBusiness());
  }

  if (aPresentationMarker.GetCompetitorAd()) {
    data[COMPETITOR_AD_SECTION_TAG] =
        GetCompetitorAdSectionData(aPresentationMarker.GetCompetitorAd());
  }

  if (aPresentationMarker.GetContact()) {
    data[CONTACT_SECTION_TAG] = GetContactSectionData(aPresentationMarker.GetContact());
  }

  if (aPresentationMarker.GetDockage()) {
    data[DOCKAGE_SECTION_TAG] = GetDockageSectionData(aPresentationMarker.GetDockage());
  }

  if (aPresentationMarker.GetFuel()) {
    data[FUEL_SECTION_TAG] = GetFuelSectionData(aPresentationMarker.GetFuel());
  }

  if (aPresentationMarker.GetMoorings()) {
    data[MOORINGS_SECTION_TAG] = GetMooringsSectionData(aPresentationMarker.GetMoorings());
  }

  if (aPresentationMarker.GetNavigation()) {
    data[NAVIGATION_SECTION_TAG] = GetNavigationSectionData(aPresentationMarker.GetNavigation());
  }

  if (aPresentationMarker.GetRetail()) {
    data[RETAIL_SECTION_TAG] = GetRetailSectionData(aPresentationMarker.GetRetail());
  }

  if (aPresentationMarker.GetReviewDetail()) {
    data[REVIEWS_SECTION_TAG] = GetReviewDetailSectionData(aPresentationMarker.GetReviewDetail());
  }

  if (aPresentationMarker.GetServices()) {
    data[SERVICES_SECTION_TAG] = GetServicesSectionData(aPresentationMarker.GetServices());
  }

  return data;
}  // end of GetPresentationMarkerData

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Render ReviewList as HTML
//!   @return Rendered HTML string
//!
//----------------------------------------------------------------
std::string GetReviewListHtml(const ReviewList& aReviewList, const RepositoryPtr& aRepositoryPtr) {
  const std::string REVIEW_LIST_PAGE = "{{> V2_ReviewListPage}}";
  const std::string ENABLE_WEB_VIEWS_TAG = "EnableWebViews";

  kainjow::mustache::mustache mustache(REVIEW_LIST_PAGE);

  kainjow::mustache::data data = GetReviewListPageData(aReviewList);

#if (acdb_WEBVIEW_SUPPORT)
  data[ENABLE_WEB_VIEWS_TAG] = true;
#endif

  MustacheContext context(aRepositoryPtr, &data);

  auto html = mustache.render(context);

  return html;
}  // end of GetReviewListHtml

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review list
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewListData(const ReviewList& aReviewList) {
  const std::string BACK_BUTTON_FIELD_TAG = "BackButtonField";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string NEXT_FIELD_TAG = "NextField";
  const std::string PREV_FIELD_TAG = "PrevField";
  const std::string REVIEW_SUMMARY_TAG = "ReviewSummary";
  const std::string REVIEWS_TAG = "Reviews";
  const std::string TITLE_TAG = "Title";
  const std::string USER_REVIEW_TAG = "UserReview";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aReviewList.GetTitle();

  data[BACK_BUTTON_FIELD_TAG] = GetLinkFieldData(aReviewList.GetBackButton());
  if (aReviewList.GetReviewSummary()) {
    data[REVIEW_SUMMARY_TAG] = GetReviewSummaryData(aReviewList.GetReviewSummary());
  }
  data[EDIT_FIELD_TAG] = GetLinkFieldData(aReviewList.GetEditField());

  if (aReviewList.GetNextField()) {
    data[NEXT_FIELD_TAG] = GetLinkFieldData(*(aReviewList.GetNextField()));
  }

  if (aReviewList.GetPrevField()) {
    data[PREV_FIELD_TAG] = GetLinkFieldData(*(aReviewList.GetPrevField()));
  }

  kainjow::mustache::list reviews_data;

  for (std::vector<ReviewField>::const_iterator it = aReviewList.GetReviews().begin();
       it != aReviewList.GetReviews().end(); ++it) {
    reviews_data.push_back(GetReviewFieldData(*it));
  }

  data[REVIEWS_TAG] = reviews_data;

  if (aReviewList.GetUserReview()) {
    data[USER_REVIEW_TAG] = GetReviewFieldData(*(aReviewList.GetUserReview()));
  }

  return data;
}  // end of GetReviewListData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review list page
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewListPageData(const ReviewList& aReviewList) {
  const std::string HEAD_TAG = "Head";
  const std::string IMG_PREFIX_TAG = "ImgPrefix";
  const std::string REVIEW_LIST_TAG = "ReviewList";

  kainjow::mustache::data data;

  data[HEAD_TAG] = sHeadContent;
  data[IMG_PREFIX_TAG] = sImagePrefix;
  data[REVIEW_LIST_TAG] = GetReviewListData(aReviewList);

  return data;
}  // end of GetReviewListPageData

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Render SectionPage as HTML
//!   @return Rendered HTML string
//!
//----------------------------------------------------------------
std::string GetSectionPageHtml(const PresentationMarker& aPresentationMarker,
                               const std::string& aSectionName,
                               const RepositoryPtr& aRepositoryPtr) {
  const std::string AMENITIES_SECTION_TAG = "AmenitiesSection";
  const std::string BACK_BUTTON_FIELD_TAG = "BackButtonField";
  const std::string DOCKAGE_SECTION_TAG = "DockageSection";
  const std::string ENABLE_WEB_VIEWS_TAG = "EnableWebViews";
  const std::string HEAD_TAG = "Head";
  const std::string IMG_PREFIX_TAG = "ImgPrefix";
  const std::string MOORINGS_SECTION_TAG = "MooringsSection";
  const std::string RETAIL_SECTION_TAG = "RetailSection";
  const std::string SERVICES_SECTION_TAG = "ServicesSection";

  static const std::map<std::string, SectionType::Value> compactSectionTypes = {
      {"amenities", SectionType::Amenities},
      {"dockage", SectionType::Dockage},
      {"moorings", SectionType::Moorings},
      {"retail", SectionType::Retail},
      {"services", SectionType::Services}};

  auto sectionTypeIter = compactSectionTypes.find(String::ToLower(aSectionName));
  if (sectionTypeIter == compactSectionTypes.end()) {
    DBG_ASSERT_ALWAYS("Invalid compact section type name.");
    return std::string();
  }

  SectionType::Value sectionType = sectionTypeIter->second;

  std::string sectionPageTemplate;

  kainjow::mustache::data data;

  data[HEAD_TAG] = sHeadContent;
  data[IMG_PREFIX_TAG] = sImagePrefix;

  switch (sectionType) {
    case SectionType::Amenities:
      sectionPageTemplate = "{{> V2_AmenitiesSectionPage}}";
      data[AMENITIES_SECTION_TAG] = GetAmenitiesSectionData(aPresentationMarker.GetAmenities());
      break;
    case SectionType::Dockage:
      sectionPageTemplate = "{{> V2_DockageSectionPage}}";
      data[DOCKAGE_SECTION_TAG] = GetDockageSectionData(aPresentationMarker.GetDockage());
      break;
    case SectionType::Moorings:
      sectionPageTemplate = "{{> V2_MooringsSectionPage}}";
      data[MOORINGS_SECTION_TAG] = GetMooringsSectionData(aPresentationMarker.GetMoorings());
      break;
    case SectionType::Retail:
      sectionPageTemplate = "{{> V2_RetailSectionPage}}";
      data[RETAIL_SECTION_TAG] = GetRetailSectionData(aPresentationMarker.GetRetail());
      break;
    case SectionType::Services:
      sectionPageTemplate = "{{> V2_ServicesSectionPage}}";
      data[SERVICES_SECTION_TAG] = GetServicesSectionData(aPresentationMarker.GetServices());
      break;
    default:
      DBG_ASSERT_ALWAYS("Invalid compact section type.");
      break;
  }

  kainjow::mustache::mustache mustache(sectionPageTemplate);

  LinkField backButtonLinkField(String::Format("summary/%" PRIu64, aPresentationMarker.GetId()),
                                std::string());
  data[BACK_BUTTON_FIELD_TAG] = GetLinkFieldData(backButtonLinkField);

#if (acdb_WEBVIEW_SUPPORT)
  data[ENABLE_WEB_VIEWS_TAG] = true;
#endif

  MustacheContext context(aRepositoryPtr, &data);

  auto html = mustache.render(context);

  return html;
}  // end of GetSectionPageHtml

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the address section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAddressSectionData(const Address* aAddress) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string STRING_FIELDS_TAG = "StringFields";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aAddress->GetTitle();
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aAddress->GetAttributeFields());
  data[STRING_FIELDS_TAG] = GetStringFieldsData(aAddress->GetStringFields());

  if (!aAddress->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aAddress->GetEditField());
  }

  return data;
}  // end of GetAddressSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the amenities section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetAmenitiesSectionData(const Amenities* aAmenities) {
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aAmenities->GetTitle();
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aAmenities->GetYesNoUnknownNearbyFields());
  if (!aAmenities->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aAmenities->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aAmenities->GetNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aAmenities->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aAmenities->GetEditField());
  }

  if (!aAmenities->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aAmenities->GetSeeAllField());
  }

  return data;
}  // end of GetAmenitySectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the business section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetBusinessSectionData(const Business* aBusiness) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string ATTRIBUTE_MULTI_VALUE_FIELDS_TAG = "AttributeMultiValueFields";
  const std::string BUSINESS_PROMOTION_LIST_TAG = "BusinessPromotionList";
  const std::string CALL_TO_ACTION_TAG = "CallToAction";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aBusiness->GetTitle();
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aBusiness->GetAttributeFields());
  data[ATTRIBUTE_MULTI_VALUE_FIELDS_TAG] =
      GetAttributeFieldsData(aBusiness->GetAttributeMultiValueFields());

  if (!aBusiness->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aBusiness->GetEditField());
  }

  if (aBusiness->GetBusinessPromotionListField() != nullptr &&
      !aBusiness->GetBusinessPromotionListField()->GetBusinessPromotions().empty()) {
    data[BUSINESS_PROMOTION_LIST_TAG] =
        GetBusinessPromotionListFieldData(*aBusiness->GetBusinessPromotionListField());
  }

  if (aBusiness->GetCallToActionField()) {
    data[CALL_TO_ACTION_TAG] = GetLinkFieldData(*aBusiness->GetCallToActionField());
  }

  return data;
}  // end of GetBusinessSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the business section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetCompetitorAdFieldData(
    const CompetitorAdField& aCompetitorAdField) {
  const std::string AD_LABEL_TAG = "AdLabel";
  const std::string PHOTO_URL_TAG = "PhotoUrl";
  const std::string POI_ID_TAG = "PoiId";
  const std::string POI_NAME_TAG = "PoiName";
  const std::string REVIEW_SUMMARY_TAG = "ReviewSummary";
  const std::string TEXT_TAG = "Text";

  kainjow::mustache::data data;

  data[AD_LABEL_TAG] = aCompetitorAdField.GetAdLabel();

  if (!aCompetitorAdField.GetPhotoUrl().empty()) {
    data[PHOTO_URL_TAG] = GetCompetitorAdPhotoData(aCompetitorAdField);
  }

  data[POI_ID_TAG] = String::Format("%" PRIu64, aCompetitorAdField.GetMarkerId());
  data[POI_NAME_TAG] = aCompetitorAdField.GetName();

  if (aCompetitorAdField.GetReviewSummary()) {
    data[REVIEW_SUMMARY_TAG] = GetReviewSummaryData(aCompetitorAdField.GetReviewSummary());
  }

  data[TEXT_TAG] = aCompetitorAdField.GetText();

  return data;
}  // end of GetCompetitorAdFieldData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the business section.
//!
//----------------------------------------------------------------
static kainjow::mustache::list GetCompetitorAdFieldsData(
    const std::vector<CompetitorAdField>& aCompetitorAdFields) {
  const std::string COMPETITOR_ADS_TAG = "CompetitorAds";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::list data;

  for (std::vector<CompetitorAdField>::const_iterator it = aCompetitorAdFields.begin();
       it != aCompetitorAdFields.end(); ++it) {
    data.push_back(GetCompetitorAdFieldData(*it));
  }

  return data;
}  // end of GetCompetitorAdFieldsData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the competitor ad photo.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetCompetitorAdPhotoData(
    const CompetitorAdField& aCompetitorAdField) {
  const std::string PHOTO_URL_TAG = "PhotoUrl";
  const std::string POI_ID_TAG = "PoiId";

  kainjow::mustache::data data;

  data[PHOTO_URL_TAG] = aCompetitorAdField.GetPhotoUrl();
  data[POI_ID_TAG] = String::Format("%" PRIu64, aCompetitorAdField.GetMarkerId());

  return data;
}  // end of GetCompetitorAdPhotoData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the business section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetCompetitorAdSectionData(const CompetitorAd* aCompetitorAd) {
  const std::string COMPETITOR_ADS_TAG = "CompetitorAds";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aCompetitorAd->GetTitle();
  data[COMPETITOR_ADS_TAG] = GetCompetitorAdFieldsData(aCompetitorAd->GetCompetitorAdFields());

  return data;
}  // end of GetCompetitorAdSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the contact section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetContactSectionData(const Contact* aContact) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aContact->GetTitle();
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aContact->GetAttributeFields());

  if (!aContact->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aContact->GetEditField());
  }

  return data;
}  // end of GetContactSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the dockage section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetDockageSectionData(const Dockage* aDockage) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string ATTRIBUTE_PRICE_FIELDS_TAG = "AttributePriceFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_MULTI_VALUE_FIELDS_TAG = "YesNoMultiValueFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aDockage->GetTitle();
  data[YES_NO_MULTI_VALUE_FIELDS_TAG] =
      GetYesNoMultiValueFieldsData(aDockage->GetYesNoMultiValueFields());
  data[ATTRIBUTE_PRICE_FIELDS_TAG] =
      GetAttributePriceFieldsData(aDockage->GetAttributePriceFields());
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aDockage->GetAttributeFields());
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aDockage->GetYesNoUnknownNearbyFields());
  if (!aDockage->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aDockage->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aDockage->GetSectionNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aDockage->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aDockage->GetEditField());
  }

  if (!aDockage->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aDockage->GetSeeAllField());
  }

  return data;
}  // end of GetDockageSectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for LinkField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetLinkFieldData(const LinkField& aLinkField) {
  const std::string LINK_URL_TAG = "LinkUrl";
  const std::string LINK_TEXT_TAG = "LinkText";

  kainjow::mustache::data data;

  data[LINK_URL_TAG] = aLinkField.GetLinkUrl();
  data[LINK_TEXT_TAG] = aLinkField.GetLinkText();

  return data;
}  // end of GetLinkFieldData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the fuel section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetFuelSectionData(const Fuel* aFuel) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_PRICE_FIELDS_TAG = "YesNoPriceFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aFuel->GetTitle();
  data[YES_NO_PRICE_FIELDS_TAG] = GetYesNoPriceFieldsData(aFuel->GetYesNoPriceFields());
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aFuel->GetAttributeFields());
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aFuel->GetYesNoUnknownNearbyFields());
  if (!aFuel->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aFuel->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aFuel->GetSectionNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aFuel->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aFuel->GetEditField());
  }

  return data;
}  // end of GetFuelSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the moorings section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetMooringsSectionData(const Moorings* aMoorings) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_PRICE_FIELDS_TAG = "YesNoPriceFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aMoorings->GetTitle();
  data[YES_NO_PRICE_FIELDS_TAG] = GetYesNoPriceFieldsData(aMoorings->GetYesNoPriceFields());
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aMoorings->GetAttributeFields());
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aMoorings->GetYesNoUnknownNearbyFields());
  if (!aMoorings->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aMoorings->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aMoorings->GetSectionNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aMoorings->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aMoorings->GetEditField());
  }

  if (!aMoorings->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aMoorings->GetSeeAllField());
  }

  return data;
}  // end of GetMooringsSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the navigation section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetNavigationSectionData(const Navigation* aNavigation) {
  const std::string ATTRIBUTE_FIELDS_TAG = "AttributeFields";
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aNavigation->GetTitle();
  data[ATTRIBUTE_FIELDS_TAG] = GetAttributeFieldsData(aNavigation->GetAttributeFields());

  auto note = aNavigation->GetSectionNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aNavigation->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aNavigation->GetEditField());
  }

  return data;
}  // end of GetNavigationSectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for POI section
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetPointOfInterestSectionData(const MarkerDetail& aMarkerDetail) {
  const std::string LAST_MODIFIED_TAG = "LastModified";
  const std::string LOCATION_TAG = "Location";
  const std::string NAME_TAG = "Name";
  const std::string REVIEW_SUMMARY_TAG = "ReviewSummary";
  const std::string BUSINESS_PHOTO_TAG = "BusinessPhoto";
  const std::string SEE_ALL_PHOTOS_TAG = "SeeAllPhotos";

  kainjow::mustache::data data;

  data[NAME_TAG] = aMarkerDetail.GetName();

  if (aMarkerDetail.GetReviewSummary()) {
    data[REVIEW_SUMMARY_TAG] = GetReviewSummaryData(aMarkerDetail.GetReviewSummary());
  }

  if (!aMarkerDetail.GetLastModified().GetValue().empty()) {
    data[LAST_MODIFIED_TAG] = GetAttributeFieldData(aMarkerDetail.GetLastModified());
  }

  data[LOCATION_TAG] = GetStringFieldData(aMarkerDetail.GetLocation());

  if (aMarkerDetail.GetBusinessPhoto()) {
    data[BUSINESS_PHOTO_TAG] = GetBusinessPhotoFieldData(*aMarkerDetail.GetBusinessPhoto());
  }

  if (aMarkerDetail.GetSeeAllPhotosField()) {
    data[SEE_ALL_PHOTOS_TAG] = GetLinkFieldData(*aMarkerDetail.GetSeeAllPhotosField());
  }

  return data;
}  // end of GetPointOfInterestSectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for response field
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetResponseFieldData(const ResponseField* aResponseField) {
  const std::string TITLE_TAG = "Title";
  const std::string TEXT_TAG = "Text";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aResponseField->GetTitle();
  data[TEXT_TAG] = aResponseField->GetText();

  return data;
}  // end of GetResponseFieldData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the retail section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetRetailSectionData(const Retail* aRetail) {
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aRetail->GetTitle();
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aRetail->GetYesNoUnknownNearbyFields());
  if (!aRetail->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aRetail->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aRetail->GetNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aRetail->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aRetail->GetEditField());
  }

  if (!aRetail->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aRetail->GetSeeAllField());
  }

  return data;
}  // end of GetRetailSectionData

//----------------------------------------------------------------
//!
//!       @private
//!       @detail Prepares the input data object for the Mustache
//!               template for the review detail section.
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewDetailSectionData(const ReviewDetail* aReviewDetail) {
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string FEATURED_REVIEW_TAG = "FeaturedReview";
  const std::string REVIEW_SUMMARY_TAG = "ReviewSummary";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aReviewDetail->GetTitle();

  if (aReviewDetail->GetReviewSummary()) {
    data[REVIEW_SUMMARY_TAG] = GetReviewSummaryData(aReviewDetail->GetReviewSummary());
  }

  if (aReviewDetail->GetFeaturedReview() != nullptr) {
    data[FEATURED_REVIEW_TAG] = GetReviewFieldData(*(aReviewDetail->GetFeaturedReview()));
  }

  if (!aReviewDetail->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aReviewDetail->GetEditField());
  }

  if (!aReviewDetail->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aReviewDetail->GetSeeAllField());
  }

  return data;
}  // end of GetReviewDetailSectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review field
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewFieldData(const ReviewField& aReviewField) {
  const std::string CAPTAIN_NAME_TAG = "CaptainName";
  const std::string DATE_VISITED_TAG = "DateVisited";
  const std::string LINK_FIELD_TAG = "LinkField";
  const std::string RESPONSE_TAG = "Response";
  const std::string REVIEW_STARS_TAG = "ReviewStars";
  const std::string REVIEW_TEXT_TAG = "Text";
  const std::string TITLE_TAG = "Title";
  const std::string VOTE_FIELD_TAG = "VoteField";
  const std::string VOTE_COUNT_TAG = "Votes";
  const std::string REVIEW_PHOTOS_TAG = "ReviewPhotos";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aReviewField.GetTitle();
  data[DATE_VISITED_TAG] = aReviewField.GetDateVisited();
  data[CAPTAIN_NAME_TAG] = aReviewField.GetCaptainName();
  data[REVIEW_TEXT_TAG] = aReviewField.GetText();
  data[REVIEW_PHOTOS_TAG] = GetReviewPhotoFieldListData(aReviewField.GetReviewPhotoFields());

  if (!aReviewField.GetStarValues().empty()) {
    data[REVIEW_STARS_TAG] = GetReviewStarData(aReviewField.GetStarValues());
  }

  if (aReviewField.GetVoteField() != nullptr) {
    data[VOTE_FIELD_TAG] = GetLinkFieldData(*(aReviewField.GetVoteField()));

    if (aReviewField.GetVotes() != 0) {
      data[VOTE_COUNT_TAG] = String::Format("%i", aReviewField.GetVotes());
    }
  }

  if (!aReviewField.GetLinkField().GetLinkUrl().empty()) {
    data[LINK_FIELD_TAG] = GetLinkFieldData(aReviewField.GetLinkField());
  }

  if (aReviewField.GetResponseField() != nullptr) {
    data[RESPONSE_TAG] = GetResponseFieldData(aReviewField.GetResponseField());
  }

  return data;
}  // end of GetReviewFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review stars
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::list GetReviewStarData(const std::vector<StringField>& aStarValues) {
  kainjow::mustache::list reviewStars;

  for (auto reviewStar : aStarValues) {
    reviewStars.push_back(GetStringFieldData(reviewStar));
  }

  return reviewStars;
}  // end of GetReviewStarData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for review summary
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewSummaryData(const ReviewSummary* aReviewSummary) {
  const std::string REVIEW_COUNT_TAG = "ReviewCount";
  const std::string REVIEW_STARS_TAG = "ReviewStars";

  kainjow::mustache::data data;

  data[REVIEW_COUNT_TAG] = String::Format("%i", aReviewSummary->GetReviewCount());

  if (!aReviewSummary->GetStarValues().empty()) {
    data[REVIEW_STARS_TAG] = GetReviewStarData(aReviewSummary->GetStarValues());
  }

  return data;
}  // end of GetReviewSummaryData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for Services section
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetServicesSectionData(const Services* aServices) {
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string SEE_ALL_FIELD_TAG = "SeeAllField";
  const std::string TITLE_TAG = "Title";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELDS_TAG = "YesNoUnknownNearbyFields";
  const std::string YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG = "YesNoUnknownNearbyFieldPairs";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aServices->GetTitle();
  data[YES_NO_UNKNOWN_NEARBY_FIELDS_TAG] =
      GetYesNoUnknownNearbyFieldListData(aServices->GetYesNoUnknownNearbyFields());
  if (!aServices->GetYesNoUnknownNearbyFieldPairs().empty()) {
    data[YES_NO_UNKNOWN_NEARBY_FIELD_PAIRS_TAG] =
        GetYesNoUnknownNearbyCompactFieldListData(aServices->GetYesNoUnknownNearbyFieldPairs());
  }

  auto note = aServices->GetNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aServices->GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aServices->GetEditField());
  }

  if (!aServices->GetSeeAllField().GetLinkUrl().empty()) {
    data[SEE_ALL_FIELD_TAG] = GetLinkFieldData(aServices->GetSeeAllField());
  }

  return data;
}  // end of GetServicesSectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for StringField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetStringFieldData(const StringField& aStringField) {
  const std::string VALUE_TAG = "Value";

  kainjow::mustache::data data;

  data[VALUE_TAG] = aStringField.GetValue();

  return data;
}  // end of GetStringFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for StringField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetStringFieldsData(const std::vector<StringField>& aStringFields) {
  kainjow::mustache::list data;

  for (auto stringField : aStringFields) {
    data.push_back(GetStringFieldData(stringField));
  }

  return data;
}  // end of GetStringFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for Summary section
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetSummarySectionData(const MarkerDetail& aMarkerDetail) {
  const std::string EDIT_FIELD_TAG = "EditField";
  const std::string POI_TYPE_TAG = "PoiType";
  const std::string SECTION_NOTE_TAG = "SectionNote";
  const std::string TITLE_TAG = "Title";

  kainjow::mustache::data data;

  data[TITLE_TAG] = aMarkerDetail.GetTitle();
  data[POI_TYPE_TAG] = GetAttributeFieldData(aMarkerDetail.GetMarkerType());

  auto note = aMarkerDetail.GetSectionNote();
  if (note) {
    data[SECTION_NOTE_TAG] = GetAttributeFieldData(*note);
  }

  if (!aMarkerDetail.GetEditField().GetLinkUrl().empty()) {
    data[EDIT_FIELD_TAG] = GetLinkFieldData(aMarkerDetail.GetEditField());
  }

  return data;
}  // end of GetSummarySectionData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoMultiValueField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoMultiValueFieldData(
    const YesNoMultiValueField& aYesNoMultiValueField) {
  const std::string CSV_TAG = "Values";

  kainjow::mustache::data data = GetYesNoUnknownNearbyFieldData(aYesNoMultiValueField);

  if (!aYesNoMultiValueField.GetCsvString().empty()) {
    data[CSV_TAG] = aYesNoMultiValueField.GetCsvString();
  }

  return data;
}  // end of GetYesNoMultiValueFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoMultiValueField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoMultiValueFieldsData(
    const std::vector<YesNoMultiValueField>& aYesNoMultiValueFields) {
  kainjow::mustache::list data;

  for (auto yesNoMultiValueField : aYesNoMultiValueFields) {
    data.push_back(GetYesNoMultiValueFieldData(yesNoMultiValueField));
  }

  return data;
}  // end of GetYesNoMultiValueFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoPriceField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoPriceFieldData(const YesNoPriceField& aYesNoPriceField) {
  const std::string PRICE_DATE_TAG = "PriceDate";
  const std::string PRICE_TAG = "Price";
  const std::string PRICING_UNIT_TAG = "PricingUnit";

  kainjow::mustache::data data = GetYesNoUnknownNearbyFieldData(aYesNoPriceField);

  if (!aYesNoPriceField.GetPrice().empty()) {
    data[PRICE_TAG] = aYesNoPriceField.GetPrice();
    data[PRICING_UNIT_TAG] = aYesNoPriceField.GetUnit();
    if (!aYesNoPriceField.GetDate().empty()) {
      data[PRICE_DATE_TAG] = aYesNoPriceField.GetDate();
    }
  }

  return data;
}  // end of GetYesNoPriceFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoPriceField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoPriceFieldsData(
    const std::vector<YesNoPriceField>& aYesNoPriceFields) {
  kainjow::mustache::list data;

  for (auto yesNoPriceField : aYesNoPriceFields) {
    data.push_back(GetYesNoPriceFieldData(yesNoPriceField));
  }

  return data;
}  // end of GetYesNoPriceFieldsData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoUnknownNearbyField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoUnknownNearbyFieldData(
    const YesNoUnknownNearbyField& aYesNoUnknownNearbyField) {
  const std::string ALT_TEXT_TAG = "AltText";
  const std::string FIELD_TAG = "Field";
  const std::string NOTE_TAG = "Note";
  const std::string VALUE_TAG = "Value";

  kainjow::mustache::data data;

  data[FIELD_TAG] = aYesNoUnknownNearbyField.GetLabel();
  data[VALUE_TAG] = aYesNoUnknownNearbyField.GetValue();
  if (!aYesNoUnknownNearbyField.GetNote().empty()) {
    data[NOTE_TAG] = aYesNoUnknownNearbyField.GetNote();
  }
  data[ALT_TEXT_TAG] = aYesNoUnknownNearbyField.GetAltText();

  return data;
}  // end of GetYesNoUnknownNearbyFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for ReviewPhotoField
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewPhotoFieldData(const ReviewPhotoField& aReviewPhotoField) {
  const std::string DOWNLOAD_URL_TAG = "DownloadUrl";

  kainjow::mustache::data data;

  data[DOWNLOAD_URL_TAG] = aReviewPhotoField.GetDownloadUrl();

  return data;
}  // end of GetReviewPhotoFieldData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoUnknownNearbyFieldPair vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoUnknownNearbyCompactFieldListData(
    const std::vector<YesNoUnknownNearbyFieldPair>& aYesNoUnknownNearbyFieldPairs) {
  const std::string LEFT_ITEM_TAG = "LeftItem";
  const std::string RIGHT_ITEM_TAG = "RightItem";

  kainjow::mustache::list data;

  for (auto it = aYesNoUnknownNearbyFieldPairs.begin(); it != aYesNoUnknownNearbyFieldPairs.end();
       ++it) {
    kainjow::mustache::data pairData;

    pairData[LEFT_ITEM_TAG] = GetYesNoUnknownNearbyFieldData(*(it->mLeftItem));

    if (it->mRightItem) {
      pairData[RIGHT_ITEM_TAG] = GetYesNoUnknownNearbyFieldData(*(it->mRightItem));
    }

    data.push_back(pairData);
  }

  return data;
}  // end of GetYesNoUnknownNearbyCompactFieldListData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for YesNoUnknownNearbyField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetYesNoUnknownNearbyFieldListData(
    const std::vector<YesNoUnknownNearbyField>& aYesNoUnknownNearbyFields) {
  kainjow::mustache::list data;

  for (auto yesNounknownNearbyField : aYesNoUnknownNearbyFields) {
    data.push_back(GetYesNoUnknownNearbyFieldData(yesNounknownNearbyField));
  }

  return data;
}  // end of GetYesNoUnknownNearbyFieldListData

//----------------------------------------------------------------
//!
//!   @brief Get Mustache data for ReviewPhotoField vector
//!   @return Mustache data container
//!
//----------------------------------------------------------------
static kainjow::mustache::data GetReviewPhotoFieldListData(
    const std::vector<ReviewPhotoField>& aReviewPhotoFields) {
  kainjow::mustache::list data;

  for (auto reviewPhotoField : aReviewPhotoFields) {
    data.push_back(GetReviewPhotoFieldData(reviewPhotoField));
  }

  return data;
}  // end of GetReviewPhotoFieldListData

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Set <head> tag content to use when rendering
//!
//----------------------------------------------------------------
void SetHeadContent(const std::string& aHeadContent) {
  sHeadContent = aHeadContent;
}  // end of SetHeadContent

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Set prefix for local image paths
//!
//----------------------------------------------------------------
void SetImagePrefix(const std::string& aImagePrefix) {
  sImagePrefix = aImagePrefix;
}  // end of SetImagePrefix

}  // end of namespace Presentation
}  // end of namespace Acdb
