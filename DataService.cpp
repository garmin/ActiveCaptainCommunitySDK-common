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
    Contains content-related functions for ActiveCaptain
    functionality.

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "DataService"

#include <vector>

#include "DBG_pub.h"
#include "ACDB_pub.h"
#include "Acdb/HeadContent.hpp"
#include "Acdb/DataService.hpp"
#include "Acdb/MapMarker.hpp"
#include "Acdb/MapMarkerFilter.hpp"
#include "Acdb/SearchMarkerFilter.hpp"
#include "Acdb/Presentation/MustacheViewFactory.hpp"
#include "Acdb/Presentation/PresentationMarker.hpp"
#include "Acdb/Repository.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
#include "GRM_pub.h"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Constructor
//!
//----------------------------------------------------------------
DataService::DataService(RepositoryPtr aRepositoryPtr, const std::string& aLanguageId)
    : mRepositoryPtr(std::move(aRepositoryPtr)) {
  SetLanguage(aLanguageId);
  SetHeadContent(ACDB_HEADCONTENT);
}  // end of DataService

//----------------------------------------------------------------
//!
//!   @public
//!   @brief
//!       Destructor
//!
//----------------------------------------------------------------
DataService::~DataService() {}  // end of ~DataService

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetPhotoListHtml call from the Repository
//!       member object to provide a public access point for
//!       the single system repository object.
//!   @return
//!       Rendered HTML for the specified marker photo list.
//!
//----------------------------------------------------------------
std::string DataService::GetBusinessPhotoListHtml(const ACDB_marker_idx_type aIdx) const {
  std::string html;

  auto photoListPtr = mRepositoryPtr->GetBusinessPhotoList(aIdx);
  if (photoListPtr) {
    html = Acdb::Presentation::GetBusinessPhotoListHtml(*photoListPtr, mRepositoryPtr);
  }

  return html;
}  // end of GetBusinessPhotoListHtml

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetPresentationMarkerHtml call from the
//!       Repository member object to provide a public access
//!       point for the single system repository object.
//!   @return
//!       Rendered HTML content view map for the specified marker.
//!
//----------------------------------------------------------------
ContentViewMapPtr DataService::GetContentViewMap(const ACDB_marker_idx_type aIdx) const {
  ContentViewMapPtr contentViewMapPtr = nullptr;

  auto presentationMarkerPtr = mRepositoryPtr->GetPresentationMarker(aIdx);
  if (presentationMarkerPtr) {
    auto reviewListPtr = mRepositoryPtr->GetReviewList(aIdx, 1, ReviewLimit);

    contentViewMapPtr = Acdb::Presentation::GetContentViewMap(*presentationMarkerPtr, reviewListPtr,
                                                              mRepositoryPtr);
  }

  return contentViewMapPtr;
}  // end of GetContentViewMap

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetMapMarker call from the Repository member
//!       object to provide a public access point for the single
//!       system repository object.
//!   @return
//!       Pointer to a Marker.
//!
//----------------------------------------------------------------
IMapMarkerPtr DataService::GetMapMarker(const ACDB_marker_idx_type aIdx) const {
  return mRepositoryPtr->GetMapMarker(aIdx);
}  // end of GetMapMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetMapMarkers call from the Repository member
//!       object to provide a public access point for the single
//!       system repository object.
//!
//----------------------------------------------------------------
void DataService::GetMapMarkersByFilter(const MapMarkerFilter& aFilter,
                                        std::vector<IMapMarkerPtr>& aResults) const {
  mRepositoryPtr->GetMapMarkersByFilter(aFilter, aResults);
}  // end of GetMapMarkers

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetPresentationMarkerHtml call from the
//!       Repository member object to provide a public access
//!       point for the single system repository object.
//!   @return
//!       Rendered HTML for the specified marker.
//!
//----------------------------------------------------------------
std::string DataService::GetPresentationMarkerHtml(const ACDB_marker_idx_type aIdx,
                                                   const std::string& aCaptainName) const {
  std::string html;

  auto presentationMarkerPtr = mRepositoryPtr->GetPresentationMarker(aIdx, aCaptainName);
  if (presentationMarkerPtr) {
    html = Acdb::Presentation::GetPresentationMarkerHtml(*presentationMarkerPtr, mRepositoryPtr);
  }

  return html;
}  // end of GetPresentationMarkerHtml

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetReviewListHtml call from the Repository
//!       member object to provide a public access point for
//!       the single system repository object.
//!   @return
//!       Rendered HTML for the specified marker's reviews.
//!
//----------------------------------------------------------------
std::string DataService::GetReviewListHtml(const ACDB_marker_idx_type aIdx, const int aPageNumber,
                                           const int aPageSize,
                                           const std::string& aCaptainName) const {
  std::string html;

  auto reviewListPtr = mRepositoryPtr->GetReviewList(aIdx, aPageNumber, aPageSize, aCaptainName);
  if (reviewListPtr) {
    html = Acdb::Presentation::GetReviewListHtml(*reviewListPtr, mRepositoryPtr);
  }

  return html;
}  // end of GetReviewListHtml

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetSearchMarker call from the Repository member
//!       object to provide a public access point for the single
//!       system repository object.
//!   @return
//!       Pointer to a Marker.
//!
//----------------------------------------------------------------
ISearchMarkerPtr DataService::GetSearchMarker(const ACDB_marker_idx_type aIdx) const {
  return mRepositoryPtr->GetSearchMarker(aIdx);
}  // end of GetSearchMarker

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetBasicSearchMarkersByFilter call from the Repository member
//!       object to provide a public access point for the single
//!       system repository object.
//!
//----------------------------------------------------------------
void DataService::GetBasicSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                                std::vector<ISearchMarkerPtr>& aResults) const {
  mRepositoryPtr->GetBasicSearchMarkersByFilter(aFilter, aResults);
}  // end of GetBasicSearchMarkersByFilter

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Wraps the GetSearchMarkersByFilter call from the Repository member
//!       object to provide a public access point for the single
//!       system repository object.
//!
//----------------------------------------------------------------
void DataService::GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                           std::vector<ISearchMarkerPtr>& aResults) const {
  mRepositoryPtr->GetSearchMarkersByFilter(aFilter, aResults);
}  // end of GetSearchMarkersByFilter

