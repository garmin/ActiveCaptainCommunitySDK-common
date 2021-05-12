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
    @brief Settings utility functions for tests

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_SettingsUtil_hpp
#define ACDB_SettingsUtil_hpp

#include "ACDB_pub_types.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

class SettingsUtil {
 public:
  SettingsUtil();
  ~SettingsUtil();

  SettingsUtil(SettingsUtil const&) = delete;             // copy constructor
  SettingsUtil(SettingsUtil&&) = delete;                  // move constructor
  SettingsUtil& operator=(SettingsUtil const&) = delete;  // assignment operator
  SettingsUtil& operator=(SettingsUtil&&) = delete;       // assignment move operator

 private:
  ACDB_coord_format_type mCoordinateFormat;
  ACDB_date_format_type mDateFormat;
  ACDB_unit_type mDistanceUnit;
  ACDB_unit_type mVolumeUnit;
};

}  // end of namespace Test
}  // end of namespace Acdb

#endif  // ACDB_SettingsUtil_hpp
