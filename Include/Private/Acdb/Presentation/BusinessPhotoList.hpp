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
    @brief Contains functionality related to
    ActiveCaptain markers.

    Copyright 2017-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_BusinessPhotoList_hpp
#define ACDB_BusinessPhotoList_hpp

#include <memory>
#include <vector>

#include "ACDB_pub_types.h"
#include "Acdb/Presentation/Field/BusinessPhotoField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/PrvTypes.hpp"

namespace Acdb {
namespace Presentation {
//! List of business photos for a marker
class BusinessPhotoList {
 public:
  // public functions
  BusinessPhotoList(std::string&& aTitle, std::vector<BusinessPhotoField>&& aBusinessPhotos,
                    LinkField&& aBackButtonField);

  const LinkField& GetBackButton() const;

  const std::vector<BusinessPhotoField>& GetBusinessPhotos() const;

  const std::string& GetTitle() const;

 private:
  std::string mTitle;
  std::vector<BusinessPhotoField> mBusinessPhotos;
  LinkField mBackButtonField;
};  // end of class BusinessPhotoList

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_BusinessPhotoList_hpp
