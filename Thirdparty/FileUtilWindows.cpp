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
    @brief File system utility functions - Windows implementation

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "FileUtil"

#include <io.h>
#include "Acdb/FileUtil.hpp"
#include "Acdb/StringUtil.hpp"
#include "DBG_pub.h"

namespace Acdb {
namespace FileUtil {

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Delete a file.
//!
//----------------------------------------------------------------
bool Delete(const std::string& aFilePath) {
  int rcode = _unlink(aFilePath.c_str());

  bool success = (rcode == 0);
  DBG_D_IF(!success, "Failed to delete file '%s'", aFilePath.c_str());

  return success;
}  // end of Delete

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Determine if file exists and is accessible.
//!
//----------------------------------------------------------------
bool Exists(const std::string& aFilePath) {
  return _access(aFilePath.c_str(), 0) == 0;
}  // end of Exists

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Get size of a file.
//!
//----------------------------------------------------------------
bool GetSize(const std::string& aFilePath, uint64_t& aFileSize) {
  bool success{false};
  struct _stat stats;
  aFileSize = 0;

  int rcode = _stat(aFilePath.c_str(), &stats);

  if (rcode == 0) {
    aFileSize = stats.st_size;
    success = true;
  }

  return success;
}  // end of GetSize

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Read data from a file.
//!
//----------------------------------------------------------------
bool Read(const std::string& aFilePath, char* aBuffer, uint32_t aBytesToRead) {
  bool success{false};
  FILE* file = fopen(aFilePath.c_str(), "r");

  if (file != NULL) {
    uint32_t bytesRead = fread(aBuffer, sizeof(char), aBytesToRead, file);
    success = (aBytesToRead == bytesRead);
    fclose(file);
  }

  return success;
}  // end of Read

//----------------------------------------------------------------
//!
//!       @public
//!       @brief Rename a file.
//!
//----------------------------------------------------------------
bool Rename(const std::string& aOldPath, const std::string& aNewPath) {
  int rcode = rename(aOldPath.c_str(), aNewPath.c_str());

  bool success = (rcode == 0);
  DBG_D_IF(!success, "Failed to rename tmp file");
  return success;
}  // end of Rename

}  // end of namespace FileUtil
}  // end of namespace Acdb
