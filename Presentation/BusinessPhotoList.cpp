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
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "BusinessPhotoList"

#include <vector>

#include "DBG_pub.h"
#include "ACDB_pub_types.h"
#include "Acdb/Presentation/BusinessPhotoList.hpp"
#include "GRM_pub_lib.h"

#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessPhotoList::BusinessPhotoList(std::string&& aTitle,
                                     std::vector<BusinessPhotoField>&& aBusinessPhotos,
                                     LinkField&& aBackButtonField)
    : mTitle(std::move(aTitle)),
      mBusinessPhotos(std::move(aBusinessPhotos)),
      mBackButtonField(std::move(aBackButtonField)) {
}  // end of BusinessPhotoList::BusinessPhotoList

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to back button field
//!
//----------------------------------------------------------------
const LinkField& BusinessPhotoList::GetBackButton() const {
  return mBackButtonField;
}  // end of BusinessPhotoList::GetBackButton()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to business photos
//!
//----------------------------------------------------------------
const std::vector<BusinessPhotoField>& BusinessPhotoList::GetBusinessPhotos() const {
  return mBusinessPhotos;
}  // end of BusinessPhotoList::GetBusinessPhotos()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return const reference to title
//!
//----------------------------------------------------------------
const std::string& BusinessPhotoList::GetTitle() const {
  return mTitle;
}  // end of BusinessPhotoList::GetTitle()

}  // end of namespace Presentation
}  // end of namespace Acdb
