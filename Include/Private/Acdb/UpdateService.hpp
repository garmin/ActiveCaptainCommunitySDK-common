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
    Contains update-related functions for ActiveCaptain
    functionality.

    Copyright 2021 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_UpdateService_hpp
#define ACDB_UpdateService_hpp

#include "ACDB_pub_types.h"
#include "Acdb/IUpdateService.hpp"
#include "Acdb/PrvTypes.hpp"
#include "Acdb/Repository.hpp"

namespace Acdb {
class UpdateService : public IUpdateService {
 public:
  UpdateService(RepositoryPtr aRepositoryPtr);

  virtual ~UpdateService();

  bool ProcessCreateMarkerResponse(const std::string& aResponseBody,
                                   ACDB_marker_idx_type& aMarkerId_out) override;

  bool ProcessMoveMarkerResponse(const std::string& aResponseBody) override;

  bool ProcessSyncMarkersResponse(const std::string& aResponseBody, const TileXY& aTileXY,
                                  std::size_t& aResultCount_out) override;

  bool ProcessSyncReviewsResponse(const std::string& aResponseBody, const TileXY& aTileXY,
                                  std::size_t& aResultCount_out) override;

  bool ProcessVoteForReviewResponse(const std::string& aResponseBody) override;

  bool ProcessWebViewResponse(const std::string& aResponseBody) override;

 private:
  RepositoryPtr mRepositoryPtr;
};

}  // end of namespace Acdb

#endif  // end of ACDB_UpdateService_hpp
