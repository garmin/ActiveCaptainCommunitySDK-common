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
    Read-write lock

    Copyright 2020-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ReadWriteLock_hpp
#define ACDB_ReadWriteLock_hpp

#include <condition_variable>
#include <mutex>

namespace Acdb {

class ReadWriteLock {
 public:
  ReadWriteLock();

  void LockExclusive();

  void LockShared();

  void Unlock();

 private:
  enum LockType { None, Shared, Exclusive };

  LockType mType;                        // Current state of the lock
  uint32_t mReaders;                     // Number of readers using the lock
  uint32_t mExclusiveLockCount;          // Number of recursive exclusive locks
  std::condition_variable mCondition;    // Condition variable
  std::mutex mStateMutex;                // State mutex
  std::recursive_mutex mExclusiveMutex;  // Exclusive mutex
};
}  // namespace Acdb

#endif  // ACDB_ReadWriteLock_hpp
