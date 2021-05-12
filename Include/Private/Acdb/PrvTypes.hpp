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
    Copyright 2017-2021 by Garmin Ltd. or its subsidiaries.
*/

#if !defined ACDB_PRV_TYPES_H
#define ACDB_PRV_TYPES_H

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/
#include <memory>
#include <string>
#include <vector>

#include "GRM_pub.h"
#include "acdb_prv_config.h"
#include "Acdb/TableDataTypes.hpp"

namespace Acdb {

template <class T, class U>
inline bool CompareSharedPtr(const std::shared_ptr<T>& aLhs, const std::shared_ptr<U>& aRhs);

template <class T, class U>
inline bool CompareUniquePtr(const std::unique_ptr<T>& aLhs, const std::unique_ptr<U>& aRhs);

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/
// Forward declaration to allow declaration of MapMarkerPtr
class MapMarker;
using MapMarkerPtr = std::unique_ptr<MapMarker>;

class SearchMarker;
using SearchMarkerPtr = std::unique_ptr<SearchMarker>;

class Repository;
using RepositoryPtr = std::shared_ptr<Repository>;

struct MarkerTableDataCollection {
  MarkerTableDataType mMarker;
  MarkerMetaTableDataType mMarkerMeta;

  std::unique_ptr<AddressTableDataType> mAddress;
  std::unique_ptr<AmenitiesTableDataType> mAmenities;
  std::unique_ptr<BusinessTableDataType> mBusiness;
  std::vector<BusinessPhotoTableDataType> mBusinessPhotos;
  std::unique_ptr<BusinessProgramTableDataType> mBusinessProgram;
  std::vector<CompetitorTableDataType> mCompetitors;
  std::unique_ptr<ContactTableDataType> mContact;
  std::unique_ptr<DockageTableDataType> mDockage;
  std::unique_ptr<FuelTableDataType> mFuel;
  std::unique_ptr<MooringsTableDataType> mMoorings;
  std::unique_ptr<NavigationTableDataType> mNavigation;
  std::unique_ptr<RetailTableDataType> mRetail;
  std::unique_ptr<ServicesTableDataType> mServices;

  bool operator==(const MarkerTableDataCollection& aRhs) {
    return mMarker == aRhs.mMarker && mMarkerMeta == aRhs.mMarkerMeta &&
           CompareUniquePtr(mAddress, aRhs.mAddress) &&
           CompareUniquePtr(mAmenities, aRhs.mAmenities) &&
           CompareUniquePtr(mBusiness, aRhs.mBusiness) && mBusinessPhotos == aRhs.mBusinessPhotos &&
           CompareUniquePtr(mBusinessProgram, aRhs.mBusinessProgram) &&
           mCompetitors == aRhs.mCompetitors && CompareUniquePtr(mContact, aRhs.mContact) &&
           CompareUniquePtr(mDockage, aRhs.mDockage) && CompareUniquePtr(mFuel, aRhs.mFuel) &&
           CompareUniquePtr(mMoorings, aRhs.mMoorings) &&
           CompareUniquePtr(mNavigation, aRhs.mNavigation) &&
           CompareUniquePtr(mRetail, aRhs.mRetail) && CompareUniquePtr(mServices, aRhs.mServices);
  }

  bool mIsDeleted{false};
};

struct ReviewTableDataCollection {
  ReviewTableDataType mReview;
  std::vector<ReviewPhotoTableDataType> mReviewPhotos;

