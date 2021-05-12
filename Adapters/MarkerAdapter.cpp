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
    @brief Encapsulates all database marker operations

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "MarkerAdapter"

#include "Acdb/MapMarker.hpp"
#include "Acdb/MarkerAdapter.hpp"
#include "Acdb/MarkerFactory.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
MarkerAdapter::MarkerAdapter(SQLite::Database& aDatabase)
    : mMarker{aDatabase},
      mSearchMarker{aDatabase},
      mReviewSummary{aDatabase} {}  // end of MarkerAdapter

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get the average star rating for the specified marker.
//!
//----------------------------------------------------------------
float MarkerAdapter::GetAverageStars(const ACDB_marker_idx_type aIdx) {
  ReviewSummaryTableDataType reviewSummaryTableData;

  if (!mReviewSummary.Get(aIdx, reviewSummaryTableData)) {
    return ACDB_INVALID_STAR_COUNT;
  };

  return reviewSummaryTableData.mAverageStars;
}  // end of GetAverageStars

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get point with specified ID.
//!
//----------------------------------------------------------------
IMapMarkerPtr MarkerAdapter::GetMapMarker(const ACDB_marker_idx_type aIdx) {
  MapMarkerPtr marker = nullptr;
  MarkerTableDataType markerTableData;
  if (mMarker.Get(aIdx, markerTableData)) {
    marker = Acdb::GetMapMarker(markerTableData);
  }

  return marker;
}  // end of GetMapMarker

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find points in the provided bounding box.
//!
//----------------------------------------------------------------
void MarkerAdapter::GetMapMarkersByFilter(const MapMarkerFilter& aFilter,
                                          std::vector<IMapMarkerPtr>& aResults) {
  std::vector<MarkerTableDataType> markerList;
  if (mMarker.GetFiltered(aFilter, markerList)) {
    for (auto& it : markerList) {
      aResults.push_back(Acdb::GetMapMarker(it));
    }
  }
}  // end of GetMapMarkers

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Get point with specified ID.
//!
//----------------------------------------------------------------
ISearchMarkerPtr MarkerAdapter::GetSearchMarker(const ACDB_marker_idx_type aIdx) {
  ISearchMarkerPtr marker = nullptr;
  ExtendedMarkerDataType markerTableData;
  if (mSearchMarker.Get(aIdx, markerTableData)) {
    marker = Acdb::GetSearchMarker(markerTableData);
  }

  return marker;
}  // end of Get

//----------------------------------------------------------------
//!
//!    @public
//!    @detail
//!    Find points in the provided bounding box.
//!
//----------------------------------------------------------------
void MarkerAdapter::GetSearchMarkersByFilter(const SearchMarkerFilter& aFilter,
                                             std::vector<ISearchMarkerPtr>& aResults) {
  std::vector<ExtendedMarkerDataType> markerList;
  if (mSearchMarker.GetFiltered(aFilter, markerList)) {
    for (auto& it : markerList) {
      aResults.push_back(Acdb::GetSearchMarker(it));
    }
  }
}  // end of GetFiltered

}  // end of namespace Acdb
