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
@brief Represents the ActiveCaptain community database
BusinessPhotoField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "BusinessPhotoField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/BusinessPhotoField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
BusinessPhotoField::BusinessPhotoField(std::string&& aDownloadUrl)
    : mDownloadUrl(aDownloadUrl) {}  // end of BusinessPhotoField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool BusinessPhotoField::operator==(const BusinessPhotoField& aRhs) const {
  return mDownloadUrl == aRhs.mDownloadUrl;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the downloadUrl member
//!
//----------------------------------------------------------------
const std::string& BusinessPhotoField::GetDownloadUrl() const {
  return mDownloadUrl;
}  // end of GetDownloadUrl

}  // end of namespace Presentation
}  // end of namespace Acdb
