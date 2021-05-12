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
    @brief Regression tests for the TranslationAdapter

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "TranslationAdapterTests"

#include "Acdb/Tests/DatabaseUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/TextTranslator.hpp"
#include "Acdb/TranslationAdapter.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test getting TextTranslator.
//!
//----------------------------------------------------------------
TF_TEST("acdb.translationadapter.get") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TranslationUtil translationUtil{state};
  TextTranslator::GetInstance().Clear();

  TranslationAdapter translationAdapter{database};

  PopulateTranslationsTable(state, database);

  std::vector<std::string> expected = {"en_US [1]", "en_US [2]", "en_US [3]", "en_US [4]",
                                       "MISSING STRING! [5]"};

  std::vector<std::string> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  translationAdapter.InitTextTranslator("en_US");

  for (int i = 1; i <= 5; i++) {
    actual.push_back(TextTranslator::GetInstance().Find(i));
  }

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "TranslationAdapter: Get");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test getting TextTranslator (partially translated
//!         language, use English for missing strings).
//!
//----------------------------------------------------------------
TF_TEST("acdb.translationadapter.get_partial") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TranslationUtil translationUtil{state};
  TextTranslator::GetInstance().Clear();

  TranslationAdapter translationAdapter{database};

  PopulateTranslationsTable(state, database);

  std::vector<std::string> expected = {"pt_BR [1]", "pt_BR [2]", "en_US [3]", "en_US [4]",
                                       "MISSING STRING! [5]"};

  std::vector<std::string> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  translationAdapter.InitTextTranslator("pt_BR");

  for (int i = 1; i <= 5; i++) {
    actual.push_back(TextTranslator::GetInstance().Find(i));
  }

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "TranslationAdapter: Get Partial");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test getting TextTranslator (nonexistent language,
//!         fallback to English).
//!
//----------------------------------------------------------------
TF_TEST("acdb.translationadapter.get_fallback") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  auto database = CreateDatabase(state);

  TranslationUtil translationUtil{state};
  TextTranslator::GetInstance().Clear();

  TranslationAdapter translationAdapter{database};

  PopulateTranslationsTable(state, database);

  std::vector<std::string> expected = {"en_US [1]", "en_US [2]", "en_US [3]", "en_US [4]",
                                       "MISSING STRING! [5]"};

  std::vector<std::string> actual;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  translationAdapter.InitTextTranslator("xx_YY");

  for (int i = 1; i <= 5; i++) {
    actual.push_back(TextTranslator::GetInstance().Find(i));
  }

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "TranslationAdapter: Get Fallback");
}

}  // end of namespace Test
}  // end of namespace Acdb
