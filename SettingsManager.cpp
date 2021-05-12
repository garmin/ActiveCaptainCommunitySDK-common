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
    @brief Contains functionality related to
    ActiveCaptain Settings.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "settings"

#include "Acdb/SettingsManager.hpp"
#include "DBG_pub.h"

#if (acdb_CLOUD_CLIENT_SUPPORT)
#include "CFG_pub.h"
#endif

namespace Acdb {
#if (acdb_CLOUD_CLIENT_SUPPORT)
static const boolean AutoDownloadDefault = true;

CFG_REGISTER_BOOL(acdb_auto_download, AutoDownloadDefault, CFG_PERSIST)
CFG_REGISTER_UINT32(acdb_data_usage, DLM_DATA_USAGE_WIFI_ONLY, CFG_PERSIST)

const std::string SettingsManager::AutoDownloadSettingIdStr{"acdb_auto_download"};
const std::string SettingsManager::DataUsageSettingIdStr{"acdb_data_usage"};
#endif

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
SettingsManager::SettingsManager()
    : mCoordFormat{ACDB_COORD_DEG_MIN},
      mDateFormat{ACDB_DATE_MONTH_ABBR},
      mDistanceUnit{ACDB_METER},
      mVolumeUnit{ACDB_LITER} {}  // end of SettingsManager

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//!   @returns reference to the single instance of the settings
//!            manager
//!
//----------------------------------------------------------------
/*static*/ SettingsManager& SettingsManager::GetInstance() {
  static SettingsManager instance;
  return instance;
}  // end of GetInstance

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//----------------------------------------------------------------
ACDB_coord_format_type SettingsManager::GetCoordinateFormat() const {
  return mCoordFormat;
}  // end of GetCoordinateFormat

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//----------------------------------------------------------------
ACDB_date_format_type SettingsManager::GetDateFormat() const {
  return mDateFormat;
}  // end of GetDateFormat

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//----------------------------------------------------------------
ACDB_unit_type SettingsManager::GetDistanceUnit() const {
  return mDistanceUnit;
}  // end of GetDistanceUnit

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//----------------------------------------------------------------
ACDB_unit_type SettingsManager::GetVolumeUnit() const {
  return mVolumeUnit;
}  // end of GetVolumeUnit

//----------------------------------------------------------------
//!
//!   @public
//!   @brief mutator
//!
//----------------------------------------------------------------
void SettingsManager::SetCoordinateFormat(const ACDB_coord_format_type aCoordFormat) {
  mCoordFormat = aCoordFormat;
}  // end of SetCoordinateFormat

//----------------------------------------------------------------
//!
//!   @public
//!   @brief mutator
//!
//----------------------------------------------------------------
void SettingsManager::SetDateFormat(const ACDB_date_format_type aDateFormat) {
  mDateFormat = aDateFormat;
}  // end of SetDateFormat

//----------------------------------------------------------------
//!
//!   @public
//!   @brief mutator
//!
//----------------------------------------------------------------
void SettingsManager::SetDistanceUnit(const ACDB_unit_type aDistanceUnit) {
  mDistanceUnit = aDistanceUnit;
}  // end of SetDistanceUnit

//----------------------------------------------------------------
//!
//!   @public
//!   @brief mutator
//!
//----------------------------------------------------------------
void SettingsManager::SetVolumeUnit(const ACDB_unit_type aVolumeUnit) {
  mVolumeUnit = aVolumeUnit;
}  // end of SetVolumeUnit

#if (acdb_CLOUD_CLIENT_SUPPORT)
//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Get ACDB automatic download setting.
//!   @return
//!       the ACDB's automatic download setting value
//!
//----------------------------------------------------------------
boolean SettingsManager::GetAutomaticDownload() const {
  boolean autoDownload = AutoDownloadDefault;

  if (CFG_SUCCESS != CFG_bool_value_get(AutoDownloadSettingIdStr.c_str(), &autoDownload)) {
    DBG_ASSERT_ALWAYS("Failed to retrieve the config value");
  }

  return autoDownload;
}  // end of GetAutomaticDownload

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Sets the ACDB's automatic download settings value to the
//!       provided value.
//!
//----------------------------------------------------------------
void SettingsManager::SetAutomaticDownload(const boolean aAutoDownloadEnable) {
  boolean result =
      (CFG_SUCCESS == CFG_bool_value_set(AutoDownloadSettingIdStr.c_str(), aAutoDownloadEnable));
  DBG_ASSERT(result, "Failed to set automatic download value");
}  // end of SetAutomaticDownload

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Get ACDB data usage setting.
//!   @return
//!       the ACDB's data usage setting value
//!
//----------------------------------------------------------------
DLM_data_usage_t32 SettingsManager::GetDataUsage() const {
  DLM_data_usage_t32 dataUsage;

  dataUsage = DLM_DATA_USAGE_COUNT;

  if (CFG_SUCCESS != CFG_uint32_value_get(DataUsageSettingIdStr.c_str(), &dataUsage)) {
    DBG_ASSERT_ALWAYS("Failed to retrieve the config value");
  }

  return dataUsage;
}  // end of GetDataUsage

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!       Sets the ACDB's data usage settings value to the provided
//!       value.
//!
//----------------------------------------------------------------
void SettingsManager::SetDataUsage(const DLM_data_usage_t32 aDataUsage) {
  boolean result;

  result = (aDataUsage < DLM_DATA_USAGE_COUNT);
  DBG_ASSERT(result, "Invalid data usage value %u", aDataUsage);

  if (result) {
    result = (CFG_SUCCESS == CFG_uint32_value_set(DataUsageSettingIdStr.c_str(), aDataUsage));
    DBG_ASSERT(result, "Failed to set data usage value");
  }
}  // end of SetDataUsage
#endif
}  // end of namespace Acdb
