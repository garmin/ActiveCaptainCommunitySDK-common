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
    @brief Stubs for debug macros

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef DBG_PUB_H
#define DBG_PUB_H

#include <string>

// Prototype this here to avoid circular dependency between ACDB and DBG modules.
namespace Acdb {
namespace String {
std::string Format(const char* const aFormatStr, ...);
}
}  // namespace Acdb

#define DBG_ASSERT(e, m)
#define DBG_ASSERT_ALWAYS(m, ...)
#define DBG_D(m, ...)
#define DBG_D_IF(e, m, ...)
#define DBG_E(m, ...)
#define DBG_E_IF(e, m, ...)
#define DBG_I(m, ...)
#define DBG_I_IF(e, m, ...)
#define DBG_V(m, ...)
#define DBG_V_IF(e, m, ...)
/* workaround -- prevents "unreferenced local variable" warning */
#define DBG_W(...) (Acdb::String::Format(__VA_ARGS__));
#define DBG_W_IF(e, m, ...)

#endif
