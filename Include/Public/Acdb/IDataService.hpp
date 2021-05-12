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

#ifndef ACDB_IDataService_hpp
#define ACDB_IDataService_hpp

#include <memory>
#include <string>
#include <vector>

#include "ACDB_pub_types.h"
#include "Acdb/PubTypes.hpp"
#include "GRM_pub.h"

namespace Acdb {
class MapMarkerFilter;
class SearchMarkerFilter;

class IDataService {
 public:
  // Functions
  virtual ~IDataService() = default;

  virtual std::string GetBusinessPhotoListHtml(const ACDB_marker_idx_type aIdx) const = 0;

  virtual ContentViewMapPtr GetContentViewMap(const ACDB_marker_idx_type aIdx) const = 0;

  virtual IMapMarkerPtr GetMapMarker(const ACDB_marker_idx_type aIdx) const = 0;

  virtual void GetMapMarkersByFilter(const MapMarkerFilter& aFilter,
                                     std::vector<IMapMarkerPtr>& aResults) const = 0;

  virtual std::string GetPresentationMarkerHtml(
      const ACDB_marker_idx_type aIdx, const std::string& aCaptainName = std::string()) const = 0;

  virtual std::string GetReviewListHtml(const ACDB_marker_idx_type aIdx, const int aPageNumber,
                                        const int aPageSize,
                                        const std::string& aCaptainName) const = 0;

  virtual ISearchMarkerPtr GetSearchMarker(const ACDB_marker_idx_type aIdx) const = 0;

  virtual void GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                        std::vector<ISearchMarkerPtr>& aResults) const = 0;

  virtual std::string GetSectionPageHtml(const ACDB_marker_idx_type aIdx,
                                         const std::string& aSectionName) const = 0;

  virtual float GetUserReviewAverageStars(const ACDB_marker_idx_type aIdx) const = 0;

  virtual void SetHeadContent(const std::string& aHeadContent) = 0;

  virtual void SetImagePrefix(const std::string& aImagePrefix) = 0;

  virtual void SetLanguage(const std::string& aLanguageId) = 0;

};  // end of class IDataService
}  // end of namespace Acdb

#endif  // end of ACDB_IDataService_hpp