  bool operator==(const ReviewTableDataCollection& aRhs) {
    return mReview == aRhs.mReview && mReviewPhotos == aRhs.mReviewPhotos;
  }
};

using TranslationDataType = std::pair<int, std::string>;

namespace Presentation {
// Forward declaration to allow declaration of PresentationMarkerPtr
class PresentationMarker;
using PresentationMarkerPtr = std::unique_ptr<PresentationMarker>;

// Forward declaration to allow declaration of AddressPtr
class Address;
using AddressPtr = std::unique_ptr<Address>;

// Forward declaration to allow declaration of AmenitiesPtr
class Amenities;
using AmenitiesPtr = std::unique_ptr<Amenities>;

// Forward declaration to allow declaration of BusinessPtr
class Business;
using BusinessPtr = std::unique_ptr<Business>;

// Forward declaration to allow declaration of CompetitorAdPtr
class CompetitorAd;
using CompetitorAdPtr = std::unique_ptr<CompetitorAd>;

// Forward declaration to allow declaration of ContactPtr
class Contact;
using ContactPtr = std::unique_ptr<Contact>;

// Forward declaration to allow declaration of DockagePtr
class Dockage;
using DockagePtr = std::unique_ptr<Dockage>;

// Forward declaration to allow declaration of FuelPtr
class Fuel;
using FuelPtr = std::unique_ptr<Fuel>;

// Forward declaration to allow declaration of MarkerDetailPtr
class MarkerDetail;
using MarkerDetailPtr = std::unique_ptr<MarkerDetail>;

// Forward declaration to allow declaration of MooringsPtr
class Moorings;
using MooringsPtr = std::unique_ptr<Moorings>;

// Forward declaration to allow declaration of NavigationPtr
class Navigation;
using NavigationPtr = std::unique_ptr<Navigation>;

// Forward declaration to allow declaration of RetailPtr
class Retail;
using RetailPtr = std::unique_ptr<Retail>;

// Forward declaration to allow declaration of ReviewDetailPtr
class ReviewDetail;
using ReviewDetailPtr = std::unique_ptr<ReviewDetail>;

// Forward declaration to allow declaration of ReviewSummaryPtr
class ReviewSummary;
using ReviewSummaryPtr = std::unique_ptr<ReviewSummary>;

// Forward declaration to allow declaration of ServicesPtr
class Services;
using ServicesPtr = std::unique_ptr<Services>;

// Forward declaration to allow declaration of ReviewListPtr
class ReviewList;
using ReviewListPtr = std::unique_ptr<ReviewList>;

// Forward declaration to allow declaration of BusinessPhotoListPtr
class BusinessPhotoList;
using BusinessPhotoListPtr = std::unique_ptr<BusinessPhotoList>;
}  // end of namespace Presentation

// Forward declaration to allow declaration of ReviewPtr
class Review;
using ReviewPtr = std::unique_ptr<Review>;

// Forward declaration to allow declaration of UserReviewStatsPtr
class UserReviewStats;
using UserReviewStatsPtr = std::unique_ptr<UserReviewStats>;

// Forward declaration to allow declaration of EnvironmentPtr
class Environment;
using EnvironmentPtr = std::shared_ptr<Environment>;

// Dlm Notification types
enum class MessageId : uint32_t {
  DownloadCompleted = 0,
  DownloadAuto,
  StateNotInstalled,
  StateInstalled,
  StateDownloadProg,
  StateDownloadError,
  StateDownloadCanceled,
  Download,
  Cancel,
  Init,
  DeleteDatabase,
  VcmCardStatusChanged,
  ApplyWebViewResponse,
  MoveMarker,
  ReportMarkerViewed,
  VoteForReview,
  CreateMarker,
  ConnectionTypeOrDataUsageChanged,
  LanguageChanged,
  UnitsChanged,
  SetBoundingBoxes
};

union MessageData {
  ACDB_marker_idx_type markerId;

  MessageData();
};

struct MessageType {
  MessageId mId;
  MessageData mData;
};

enum class TileUpdateType : uint32_t { None, Download, Sync, Delete };

struct TileUpdateOperation {
  TileUpdateOperation()
      : mMarkerUpdateType(TileUpdateType::None), mReviewUpdateType(TileUpdateType::None) {}

  TileUpdateType mMarkerUpdateType;
  TileUpdateType mReviewUpdateType;
};

struct LastUpdateInfoType {
  LastUpdateInfoType() : mMarkerLastUpdate(0), mUserReviewLastUpdate(0) {}

  LastUpdateInfoType(const LastUpdateInfoType& aRhs)
      : mMarkerLastUpdate(aRhs.mMarkerLastUpdate),
        mUserReviewLastUpdate(aRhs.mUserReviewLastUpdate) {}

  bool operator==(const LastUpdateInfoType& aRhs) const {
    return mMarkerLastUpdate == aRhs.mMarkerLastUpdate &&
           mUserReviewLastUpdate == aRhs.mUserReviewLastUpdate;
  }

  uint64_t mMarkerLastUpdate;
  uint64_t mUserReviewLastUpdate;
};

struct TileXY {
  TileXY() : mX(-1), mY(-1) {}

  TileXY(int aX, int aY) : mX(aX), mY(aY) {}

  TileXY(const TileXY& aRhs) GRM_NOEXCEPT : mX(aRhs.mX), mY(aRhs.mY) {}

  // Needed because we use this in a std::set.
  bool operator<(const TileXY& aRhs) const {
    return (mY == aRhs.mY && mX < aRhs.mX) || mY < aRhs.mY;
  }

  bool operator==(const TileXY& aRhs) const { return mX == aRhs.mX && mY == aRhs.mY; }

  int mX;
  int mY;
};

struct ExportFile {
  ExportFile() : mUrl(), mMd5(), mSize(0), mTileXY() {}

  std::string mUrl;
  std::string mMd5;
  uint64_t mSize;
  TileXY mTileXY;
};

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

extern const std::string InternalDbPath;
extern const std::string DbName;
extern const std::string DbExt;
extern const std::string TmpExt;
extern const std::string ZipExt;
extern const std::string SupportedSchemaVer;

extern const char* ExternalDbPath;

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

//----------------------------------------------------------------
//!
//!   @return true if both pointers are nullptr or point to
//!           equivalent objects
//!
//----------------------------------------------------------------
template <class T, class U>
inline bool CompareSharedPtr(const std::shared_ptr<T>& aLhs, const std::shared_ptr<U>& aRhs) {
  return (aLhs == aRhs || (aLhs && aRhs && *aLhs == *aRhs));
}  // end of CompareSharedPtr

//----------------------------------------------------------------
//!
//!   @return true if both pointers are nullptr or point to
//!           equivalent objects
//!
//----------------------------------------------------------------
template <class T, class U>
inline bool CompareUniquePtr(const std::unique_ptr<T>& aLhs, const std::unique_ptr<U>& aRhs) {
  return (aLhs == aRhs || (aLhs && aRhs && *aLhs == *aRhs));
}  // end of CompareUniquePtr

}  // namespace Acdb

#endif /* ACDB_PRV_TYPES_H */
