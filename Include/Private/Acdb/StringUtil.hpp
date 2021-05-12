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
    String manipulation functions.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_StringUtil_hpp
#define ACDB_StringUtil_hpp

#include <string>
#include <vector>

namespace Acdb {
namespace String {

std::string Format(const char* const aFormatStr, ...);

std::string Join(const std::vector<std::string>& aStrs, const std::string& aSeparator);

void Replace(std::string& aStr, const std::string& aFrom, const std::string& aTo,
             const std::size_t aStartingChar = 0, const bool aReplaceAll = true);

std::vector<std::string> Split(const std::string& aStr, const char aDelimiter,
                               std::size_t aMaxCount = 0);

int ToInt(const std::string& aStr);

unsigned int ToUInt(const std::string& aStr);

uint64_t ToUInt64(const std::string& aStr);

std::string ToLower(const std::string& aStr);

std::string ToUpper(const std::string& aStr);

}  // end of namespace String
}  // end of namespace Acdb

#endif  // end of ACDB_StringUtil_hpp
