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
    Translation layer for retrieving/storing marker
    data from/to the database.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MarkerAdapter_hpp
#define ACDB_MarkerAdapter_hpp

#include <vector>

#include "Acdb/Queries/MarkerQuery.hpp"
#include "Acdb/Queries/SearchMarkerQuery.hpp"
#include "Acdb/Queries/ReviewSummaryQuery.hpp"
#include "Acdb/MapMarkerFilter.hpp"
#include "Acdb/SearchMarkerFilter.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/PubTypes.hpp"

namespace Acdb {

class MarkerAdapter {
 public:
  MarkerAdapter(SQLite::Database& aDatabase);

  float GetAverageStars(const ACDB_marker_idx_type aIdx);

  IMapMarkerPtr GetMapMarker(const ACDB_marker_idx_type aIdx);

  void GetMapMarkersByFilter(const MapMarkerFilter& aFilter, std::vector<IMapMarkerPtr>& aResults);

  ISearchMarkerPtr GetSearchMarker(const ACDB_marker_idx_type aIdx);

  void GetBasicSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                     std::vector<ISearchMarkerPtr>& aResults);

  void GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                std::vector<ISearchMarkerPtr>& aResults);

 private:
  MarkerQuery mMarker;
  SearchMarkerQuery mSearchMarker;
  ReviewSummaryQuery mReviewSummary;

};  // end of class MarkerAdapter
}  // end of namespace Acdb

#endif  // end of ACDB_MarkerAdapter_hpp
