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
Definition of the MarkerDetail component of a Poi.

Copyright 2018-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MarkerDetail_hpp
#define ACDB_MarkerDetail_hpp

#include "ACDB_pub_types.h"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/TextHandle.hpp"
#include "Acdb/Presentation/Field/AttributeField.hpp"
#include "Acdb/Presentation/Field/BusinessPhotoField.hpp"
#include "Acdb/Presentation/Field/LinkField.hpp"
#include "Acdb/Presentation/Field/ReviewSummary.hpp"
#include "Acdb/Presentation/Field/StringField.hpp"
#include "GRM_pub.h"

namespace Acdb {
namespace Presentation {

class MarkerDetail {
 public:
  // functions
  MarkerDetail(std::string&& aName, ReviewSummaryPtr aReviewSummary, AttributeField&& aLastModified,
               StringField&& aLocation, std::string&& aTitle, AttributeField&& aMarkerType,
               std::unique_ptr<AttributeField> aSectionNote, LinkField&& aEditField,
               std::unique_ptr<BusinessPhotoField> aBusinessPhoto,
               std::unique_ptr<LinkField> aSeeAllPhotosField);

  bool operator==(const MarkerDetail& aRhs) const;

  const BusinessPhotoField* GetBusinessPhoto() const;

  const LinkField& GetEditField() const;

  const AttributeField& GetLastModified() const;

  const StringField& GetLocation() const;

  const AttributeField& GetMarkerType() const;

  const std::string& GetName() const;

  const ReviewSummary* GetReviewSummary() const;

  const AttributeField* GetSectionNote() const;

  const LinkField* GetSeeAllPhotosField() const;

  const std::string& GetTitle() const;

 private:
  // Variables
  std::string mName;
  ReviewSummaryPtr mReviewSummary;
  AttributeField mLastModified;
  StringField mLocation;
  std::string mTitle;
  AttributeField mMarkerType;
  std::unique_ptr<AttributeField> mSectionNote;

  LinkField mEditField;

  std::unique_ptr<BusinessPhotoField> mBusinessPhoto;
  std::unique_ptr<LinkField> mSeeAllPhotosField;

};  // end of class MarkerDetail

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_MarkerDetail_hpp
