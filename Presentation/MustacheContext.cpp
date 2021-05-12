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

#include "Acdb/Presentation/MustacheContext.hpp"
#include "Acdb/Repository.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {

MustacheContext::MustacheContext(RepositoryPtr aRepositoryPtr,
                                 const kainjow::mustache::data* aContext)
    : kainjow::mustache::context<std::string>(aContext),
      mRepositoryPtr(std::move(aRepositoryPtr)) {}  // end of MustacheContext::MustacheContext()

const kainjow::mustache::basic_data<std::string>* MustacheContext::get_partial(
    const std::string& aName) const {
  auto it = m_partials.find(aName);
  if (it != m_partials.end()) {
    return &it->second;
  }

  std::string templateContents = mRepositoryPtr->GetMustacheTemplate(aName);
  if (templateContents.empty()) {
    return nullptr;
  }

  return &m_partials
              .insert(std::make_pair(aName, kainjow::mustache::partial{[templateContents]() {
                                       return templateContents;
                                     }}))
              .first->second;
}  // end of MustacheContext::get_partial()

}  // end of namespace Presentation
}  // end of namespace Acdb
