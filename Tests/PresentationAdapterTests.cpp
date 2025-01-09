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
    @brief Regression tests for the PresentationAdapter

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "PresentationAdapterTests"

#include "Acdb/PresentationAdapter.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/Tests/SettingsUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextTranslator.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
using namespace Presentation;

namespace Test {
static PresentationMarkerPtr GetExpectedPresentationMarker(TF_state_type* aState);

//----------------------------------------------------------------
//!
//!   @private
//!   @detail
//!         Get expected presentation marker.  Values are used by
//!         tests below.  Modify with caution.
//!
//----------------------------------------------------------------
static PresentationMarkerPtr GetExpectedPresentationMarker(TF_state_type* aState) {
  static const char* DEGREE_SYMBOL = "\xC2\xB0";

  MarkerDetail markerDetail{
      "Test Marina 1",
      ReviewSummaryPtr{new ReviewSummary{2.25, 4, true}},
      AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::DateLastModifiedLabel)),
          "23-[175]-2018", "", ""},
      StringField{
          String::Format("00%s00.001'[148], 00%s00.001'[150]", DEGREE_SYMBOL, DEGREE_SYMBOL)},
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SummaryTitle)),
      AttributeField{"",
                     TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MarinaValue)),
                     "", ""},
      std::unique_ptr<AttributeField>{new AttributeField{"", "Marker note here.", "", ""}},
      LinkField{"edit/1/Summary", ""},
      std::unique_ptr<BusinessPhotoField>{
          new BusinessPhotoField{"https://activecaptain.garmin.com/photos/999.jpg"}},
      std::unique_ptr<LinkField>{new LinkField{
          "photos/1",
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAllPhotos))}}};

  AddressPtr address(
      new Address(TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AddressTitle)),
                  std::vector<StringField>{StringField{"1200 W 151st St"},
                                           StringField{"Olathe, KS 66062"}, StringField{"US"}},
                  std::vector<AttributeField>{
                      AttributeField{"", "www.garmin.com", "", "http://www.garmin.com"}},
                  LinkField("edit/1/Address", "")));

  AmenitiesPtr amenities(new Amenities(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AmenitiesTitle)),
      std::vector<YesNoUnknownNearbyField>{
          YesNoUnknownNearbyField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::BoatRampLabel)),
              "Nearby", "Great stuff nearby.",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NearbyValue))},
          YesNoUnknownNearbyField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CellReceptionLabel)),
              "No", "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NoValue))}},
      std::vector<YesNoUnknownNearbyFieldPair>{YesNoUnknownNearbyFieldPair{
          std::shared_ptr<YesNoUnknownNearbyField>{new YesNoUnknownNearbyField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::BoatRampLabel)),
              "Nearby", "Great stuff nearby.",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NearbyValue))}}}},
      std::unique_ptr<AttributeField>{new AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AmenitiesNotesLabel)),
          "Section note here.", "", ""}},
      LinkField("edit/1/Amenities", ""),
      LinkField("seeAll/1/Amenities",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll)))));

  BusinessPtr business(new Business(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::BusinessTitle)),
      std::vector<AttributeField>{
          AttributeField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PublicLabel)),
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue)), "", ""},
          AttributeField{
              TextTranslator::GetInstance().Find(
                  static_cast<int>(TextHandle::OperatingSeasonLabel)),
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YearRoundValue)), "",
              ""}},
      std::vector<AttributeField>{AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PaymentLabel)),
          std::string{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CashValue)) + ", " +
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CreditValue))},
          "", ""}},
      LinkField("edit/1/BusinessInfo", ""),
      std::unique_ptr<BusinessPromotionListField>{new BusinessPromotionListField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PromotionsLabel)),
          std::vector<BusinessPromotionField>{BusinessPromotionField{
              "Title here", "Details here", "1970-01-01T00:00:00Z", "2100-12-31T23:59:59Z"}}}},
      std::unique_ptr<LinkField>{new LinkField{"http://www.garmin.com", "Click Here!"}}));

  ContactPtr contact(
      new Contact(TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ContactTitle)),
                  std::vector<AttributeField>(), LinkField("edit/1/ContactInfo", "")));

  DockagePtr dockage(new Dockage(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::DockageTitle)),
      std::vector<YesNoMultiValueField>{
          YesNoMultiValueField(
              YesNoUnknownNearbyField(
                  TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::HasDocksLabel)),
                  "Yes", "",
                  TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue))),
              std::string(TextTranslator::GetInstance().Find(
                              static_cast<int>(TextHandle::ConcreteDockValue)) +
                          ", " +
                          TextTranslator::GetInstance().Find(
                              static_cast<int>(TextHandle::FloatingDockValue)) +
                          ", " +
                          TextTranslator::GetInstance().Find(
                              static_cast<int>(TextHandle::TAlongsideDockValue)))),
          YesNoMultiValueField(
              YesNoUnknownNearbyField(
                  TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ShorePowerLabel)),
                  "No", "",
                  TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NoValue))),
              "")},
      std::vector<AttributePriceField>{AttributePriceField(
          AttributeField(TextTranslator::GetInstance().Find(
                             static_cast<int>(TextHandle::AveragePricePerDayLabel)),
                         "", "", ""),
          "1.25 USD",
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PerFootValue)),
          "23-[175]-2018")},
      std::vector<AttributeField>{
          AttributeField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::TotalLabel)), "500",
              "", ""),
          AttributeField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::LoaMaxLabel)),
              std::string(std::string("150.00 ") + TextTranslator::GetInstance().Find(
                                                       static_cast<int>(TextHandle::FeetUnit))),
              "", "")},
      std::unique_ptr<AttributeField>(new AttributeField(
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::DockageNotesLabel)),
          "Section note here.", "", "")),
      std::vector<YesNoUnknownNearbyField>{
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PhoneLabel)),
              "Unknown", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::UnknownValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CableTvLabel)),
              "Nearby", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NearbyValue))),
      },
      std::vector<YesNoUnknownNearbyFieldPair>{YesNoUnknownNearbyFieldPair{
          std::shared_ptr<YesNoUnknownNearbyField>{new YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CableTvLabel)),
              "Nearby", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NearbyValue)))}}},
      LinkField("edit/1/Dockage", ""),
      LinkField("seeAll/1/Dockage",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll)))));

  FuelPtr fuel(new Fuel(TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::FuelTitle)),
                        std::vector<YesNoPriceField>(), std::vector<AttributeField>(),
                        std::vector<YesNoUnknownNearbyField>(),
                        std::vector<YesNoUnknownNearbyFieldPair>(), nullptr,
                        LinkField("edit/1/Fuel", "")));

  MooringsPtr moorings(new Moorings(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MooringsTitle)),
      std::vector<YesNoPriceField>{
          YesNoPriceField{
              YesNoUnknownNearbyField{
                  TextTranslator::GetInstance().Find(
                      static_cast<int>(TextHandle::HasMooringsLabel)),
                  "Yes", "",
                  TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue))},
              "1.00 USD",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PerFootValue)),
              "17-[174]-2018"},
      },
      std::vector<AttributeField>{AttributeField{TextTranslator::GetInstance().Find(
                                                     static_cast<int>(TextHandle::TotalLabel)),
                                                 "772", "", ""},
                                  AttributeField{TextTranslator::GetInstance().Find(
                                                     static_cast<int>(TextHandle::TransientLabel)),
                                                 "100", "", ""}},
      std::unique_ptr<AttributeField>{new AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MooringNotesLabel)),
          "Section note here.", "", ""}},
      std::vector<YesNoUnknownNearbyField>{
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::DinghyLabel)), "No",
              "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NoValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::LaunchLabel)), "Yes",
              "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::LiveaboardLabel)),
              "Unknown", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::UnknownValue)))},
      std::vector<YesNoUnknownNearbyFieldPair>{YesNoUnknownNearbyFieldPair{
          std::shared_ptr<YesNoUnknownNearbyField>{new YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::LaunchLabel)), "Yes",
              "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue)))}}},
      LinkField("edit/1/Moorings", ""),
      LinkField("seeAll/1/Moorings",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll)))));

  NavigationPtr navigation(new Navigation(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NavigationTitle)),
      std::vector<AttributeField>{
          AttributeField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::TideLabel)),
              "1.00 [146]", "", ""},
          AttributeField{
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CurrentLabel)),
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ModerateValue)), "",
              ""}},
      std::unique_ptr<AttributeField>{new AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NavigationNotesLabel)),
          "Section note here.", "", ""}},
      LinkField("edit/1/Navigation", "")));

  RetailPtr retail(new Retail(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::RetailTitle)),
      std::vector<YesNoUnknownNearbyField>{
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(
                  static_cast<int>(TextHandle::FishingSuppliesLabel)),
              "No", "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NoValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::GroceryLabel)), "Yes",
              "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::HardwareLabel)),
              "Unknown", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::UnknownValue)))},
      std::vector<YesNoUnknownNearbyFieldPair>{YesNoUnknownNearbyFieldPair{
          std::shared_ptr<YesNoUnknownNearbyField>{new YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::GroceryLabel)), "Yes",
              "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue)))}}},
      std::unique_ptr<AttributeField>{new AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::RetailNotesLabel)),
          "Section note here.", "", ""}},
      LinkField("edit/1/Retail", ""),
      LinkField("seeAll/1/Retail",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll)))));

  ReviewDetailPtr reviewDetail(new ReviewDetail(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReviewsTitle)),
      std::unique_ptr<ReviewField>(new ReviewField(
          "Review Title 1", 1, "23-[175]-2018", "Test Captain 1", "Review Text 1", true,
          std::unique_ptr<LinkField>(new LinkField("vote/1/1", std::string())), 0,
          LinkField("report/1/1",
                    TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel))),
          nullptr,
          std::vector<ReviewPhotoField>{
              ReviewPhotoField("https://activecaptain.garmin.com/photos/10101.jpg")})),
      LinkField("edit/1/Reviews",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::WriteReview))),
      LinkField("seeAll/1/Reviews/1",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll))),
      ReviewSummaryPtr(new ReviewSummary(2.25, 4, true))));

  ServicesPtr services(new Services(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ServicesTitle)),
      std::vector<YesNoUnknownNearbyField>{
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CarpentryLabel)),
              "No", "", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NoValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CharterLabel)), "Yes",
              "Local", TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue))),
          YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ElectronicsLabel)),
              "Unknown", "",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::UnknownValue)))},
      std::vector<YesNoUnknownNearbyFieldPair>{YesNoUnknownNearbyFieldPair{
          std::shared_ptr<YesNoUnknownNearbyField>{new YesNoUnknownNearbyField(
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::CharterLabel)), "Yes",
              "Local",
              TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::YesValue)))}}},
      std::unique_ptr<AttributeField>{new AttributeField{
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ServicesNotesLabel)),
          "Section note here.", "", ""}},
      LinkField("edit/1/Services", ""),
      LinkField("seeAll/1/Services",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll)))));

  return PresentationMarkerPtr(new PresentationMarker(
      1, std::move(markerDetail), std::move(address), std::move(amenities), std::move(business),
      nullptr, std::move(contact), std::move(dockage), std::move(fuel), std::move(moorings),
      std::move(navigation), std::move(retail), std::move(reviewDetail), std::move(services)));
}  // end of GetExpectedPresentationMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving business photo list.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_business_photo_list", 20) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  TranslationUtil translationUtil{state};

  std::vector<BusinessPhotoField> businessPhotoFields{
      BusinessPhotoField{"https://activecaptain.garmin.com/photos/999.jpg"},
      BusinessPhotoField{"https://activecaptain.garmin.com/photos/1000.jpg"},
  };

  BusinessPhotoList expected{
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::PhotosTitle)),
      std::move(businessPhotoFields), LinkField{"summary/1", ""}};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  BusinessPhotoListPtr actual = presentationAdapter.GetBusinessPhotoList(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "BusinessPhotoList: Unexpected nullptr");
  TF_assert_msg(state, expected.GetTitle() == actual->GetTitle(), "BusinessPhotoList: Title");
  TF_assert_msg(state, expected.GetBusinessPhotos() == actual->GetBusinessPhotos(),
                "BusinessPhotoList: BusinessPhotos");
  TF_assert_msg(state, expected.GetBackButton() == actual->GetBackButton(),
                "BusinessPhotoList: Back button");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving presentation marker.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_presentation_marker", 20) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  PresentationMarkerPtr expected = GetExpectedPresentationMarker(state);

  std::vector<StringField> expectedStarValues{StringField("Full"), StringField("Full"),
                                              StringField("Empty"), StringField("Empty"),
                                              StringField("Empty")};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "PresentationMarker: Unexpected nullptr");
  TF_assert_msg(state, expected->GetId() == actual->GetId(), "PresentationMarker: ID");
  TF_assert_msg(state, expected->GetMarkerDetail() == actual->GetMarkerDetail(),
                "PresentationMarker: MarkerDetail");
  TF_assert_msg(state, actual->GetMarkerDetail().GetReviewSummary() != nullptr,
                "PresentationMarker: ReviewSummary unexpected nullptr");
  TF_assert_msg(state,
                expectedStarValues == actual->GetMarkerDetail().GetReviewSummary()->GetStarValues(),
                "PresentationMarker: ReviewSummary stars");
  TF_assert_msg(state, *(expected->GetAddress()) == *(actual->GetAddress()),
                "PresentationMarker: Address");
  TF_assert_msg(state, *(expected->GetAmenities()) == *(actual->GetAmenities()),
                "PresentationMarker: Amenities");
  TF_assert_msg(state, *(expected->GetBusiness()) == *(actual->GetBusiness()),
                "PresentationMarker: Business");
  TF_assert_msg(
      state, actual->GetCompetitorAd() == nullptr,
      "PresentationMarker: CompetitorAd");  // this POI is a tier 3 and can't be targeted for ads
  TF_assert_msg(state, *(expected->GetContact()) == *(actual->GetContact()),
                "PresentationMarker: Contact");
  TF_assert_msg(state, *(expected->GetDockage()) == *(actual->GetDockage()),
                "PresentationMarker: Dockage");
  TF_assert_msg(state, *(expected->GetFuel()) == *(actual->GetFuel()), "PresentationMarker: Fuel");
  TF_assert_msg(state, *(expected->GetMoorings()) == *(actual->GetMoorings()),
                "PresentationMarker: Moorings");
  TF_assert_msg(state, *(expected->GetNavigation()) == *(actual->GetNavigation()),
                "PresentationMarker: Navigation");
  TF_assert_msg(state, *(expected->GetRetail()) == *(actual->GetRetail()),
                "PresentationMarker: Retail");
  TF_assert_msg(state, *(expected->GetReviewDetail()) == *(actual->GetReviewDetail()),
                "PresentationMarker: ReviewDetail");
  TF_assert_msg(state, *(expected->GetServices()) == *(actual->GetServices()),
                "PresentationMarker: Services");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving presentation marker (checking competitor ads).
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_presentation_marker_competitor_ads", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  CompetitorAdField competitorAdField{
      1,
      "Test Marina 1",
      "Stay with us instead!",
      "https://activecaptain.garmin.com/photos/999.jpg",
      ReviewSummaryPtr{new ReviewSummary{2, 4, true}},
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AdLabel))};

  std::vector<CompetitorAdField> competitorAdFields;
  competitorAdFields.push_back(std::move(competitorAdField));

  CompetitorAd expected{TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::AdsTitle)),
                        std::move(competitorAdFields)};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  std::vector<PresentationMarkerPtr> actualAds;
  actualAds.push_back(presentationAdapter.GetMarker(2));
  actualAds.push_back(presentationAdapter.GetMarker(7));

  std::vector<PresentationMarkerPtr> actualNoAds;
  actualNoAds.push_back(presentationAdapter.GetMarker(3));  // Also a tier-3 participant
  actualNoAds.push_back(
      presentationAdapter.GetMarker(8));  // Competitor, but not in top 5 eligible ad targets
  actualNoAds.push_back(presentationAdapter.GetMarker(10));  // Not marked as a competitor

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actualAds.size() == 2, "PresentationMarker: CompetitorAd");
  for (std::size_t i = 0; i < actualAds.size(); i++) {
    TF_assert_msg(state, actualAds[i] != nullptr, "PresentationMarker: Unexpected nullptr");
    TF_assert_msg(state, actualAds[i]->GetCompetitorAd() != nullptr,
                  "PresentationMarker: CompetitorAd nullptr");
    TF_assert_msg(state, expected == *(actualAds[i]->GetCompetitorAd()),
                  "PresentationMarker: CompetitorAd");
  }

  TF_assert_msg(state, actualNoAds.size() == 3, "PresentationMarker: CompetitorAd");
  for (std::size_t i = 0; i < actualNoAds.size(); i++) {
    TF_assert_msg(state, actualNoAds[i] != nullptr, "PresentationMarker: Unexpected nullptr");
    TF_assert_msg(state, actualNoAds[i]->GetCompetitorAd() == nullptr,
                  "PresentationMarker: expected nullptr");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving presentation marker where user wrote featured review.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_presentation_marker_own_review", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  ReviewDetail expected(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReviewsTitle)),
      std::unique_ptr<ReviewField>(new ReviewField(
          "Review Title 1", 1, "23-[175]-2018", "Test Captain 1", "Review Text 1", true, nullptr, 0,
          LinkField("edit/1/Reviews",
                    TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::EditReview))),
          nullptr,
          std::vector<ReviewPhotoField>{
              ReviewPhotoField("https://activecaptain.garmin.com/photos/10101.jpg")})),
      LinkField("edit/1/Reviews",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::WriteReview))),
      LinkField("seeAll/1/Reviews/1",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::SeeAll))),
      ReviewSummaryPtr(new ReviewSummary(2.25, 4, true)));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  PresentationMarkerPtr actual = presentationAdapter.GetMarker(1, "Test Captain 1");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == *(actual->GetReviewDetail()),
                "PresentationMarker: ReviewDetail");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving invalid presentation marker.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_presentation_marker_invalid", 30) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  PresentationMarkerPtr expected = nullptr;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  PresentationMarkerPtr actual = presentationAdapter.GetMarker(ACDB_INVALID_MARKER_IDX);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, nullptr == actual, "PresentationMarker: expected nullptr");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test retrieving review list.
