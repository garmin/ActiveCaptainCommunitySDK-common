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
    Functions for processing acdb:// URLs.

    Copyright 2019-2020 by Garmin Ltd. or its subsidiaries.
*/

#ifndef ACDB_AcdbUrlAction_hpp
#define ACDB_AcdbUrlAction_hpp

#include <memory>
#include <string>
#include "ACDB_pub_types.h"

namespace Acdb {
// The primary API for getting an URL action is ParseAcdbUrl(), which can be found further down this
// file. The function will evaluate an URL and return one of the action types listed below.

class AcdbUrlAction;
using AcdbUrlActionPtr = std::unique_ptr<AcdbUrlAction>;

class AcdbUrlAction {
 public:
  enum class ActionType { Edit, ReportReview, SeeAll, ShowPhotos, ShowSummary, VoteReview };

  AcdbUrlAction(ActionType aAction);

  ActionType GetAction() const;

 private:
  ActionType mAction;
};

class EditAction : public AcdbUrlAction {
 public:
  EditAction(std::string&& aUrl);

  std::string GetUrl() const;

 private:
  std::string mUrl;
};

class ReportReviewAction : public AcdbUrlAction {
 public:
  ReportReviewAction(std::string&& aUrl);

  std::string GetUrl() const;

 private:
  std::string mUrl;
};

class SeeAllAction : public AcdbUrlAction {
 public:
  SeeAllAction(const ACDB_marker_idx_type aMarkerId, const std::string&& aSection,
               const uint32_t aPageNumber);

  ACDB_marker_idx_type GetMarkerId() const;

  uint32_t GetPageNumber() const;

  std::string GetSection() const;

 private:
  ACDB_marker_idx_type mMarkerId;
  uint32_t mPageNumber;
  std::string mSection;
};

class ShowPhotosAction : public AcdbUrlAction {
 public:
  ShowPhotosAction(const ACDB_marker_idx_type aMarkerId);

  ACDB_marker_idx_type GetMarkerId() const;

 private:
  ACDB_marker_idx_type mMarkerId;
};

class ShowSummaryAction : public AcdbUrlAction {
 public:
  ShowSummaryAction(const ACDB_marker_idx_type aMarkerId);

  ACDB_marker_idx_type GetMarkerId() const;

 private:
  ACDB_marker_idx_type mMarkerId;
};

class VoteReviewAction : public AcdbUrlAction {
 public:
  VoteReviewAction(const ACDB_marker_idx_type aMarkerId, const ACDB_review_idx_type aReviewId);

  ACDB_marker_idx_type GetMarkerId() const;

  ACDB_review_idx_type GetReviewId() const;

 private:
  ACDB_marker_idx_type mMarkerId;
  ACDB_review_idx_type mReviewId;
};

bool ParseAcdbUrl(const std::string& aUrl, AcdbUrlActionPtr& aAction_out);

bool IsReviewsSection(const std::string& aSectionName);
}  // namespace Acdb
#endif /* ACDB_AcdbUrlAction_hpp */
