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

//////////////////////////////////////////////////////////////////////////
//! @file
//!
//! @author S.V. Raghava Rao
//!
//! @date   09 Oct 06
//!
//! SYSTEM: NAVIONICS SmartSDK project
//!
//! COPYRIGHT:  Copyright NAVIONICS S.p.A 2008
//!
//! FUNCTION:   Holds the declaration of class NavDateTime
//!
//! History of changes
//!
//! <table   width="80%" >
//! <tr>
//!     <th  colspan="1"  > Date </th>
//!     <th  colspan="1"  > Author </th>
//!     <th  colspan="1"  > Change </th>
//! </tr>
//! <tr>
//!     <td> 27 Jul 11      </td>
//!     <td> T.Giunti   </td>
//!     <td>
//!     - New Date String Type Added..
//!     </td>
//! </tr>
//! <tr>
//!     <td> 20 Oct 11      </td>
//!     <td> F.Ilario   </td>
//!     <td>
//!     - Revisited ToString and FromString methods to allow formats
//!       with any delimiter or without delimiter. Implemented
//!       all formats provided by the class.
//!     </td>
//! </tr>
//! </table>
///////////////////////////////////////////////////////////////////////////

#ifndef NAVIONICS_SYSTEM_DATETIME_H
#define NAVIONICS_SYSTEM_DATETIME_H

#include <NavTimeSpan.h>
#include "NavString.h"

//! Time origin. It is assumed to be 1 Jan 2036 at 00:00
#define TIME_ORIGIN 2036

//! Default day of week. It is the day of week of 1 Jan 2040
#define TIME_DEFAULT_DAY_OF_WEEK 1

//! Max Time allowed. It is assumed to be 31 Dec 2100 just before midnight
#define TIME_MAX_YEAR 2100

//! Min Time allowed. It is assumed to be 1 Jan 1970 at 00:00
#define TIME_MIN_YEAR 1970

//! Number of days per year (not a leap year)
#define TIME_DAYS_PER_YEAR 365

//! Number of months per year
#define TIME_MONTHS_PER_YEAR 12

//! Number of days per week
#define TIME_DAYS_PER_WEEK 7

//! Number of leap years before time origin.
#define TIME_LEAP_YEARS_BEFORE ((TIME_ORIGIN - TIME_MIN_YEAR) / 4)

//! Number of leap years after time origin.
#define TIME_LEAP_YEARS_AFTER ((TIME_MAX_YEAR - TIME_ORIGIN) / 4)

//! Minimum offset from time origin expressed in days
#define TIME_MIN_OFFSET \
  ((TIME_ORIGIN - TIME_MIN_YEAR) * TIME_DAYS_PER_YEAR + TIME_LEAP_YEARS_BEFORE)

//! Maximum offset from time origin expressed in days
#define TIME_MAX_OFFSET \
  ((TIME_MAX_YEAR - TIME_ORIGIN + 1) * TIME_DAYS_PER_YEAR + TIME_LEAP_YEARS_AFTER)

//! Character used to specify the conversion of year with 4 digits
#define TIME_YEAR_4_CHARACTER "%Y"

//! Character used to specify the conversion of year with 2 digits
#define TIME_YEAR_2_CHARACTER "%y"

//! Character used to specify the conversion of month with 2 digits, zero filled
#define TIME_MONTH_2_CHARACTER "%M"

//! Character used to specify the conversion of month, without filling
#define TIME_MONTH_1_CHARACTER "%m"

//! Character used to specify the conversion of day with 2 digits, zero filled
#define TIME_DAY_2_CHARACTER "%D"

//! Character used to specify the conversion of day, without filling
#define TIME_DAY_1_CHARACTER "%d"

//! Character used to specify the conversion of hours, 0 to 23, with 2 digits,
//! zero filled
#define TIME_HOUR_2_CHARACTER "%H"

//! Character used to specify the conversion of hours, 0 to 23, without filling
#define TIME_HOUR_1_CHARACTER "%h"

//! Character used to specify the conversion of hours, 0 to 11, with 2 digits,
//! zero filled
#define TIME_AMPM_HOUR_2_CHARACTER "%G"

//! Character used to specify the conversion of hours, 0 to 11, without filling
#define TIME_AMPM_HOUR_1_CHARACTER "%g"

//! Character used to specify the conversion of minutes with 2 digits, zero
//! filled
#define TIME_MINUTE_2_CHARACTER "%N"

