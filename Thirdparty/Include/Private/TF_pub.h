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
    @brief TF/Catch translation macros

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef TF_pub_h
#define TF_pub_h

#include <catch2/catch.hpp>
#define TF_TEST TEST_CASE
#define TF_assert(s, e) REQUIRE(e)
#define TF_assert_msg(s, e, m, ...) REQUIRE(e)
#define TF_state_type void
#define state nullptr

#endif
