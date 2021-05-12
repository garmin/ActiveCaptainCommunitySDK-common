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
Definition of the CompetitorAd component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_CompetitorAd_hpp
#define ACDB_CompetitorAd_hpp

#include "Acdb/Presentation/Field/CompetitorAdField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {
class CompetitorAd {
 public:
  // functions
  CompetitorAd(std::string&& aTitle, std::vector<CompetitorAdField>&& aCompetitorAdFields);

  bool operator==(const CompetitorAd& aRhs) const;

  const std::vector<CompetitorAdField>& GetCompetitorAdFields() const;

  const std::string& GetTitle() const;

 private:
  // Variables
  std::string mTitle;
  std::vector<CompetitorAdField> mCompetitorAdFields;
};  // end of class CompetitorAd

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_CompetitorAd_hpp
