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
    SQLiteCpp utility functions.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

// TODO -- Move from ACDB module.
// TODO -- Add user version handling.

#ifndef ACDB_SqliteCppUtil_hpp
#define ACDB_SqliteCppUtil_hpp

#include <memory>
#include <string>
#include <vector>
#include "SQLiteCpp/Database.h"

namespace Acdb {
namespace SqliteCppUtil {

enum class JournalMode { Delete, Truncate, Persist, Wal };

enum class LockingMode { Normal, Exclusive };

bool DropDatabaseFile(const std::string& aPath);

bool DropDatabaseFileExt(const std::string& aPath,
                         const std::vector<std::string>& aVfsIds = std::vector<std::string>{});

bool FlushWalFile(SQLite::Database& aDatabase);

std::unique_ptr<SQLite::Database> OpenDatabaseFile(const std::string& aPath, const int aFlags,
                                                   const int aBusyTimeoutMs = 0);

std::unique_ptr<SQLite::Database> OpenDatabaseFileExt(
    const std::string& aPath, const int aFlags, const int aBusyTimeoutMs = 0,
    const std::vector<std::string>& aVfsIds = std::vector<std::string>{});

bool SetJournalMode(SQLite::Database& aDatabase, const JournalMode aJournalMode);

bool SetLockingMode(SQLite::Database& aDatabase, const LockingMode aLockingMode);

}  // end of namespace SqliteCppUtil
}  // end of namespace Acdb

#endif  // end of ACDB_StringUtil_hpp
