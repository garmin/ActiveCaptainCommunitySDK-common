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
    File system abstraction functions.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACB_FileUtil_hpp
#define ACB_FileUtil_hpp

#include <cstdint>
#include <string>
#include <vector>

namespace Acdb {
namespace FileUtil {

bool Delete(const std::string& aFilePath);

bool Exists(const std::string& aFilePath);

bool GetSize(const std::string& aFilePath, uint64_t& aFileSize);

bool Read(const std::string& aFilePath, char* aBuffer, uint32_t aBytesToRead);

bool Rename(const std::string& aOldPath, const std::string& aNewPath);

}  // end of namespace FileUtil
}  // end of namespace Acdb

#endif  // end of ACB_FileUtil_hpp
