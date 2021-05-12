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
    @brief helper for settings during a test

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "SettingsUtil"

#include "Acdb/SettingsManager.hpp"
#include "Acdb/Tests/SettingsUtil.hpp"
#include "DBG_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Constructor, saves current settings and reinitializes
//!         SettingsManager for tests.
//!
//----------------------------------------------------------------
SettingsUtil::SettingsUtil() {
  mCoordinateFormat = SettingsManager::GetInstance().GetCoordinateFormat();
  mDateFormat = SettingsManager::GetInstance().GetDateFormat();
  mDistanceUnit = SettingsManager::GetInstance().GetDistanceUnit();
  mVolumeUnit = SettingsManager::GetInstance().GetVolumeUnit();

  SettingsManager::GetInstance().SetCoordinateFormat(ACDB_COORD_DEG_MIN);
  SettingsManager::GetInstance().SetDateFormat(ACDB_DATE_MONTH_ABBR);
  SettingsManager::GetInstance().SetDistanceUnit(ACDB_FEET);
  SettingsManager::GetInstance().SetVolumeUnit(ACDB_GALLON);
}  // end of SettingsUtil

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Destructor, restores settings to state prior to tests
//!         being run.
//!
//----------------------------------------------------------------
SettingsUtil::~SettingsUtil() {
  SettingsManager::GetInstance().SetCoordinateFormat(mCoordinateFormat);
  SettingsManager::GetInstance().SetDateFormat(mDateFormat);
  SettingsManager::GetInstance().SetDistanceUnit(mDistanceUnit);
  SettingsManager::GetInstance().SetVolumeUnit(mVolumeUnit);
}  // end of ~SettingsUtil

}  // end of namespace Test
}  // end of namespace Acdb
