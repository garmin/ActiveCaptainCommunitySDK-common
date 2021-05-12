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

#include "Acdb/RwlLocker.hpp"

namespace Acdb {

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Constructor, lock the RWL for the lifetime of the object
//!
//----------------------------------------------------------------
RwlLocker::RwlLocker(ReadWriteLock& aReadWriteLock, const bool aExclusive)
    : mReadWriteLock{aReadWriteLock} {
  if (aExclusive) {
    mReadWriteLock.LockExclusive();
  } else {
    mReadWriteLock.LockShared();
  }
}  // end of RwlLocker

//----------------------------------------------------------------
//!
//!   @public
//!   @brief Destructor, releases the lock
//!
//----------------------------------------------------------------
RwlLocker::~RwlLocker() { mReadWriteLock.Unlock(); }  // end of ~RwlLocker

}  // end of namespace Acdb