//!
//----------------------------------------------------------------
TF_TEST_AUTO_SLOW("acdb.presentationadapter.get_review_list", 20) {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  PopulateDatabase(state, database);

  PresentationAdapter presentationAdapter{database};

  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  ReviewSummaryPtr reviewSummary(new ReviewSummary(2.25, 4, true));

  std::vector<ReviewField> reviews;
  reviews.push_back(ReviewField(
      "Review Title 2", 2, "23-[175]-2018", "Test Captain 2", "Review Text 2", true,
      std::unique_ptr<LinkField>(new LinkField("vote/1/2", std::string())), 0,
      LinkField("report/1/2",
                TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReportLabel))),
      nullptr,
      std::vector<ReviewPhotoField>{
          ReviewPhotoField("https://activecaptain.garmin.com/photos/10102.jpg"),
          ReviewPhotoField("https://activecaptain.garmin.com/photos/10102-2.jpg")}));

  ReviewList expected(
      TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::ReviewsTitle)),
      std::move(reviewSummary),
      std::unique_ptr<ReviewField>(new ReviewField(
          "Review Title 1", 1, "23-[175]-2018", "Test Captain 1", "Review Text 1", true, nullptr, 0,
          LinkField("edit/1/Reviews",
                    TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::EditReview))),
          nullptr,
          std::vector<ReviewPhotoField>{
              ReviewPhotoField("https://activecaptain.garmin.com/photos/10101.jpg")})),
      std::move(reviews), LinkField("summary/1", std::string()),
      LinkField("edit/1/Reviews", std::string()), nullptr,
      std::unique_ptr<LinkField>(new LinkField(
          "seeAll/1/Reviews/2",
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::NextLabel)))));

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  ReviewListPtr actual = presentationAdapter.GetReviewList(1, 1, 2, "Test Captain 1");

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, actual != nullptr, "ReviewList: Unexpected nullptr");
  TF_assert_msg(state, expected.GetBackButton() == actual->GetBackButton(),
                "ReviewList: back button");
  TF_assert_msg(state, expected.GetEditField() == actual->GetEditField(), "ReviewList: edit field");
  TF_assert_msg(state, *(expected.GetNextField()) == *(actual->GetNextField()),
                "ReviewList: next field");
  TF_assert_msg(state, nullptr == actual->GetPrevField(), "ReviewList: prev field");
  TF_assert_msg(state, expected.GetReviews() == actual->GetReviews(), "ReviewList: reviews");
  TF_assert_msg(state, *(expected.GetReviewSummary()) == *(actual->GetReviewSummary()),
                "ReviewList: review summary");
  TF_assert_msg(state, expected.GetTitle() == actual->GetTitle(), "ReviewList: title");
  TF_assert_msg(state, *(expected.GetUserReview()) == *(actual->GetUserReview()),
                "ReviewList: user review");
}

}  // end of namespace Test
}  // end of namespace Acdb
