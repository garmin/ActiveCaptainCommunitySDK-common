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
    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ISettingsManager_hpp
#define ACDB_ISettingsManager_hpp

#include "acdb_prv_config.h"
#include "ACDB_pub_types.h"
#if (acdb_CLOUD_CLIENT_SUPPORT)
#include "DLM_pub_types.h"
#endif

namespace Acdb {
class ISettingsManager {
 public:
  // functions
  static ISettingsManager& GetISettingsManager();

  virtual ACDB_coord_format_type GetCoordinateFormat() const = 0;

  virtual ACDB_date_format_type GetDateFormat() const = 0;

  virtual ACDB_unit_type GetDistanceUnit() const = 0;

  virtual ACDB_unit_type GetVolumeUnit() const = 0;

  virtual void SetCoordinateFormat(const ACDB_coord_format_type aCoordFormat) = 0;

  virtual void SetDateFormat(const ACDB_date_format_type aDateFormat) = 0;

  virtual void SetDistanceUnit(const ACDB_unit_type aDistanceUnit) = 0;

  virtual void SetVolumeUnit(const ACDB_unit_type aVolumeUnit) = 0;

#if (acdb_CLOUD_CLIENT_SUPPORT)
  virtual boolean GetAutomaticDownload() const = 0;

  virtual DLM_data_usage_t32 GetDataUsage() const = 0;

  virtual void SetDataUsage(const DLM_data_usage_t32 aDataUsage) = 0;
#endif

};  // end of class ISettingsManager

}  // end of namespace Acdb

#endif  // end of ACDB_ISettingsManager_hpp
