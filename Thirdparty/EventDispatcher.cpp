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
    @detail
       Stubs for EventDispatcher functions.

    Copyright 2020 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "EventDispatcher"

#include "Acdb/EventDispatcher.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {

/* static */ MessageData::MessageData() {
  memset(this, 0, sizeof(MessageData));
}  // End of MessageData

/* static */ void EventDispatcher::SendEvent(const MessageId aMessageId) {
  // Not used in third-party builds.
}  // EventDispatcher

/* static */ void EventDispatcher::SendEvent(const MessageId aMessageId, const MessageData& aData) {
  // Not used in third-party builds.
}  // EventDispatcher
}  // end of namespace Acdb