//! Character used to specify the conversion of minutes, without filling
#define TIME_MINUTE_1_CHARACTER "%n"

//! Character used to specify the conversion of seconds with 2 digits, zero
//! filled
#define TIME_SECOND_2_CHARACTER "%S"

//! Character used to specify the conversion of seconds, without filling
#define TIME_SECOND_1_CHARACTER "%s"

//! Character used to specify the conversion of milliseconds, without filling
#define TIME_MILLISECOND_CHARACTER "%l"

//! Character used to specify the conversion of month, with full name
#define TIME_LONG_MONTH_CHARACTER "%T"

//! Character used to specify the conversion of month, with abbreviated name
#define TIME_SHORT_MONTH_CHARACTER "%t"

//! Character used to specify the conversion of day, with full name
#define TIME_LONG_DAY_CHARACTER "%W"

//! Character used to specify the conversion of day, with abbreviated name
#define TIME_SHORT_DAY_CHARACTER "%w"

//! Character used to specify the conversion of AM/PM string
#define TIME_AM_PM_CHARACTER "%A"

//! Maximum number of digits in milliseconds conversion
#define TIME_MAX_MILLISECONDS_DIGITS 3

//! Unix Epoch base date

//! Garmin Epoch base date

//! @enum DateStringType
//! type of format used for Date into a string.
//! It support both date with and without delimiter except
//! YYYYMMDDTHHMMSSZ_FORMAT format.
enum DateStringType {
  NO_SPECIFIED_FORMAT,     // e.g.: 2011/10/29, 20111029, 2011-10-29
  YYYYMMDD_FORMAT,         // e.g.: 2011/10/29, 20111029, 2011-10-29
  YYYYMMDDHH_FORMAT,       // e.g.: 2011102913, 2011-10-29-13
  YYYYDDMM_FORMAT,         // e.g.: 2011/29/10, 20112910, 2011-29-10
  MMDDYYYY_FORMAT,         // e.g.: 10/29/2011, 10292011, 10-29-2011
  DDMMYYYY_FORMAT,         // e.g.: 29/10/2011, 29102011, 29-10-2011
  YYYYMMDD_HHMMSS_FORMAT,  // e.g.: 2014-09-25 10:50:17
  YYYYMMDDTHHMMSSZ_FORMAT, /* OSM timestamp standard
                                       "T" separator beteween date and time
                                       "Z" at the end means "Zulu Time", confidential for UTC. */
  YYYYMMDDTHHMMSSMMMZ_FORMAT,
  YYYYMMDD_HHMMSS_MMM_FORMAT,  // e.g.: 2014-09-25 10:50:17.456, 2014-09-25_10:50:17.456,
                               // 2014-09-25/10:50:17.456
  DDD_DD_MMM_YYYY_HHMMSS_GMT_FORMAT
};

//! @enum DateDelimiterToken
//! Date delimiter allowed for any date format to be provided
//! to ToString method.
enum DateDelimiterToken {
  DATE_DELIMITER_NONE = 0,
  DATE_DELIMITER_DASH = '-',
  DATE_DELIMITER_SLASH = '/',
  DATE_DELIMITER_BACKSLASH = '\\',
  DATE_DELIMITER_DOT = '.',
  DATE_DELIMITER_UNDERSCORE = '_',
  DATE_DELIMITER_SPACE = ' '
};

//! @enum EpochType
//! Differentiates between different varieties of epoch base years

namespace Navionics {

////////////////////////////////////////////////////////////////////////////
//! @class NavDateTime
//! NavDateTime class provides functions and operators to manipulate
//! date and time with resolution of milliseconds.
//! Date and time range is from year TIME_MIN_YEAR to TIME_MAX_YEAR.
//! @see TIME_MIN_YEAR
//! @see TIME_MAX_YEAR
//!
//! <table   width="80%" >
//! <tr>
//!     <th  colspan="2"  > Class Information </th>
//! </tr>
//! <tr>
//!     <td>Abstract Base Class </td>   <td>    No  </td>
//! </tr>
//! <tr>
//!     <td>Can be inherited    </td>   <td>    Yes </td>
//! </tr>
//! <tr>
//!     <td>Singleton Class </td>   <td>    No  </td>
//! </tr>
//! </table>
//!
//! <b> Macro Definitions and Constants </b>
//!
////////////////////////////////////////////////////////////////////////////
class NavDateTime : protected NavTimeSpan {
 public:
  NavDateTime(void);

