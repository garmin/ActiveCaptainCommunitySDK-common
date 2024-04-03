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

#define DBG_MODULE "SqliteCpp"
#define DBG_TAG "SqliteCppUtil"

#include <map>
#include "Acdb/SqliteCppUtil.hpp"
#include "Acdb/SqliteCppUtilPrj.hpp"
#include "DBG_pub.h"
#include "sqlite3.h"

namespace Acdb {
namespace SqliteCppUtil {

static const std::map<JournalMode, std::string> JournalModeStrs{{JournalMode::Delete, "delete"},
                                                                {JournalMode::Truncate, "truncate"},
                                                                {JournalMode::Persist, "persist"},
                                                                {JournalMode::Wal, "wal"}};

static const std::map<LockingMode, std::string> LockingModeStrs{
    {LockingMode::Normal, "normal"}, {LockingMode::Exclusive, "exclusive"}};

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Drop database file using the native VFS
//!
//----------------------------------------------------------------
bool DropDatabaseFile(const std::string& aPath) {
  return DropDatabaseFileExt(aPath, std::vector<std::string>{""});
}  // end of DropDatabaseFile

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Drop database file using the specified VFSes.  Each
//!           VFS will be given an opportunity to drop.
//!
//----------------------------------------------------------------
bool DropDatabaseFileExt(const std::string& aPath, const std::vector<std::string>& aVfsIds) {
  bool success{false};
  DBG_V("DropDatabaseFileExt(%s)", aPath.c_str());

  if (aPath.empty()) {
    DBG_W("DropDatabaseFileExt() failed with an invalid database path.");
    return false;
  }

  if (aPath == ":memory:") {
    return true;
  }

  const std::vector<std::string>& vfsIds = aVfsIds.empty() ? DefaultVfsIds : aVfsIds;

  for (std::size_t i = 0; i < vfsIds.size(); i++) {
    sqlite3_vfs* vfs = sqlite3_vfs_find(vfsIds[i].empty() ? nullptr : vfsIds[i].c_str());

    if (vfs != nullptr) {
      std::unique_ptr<char[]> fullPathUtf8 = std::unique_ptr<char[]>{new char[vfs->mxPathname]};

      int sqliteResult =
          vfs->xFullPathname(vfs, aPath.c_str(), vfs->mxPathname, fullPathUtf8.get());

      if ((SQLITE_OK == sqliteResult) && (SQLITE_OK == vfs->xDelete(vfs, fullPathUtf8.get(), 0))) {
        success = true;
      }
    }
  }

  return success;
}  // end of DropDatabaseFileExt

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Flush the WAL file
//!
//----------------------------------------------------------------
bool FlushWalFile(SQLite::Database& aDatabase) {
  const std::string FlushWalFileStr{"PRAGMA wal_checkpoint(TRUNCATE);"};
  bool success{true};

  DBG_D("Flushing WAL file.");

  try {
    SQLite::Statement statement{aDatabase, FlushWalFileStr};

    if (success) {
      success = (SQLITE_ROW == statement.tryExecuteStep());
      DBG_W_IF(!success, "Received incorrect response from SQL_step.");
    }

    if (success) {
      enum Columns {
        Success = 0,         // 0 = success, 1 = failure
        WalPages = 1,        // pages written to the WAL file
        WalPagesWritten = 2  // pages successfully moved back to the database
      };

      uint32_t stmtFailed = statement.getColumn(Columns::Success);
      uint32_t walPages = statement.getColumn(Columns::WalPages);
      uint32_t walPagesWritten = statement.getColumn(Columns::WalPagesWritten);

      DBG_D("Read SQL row: (%u %u %u).", stmtFailed, walPages, walPagesWritten);

      // Make sure the number of pages in the WAL file match the number of pages
      // written back to the database.
      success = (!stmtFailed && (walPages == walPagesWritten));
      DBG_W_IF(!success, "Flushing WAL file failed (%u %u %u).", stmtFailed, walPages,
               walPagesWritten);
    }

    if (success) {
      success = (SQLITE_DONE == statement.tryExecuteStep());
      DBG_W_IF(!success, "Received incorrect response from second SQL_step.");
    }
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    success = false;
  }

  return success;
}  // end of FlushWalFile

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Open database file using the native VFS
//!
//----------------------------------------------------------------
std::unique_ptr<SQLite::Database> OpenDatabaseFile(const std::string& aPath, const int aFlags,
                                                   const int aBusyTimeoutMs) {
  return OpenDatabaseFileExt(aPath, aFlags, aBusyTimeoutMs, std::vector<std::string>{""});
}  // end of OpenDatabaseFile

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Open database file using the specified VFSes.  The
//!           first VFS which opens the file successfully be used.
//!
//----------------------------------------------------------------
std::unique_ptr<SQLite::Database> OpenDatabaseFileExt(const std::string& aPath, const int aFlags,
                                                      const int aBusyTimeoutMs,
                                                      const std::vector<std::string>& aVfsIds) {
  std::unique_ptr<SQLite::Database> result;

  if (aPath.empty()) {
    return result;
  }

  const std::vector<std::string>& vfsIds = aVfsIds.empty() ? DefaultVfsIds : aVfsIds;

  // Attempt each VFS in turn until one succeeds.
  for (std::size_t i = 0; result == nullptr && i < vfsIds.size(); i++) {
    try {
      result.reset(new SQLite::Database{aPath, aFlags, aBusyTimeoutMs, vfsIds[i]});
    } catch (...) {
      result.reset();
    }
  }

  return result;
}  // end of OpenDatabaseFileExt

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Set journal mode PRAGMA
//!           (https://www.sqlite.org/pragma.html#pragma_journal_mode)
//!
//----------------------------------------------------------------
bool SetJournalMode(SQLite::Database& aDatabase, const JournalMode aJournalMode) {
  const std::string JournalModeSql = "PRAGMA journal_mode = ";

  auto it = JournalModeStrs.find(aJournalMode);
  if (it == JournalModeStrs.end()) {
    DBG_ASSERT_ALWAYS("Invalid journal mode");
    return false;
  }

  std::string sql{JournalModeSql + it->second + ";"};
  try {
    aDatabase.exec(sql);
    return true;
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    return false;
  }
}  // end of SetJournalMode

//----------------------------------------------------------------
//!
//!   @public
//!   @detail Set locking mode PRAGMA
//!           (https://www.sqlite.org/pragma.html#pragma_locking_mode)
//!
//----------------------------------------------------------------
bool SetLockingMode(SQLite::Database& aDatabase, const LockingMode aLockingMode) {
  const std::string LockingModeSql = "PRAGMA locking_mode = ";

  auto it = LockingModeStrs.find(aLockingMode);
  if (it == LockingModeStrs.end()) {
    DBG_ASSERT_ALWAYS("Invalid locking mode");
    return false;
  }

  std::string sql{LockingModeSql + it->second + ";"};

  try {
    aDatabase.exec(sql);
    return true;
  } catch (const SQLite::Exception& e) {
    DBG_W("SQLite Exception: %i %s", e.getErrorCode(), e.getErrorStr());
    return false;
  }
}  // end of SetLockingMode

}  // end of namespace SqliteCppUtil
}  // end of namespace Acdb
