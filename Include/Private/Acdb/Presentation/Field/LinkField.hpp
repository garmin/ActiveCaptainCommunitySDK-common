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
Definition of the Link field.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_LinkField_hpp
#define ACDB_LinkField_hpp

#include <string>

namespace Acdb {
namespace Presentation {

class LinkField {
 public:
  // functions
  explicit LinkField(std::string&& aLinkUrl, std::string&& aLinkText);

  bool operator==(const LinkField& aRhs) const;

  const std::string& GetLinkUrl() const;

  const std::string& GetLinkText() const;

 private:
  // Variables
  std::string mLinkUrl;
  std::string mLinkText;
};  // end of class LinkField

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_LinkField_hpp
