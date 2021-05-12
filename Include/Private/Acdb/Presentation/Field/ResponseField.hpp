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
Definition of the Response Field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_ResponseField_hpp
#define ACDB_ResponseField_hpp

#include <string>

namespace Acdb {
namespace Presentation {

class ResponseField {
 public:
  // Functions
  ResponseField(std::string&& aTitle, std::string&& aText);

  bool operator==(const ResponseField& aRhs) const;

  const std::string& GetTitle() const;

  const std::string& GetText() const;

 private:
  // Variables
  std::string mTitle;
  std::string mText;

};  // end of class ResponseField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_ReviewField_hpp
