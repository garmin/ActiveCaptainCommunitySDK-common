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
    Maintains the database configuration for ACDB.

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "DatabaseConfig"

#include "acdb_prv_config.h"

#include "Acdb/DatabaseConfig.hpp"
#include "Acdb/StringUtil.hpp"
#include "DBG_pub.h"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       get the database path, as used by the repository
//!
//----------------------------------------------------------------
std::string DatabaseConfig::GetBasePath() {
  // Base path is not used in third-party build.
  return std::string{};
}  // end of GetBasePath

//----------------------------------------------------------------
//!
//!       @public
//!       @details
//!       Converts a local path to the full expanded path.
//!
//!       @returns the path or an empty string on failure.
//!
//----------------------------------------------------------------
std::string DatabaseConfig::GetExpandedPath(const std::string& aPath) {
  // Path expansion is not used in third-party build.
  return aPath;
}  // End of GetExpandedPath()

}  // end of namespace Acdb
