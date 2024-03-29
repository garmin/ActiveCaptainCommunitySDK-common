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
    Copyright 2017 by Garmin Ltd. or its subsidiaries.
*/

#if !defined ACDB_EVENT_DISPATCHER_HPP
#define ACDB_EVENT_DISPATCHER_HPP

#include "Acdb/PrvTypes.hpp"

namespace Acdb {
class EventDispatcher {
 public:
  static void SendEvent(const MessageId aMessageId);

  static void SendEvent(const MessageId aMessageId, const MessageData& aData);
};
}  // namespace Acdb
#endif /* ACDB_EVENT_DISPATCHER_HPP */
