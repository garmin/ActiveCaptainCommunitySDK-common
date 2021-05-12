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
@brief Represents the ActiveCaptain community database
TextTranslator from the SQLite database

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "TextTranslator"

#include <unordered_map>
#include "DBG_pub.h"
#include "Acdb/StringUtil.hpp"
#include "Acdb/TextTranslator.hpp"

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
TextTranslator::TextTranslator() : mValue() {}  // end of TextTranslator

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Erases all text handle/string pairs.
//!
//----------------------------------------------------------------
void TextTranslator::Clear() { mValue.clear(); }  // end of Clear

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Accessor
//!
//!   @return translation for given text handle ID
//!
//----------------------------------------------------------------
std::string TextTranslator::Find(const int aTranslationId) const {
  std::string result;
  auto record = mValue.find(aTranslationId);
  if (mValue.end() != record) {
    result = record->second;
  } else {
    result = String::Format("MISSING STRING! [%i]", aTranslationId);
  }

  return result;
}  // end of Find

//----------------------------------------------------------------
//!
//!       @public
//!       @brief accessor
//!
//!       @returns reference to the single instance
//!                of the text translator
//!
//----------------------------------------------------------------
/*static*/ TextTranslator& TextTranslator::GetInstance() {
  static TextTranslator instance;
  return instance;
}  // end of GetInstance

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Modifier
//!
//!   @return success of operation
//!
//----------------------------------------------------------------
bool TextTranslator::Insert(const int aTranslationId, std::string&& aValue) {
  return mValue.insert(std::make_pair(aTranslationId, std::move(aValue))).second;
}  // end of Insert
}  // end of namespace Acdb
