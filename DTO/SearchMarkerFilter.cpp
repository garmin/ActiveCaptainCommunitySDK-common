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
    @brief Contains functionality related to filtering
    ActiveCaptain searches

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "SearchMarkerFilter"

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/SearchMarkerFilter.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Default Constructor
//!
//----------------------------------------------------------------
SearchMarkerFilter::SearchMarkerFilter() : mBaseFilter() {}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return filter bounding box
//!
//----------------------------------------------------------------
const bbox_type& SearchMarkerFilter::GetBbox() const { return mBaseFilter.GetBbox(); }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return database filter type value
//!
//----------------------------------------------------------------
uint32_t SearchMarkerFilter::GetAllowedTypes() const { return mBaseFilter.GetAllowedTypes(); }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return search string
//!
//----------------------------------------------------------------
const std::string& SearchMarkerFilter::GetSearchString() const { return mSearchString; }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return string matching mode
//!
//----------------------------------------------------------------
SearchMarkerFilter::StringMatchMode SearchMarkerFilter::GetStringMatchMode() const {
  return mStringMatchMode;
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return database filter category value
//!
//----------------------------------------------------------------
uint64_t SearchMarkerFilter::GetAllowedCategories() const {
  if (0 != mCategoriesBitmask) {
    return mCategoriesBitmask;
  } else {
    // not category filter is set => all categories allowed
    return MarkerCategory::Any;
  }
}  // end of SearchMarkerFilter::GetCategory()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief set the filter's bounding box
//!
//----------------------------------------------------------------
void SearchMarkerFilter::SetBbox(const bbox_type& aBoundingBox) {
  mBaseFilter.SetBbox(aBoundingBox);
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief add another type to the filter's mask
//!
//----------------------------------------------------------------
void SearchMarkerFilter::AddType(ACDB_type_type aType) { mBaseFilter.AddType(aType); }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief add another category to the filter's mask
//!
//----------------------------------------------------------------
void SearchMarkerFilter::AddCategory(MarkerCategory aCategory) {
  mCategoriesBitmask |= static_cast<uint64_t>(aCategory);
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief set the filter's search string
//!
//----------------------------------------------------------------
void SearchMarkerFilter::SetSearchString(const std::string& aSearchString,
                                         StringMatchMode aMatchMode) {
  mSearchString = aSearchString;
  mStringMatchMode = aMatchMode;
}

//----------------------------------------------------------------
//!
//!   @public
//!   @brief get the filter's max result count
//!
//----------------------------------------------------------------
int32_t SearchMarkerFilter::GetMaxResults() const { return mMaxResults; }

//----------------------------------------------------------------
//!
//!   @public
//!   @brief set the filter's max result count.  Unlimited if < 0.
//!
//----------------------------------------------------------------
void SearchMarkerFilter::SetMaxResults(const int32_t aMaxResults) { mMaxResults = aMaxResults; }

}  // end of namespace Acdb
