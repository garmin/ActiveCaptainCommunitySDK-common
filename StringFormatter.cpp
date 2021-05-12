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
    Contains functionality to format date/time, depth, and location.

    Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "StringFormatter"

#include <cmath>
#include "DBG_pub.h"
#include "Acdb/SettingsManager.hpp"
#include "Acdb/StringFormatter.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/TextTranslator.hpp"
#include "NavDateTimeExtensions.hpp"
#include "UTL_pub_lib_cnvt.h"

#define DATE_TIME_STR_MAX_LEN 64
static const char* DEGREE_SYMBOL = "\xC2\xB0";

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
StringFormatter::StringFormatter() {}  // end of StringFormatter

//----------------------------------------------------------------
//!
//!   @public
//!   @brief print a position
//!   @detail formats a position using the user's chosen coordinate format
//!
//----------------------------------------------------------------
std::string StringFormatter::FormatPosition(const scposn_type& aPosition) const {
  std::string result;

  double latitude = aPosition.lat * UTL_SEMI_TO_DEG;
  double longitude = aPosition.lon * UTL_SEMI_TO_DEG;

  std::string latDir = TextTranslator::GetInstance().Find(
      static_cast<int>(latitude >= 0.0f ? TextHandle::NorthAbbr : TextHandle::SouthAbbr));
  std::string lonDir = TextTranslator::GetInstance().Find(
      static_cast<int>(longitude >= 0.0f ? TextHandle::EastAbbr : TextHandle::WestAbbr));

  switch (SettingsManager::GetInstance().GetCoordinateFormat()) {
    case ACDB_COORD_DEG_MIN:
      result =
          String::Format("%s%s, %s%s", GetDegreesMinutesString(latitude).c_str(), latDir.c_str(),
                         GetDegreesMinutesString(longitude).c_str(), lonDir.c_str());
      break;
    case ACDB_COORD_DEG_MIN_SEC:
      result = String::Format("%s%s, %s%s", GetDegreesMinutesSecondsString(latitude).c_str(),
                              latDir.c_str(), GetDegreesMinutesSecondsString(longitude).c_str(),
                              lonDir.c_str());
      break;
    case ACDB_COORD_DEC_DEG:
    default:
      result = String::Format("%0.4f%s%s, %0.4f%s%s", std::abs(latitude), DEGREE_SYMBOL,
                              latDir.c_str(), std::abs(longitude), DEGREE_SYMBOL, lonDir.c_str());
      break;
  }

  return result;
}  // end of FormatPosition

//----------------------------------------------------------------
//!
//!   @public
//!   @brief print a depth
//!   @detail formats a depth value using the user's chosen depth unit
//!
//----------------------------------------------------------------
std::string StringFormatter::FormatDepthValue(const double aMeters) const {
  if (aMeters < 0) {
    return std::string{};
  }

  double value;
  std::string unitString;

  switch (SettingsManager::GetInstance().GetDistanceUnit()) {
    case ACDB_FEET:
      value = aMeters * UTL_MT_TO_FT;
      unitString = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::FeetUnit));
      break;
    case ACDB_METER:
    default:
      value = aMeters;
      unitString = TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MetersUnit));
  }

  return String::Format("%.2f %s", value, unitString.c_str());
}  // end of FormatDepthValue

//----------------------------------------------------------------
//!
//!   @public
//!   @brief print a date
//!   @detail Formats a Unix timestamp as a date string.
//!
//----------------------------------------------------------------
std::string StringFormatter::FormatDate(const uint64_t aUnixTimestamp) const {
  return GetDateString(
      NavDateTimeExtensions::EpochToNavDateTime(Acdb::EpochType::UNIX_EPOCH, aUnixTimestamp));
}  // end of FormatDate

//----------------------------------------------------------------
//!
//!   @public
//!   @brief print a date and time
//!   @detail Formats an ISO 8601 string (yyyy-mm-ddThh:mm:ssZ) as
//!           a local date string.
//!
//----------------------------------------------------------------
std::string StringFormatter::FormatDate(const std::string& aIso8601DateTimeStr) const {
  std::string result;

  NavDateTime navDateTime;

  bool success = navDateTime.FromString(aIso8601DateTimeStr, YYYYMMDDTHHMMSSZ_FORMAT);

  if (!success) {
    // Fallback -- try to convert with milliseconds.
    success = navDateTime.FromString(aIso8601DateTimeStr, YYYYMMDDTHHMMSSMMMZ_FORMAT);
  }

  if (success) {
    result = GetDateString(navDateTime);
  }

  if (result.empty()) {
    DBG_E("Failed to convert date/time from ISO8601 string -- %s", aIso8601DateTimeStr.c_str());
  }

  return result;
}  // end of FormatDate

