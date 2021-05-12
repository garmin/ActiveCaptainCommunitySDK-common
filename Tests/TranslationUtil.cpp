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
    @brief helper for translations during a test

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "TranslationUtil"

#include "Acdb/StringUtil.hpp"
#include "Acdb/Tests/TranslationUtil.hpp"
#include "Acdb/TextTranslator.hpp"
#include "Acdb/TextHandle.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {
//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Constructor, saves current translations and
//!         reinitializes TextTranslator for tests.
//!
//----------------------------------------------------------------
TranslationUtil::TranslationUtil(TF_state_type* aState) {
  for (ACDB_text_handle_type i = 0;
       i < static_cast<ACDB_text_handle_type>(TextHandle::TextHandleCount); i++) {
    mTranslations[i] = TextTranslator::GetInstance().Find(static_cast<int>(i));
  }

  TextTranslator::GetInstance().Clear();

  for (ACDB_text_handle_type i = 0;
       i < static_cast<ACDB_text_handle_type>(TextHandle::TextHandleCount); i++) {
    TF_assert_msg(aState, TextTranslator::GetInstance().Insert(i, String::Format("[%i]", i)),
                  "TextTranslator: Failed to insert string");
  }
}  // end of TranslationUtil

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Destructor, restores translations to state prior to
//!         tests being run.
//!
//----------------------------------------------------------------
TranslationUtil::~TranslationUtil() {
  TextTranslator::GetInstance().Clear();

  for (ACDB_text_handle_type i = 0;
       i < static_cast<ACDB_text_handle_type>(TextHandle::TextHandleCount); i++) {
    TextTranslator::GetInstance().Insert(static_cast<int>(i), std::move(mTranslations[i]));
  }
}  // end of ~TranslationUtil

}  // end of namespace Test
}  // end of namespace Acdb
