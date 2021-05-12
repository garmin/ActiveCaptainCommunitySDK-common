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
    Custom context class for use with Mustache library.

    Copyright 2018 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MustacheContext_hpp
#define ACDB_MustacheContext_hpp

#include "mustache.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
class Repository;

namespace Presentation {
class MustacheContext : public kainjow::mustache::context<std::string> {
 public:
  MustacheContext(RepositoryPtr aRepositoryPtr, const kainjow::mustache::data* aContext);

  const kainjow::mustache::basic_data<std::string>* get_partial(
      const std::string& aName) const override;

 private:
  RepositoryPtr mRepositoryPtr;
  mutable std::unordered_map<std::string, kainjow::mustache::data> m_partials;
};  // end of class MustacheContext

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_MustacheContext_hpp