//----------------------------------------------------------------
//!
//!   @public
//!   @return
//!       Rendered HTML for the specified marker and section.
//!
//----------------------------------------------------------------
std::string DataService::GetSectionPageHtml(const ACDB_marker_idx_type aIdx,
                                            const std::string& aSectionName) const {
  std::string html;

  auto presentationMarkerPtr = mRepositoryPtr->GetPresentationMarker(aIdx);
  if (presentationMarkerPtr) {
    html = Acdb::Presentation::GetSectionPageHtml(*presentationMarkerPtr, aSectionName,
                                                  mRepositoryPtr);
  }

  return html;
}  // end of GetSectionPageHtml

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get the average star rating for the specified marker.
//!
//----------------------------------------------------------------
float DataService::GetUserReviewAverageStars(const ACDB_marker_idx_type aIdx) const {
  return mRepositoryPtr->GetUserReviewAverageStars(aIdx);
}  // end of GetUserReviewAverageStars

//----------------------------------------------------------------
//!
//!    @public
//!    @brief
//!        Reinitialize content of <head> tag.
//!
//----------------------------------------------------------------
void DataService::SetHeadContent(const std::string& aHeadContent) {
  Acdb::Presentation::SetHeadContent(aHeadContent);
}  // end of SetHeadContent

//----------------------------------------------------------------
//!
//!    @public
//!    @brief
//!        Reinitialize image prefix.
//!
//----------------------------------------------------------------
void DataService::SetImagePrefix(const std::string& aImagePrefix) {
  Acdb::Presentation::SetImagePrefix(aImagePrefix);
}  // end of SetImagePrefix

//----------------------------------------------------------------
//!
//!    @public
//!    @brief
//!        Reinitialize mTranslator for the current language.
//!
//----------------------------------------------------------------
void DataService::SetLanguage(const std::string& aLanguageId) {
  mRepositoryPtr->SetLanguage(aLanguageId);
}  // end of SetLanguage

}  // end of namespace Acdb
