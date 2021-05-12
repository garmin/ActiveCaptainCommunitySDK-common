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
    @brief Regression tests for StringFormatter

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "Acdb"
#define DBG_TAG "StringFormatterTests"

#include "ACDB_pub_types.h"
#include "Acdb/SettingsManager.hpp"
#include "Acdb/StringFormatter.hpp"
#include "Acdb/StringUtil.hpp"
#include "Acdb/Tests/SettingsUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextTranslator.hpp"
#include "TF_pub.h"
#include "UTL_pub_lib_cnvt.h"

namespace Acdb {
namespace Test {
static const char* DEGREE_SYMBOL = "\xC2\xB0";

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatDate()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.date.string") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const std::string input{"2018-05-23T09:30:00Z"};

  std::vector<std::string> expected = {
      "23-[175]-2018",  // ACDB_DATE_MONTH_ABBR
      "23/05/2018",     // ACDB_DATE_DMY_SLASH
      "05/23/2018",     // ACDB_DATE_MDY_SLASH
      "23-05-2018",     // ACDB_DATE_DMY_DASH
      "05-23-2018"      // ACDB_DATE_MDY_DASH
  };

  std::vector<ACDB_date_format_type> dateFormats{ACDB_DATE_MONTH_ABBR, ACDB_DATE_DMY_SLASH,
                                                 ACDB_DATE_MDY_SLASH, ACDB_DATE_DMY_DASH,
                                                 ACDB_DATE_MDY_DASH};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto dateFormat : dateFormats) {
    SettingsManager::GetInstance().SetDateFormat(dateFormat);
    actual.emplace_back(StringFormatter::GetInstance().FormatDate(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Date count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Date (string)");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatDate()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.date.timestamp") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const uint64_t input{1527067800};

  std::vector<std::string> expected = {
      "23-[175]-2018",  // ACDB_DATE_MONTH_ABBR
      "23/05/2018",     // ACDB_DATE_DMY_SLASH
      "05/23/2018",     // ACDB_DATE_MDY_SLASH
      "23-05-2018",     // ACDB_DATE_DMY_DASH
      "05-23-2018"      // ACDB_DATE_MDY_DASH
  };

  std::vector<ACDB_date_format_type> dateFormats{ACDB_DATE_MONTH_ABBR, ACDB_DATE_DMY_SLASH,
                                                 ACDB_DATE_MDY_SLASH, ACDB_DATE_DMY_DASH,
                                                 ACDB_DATE_MDY_DASH};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto dateFormat : dateFormats) {
    SettingsManager::GetInstance().SetDateFormat(dateFormat);
    actual.emplace_back(StringFormatter::GetInstance().FormatDate(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Date count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Date (timestamp)");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatDepthValue()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.depth") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const double input = 45.72;

  std::vector<std::string> expected{std::string{"150.00 [146]"},  // ACDB_FEET
                                    std::string{"45.72 [147]"}};  // ACDB_METER

  std::vector<ACDB_unit_type> depthUnits{ACDB_FEET, ACDB_LITER};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto depthUnit : depthUnits) {
    SettingsManager::GetInstance().SetDistanceUnit(depthUnit);
    actual.emplace_back(StringFormatter::GetInstance().FormatDepthValue(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Depth count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Depth");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatPosition()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.position") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const scposn_type input{static_cast<int32_t>(38.8565 * UTL_DEG_TO_SEMI),
                          static_cast<int32_t>(-94.8 * UTL_DEG_TO_SEMI)};

  std::vector<std::string> expected{
      String::Format("38.8565%s[148], 94.8000%s[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEC_DEG
      String::Format("38%s51.390'[148], 94%s48.000'[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN
      String::Format("38%s51'23.4\"[148], 94%s48'00.0\"[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN_SEC
  };

  std::vector<ACDB_coord_format_type> coordinateFormats{ACDB_COORD_DEC_DEG, ACDB_COORD_DEG_MIN,
                                                        ACDB_COORD_DEG_MIN_SEC};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto coordinateFormat : coordinateFormats) {
    SettingsManager::GetInstance().SetCoordinateFormat(coordinateFormat);
    actual.emplace_back(StringFormatter::GetInstance().FormatPosition(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Position count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Position");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatPosition()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.position_min") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const scposn_type input{ACDB_MIN_LAT, ACDB_MIN_LON};  // Min latitude is -90 degrees.

  std::vector<std::string> expected{
      String::Format("90.0000%s[149], 180.0000%s[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEC_DEG
      String::Format("90%s00.000'[149], 180%s00.000'[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN
      String::Format("90%s00'00.0\"[149], 180%s00'00.0\"[151]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN_SEC
  };

  std::vector<ACDB_coord_format_type> coordinateFormats{ACDB_COORD_DEC_DEG, ACDB_COORD_DEG_MIN,
                                                        ACDB_COORD_DEG_MIN_SEC};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto coordinateFormat : coordinateFormats) {
    SettingsManager::GetInstance().SetCoordinateFormat(coordinateFormat);
    actual.emplace_back(StringFormatter::GetInstance().FormatPosition(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Position count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Position (min)");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test StringFormatter::FormatPosition()
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringformatter.position_max") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  SettingsUtil settingsUtil{};
  TranslationUtil translationUtil{state};

  const scposn_type input{ACDB_MAX_LAT, ACDB_MAX_LON};  // Max latitude is 90 degrees.

  std::vector<std::string> expected{
      String::Format("90.0000%s[148], 180.0000%s[150]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEC_DEG
      String::Format("90%s00.000'[148], 180%s00.000'[150]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN
      String::Format("90%s00'00.0\"[148], 180%s00'00.0\"[150]", DEGREE_SYMBOL,
                     DEGREE_SYMBOL),  // ACDB_COORD_DEG_MIN_SEC
  };

  std::vector<ACDB_coord_format_type> coordinateFormats{ACDB_COORD_DEC_DEG, ACDB_COORD_DEG_MIN,
                                                        ACDB_COORD_DEG_MIN_SEC};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  std::vector<std::string> actual;

  for (auto coordinateFormat : coordinateFormats) {
    SettingsManager::GetInstance().SetCoordinateFormat(coordinateFormat);
    actual.emplace_back(StringFormatter::GetInstance().FormatPosition(input));
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected.size() == actual.size(), "Position count");
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "Position (max)");
  }
}

}  // end of namespace Test
}  // end of namespace Acdb
