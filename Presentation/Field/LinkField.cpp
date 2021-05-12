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
LinkField from the sqlite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "LinkField"

#include "DBG_pub.h"

#include "Acdb/Presentation/Field/LinkField.hpp"

namespace Acdb {
namespace Presentation {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
LinkField::LinkField(std::string&& aLinkUrl, std::string&& aLinkText)
    : mLinkUrl(std::move(aLinkUrl)), mLinkText(std::move(aLinkText)) {}  // end of LinkField

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Equality operator
//!
//----------------------------------------------------------------
bool LinkField::operator==(const LinkField& aRhs) const {
  return mLinkUrl == aRhs.mLinkUrl && mLinkText == aRhs.mLinkText;
}  // end of operator==

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the link URL
//!
//----------------------------------------------------------------
const std::string& LinkField::GetLinkUrl() const { return mLinkUrl; }  // end of GetLinkUrl

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return value of the link text
//!
//----------------------------------------------------------------
const std::string& LinkField::GetLinkText() const { return mLinkText; }  // end of GetLinkText

}  // end of namespace Presentation
}  // end of namespace Acdb
