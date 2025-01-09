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

#if !defined ACDB_PUB_TYPES_H
#define ACDB_PUB_TYPES_H

/*--------------------------------------------------------------------
                           GENERAL INCLUDES
--------------------------------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>
#include "GRM_pub.h"

#ifdef __cplusplus
extern "C" {
#endif

/*--------------------------------------------------------------------
                          LITERAL CONSTANTS
--------------------------------------------------------------------*/

/*----------------------------------------------------------
Max value for star ratings
----------------------------------------------------------*/
#define ACDB_MAX_RATING 5

/*----------------------------------------------------------
Invalid Values
----------------------------------------------------------*/

#define ACDB_INVALID_RATING (0)
#define ACDB_INVALID_DATE_TIME (0)
#define ACDB_INVALID_STAR_COUNT (0)
#define ACDB_INVALID_MARKER_IDX (-1)
#define ACDB_INVALID_REVIEW_IDX (-1)
#define ACDB_INVALID_BUSINESS_PROGRAM_TIER (-1)

// used for bit field definition within MapMarkerFilter/SearchMarkerFilter class
// also stored within the ActiveCaptain database from the server
typedef int32_t ACDB_type_type;
enum {
  ACDB_UNKNOWN_TYPE = 1 << 0,
  ACDB_ANCHORAGE = 1 << 1,
  ACDB_HAZARD = 1 << 2,
  ACDB_MARINA = 1 << 3,
  // ACDB_LOCAL_KNOWLEDGE = 1 << 4,    -> old type from the previous DB version. Not used anymore
  // ACDB_NAVIGATIONAL = 1 << 5,       -> old type from the previous DB version. Not used anymore
  ACDB_BOAT_RAMP = 1 << 6,
  ACDB_BUSINESS = 1 << 7,
  ACDB_INLET = 1 << 8,
  ACDB_BRIDGE = 1 << 9,
  ACDB_LOCK = 1 << 10,
  ACDB_DAM = 1 << 11,
  ACDB_FERRY = 1 << 12,
  // ACDB_AIRPORT = 1 << 13,           -> deprecated type.

  ACDB_ALL_LOCAL_KNOWLEDGE =
      ACDB_BOAT_RAMP | ACDB_BUSINESS | ACDB_INLET | ACDB_BRIDGE | ACDB_LOCK | ACDB_DAM | ACDB_FERRY,

  // Leave most significant but clear to avoid an "ISO C restricts enumerator values..." error.
  ACDB_ALL_TYPES = 0x7FFFFFFF
};

typedef uint8_t ACDB_coord_format_type;
enum { ACDB_COORD_DEC_DEG = 0, ACDB_COORD_DEG_MIN = 1, ACDB_COORD_DEG_MIN_SEC = 2 };

typedef uint8_t ACDB_date_format_type;
enum {
  ACDB_DATE_MONTH_ABBR = 0, /* 31-DEC-2019 */
  ACDB_DATE_DMY_SLASH = 1,  /* 31/12/2019 */
  ACDB_DATE_MDY_SLASH = 2,  /* 12/31/2019 */
  ACDB_DATE_DMY_DASH = 3,   /* 31-12-2019 */
  ACDB_DATE_MDY_DASH = 4    /* 12-31-2019 */
};

typedef uint8_t ACDB_unit_type;
enum { ACDB_UNKNOWN_UNIT = 0, ACDB_FEET = 1, ACDB_METER = 2, ACDB_GALLON = 3, ACDB_LITER = 4 };

#define ACDB_MAX_LAT ((int32_t)0x40000000) /* maximum valid latitude in semicircles */
#define ACDB_MAX_LON ((int32_t)0x7fffffff) /* maximum (signed) longitude in semicircles */
#define ACDB_MIN_LAT ((int32_t)0xC0000000) /* minimum valid latitude in semicircles */
#define ACDB_MIN_LON ((int32_t)0x80000000) /* minimum (signed) longitude in semicircles */

typedef uint32_t ACDB_client_status_type;
enum {
  /*------------------------------------------------------
  The Active Captain database is installed and open.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_INSTALLED,

  /*------------------------------------------------------
  The ACDB client is currently downloading files.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_DOWNLOAD_IN_PROGRESS,

  /*------------------------------------------------------
  The ACDB client is currently installing the downloaded files.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_INSTALLING,

  /*------------------------------------------------------
  An error occurred downloading files.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_DOWNLOAD_ERROR,

  /*------------------------------------------------------
  The Active Captain data is not installed.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_NOT_INSTALLED,

  /*------------------------------------------------------
  The Active Captain data is updating.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_UPDATING,

  /*------------------------------------------------------
  The ACDB client is currently in the process of sideloading
  a database to a vessel.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_SIDELOAD_IN_PROGRESS,

  /*------------------------------------------------------
  The database has been transferred to the vessel and
  the ACDB server is currently validating
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_SIDELOAD_VALIDATION_IN_PROGRESS,

  /*------------------------------------------------------
  The transfer was interupted for a brief length of time
  and is considered paused. If the transfer does not pick
  back up, we will eventually error out.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_SIDELOAD_PAUSED,

  /*------------------------------------------------------
  An error occurred sideloading the update package.
  Like something could go wrong.
  ------------------------------------------------------*/
  ACDB_CLIENT_STATUS_SIDELOAD_ERROR,

  ACDB_CLIENT_STATUS_COUNT,
  ACDB_CLIENT_STATUS_INVALID,
};

/*--------------------------------------------------------------------
                                TYPES
--------------------------------------------------------------------*/
typedef uint64_t ACDB_marker_idx_type;

typedef uint64_t ACDB_review_idx_type;

typedef struct {
  bbox_type bbox;
  bool include_marinas;
  bool include_hazards;
  bool include_anchorages;
  bool include_local_knowledge;
  char const* search_string;
  uint32_t max_num_results;
} ACDB_marker_filter_type;

typedef int32_t ACDB_text_handle_type;

/*--------------------------------------------------------------------
                           PROJECT INCLUDES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                           MEMORY CONSTANTS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              VARIABLES
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                                MACROS
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
                              PROCEDURES
--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif /* ACDB_PUB_TYPES_H */
