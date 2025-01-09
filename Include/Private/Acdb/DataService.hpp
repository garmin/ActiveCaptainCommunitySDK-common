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

#ifndef ACDB_DataService_hpp
#define ACDB_DataService_hpp

#include <memory>
#include <vector>

#include "Acdb/IDataService.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"
#include "Acdb/StringFormatter.hpp"
#include "Acdb/TextTranslator.hpp"
#include "GRM_pub.h"

namespace Acdb {
class MapMarkerFilter;
class Repository;
class SearchMarkerFilter;

class DataService : public IDataService {
 public:
  // Functions
  DataService(RepositoryPtr aRepositoryPtr, const std::string& aLanguage);

  virtual ~DataService();

  std::string GetBusinessPhotoListHtml(const ACDB_marker_idx_type aIdx) const override;

  ContentViewMapPtr GetContentViewMap(const ACDB_marker_idx_type aIdx) const override;

  IMapMarkerPtr GetMapMarker(const ACDB_marker_idx_type aIdx) const override;

  void GetMapMarkersByFilter(const MapMarkerFilter& aFilter,
                             std::vector<IMapMarkerPtr>& aResults) const override;

  std::string GetPresentationMarkerHtml(
      const ACDB_marker_idx_type aIdx,
      const std::string& aCaptainName = std::string()) const override;

  std::string GetReviewListHtml(const ACDB_marker_idx_type aIdx, const int aPageNumber,
                                const int aPageSize,
                                const std::string& aCaptainName) const override;

  ISearchMarkerPtr GetSearchMarker(const ACDB_marker_idx_type aIdx) const override;

  void GetBasicSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                     std::vector<ISearchMarkerPtr>& aResults) const override;

  void GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                std::vector<ISearchMarkerPtr>& aResults) const override;

  std::string GetSectionPageHtml(const ACDB_marker_idx_type aIdx,
                                 const std::string& aSectionName) const override;

  float GetUserReviewAverageStars(const ACDB_marker_idx_type aIdx) const override;

  void SetHeadContent(const std::string& aHeadContent) override;

  void SetImagePrefix(const std::string& aImagePrefix) override;

  void SetLanguage(const std::string& aLanguageId) override;

 private:
  // Constants
  static const int ReviewLimit = 10;

  // Variables
  RepositoryPtr mRepositoryPtr;

};  // end of class Manager
}  // end of namespace Acdb

#endif  // end of ACDB_DataService_hpp
