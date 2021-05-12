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
    @brief Stubs for conversion macros

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef UTL_PUB_LIB_CNVT_H
#define UTL_PUB_LIB_CNVT_H

#define UTL_DEG_TO_SEMI (2147483648.0 / 180.0)
#define UTL_SEMI_TO_DEG (180.0 / 2147483648.0)

#define UTL_FT_TO_MT (0.3048)
#define UTL_MT_TO_FT (1.0 / UTL_FT_TO_MT);

#endif
