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
    @brief Translation utility functions for tests

    Copyright 2017-2018 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_TranslationUtil_hpp
#define ACDB_TranslationUtil_hpp

#include <unordered_map>
#include "ACDB_pub_types.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

class TranslationUtil {
 public:
  TranslationUtil(TF_state_type* aState);
  ~TranslationUtil();

  TranslationUtil(TranslationUtil const&) = delete;             // copy constructor
  TranslationUtil(TranslationUtil&&) = delete;                  // move constructor
  TranslationUtil& operator=(TranslationUtil const&) = delete;  // assignment operator
  TranslationUtil& operator=(TranslationUtil&&) = delete;       // assignment move operator

 private:
  std::unordered_map<int, std::string> mTranslations;
};

}  // end of namespace Test
}  // end of namespace Acdb

#endif  // ACDB_TranslationUtil_hpp