  NavDateTime(unsigned int Day, unsigned int Month, unsigned int Year);

  NavDateTime(unsigned int m_uIYear, unsigned int m_uIMonth, unsigned int m_uIDay,
              unsigned int m_uIHour, unsigned int m_uIMinute, unsigned int m_uISecond,
              unsigned int m_uIMilliSecond);

  NavDateTime operator-(const NavTimeSpan& rTimeSpan) const;

  const NavDateTime& operator-=(const NavTimeSpan& rTimeSpan);

  NavTimeSpan operator-(const NavDateTime& rTime) const;

  NavDateTime operator+(const NavTimeSpan& rTimeSpan) const;

  const NavDateTime& operator+=(const NavTimeSpan& rTimeSpan);

  bool operator==(const NavDateTime& rTime) const;

  bool operator!=(const NavDateTime& rTime) const;

  bool operator>(const NavDateTime& rTime) const;

  bool operator>=(const NavDateTime& rTime) const;

  bool operator<(const NavDateTime& rTime) const;

  bool operator<=(const NavDateTime& rTime) const;

  void SetDate(unsigned int Day, unsigned int Month, unsigned int Year);

  void GetDate(unsigned int& rDay, unsigned int& rMonth, unsigned int& rYear) const;

  void SetTimeOfDay(unsigned int Hour, unsigned int Minute, unsigned int Second);

  void GetTimeOfDay(unsigned int& rHour, unsigned int& rMinute, unsigned int& rSecond) const;

  unsigned int GetDayOfWeek(void) const;

  bool IsSameDay(const NavDateTime& rTime) const;

  static bool IsLeapYear(const unsigned int& cuIYear);

  bool FromString(const NavString& strDate, DateStringType sType = NO_SPECIFIED_FORMAT);
  bool ToString(NavString& outStrDate, DateStringType sType,
                DateDelimiterToken inDelimiter = DATE_DELIMITER_NONE) const;

 private:
  //! @property DaysPerMonth
  //! Table containing number of days per month for normal year
  static const unsigned int DaysPerMonth[TIME_MONTHS_PER_YEAR];

  //! @property LeapDaysPerMonth
  //! Table containing number of days per month for leap year
  static const unsigned int LeapDaysPerMonth[TIME_MONTHS_PER_YEAR];

  //! @property SumOfDays
  //! Table containing the partial sum of days before given month
  static const unsigned int SumOfDays[TIME_MONTHS_PER_YEAR];

  //! property DaysOfWeek
  //! Table containing the name (english, 3 char only) of the days of week
  static const char* DaysOfWeek[TIME_DAYS_PER_WEEK];

  //! property Months
  //! Table containing the name (english, 3 char only) of the months
  static const char* Months[TIME_MONTHS_PER_YEAR];

  //! @property MinTimeSpan
  //! Constant representing a minimum time range
  //! expressed as multiple of days of time span object
  static const NavTimeSpan MinTimeSpan;

  //! @property MaxTimeSpan
  //! Constant representing a maximum time range
  //! expressed as multiple of days of time span object
  static const NavTimeSpan MaxTimeSpan;

  // Constructor for internal use
  NavDateTime(const NavTimeSpan& rTimeSpan);

  unsigned int GetMilliSecsFromMidnight(void) const;
  bool FromFormattedString(const NavString& inFormattedDate);

  // Facility API to create a formatter in Debugger and improve human readibility.
  // e.g. on Xcode type:
  // (lldb) type summary add --summary-string "{$VAR.ToString()}:s" NavDateTime
  NavString ToString();

  //! Returns the number of the month given the month name (3 chars) as ouput parameter.
  //! On success returns true, false otherwise.
  static bool GetMonthNumberFromName(const char* inMonthName, unsigned int& outMonthIdx);

  //! Returns the name (3 chars) of the weekday which index (0-6) is given by argument.
  //! If month number is not between 1-12 it returns NULL.
  static const char* GetMonthNameFromNumber(unsigned int inMonthIdx);

  //! Returns the name (3 chars) of the weekday which index (0-6) is given by argument
  //! If weekday index is not between 0-6 it returns NULL.
  static const char* GetWeekDayNameFromDayIdx(unsigned int inDayIdx);
};
NavDateTime operator+(const NavTimeSpan& rTimeSpan, const NavDateTime& rTime);
}  // namespace Navionics

#endif  // NAVIONICS_SYSTEM_DATETIME_H
