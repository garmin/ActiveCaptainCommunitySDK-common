/*------------------------------------------------------------------------------
Copyright 2022 Garmin Ltd. or its subsidiaries.

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
    @brief Regression tests for StringUtil

    Copyright 2022 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "ACDB"
#define DBG_TAG "StringUtilTests"

#include "Acdb/StringUtil.hpp"
#include "DBG_pub.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test HtmlEscape.
//!
//----------------------------------------------------------------
TF_TEST("acdb.stringutil.htmlescape") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  std::vector<std::string> inputs = {
      "nothing to do here",              // no characters to encode
      "abcedfghijklmnopqrstuvwxyz",      // alpha lower - no change
      "ABCEDFGHIJKLMNOPQRSTUVWXYZ",      // alpha upper - no change
      "0123456789!@#$%^*()+[]",          // numbers and symbols - no change
      "<body>",                          // single tag
      "<script>var i = 1000;</script>",  // multiple tags
      "some text & some more text",      // single ampersand
      "some text < some more text",      // single less than
      "some text > some more text",      // single greater than
      "some text \" some more text",     // single "
      "some text ' some more text",      // single '
      "< > \" ' &",                      // one of each
      "\"hello, world\"",                // double quoted string
      "'hello, world'",                  // single quoted string
      "&some <text />"                   // ampersand and a tag
  };

  std::vector<std::string> expected = {
      "nothing to do here",                          // no characters to encode
      "abcedfghijklmnopqrstuvwxyz",                  // alpha lower - no change
      "ABCEDFGHIJKLMNOPQRSTUVWXYZ",                  // alpha upper - no change
      "0123456789!@#$%^*()+[]",                      // numbers and symbols - no change
      "&lt;body&gt;",                                // single tag
      "&lt;script&gt;var i = 1000;&lt;/script&gt;",  // multiple tags
      "some text &amp; some more text",              // single ampersand
      "some text &lt; some more text",               // single less than
      "some text &gt; some more text",               // single greater than
      "some text &quot; some more text",             // single "
      "some text &#39; some more text",              // single '
      "&lt; &gt; &quot; &#39; &amp;",                // one of each
      "&quot;hello, world&quot;",                    // double quoted string
      "&#39;hello, world&#39;",                      // single quoted string
      "&amp;some &lt;text /&gt;"                     // ampersand and a tag
  };

  std::vector<std::string> actual;
  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  for (auto input : inputs) {
    String::HtmlEscape(input);
    actual.emplace_back(input);
  };

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  for (std::size_t i = 0; i < expected.size(); i++) {
    TF_assert_msg(state, expected[i] == actual[i], "StringUtil: HtmlEscape");
  }
}

}  // end of namespace Test
}  // end of namespace Acdb
