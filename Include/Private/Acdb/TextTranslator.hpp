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
Definition of the Translation Manager class

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_TextTranslator_hpp
#define ACDB_TextTranslator_hpp

#include <unordered_map>

namespace Acdb {
class TextTranslator {
 public:
  // functions
  TextTranslator(TextTranslator const&) = delete;
  TextTranslator(TextTranslator&&) = delete;
  TextTranslator& operator=(TextTranslator const&) = delete;
  TextTranslator& operator=(TextTranslator&&) = delete;

  void Clear();

  std::string Find(const int aTranslationId) const;

  static TextTranslator& GetInstance();

  bool Insert(const int aTranslationId, std::string&& aValue);

 private:
  // functions
  TextTranslator();

  // Variables
  std::unordered_map<int, std::string> mValue;

};  // end of class TextTranslator
}  // end of namespace Acdb

#endif  // end of ACDB_TextTranslator_hpp
