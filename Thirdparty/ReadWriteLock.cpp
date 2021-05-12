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
    @brief read-write lock implementation

    Copyright 2020-2021 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "ReadWriteLock"

#include "Acdb/ReadWriteLock.hpp"
#include "DBG_pub.h"

/*--------------------------------------------------------------------------------------------------
The reader/writer lock generally works as follows:

All threads that want to acquire the lock (either shared or exclusive) will first attempt to acquire
the exclusive mutex.  Once that is acquired, they will acquire the state mutex.

After the state mutex is acquired, the lock behavior diverges based on the type of lock:

For an exclusive (writer) lock, the thread will check if there are currently any readers (by
checking the reader count) and will wait on a condition variable for all of the readers to release
the lock. The thread will never release the exclusive mutex during this time, meaning that no more
readers will be allowed to enter the lock. Once all of the readers have released the lock, the
thread will simply continue on its way and release the exclusive mutex once it releases the lock.

A shared (reader) lock will simply increment the reader count then release both the state and the
exclusive lock. We know that there are no pending writers as soon as we have successfully acquired
the exclusive lock, because a writer keeps the lock while waiting for the readers to exit. When
releasing the lock, the thread will just decrement the reader count and then signal any writer that
may be waiting if the new reader count is zero.

The following rules can be constructed:
1. If there are only readers who have acquired the lock, a writer that attempts to acquire the lock
   will block, but will be guaranteed to be the next thread that acquires the lock.

2. If there is a writer that has acquired the lock or is waiting to acquire the lock, any subsequent
   locks will be queued. Once the writer releases the lock the highest priority thread (regardless
   of whether it is a reader or a writer) will acquire the lock.
--------------------------------------------------------------------------------------------------*/

namespace Acdb {
//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor
//!
//----------------------------------------------------------------
ReadWriteLock::ReadWriteLock()
    : mType{LockType::None},
      mReaders{0},
      mExclusiveLockCount{0},
      mCondition{},
      mStateMutex{},
      mExclusiveMutex{} {}  // end of ReadWriteLock

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Lock for exclusive access
//!
//----------------------------------------------------------------
void ReadWriteLock::LockExclusive() {
  mExclusiveMutex.lock();
  std::unique_lock<std::mutex> stateLock{mStateMutex};

  // Wait for all of the current readers to exit
  while (mReaders != 0) {
    // wait() unlocks mStateMutex and re-locks it when this thread wakes up.
    mCondition.wait(stateLock);
  }

  mType = LockType::Exclusive;
  mExclusiveLockCount++;
}  // end of LockExclusive

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Lock for shared access
//!
//----------------------------------------------------------------
void ReadWriteLock::LockShared() {
  std::unique_lock<std::recursive_mutex> exclusiveLock{mExclusiveMutex};
  std::unique_lock<std::mutex> stateLock{mStateMutex};

  mReaders++;
  mType = LockType::Shared;
}  // end of LockShared

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Unlock
//!
//----------------------------------------------------------------
void ReadWriteLock::Unlock() {
  std::unique_lock<std::mutex> stateLock{mStateMutex};

  if (mType == LockType::Exclusive) {
    mExclusiveMutex.unlock();

    // Decrement the exclusive lock recursion count.  If it is zero (meaning this thread has
    // completely released the lock), change the lock type to none.
    mExclusiveLockCount--;

    if (mExclusiveLockCount == 0) {
      mType = LockType::None;
    }
  } else if (mType == LockType::Shared) {
    if (mReaders > 1) {
      mReaders--;
    } else {
      // No more readers.  Wake up any waiting writer.
      mReaders = 0;
      mType = LockType::None;
      mCondition.notify_one();
    }
  } else {
    // Trying to unlock when the lock is already unlocked.
    DBG_ASSERT_ALWAYS("Read-write lock is already unlocked.");
  }
}  // end of Unlock

}  // end of namespace Acdb
