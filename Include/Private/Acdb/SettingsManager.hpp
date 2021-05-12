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
    SettingsManager definition for ActiveCaptain.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_SettingsManager_hpp
#define ACDB_SettingsManager_hpp

#include <string>
#include "acdb_prv_config.h"
#include "ACDB_pub_types.h"
#include "Acdb/ISettingsManager.hpp"
#if (acdb_CLOUD_CLIENT_SUPPORT)
#include "DLM_pub_types.h"
#endif

namespace Acdb {
class SettingsManager : public ISettingsManager {
 public:
  // functions
  // As this is a singleton we cannot allow copy,
  // move or assignment to be used.
  SettingsManager(SettingsManager const&) = delete;             // copy constructor
  SettingsManager(SettingsManager&&) = delete;                  // move constructor
  SettingsManager& operator=(SettingsManager const&) = delete;  // assignment operator
  SettingsManager& operator=(SettingsManager&&) = delete;       // assignment move operator

  static SettingsManager& GetInstance();

  ACDB_coord_format_type GetCoordinateFormat() const;

  ACDB_date_format_type GetDateFormat() const;

  ACDB_unit_type GetDistanceUnit() const;

  ACDB_unit_type GetVolumeUnit() const;

  void SetCoordinateFormat(const ACDB_coord_format_type aCoordFormat);

  void SetDateFormat(const ACDB_date_format_type aDateFormat);

  void SetDistanceUnit(const ACDB_unit_type aDistanceUnit);

  void SetVolumeUnit(const ACDB_unit_type aVolumeUnit);

#if (acdb_CLOUD_CLIENT_SUPPORT)
  boolean GetAutomaticDownload() const;

  DLM_data_usage_t32 GetDataUsage() const;

  void SetAutomaticDownload(const boolean aAutoDownloadEnable);

  void SetDataUsage(const DLM_data_usage_t32 aDataUsage);

  static const std::string AutoDownloadSettingIdStr;
  static const std::string DataUsageSettingIdStr;
#endif

 private:
  ACDB_coord_format_type mCoordFormat;
  ACDB_date_format_type mDateFormat;
  ACDB_unit_type mDistanceUnit;
  ACDB_unit_type mVolumeUnit;

  SettingsManager();

};  // end of class SettingsManager
}  // end of namespace Acdb

#endif  // end of ACDB_SettingsManager_hpp
