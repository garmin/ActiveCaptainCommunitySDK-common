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
    Encapsulates all database translation operations.

    Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_TranslationAdapter_hpp
#define ACDB_TranslationAdapter_hpp

#include "Acdb/Queries/TranslatorQuery.hpp"

namespace Acdb {
class TextTranslator;

class TranslationAdapter {
 public:
  TranslationAdapter(SQLite::Database& aDatabase);

  void InitTextTranslator(const std::string& aLanguage);

 private:
  // Variables
  TranslatorQuery mTranslator;

};  // end of class TranslatorAdapter
}  // end of namespace Acdb

#endif  // end of ACDB_TranslatorAdapter_hpp
