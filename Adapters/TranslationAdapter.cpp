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
    @brief Encapsulates all database info operations.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "TranslationAdapter"

#include "DBG_pub.h"
#include "Acdb/TranslationAdapter.hpp"
#include "Acdb/TextTranslator.hpp"

namespace Acdb {

const std::string DefaultLanguage{"en_US"};

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
TranslationAdapter::TranslationAdapter(SQLite::Database& aDatabase)
    : mTranslator{aDatabase} {}  // End of TranslationAdapter

//----------------------------------------------------------------
//!
//!       @private
//!       @brief Read all of the stored translations
//!
//----------------------------------------------------------------
void TranslationAdapter::InitTextTranslator(const std::string& aLanguage) {
  std::vector<TranslationDataType> results;
  bool success = mTranslator.Get(aLanguage, results);
  if (success && results.size() == 0) {
    // we fallback to English if the requested language does not exist
    success = mTranslator.Get(DefaultLanguage, results);
  }

  TextTranslator::GetInstance().Clear();
  if (success) {
    for (auto item : results) {
      TextTranslator::GetInstance().Insert(item.first, std::move(item.second));
    }
  }
}  // end of InitTextTranslator
}  // end of namespace Acdb
