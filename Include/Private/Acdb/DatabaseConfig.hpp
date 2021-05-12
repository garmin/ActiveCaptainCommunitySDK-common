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

    Copyright 2019 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_DatabaseConfig_hpp
#define ACDB_DatabaseConfig_hpp

#include <string>

namespace Acdb {
class DatabaseConfig {
 public:
  static std::string GetBasePath();

  static std::string GetExpandedPath(const std::string& aPath);

};  // end of class DatabaseConfig

}  // end of namespace Acdb

#endif  // end of ACDB_DatabaseConfig_hpp