//----------------------------------------------------------------
//!
//!   @public
//!   @brief accessor
//!
//!   @returns reference to the single instance of the string
//!            formatter
//!
//----------------------------------------------------------------
/*static*/ StringFormatter& StringFormatter::GetInstance() {
  static StringFormatter instance;
  return instance;
}  // end of GetInstance

//----------------------------------------------------------------
//!
//!   @private
//!   @detail Convert NavDateTime to date string
//!
//----------------------------------------------------------------
std::string StringFormatter::GetDateString(const Navionics::NavDateTime& aNavDateTime) const {
  DateStringType dateStringType;
  DateDelimiterToken delimiter;

  std::string result;

  switch (SettingsManager::GetInstance().GetDateFormat()) {
    case ACDB_DATE_MONTH_ABBR: {
      unsigned int month;
      unsigned int day;
      unsigned int year;
      aNavDateTime.GetDate(day, month, year);

      std::string monthStr =
          TextTranslator::GetInstance().Find(static_cast<int>(TextHandle::MonthJan) + month - 1);

      result = String::Format("%u-%s-%u", day, monthStr.c_str(), year);
      break;
    }
    case ACDB_DATE_MDY_SLASH:
      dateStringType = DateStringType::MMDDYYYY_FORMAT;
      delimiter = DateDelimiterToken::DATE_DELIMITER_SLASH;
      break;
    case ACDB_DATE_DMY_SLASH:
      dateStringType = DateStringType::DDMMYYYY_FORMAT;
      delimiter = DateDelimiterToken::DATE_DELIMITER_SLASH;
      break;
    case ACDB_DATE_MDY_DASH:
      dateStringType = DateStringType::MMDDYYYY_FORMAT;
      delimiter = DateDelimiterToken::DATE_DELIMITER_DASH;
      break;
    case ACDB_DATE_DMY_DASH:
    default:
      dateStringType = DateStringType::DDMMYYYY_FORMAT;
      delimiter = DateDelimiterToken::DATE_DELIMITER_DASH;
      break;
  }

  if (result.empty()) {
    aNavDateTime.ToString(result, dateStringType, delimiter);
  }

  return result;
}  // end of GetDateString

//----------------------------------------------------------------
//!
//!   @private
//!   @detail Convert decimal degrees to degrees/minutes string
//!
//----------------------------------------------------------------
std::string StringFormatter::GetDegreesMinutesString(const double aDegrees) const {
  double absDegrees = abs(aDegrees);

  uint32_t degrees = static_cast<uint32_t>(absDegrees);
  double minutes = (absDegrees - degrees) * 60;

  // Check whether minutes will round up to 60.
  if (minutes > 59.9995) {
    minutes = 0;
    degrees++;
  }

  // If updating minutes precision, must make a corresponding change in the rounding check above.
  return String::Format("%02i%s%06.3f'", degrees, DEGREE_SYMBOL, minutes);
}  // end of GetDegreesMinutesString

//----------------------------------------------------------------
//!
//!   @private
//!   @detail Convert decimal degrees to degrees/minutes/seconds string
//!
//----------------------------------------------------------------
std::string StringFormatter::GetDegreesMinutesSecondsString(const double aDegrees) const {
  double absDegrees = abs(aDegrees);

  uint32_t degrees = static_cast<uint32_t>(absDegrees);
  uint32_t minutes = static_cast<uint32_t>((absDegrees - degrees) * 60);
  double seconds = ((absDegrees - degrees) * 3600) - (60 * minutes);

  // Check whether minutes or seconds will round up to 60.
  if (seconds > 59.95) {
    seconds = 0;
    minutes++;
  }

  if (minutes == 60) {
    minutes = 0;
    degrees++;
  }

  // If updating the seconds precision, must make a corresponding change to the rounding check
  // above.
  return String::Format("%02i%s%02i'%04.1f\"", degrees, DEGREE_SYMBOL, minutes, seconds);
}  // end of GetDegreesMinutesSecondsString

}  // end of namespace Acdb
