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
    Generates Marker objects.

    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_MustacheViewFactory_hpp
#define ACDB_MustacheViewFactory_hpp

#include "ACDB_pub_types.h"
#include "Acdb/PubTypes.hpp"
#include "GRM_pub.h"

#include "Acdb/PrvTypes.hpp"

namespace Acdb {
class Repository;

namespace Presentation {

std::string GetBusinessPhotoListHtml(const BusinessPhotoList& aBusinessPhotoList,
                                     const RepositoryPtr& aRepositoryPtr);

ContentViewMapPtr GetContentViewMap(const PresentationMarker& aPresentationMarker,
                                    const ReviewListPtr& aReviewListPtr,
                                    const RepositoryPtr& aRepositoryPtr);

std::string GetPresentationMarkerHtml(const PresentationMarker& aPresentationMarker,
                                      const RepositoryPtr& aRepositoryPtr);

std::string GetReviewListHtml(const ReviewList& aReviewList, const RepositoryPtr& aRepositoryPtr);

std::string GetSectionPageHtml(const PresentationMarker& aPresentationMarker,
                               const std::string& aSectionName,
                               const RepositoryPtr& aRepositoryPtr);

void SetHeadContent(const std::string& aHeadContent);

void SetImagePrefix(const std::string& aImagePrefix);

}  // end of namespace Presentation
}  // end of namespace Acdb

#endif  // end of ACDB_MustacheViewFactory_hpp
