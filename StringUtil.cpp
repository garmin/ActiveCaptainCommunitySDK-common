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

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <memory>
#include <sstream>
#include "Acdb/StringUtil.hpp"

namespace Acdb {
namespace String {

//----------------------------------------------------------------
//!
//!   @public
//!   @return Formatted string, similar to printf
//!
//----------------------------------------------------------------
std::string Format(const char* const aFormatStr, ...) {
  va_list vaArgs;
  va_start(vaArgs, aFormatStr);

  // Determine length of the result string.
  va_list vaArgsCopy;
  va_copy(vaArgsCopy, vaArgs);
  const int length =
      std::vsnprintf(nullptr, 0, aFormatStr, vaArgsCopy) + 1;  // + 1 for null-terminator
  va_end(vaArgsCopy);

  // vsnprintf can return a negative number.
  if (length <= 0) {
    return std::string();
  }

  // Write the result string and return it.
  auto result = std::make_unique<char[]>(length);
  std::vsnprintf(result.get(), length, aFormatStr, vaArgs);
  va_end(vaArgs);

  return std::string(result.get(), result.get() + length - 1);
}  // end of Format()

//----------------------------------------------------------------
//!
//!   @public
//!   @return Concatenated string, with separator between elements
//!
//----------------------------------------------------------------
std::string Join(const std::vector<std::string>& aStrs, const std::string& aSeparator) {
  std::stringstream stringstream;
  for (const auto& str : aStrs) {
    // If we're not processing the first element in the vector, add the separator to the output.
    if (&str != &aStrs[0]) {
      stringstream << aSeparator;
    }

    stringstream << str;
  }

  return stringstream.str();
}  // end of Join()

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Find/replace instances of one string with another
//!
//----------------------------------------------------------------
void Replace(std::string& aStr, const std::string& aFrom, const std::string& aTo,
             const std::size_t aStartingChar, const bool aReplaceAll) {
  std::size_t posn = aStr.find(aFrom, aStartingChar);
  bool finished = (posn == std::string::npos);

  while (!finished) {
    aStr.replace(posn, aFrom.length(), aTo);
    if (aReplaceAll) {
      posn = aStr.find(aFrom, posn + aTo.length());
      finished = (posn == std::string::npos);
    } else {
      finished = true;
    }
  }
}  // end of Replace()

//----------------------------------------------------------------
//!
//!   @public
//!   @return Vector of strings, split by given delimiter
//!
//----------------------------------------------------------------
std::vector<std::string> Split(const std::string& aStr, const char aDelimiter,
                               std::size_t aMaxCount) {
  std::string remainder{aStr};
  std::vector<std::string> results;

  while (!remainder.empty() && (aMaxCount == 0 || results.size() < aMaxCount)) {
    size_t idx = remainder.find_first_of(aDelimiter, 0);
    if (idx == std::string::npos) {
      results.push_back(remainder);
      remainder.clear();
    } else {
      results.push_back(remainder.substr(0, idx));
      remainder = remainder.substr(idx + 1);
    }
  }

  return results;
}  // end of Split()

//----------------------------------------------------------------
//!
//!   @public
//!   @return Integer interpreted from given string
//!
//----------------------------------------------------------------
int ToInt(const std::string& aStr) { return std::stoi(aStr); }  // end of ToInt()

//----------------------------------------------------------------
//!
//!   @public
//!   @return Unsigned integer interpreted from given string
//!
//----------------------------------------------------------------
unsigned int ToUInt(const std::string& aStr) {
  return static_cast<unsigned int>(std::stoul(aStr.c_str(), nullptr, 10));
}  // end of ToUInt()

//----------------------------------------------------------------
//!
//!   @public
//!   @return 64-bit unsigned integer interpreted from given string
//!
//----------------------------------------------------------------
uint64_t ToUInt64(const std::string& aStr) {
  return static_cast<uint64_t>(std::stoull(aStr.c_str(), nullptr, 10));
}  // end of ToUInt64()

//----------------------------------------------------------------
//!
//!   @public
//!   @return String with all letters converted to lowercase
//!
//----------------------------------------------------------------
std::string ToLower(const std::string& aStr) {
  std::string result;
  result.resize(aStr.size());

  std::transform(aStr.begin(), aStr.end(), result.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  return result;
}  // end of ToLower()

//----------------------------------------------------------------
//!
//!   @public
//!   @return String with all letters converted to uppercase
//!
//----------------------------------------------------------------
std::string ToUpper(const std::string& aStr) {
  std::string result;
  result.resize(aStr.size());

  std::transform(aStr.begin(), aStr.end(), result.begin(),
                 [](unsigned char c) { return std::toupper(c); });

  return result;
}  // end of ToUpper()

}  // end of namespace String
}  // end of namespace Acdb
