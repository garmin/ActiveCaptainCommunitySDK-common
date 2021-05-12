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

#if !defined ACDB_PUB_TYPES_HPP
#define ACDB_PUB_TYPES_HPP

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/
#include <memory>
#include <map>
#include <string>

namespace Acdb {

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/
// Forward declaration to allow declaration of IMapMarkerPtr
class IMapMarker;
typedef std::unique_ptr<IMapMarker> IMapMarkerPtr;

class ISearchMarker;
typedef std::unique_ptr<ISearchMarker> ISearchMarkerPtr;

// Forward declaration to allow declaration of IPresentationMarkerPtr
class IPresentationMarker;
typedef std::unique_ptr<IPresentationMarker> IPresentationMarkerPtr;

// Forward declaration to allow declaration of IUserReviewStatsPtr
class IUserReviewStats;
typedef std::unique_ptr<IUserReviewStats> IUserReviewStatsPtr;

// Forward declaration to allow declaration of IClientStatePtr
class IClientState;
typedef std::unique_ptr<IClientState> IClientStatePtr;

// Content view types
enum ContentViewStringType : uint16_t {
  ContentViewGeneralInformation = 0,
  ContentViewNavigation = 1,
  ContentViewDockage = 2,
  ContentViewFuel = 3,
  ContentViewServices = 4,
  ContentViewUserReview = 5
};

typedef std::pair<ContentViewStringType, std::string> ContentViewPair;
typedef std::map<ContentViewStringType, std::string> ContentViewMap;
typedef std::unique_ptr<ContentViewMap> ContentViewMapPtr;

enum class EnvironmentType { Test, Stage, Production };

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/
/*----------------------------------------------------------
Max value for review star ratings
----------------------------------------------------------*/
const int MaxReviewRating = 5;

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

}  // namespace Acdb

#endif /* ACDB_PUB_TYPES_HPP */
