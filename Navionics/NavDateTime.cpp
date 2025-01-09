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
//! FUNCTION:   Holds the implementation of class NavDateTime
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
//!     <td> 20 Oct 11  </td>
//!     <td> F.Ilario   </td>
//!     <td>
//!     - Revisited ToString and FromString methods to allow formats
//!       with any delimiter or without delimiter. Implemented
//!       all formats provided by the class.
//!     </td>
//! </tr>
//! </table>
///////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <vector>

#include "navionics.h"
#include "NavDateTime.h"

//! Constant representing the default format for date conversion
#define DATE_FORMAT L"%D/%M/%Y"

//! Constant representing the default format for time conversion
#define TIME_FORMAT L"%H:%N:%S"

namespace Navionics {

using namespace std;

//! Table containing number of days per month for normal year
const unsigned int NavDateTime::DaysPerMonth[TIME_MONTHS_PER_YEAR] = {31, 28, 31, 30, 31, 30,
                                                                      31, 31, 30, 31, 30, 31};

//! Table containing number of days per month for leap year
const unsigned int NavDateTime::LeapDaysPerMonth[TIME_MONTHS_PER_YEAR] = {31, 29, 31, 30, 31, 30,
                                                                          31, 31, 30, 31, 30, 31};

//! Table containing the partial sum of days before given month
const unsigned int NavDateTime::SumOfDays[TIME_MONTHS_PER_YEAR] = {0,   31,  59,  90,  120, 151,
                                                                   181, 212, 243, 273, 304, 334};

const char* NavDateTime::DaysOfWeek[TIME_DAYS_PER_WEEK] = {"Mon", "Tue", "Wed", "Thu",
                                                           "Fri", "Sat", "Sun"};

const char* NavDateTime::Months[TIME_MONTHS_PER_YEAR] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                                         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

//! Constant representing a minimum time range
//! expressed as multiple of days of time span object
const NavTimeSpan NavDateTime::MinTimeSpan(NavTimeSpan(TIME_MIN_OFFSET* SECONDS_PER_DAY));

//! Constant representing a maximum time range
//! expressed as multiple of days of time span object
const NavTimeSpan NavDateTime::MaxTimeSpan(NavTimeSpan(TIME_MAX_OFFSET* SECONDS_PER_DAY));

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Default constructor which sets default date and time
//!
//! @param      None
//!
//! @returns        None
//!
//! @throw      None
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime::NavDateTime(void) { SetDate(1, 1, TIME_ORIGIN); }
///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Constructor to initialize NavDateTime object using
//! individual values given as parameters.
//!
//! @param      :\n
//!         m_uIYear    the year within [TIME_MIN_YEAR, TIME_MAX_YEAR]\n
//!         m_uIMonth   the month within [1, 12]\n
//!         m_uIDay     the day within [1, days_per_month]\n
//!         m_uIHour    hour of the day within [0, 23]\n
//!         m_uIMinute  minutes of the day within [0, 59]\n
//!         m_uISecond  seconds of the day within [0, 59]\n
//!         m_uIMilliSecond milliseconds of the day within [0, 999]\n
//!         m_uIDayOfWeek   not used in this operation.
//!
//! @throw      INVALID_ARGUMENT If at least one component
//!         exceed its range
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime::NavDateTime(unsigned int m_uIYear, unsigned int m_uIMonth, unsigned int m_uIDay,
                         unsigned int m_uIHour, unsigned int m_uIMinute, unsigned int m_uISecond,
                         unsigned int m_uIMilliSecond) {
  int seconds;

  SetDate(m_uIDay, m_uIMonth, m_uIYear);

  if (m_uIHour >= HOURS_PER_DAY || m_uIMinute >= MINUTES_PER_HOUR ||
      m_uISecond >= SECONDS_PER_MINUTE || m_uIMilliSecond >= MILLISECONDS_PER_SECOND) {
    throw make_arithmetic_exception(INVALID_ARGUMENT);
  }

  seconds = int((m_uIHour * SECONDS_PER_HOUR) + (m_uIMinute * SECONDS_PER_MINUTE) + m_uISecond);

  *this += NavTimeSpan(seconds, m_uIMilliSecond);
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Constructor to initialize NavDateTime object using
//! a specified Epoch time.
//!
//! @param      :\n
//!         epoch   seconds since a specified base year\n
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Sets the date of the time object
//!
//! @param      Day the day within [1, days_per_month]
//! @param      Month   the month within [1, 12]
//! @param      Year    the year within [TIME_MIN_YEAR, TIME_MAX_YEAR]
//!
//! @returns        None
//!
//! @throw      INVALID_ARGUMENT if at least one component
//!         exceed its range
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
void NavDateTime::SetDate(unsigned int Day, unsigned int Month, unsigned int Year) {
  int total_days;
  unsigned int days_per_month;

  if (Month < 1 || Month > TIME_MONTHS_PER_YEAR) {
    throw make_arithmetic_exception(INVALID_ARGUMENT);
  }

  if (Year < TIME_MIN_YEAR || Year > TIME_MAX_YEAR) {
    throw make_arithmetic_exception(OUT_OF_RANGE);
  }

  days_per_month = DaysPerMonth[Month - 1];
  if (IsLeapYear(Year) && Month == 2) days_per_month++;
  if (Day < 1 || Day > days_per_month) {
    throw make_arithmetic_exception(INVALID_ARGUMENT);
  }

  // Calcs the extra days due to leap years
  if (Year < TIME_ORIGIN)
    // Doesn't contains the extra day of TIME_ORIGIN
    total_days = -int((TIME_ORIGIN - Year) / 4);
  else
    // It contains the extra day of TIME_ORIGIN
    total_days = (Year - TIME_ORIGIN + 3) / 4;

  // Add number of days of given month
  total_days += Day - 1;

  // Add number of days before given month
  total_days += SumOfDays[Month - 1];

  // Add one more day if date of a leap year is after february
  if (IsLeapYear(Year) && Month > 2) total_days++;

  // If the date is before TIME_ORIGIN it subtracts the number
  // of days from the beginning of the given year (i.e. 1st Jan)
  // If the date is after TIME_ORIGIN it adds the number of days
  // elapsed after the beginning of the given year (i.e 1st Jan)
  total_days += (int(Year) - TIME_ORIGIN) * TIME_DAYS_PER_YEAR;

  // Create a NavTimeSpan as a multiple of number of days
  // NOTE: Since this method is called while creating a static objects
  // we cannot use OneDay because it is dynamically initialised (i.e.
  // not computed at compile time)
  *(static_cast<NavTimeSpan*>(this)) = NavTimeSpan(SECONDS_PER_DAY) * total_days;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Retrieves the date of time object
//!
//! @param      rDay    the day within [1, days_per_month]
//! @param      rMonth  the month within [1, 12]
//! @param      rYear   the year within [TIME_MIN_YEAR, TIME_MAX_YEAR]
//!
//! @returns        None
//!
//! @throw      None
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
void NavDateTime::GetDate(unsigned int& rDay, unsigned int& rMonth, unsigned int& rYear) const {
  int total_days;
  const unsigned int* days_per_month_table;

  // Integer number of hours from origin
  total_days =
      (*static_cast<const NavTimeSpan*>(this) - OneMilliSecond * GetMilliSecsFromMidnight())
          .GetTotalHours();

  // Convert to days
  total_days = total_days / int(HOURS_PER_DAY);

  if (IsNegative()) {
    // Calcs year offset
    rYear = ((-total_days - -total_days / TIME_DAYS_PER_YEAR / 4) / TIME_DAYS_PER_YEAR);

    // Calcs the remaining days of current year
    total_days += rYear * TIME_DAYS_PER_YEAR + rYear / 4;

    // Final year
    rYear = TIME_ORIGIN - rYear;

    if (total_days < 0)
      total_days += IsLeapYear(--rYear) ? TIME_DAYS_PER_YEAR + 1 : TIME_DAYS_PER_YEAR;
  } else {
    // Calcs year offset
    rYear = (total_days - (total_days / TIME_DAYS_PER_YEAR + 3) / 4) / TIME_DAYS_PER_YEAR;

    // Calcs the remaining days of current year
    total_days -= rYear * TIME_DAYS_PER_YEAR + (rYear + 3) / 4;

    // Final year
    rYear += TIME_ORIGIN;
  }

  NavAssert(total_days < TIME_DAYS_PER_YEAR ||
            (IsLeapYear(rYear) && total_days <= TIME_DAYS_PER_YEAR));

  days_per_month_table = (IsLeapYear(rYear)) ? LeapDaysPerMonth : DaysPerMonth;

  for (rMonth = 0; (unsigned int)(total_days) >= days_per_month_table[rMonth]; ++rMonth)
    total_days -= days_per_month_table[rMonth];

  rDay = total_days + 1;
  rMonth++;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Sets the time of the day
//!
//! @param      Hour    hour within [0, 23]
//! @param      Minute  minute within [0, 59]
//! @param      Second  second within [0, 59]
//!
//! @returns        None
//!
//! @throw      INVALID_ARGUMENT if at least one component
//!         exceed its range
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
void NavDateTime::SetTimeOfDay(unsigned int Hour, unsigned int Minute, unsigned int Second) {
  int seconds;

  if (Hour >= HOURS_PER_DAY || Minute >= MINUTES_PER_HOUR || Second >= SECONDS_PER_MINUTE) {
    throw make_arithmetic_exception(INVALID_ARGUMENT);
  }

  seconds = (Hour * SECONDS_PER_HOUR) + (Minute * SECONDS_PER_MINUTE) + Second;

  *this += OneSecond * seconds - OneMilliSecond * GetMilliSecsFromMidnight();
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Retrieves the time of the day
//!
//! @param      rSecond second within [0, 59]
//! @param      rMinute minute within [0, 59]
//! @param      rHour   hour within [0, 23]
//!
//! @returns        None
//!
//! @throw      None
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
void NavDateTime::GetTimeOfDay(unsigned int& rHour, unsigned int& rMinute,
                               unsigned int& rSecond) const {
  rSecond = GetMilliSecsFromMidnight() / MILLISECONDS_PER_SECOND;

  rHour = rSecond / SECONDS_PER_HOUR;
  rMinute = (rSecond % SECONDS_PER_HOUR) / SECONDS_PER_MINUTE;
  rSecond %= SECONDS_PER_MINUTE;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Returns the day of week
//!
//! @param      None
//!
//! @returns        Day of week within [0=monday, 6=sunday]
//!
//! @throw      None
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
unsigned int NavDateTime::GetDayOfWeek(void) const {
  unsigned int result;

  // Integer number of hours from origin
  result = (*static_cast<const NavTimeSpan*>(this) - OneMilliSecond * GetMilliSecsFromMidnight())
               .GetTotalHours();

  // Add an offset, without changing the day in the week, to
  // play with positive number only
  result += (TIME_DAYS_PER_WEEK - 1 + TIME_MIN_OFFSET) / TIME_DAYS_PER_WEEK * TIME_DAYS_PER_WEEK *
            HOURS_PER_DAY;

  return (result / HOURS_PER_DAY + TIME_DEFAULT_DAY_OF_WEEK) % TIME_DAYS_PER_WEEK;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks whether the two Time objects refer to the same day or not.
//!
//! @param      rTime   time object
//!
//! @returns        bool    true if day is same false otherwise
//!
//! @throw      None
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::IsSameDay(const NavDateTime& rTime) const {
  return *static_cast<const NavTimeSpan*>(this) - OneMilliSecond * GetMilliSecsFromMidnight() ==
         static_cast<const NavTimeSpan&>(rTime) - OneMilliSecond * rTime.GetMilliSecsFromMidnight();
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Get the number of milliseconds from latest midnight
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        number of milliseconds
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
unsigned int NavDateTime::GetMilliSecsFromMidnight(void) const {
  int hours;

  // Add an offset to play with positive numbers only.
  // NOTE: Cast to NavTimeSpan is necessary to avoid overflow
  hours = (*(static_cast<const NavTimeSpan*>(this)) + MinTimeSpan).GetTotalHours();

  hours -= hours % HOURS_PER_DAY;

  // Get number of second since beginning of current day
  const NavTimeSpan theThis = *(static_cast<const NavTimeSpan*>(this));
  const NavTimeSpan midnight = (OneHour * hours - MinTimeSpan);
  const NavTimeSpan temp = theThis - midnight;

  const int msec = temp.GetTotalMilliSeconds();
  return msec;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Constructor to initialize NavDateTime object using
//! individual date components given as parameters.
//!
//! @param      Day the day within [1, days_per_month]
//! @param      Month   the month within [1, 12]
//! @param      Year    the year within [TIME_MIN_YEAR, TIME_MAX_YEAR]
//!
//! @throw      INVALID_ARGUMENT If at least one component
//!         exceed its range
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime::NavDateTime(unsigned int Day, unsigned int Month, unsigned int Year) {
  SetDate(Day, Month, Year);
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Subtracts interval time given as NavTimeSpan object from
//! current Time object.
//!
//! @param      rTimeSpan   reference to NavTimeSpan class object
//!
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        resulting time
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime NavDateTime::operator-(const NavTimeSpan& rTimeSpan) const {
  NavTimeSpan result;

  result = *static_cast<const NavTimeSpan*>(this) - rTimeSpan;

  if (result < -MinTimeSpan || result > MaxTimeSpan) throw make_arithmetic_exception(OUT_OF_RANGE);

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Subtracts interval time given as NavTimeSpan object from
//! current Time object changing it.
//!
//! @param      rTimeSpan   reference to NavTimeSpan class object
//!
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        reference to resulting time
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
const NavDateTime& NavDateTime::operator-=(const NavTimeSpan& rTimeSpan) {
  *static_cast<NavTimeSpan*>(this) -= rTimeSpan;

  if (*this < -MinTimeSpan || *this > MaxTimeSpan) throw make_arithmetic_exception(OUT_OF_RANGE);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Subtracts a given Time object from current Time object returning
//! the NavTimeSpan object representing the difference between two
//! Times.
//!
//! @param      rTime   reference to NavDateTime class object
//!
//! @throw      None
//!
//! @returns        Time interval difference
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavDateTime::operator-(const NavDateTime& rTime) const {
  // It calls CNavTimeSpan operator -
  return *(static_cast<const NavTimeSpan*>(this)) - *(static_cast<const NavTimeSpan*>(&rTime));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Adds interval time given as NavTimeSpan object to
//! current Time object.
//!
//! @param      rTimeSpan   reference to NavTimeSpan class object
//!
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        resulting time
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime NavDateTime::operator+(const NavTimeSpan& rTimeSpan) const {
  NavTimeSpan result;

  result = *static_cast<const NavTimeSpan*>(this) + rTimeSpan;

  if (result < -MinTimeSpan || result > MaxTimeSpan) throw make_arithmetic_exception(OUT_OF_RANGE);

  return result;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Adds interval time given as NavTimeSpan object to
//! current Time object changing it.
//!
//! @param      rTimeSpan   reference to NavTimeSpan class object
//!
//! @throw      OUT_OF_RANGE    if an overflow is detected
//!
//! @returns        reference to resulting time
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
const NavDateTime& NavDateTime::operator+=(const NavTimeSpan& rTimeSpan) {
  *static_cast<NavTimeSpan*>(this) += rTimeSpan;

  if (*this < -MinTimeSpan || *this > MaxTimeSpan) throw make_arithmetic_exception(OUT_OF_RANGE);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is equal to the given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the two objects are equal
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator==(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) == *(static_cast<const NavTimeSpan*>(&rTime));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is not equal to the given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the two objects are not equal
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator!=(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) != *(static_cast<const NavTimeSpan*>(&rTime));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is greater than given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the condition is satisfied
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator>(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) > *(static_cast<const NavTimeSpan*>(&rTime));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is greater than or equal to
//! the given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the condition is satisfied
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator>=(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) >= *(static_cast<const NavTimeSpan*>(&rTime));
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is less than the given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the condition is satisfied
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator<(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) < *(static_cast<const NavTimeSpan*>(&rTime));
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks if current Time object is is less than or equal to
//! the given one.
//!
//! @param      rTime   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool    true if the condition is satisfied
//!             false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavDateTime::operator<=(const NavDateTime& rTime) const {
  return *(static_cast<const NavTimeSpan*>(this)) <= *(static_cast<const NavTimeSpan*>(&rTime));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Checks whether a given year is leap year or not.
//!
//! @param  cuIYear Year to check whether it is leap year or not.
//!
//! @throw  None
//!
//! @returns    bool    true if the year is leap year and false otherwise
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::IsLeapYear(const unsigned int& cuIYear) {
  return (((cuIYear % 4 == 0) && (cuIYear % 100 != 0)) || (cuIYear % 400 == 0));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Constructor which takes a NavTimeSpan parameter.
//! Needed to implement the operators or any other methods which requires
//! conversion from NavTimeSpan object.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime::NavDateTime(const NavTimeSpan& rTimeSpan) : NavTimeSpan(rTimeSpan) {}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Adds an interval time given as left operand to a Time object.
//!
//!  @param     rTimeSpan   reference to NavTimeSpan class object
//!  @param     rTime       reference to NavDateTime class Object.
//!
//!  @throw     OUT_OF_RANGE    if an overflow is detected
//!
//!  @returns       NavDateTime Object
//!
//!  \b Thread_Safety:  NA
//!
///////////////////////////////////////////////////////////////////////////////
NavDateTime operator+(const NavTimeSpan& rTimeSpan, const NavDateTime& rTime) {
  return rTime + rTimeSpan;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Convert a NavString (decoding it according to the type provided)
//! into a NavDateTime
//!
//! @param      strDate         string expressing Date
//! @param      sType               type of string to decode
//!
//! @throw      None
//!
//! @returns    true: conversion success
//!             false: convertion impossible
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::FromString(const NavString& strDate,
                             DateStringType sType /* = NO_SPECIFIED_FORMAT */) {
  bool retVal = true;
  unsigned int rDay, rMonth, rYear, rHour, rMinute, rSecond, rMilliSecond;
  size_t token2, token3, token4;

  if (strDate.empty()) return false;

  if (sType == NO_SPECIFIED_FORMAT) return FromFormattedString(strDate);

  rDay = 1;
  rMonth = 1;
  rYear = 2000;
  rHour = 0;
  rMinute = 0;
  rSecond = 0;
  rMilliSecond = 0;

  switch (sType) {
    case YYYYMMDD_FORMAT:
      if (strDate.size() == 8) {
        token2 = 4;
        token3 = 6;
      } else if (strDate.size() == 10) {
        token2 = 5;
        token3 = 8;
      } else
        return false;

      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(token2, 2).c_str());
      rDay = atoi(strDate.substr(token3, 2).c_str());
      break;

    case YYYYMMDDHH_FORMAT:
      if (strDate.size() == 10) {
        token2 = 4;
        token3 = 6;
        token4 = 8;
      } else if (strDate.size() == 13) {
        token2 = 5;
        token3 = 8;
        token4 = 11;
      } else
        return false;

      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(token2, 2).c_str());
      rDay = atoi(strDate.substr(token3, 2).c_str());
      rHour = atoi(strDate.substr(token4, 2).c_str());
      break;

    case YYYYDDMM_FORMAT:
      if (strDate.size() == 8) {
        token2 = 4;
        token3 = 6;
      } else if (strDate.size() == 10) {
        token2 = 5;
        token3 = 8;
      } else
        return false;

      rYear = atoi(strDate.substr(0, 4).c_str());
      rDay = atoi(strDate.substr(token2, 2).c_str());
      rMonth = atoi(strDate.substr(token3, 2).c_str());
      break;

    case MMDDYYYY_FORMAT:
      if (strDate.size() == 8) {
        token2 = 2;
        token3 = 4;
      } else if (strDate.size() == 10) {
        token2 = 3;
        token3 = 6;
      } else
        return false;

      rMonth = atoi(strDate.substr(0, 2).c_str());
      rDay = atoi(strDate.substr(token2, 2).c_str());
      rYear = atoi(strDate.substr(token3, 4).c_str());
      break;

    case DDMMYYYY_FORMAT:
      if (strDate.size() == 8) {
        token2 = 2;
        token3 = 4;
      } else if (strDate.size() == 10) {
        token2 = 3;
        token3 = 6;
      } else
        return false;

      rDay = atoi(strDate.substr(0, 2).c_str());
      rMonth = atoi(strDate.substr(token2, 2).c_str());
      rYear = atoi(strDate.substr(token3, 4).c_str());
      break;

    case YYYYMMDD_HHMMSS_FORMAT:
      if (strDate.size() < 19) return false;

      /* e.g. 2010-12-15 22:34:12 */

      /* format validation, checking for tokens */
      if (strDate[10] != ' ') return false;

      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(5, 2).c_str());
      rDay = atoi(strDate.substr(8, 2).c_str());
      rHour = atoi(strDate.substr(11, 2).c_str());
      rMinute = atoi(strDate.substr(14, 2).c_str());
      rSecond = atoi(strDate.substr(17, 2).c_str());
      break;

    case YYYYMMDDTHHMMSSZ_FORMAT:
      if (strDate.size() < 20) return false;

      /* e.g. 2010-12-15T22:34:12Z */

      /* format validation, checking for tokens */
      if (strDate[10] != 'T' || strDate[19] != 'Z') return false;
      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(5, 2).c_str());
      rDay = atoi(strDate.substr(8, 2).c_str());
      rHour = atoi(strDate.substr(11, 2).c_str());
      rMinute = atoi(strDate.substr(14, 2).c_str());
      rSecond = atoi(strDate.substr(17, 2).c_str());
      break;
    case YYYYMMDDTHHMMSSMMMZ_FORMAT:
      if (strDate.size() < 20) return false;

      if (strDate[10] != 'T' || strDate[strDate.size() - 1] != 'Z') return false;
      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(5, 2).c_str());
      rDay = atoi(strDate.substr(8, 2).c_str());
      rHour = atoi(strDate.substr(11, 2).c_str());
      rMinute = atoi(strDate.substr(14, 2).c_str());
      rSecond = atoi(strDate.substr(17, 2).c_str());
      if (strDate[19] == '.') {
        if (strDate.size() == 22)
          rMilliSecond = atoi(strDate.substr(20, 1).c_str()) * 100;
        else if (strDate.size() == 23)
          rMilliSecond = atoi(strDate.substr(20, 2).c_str()) * 10;
        else if (strDate.size() >= 24)
          rMilliSecond = atoi(strDate.substr(20, 3).c_str());
      }
      break;

    case YYYYMMDD_HHMMSS_MMM_FORMAT:
      if (strDate.size() < 22) return false;

      // Examples:
      // 2010-12-15_22:34:12.666
      // 2010-12-15 22:34:12.666
      // 2010-12-1522:34:12.666
      rYear = atoi(strDate.substr(0, 4).c_str());
      rMonth = atoi(strDate.substr(5, 2).c_str());
      rDay = atoi(strDate.substr(8, 2).c_str());

      // check if there is a delimiter or not
      token2 = 10;
      if (strDate[19] == '.') ++token2;

      rHour = atoi(strDate.substr(token2, 2).c_str());
      rMinute = atoi(strDate.substr(token2 + 3, 2).c_str());
      rSecond = atoi(strDate.substr(token2 + 6, 2).c_str());
      rMilliSecond = atoi(strDate.substr(token2 + 9, 3).c_str());
      break;

    case DDD_DD_MMM_YYYY_HHMMSS_GMT_FORMAT:
      if (strDate.size() != 29) return false;

      /* e.g. Wed, 01 Sep 2004 13:24:52 GMT */

      /* format validation, checking for tokens */
      if (strDate[3] != ',' || strDate[4] != ' ' || strDate[7] != ' ' || strDate[11] != ' ' ||
          strDate[16] != ' ' || strDate[25] != ' ')
        return false;

      if (GetMonthNumberFromName(strDate.substr(8, 3).c_str(), rMonth) == false) return false;

      rDay = atoi(strDate.substr(5, 2).c_str());
      rYear = atoi(strDate.substr(12, 4).c_str());

      rHour = atoi(strDate.substr(17, 2).c_str());
      rMinute = atoi(strDate.substr(20, 2).c_str());
      rSecond = atoi(strDate.substr(23, 2).c_str());
      rMilliSecond = 0;
      break;

    default:
      // unknown format
      retVal = false;
      break;
  }

  if (retVal) {
    try {
      this->SetDate(rDay, rMonth, rYear);
      this->SetTimeOfDay(rHour, rMinute, rSecond);
      NavTimeSpan msec(0, rMilliSecond);
      (*this) += (msec);
    } catch (...) {
      retVal = false;
    }
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Set the date object from given formatted date string
//! The format string contains a set of placeholders that identify the
//! date and time components to be converted.
//! The following is the set of placeholders that can be used as parameters.
//!
//! \n%Y\n      Converts to year (4 digits, zero filled)
//! \n%y\n      Converts to year (2 digits, zero filled)
//! \n%M\n      Converts to month number (starting from 1, 2 digits,
//!             zero filed)
//! \n%m\n      Converts to month number (starting from 1, 1 or 2
//!             digits, not filled)
//! \n%D\n      Converts to day number (starting from 1, 2 digits, zero
//!             filled)
//! \n%d\n      Converts to day number (starting from 1, 1 or 2 digits,
//!             not filled)
//! \n%H\n      Converts to hour number (2 digits, 24 hours, zero
//!             filled)
//! \n%h\n      Converts to hour number (1 or 2 digits, 24 hours, not
//!             filled)
//! \n%G\n      Converts to hour number (2 digits, 12 hours, zero
//!             filled)
//! \n%g\n      Converts to hour number (1 or 2 digits, 12 hours, not
//!             filled)
//! \n%N\n      Converts to minute number (2 digits, zero filled)
//! \n%n\n      Converts to minute number (1 or 2 digits, not filled)
//! \n%S\n      Converts to second number (2 digits, zero filled)
//! \n%s\n      Converts to second number (1 or 2 digits, not filled)
//! \n%<k>l\n   Converts to milliseconds on k digit; k is at most 3
//!             digit number; if it is missing or zero, default
//!             is 3
//! \n%<k>T\n   Converts to month name (long form, blank filled)
//! \n%<k>t\n   Converts to month name (short form, blank filled)
//! \n%<k>W\n   Converts to day of week name (long form, blank filled)
//! \n%<k>w\n   Converts to day of week name (short form, blank filled)
//! \n%<k>A\n   Converts into AM/PM string (blank filled)
//!
//!         If a length <k> is specified, it must be one or two
//!         digits long.
//!
//!         Conversions %T, %t, %W, %w and %A are valid only if the
//!         template parameter is wchar_t.
//!
//!         All conversions truncate the time to lower integer
//!         value, so 999 milliseconds when printed with two digits
//!         is converted as 9.
//!
//! @param      Format the string that specifies the format
//!
//! @returns    true if date object is set
//!            false if formatted string is not supported or invalid
//!
//! \b Thread_Safety:     NA
//!
/////////////////////////////////////////////////////////////////////////////
bool NavDateTime::FromFormattedString(const NavString& inFormattedDate) {
  unsigned int rDay, rMonth, rYear;
  unsigned int rHour, rMin, rSec, rMSec;
  bool retVal = true;

  rDay = 1;
  rMonth = 1;
  rYear = 2000;
  rHour = 0;
  rMin = 0;
  rSec = 0;
  rMSec = 0;
  try {
    NavString delimiters[] = {
        TIME_YEAR_4_CHARACTER,      TIME_YEAR_2_CHARACTER,      TIME_MONTH_2_CHARACTER,
        TIME_MONTH_1_CHARACTER,     TIME_DAY_2_CHARACTER,       TIME_DAY_1_CHARACTER,
        TIME_HOUR_2_CHARACTER,      TIME_HOUR_1_CHARACTER,      TIME_AMPM_HOUR_2_CHARACTER,
        TIME_AMPM_HOUR_1_CHARACTER, TIME_MINUTE_2_CHARACTER,    TIME_MINUTE_1_CHARACTER,
        TIME_SECOND_2_CHARACTER,    TIME_SECOND_1_CHARACTER,    TIME_MILLISECOND_CHARACTER,
        TIME_LONG_MONTH_CHARACTER,  TIME_SHORT_MONTH_CHARACTER, TIME_LONG_DAY_CHARACTER,
        TIME_SHORT_DAY_CHARACTER};

    vector<NavString::size_type> startTokens;

    // Search for all tokens including format strings
    NavString::size_type tokenPos = 0;
    while (tokenPos != NavString::npos) {
      tokenPos = inFormattedDate.find("%", tokenPos);
      if (tokenPos != NavString::npos) {
        startTokens.push_back(tokenPos);
        ++tokenPos;
      }
    }

    // Create all pair of tokens (value and type)
    tokenPos = 0;
    vector<pair<NavString, NavString> > tokens;
    for (vector<NavString::size_type>::iterator it = startTokens.begin(); it != startTokens.end();
         ++it) {
      NavString::size_type delimiterLen = delimiters[0].size();

      NavString::size_type tokenLen = *it - tokenPos;

      // token type string
      NavString token_type = inFormattedDate.substr(*it, delimiterLen);
      for (const auto& delimeter : delimiters) {
        if (token_type == delimeter) {
          NavString token_value = inFormattedDate.substr(tokenPos, tokenLen);
          tokens.push_back(make_pair(token_value, token_type));
          break;
        }
      }

      // move to the next pair
      tokenPos = *it + delimiterLen;
    }

    for (vector<pair<NavString, NavString> >::iterator it = tokens.begin(); it != tokens.end();
         ++it) {
      if (it->second == TIME_YEAR_4_CHARACTER || it->second == TIME_YEAR_2_CHARACTER)
        rYear = atoi((it->first).c_str());
      else if (it->second == TIME_MONTH_2_CHARACTER || it->second == TIME_MONTH_2_CHARACTER)
        rMonth = atoi((it->first).c_str());
      else if (it->second == TIME_SHORT_MONTH_CHARACTER) {
        if (GetMonthNumberFromName((it->first).c_str(), rMonth) == false)
          throw make_arithmetic_exception(INVALID_ARGUMENT);
      } else if (it->second == TIME_DAY_2_CHARACTER || it->second == TIME_DAY_1_CHARACTER)
        rDay = atoi((it->first).c_str());
      else if (it->second == TIME_HOUR_2_CHARACTER || it->second == TIME_HOUR_1_CHARACTER)
        rHour = atoi((it->first).c_str());
      else if (it->second == TIME_MINUTE_2_CHARACTER || it->second == TIME_MINUTE_1_CHARACTER)
        rMin = atoi((it->first).c_str());
      else if (it->second == TIME_SECOND_2_CHARACTER || it->second == TIME_SECOND_1_CHARACTER)
        rSec = atoi((it->first).c_str());
      else if (it->second == TIME_MILLISECOND_CHARACTER)
        rMSec = atoi((it->first).c_str());
      else
        throw make_arithmetic_exception(INVALID_ARGUMENT);
    }

    SetDate(rDay, rMonth, rYear);
    SetTimeOfDay(rHour, rMin, rSec);
    NavTimeSpan msec(0, rMSec);
    (*this) += (msec);
  } catch (...) {
    retVal = false;
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Create a NavString (encoding it according to the type provided)
//! form a NavDateTime
//!
//! @param      outStrDate      string expressing Date
//! @param      sType               type of string to decode
//!
//! @throw      None
//!
//! @returns    true: conversion success
//!             false: convertion impossible
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::ToString(NavString& outStrDate, DateStringType sType,
                           DateDelimiterToken inDelimiter /* = DATE_DELIMITER_NONE */) const {
  bool retVal = false;
  unsigned int rDay, rMonth, rYear, rHour, rMinute, rSecond, rMillisec;

  char buff[128];
  char token[2] = {(char)inDelimiter, '\0'};

  outStrDate.clear();

  GetDate(rDay, rMonth, rYear);
  GetTimeOfDay(rHour, rMinute, rSecond);
  rMillisec = GetMilliSecsFromMidnight() % MILLISECONDS_PER_SECOND;

  switch (sType) {
    case YYYYMMDD_FORMAT:
      sprintf(buff, "%04u%s%02u%s%02u", rYear, token, rMonth, token, rDay);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYMMDDHH_FORMAT:
      sprintf(buff, "%04u%s%02u%s%02u%s%02u", rYear, token, rMonth, token, rDay, token, rHour);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYDDMM_FORMAT:
      sprintf(buff, "%04u%s%02u%s%02u", rYear, token, rDay, token, rMonth);
      outStrDate = buff;
      retVal = true;
      break;

    case MMDDYYYY_FORMAT:
      sprintf(buff, "%02u%s%02u%s%04u", rMonth, token, rDay, token, rYear);
      outStrDate = buff;
      retVal = true;
      break;

    case DDMMYYYY_FORMAT:
      sprintf(buff, "%02u%s%02u%s%04u", rDay, token, rMonth, token, rYear);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYMMDD_HHMMSS_FORMAT:
      sprintf(buff, "%04u-%02u-%02u %02u:%02u:%02u", rYear, rMonth, rDay, rHour, rMinute, rSecond);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYMMDDTHHMMSSZ_FORMAT:
      // DateDelimiterToken insensitive. Standard format defined.
      sprintf(buff, "%04u-%02u-%02uT%02u:%02u:%02uZ", rYear, rMonth, rDay, rHour, rMinute, rSecond);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYMMDDTHHMMSSMMMZ_FORMAT:
      // DateDelimiterToken insensitive. Standard format defined.
      sprintf(buff, "%04u-%02u-%02uT%02u:%02u:%02u.%03uZ", rYear, rMonth, rDay, rHour, rMinute,
              rSecond, rMillisec);
      outStrDate = buff;
      retVal = true;
      break;

    case YYYYMMDD_HHMMSS_MMM_FORMAT:
      sprintf(buff, "%04u-%02u-%02u%s%02u:%02u:%02u.%03u", rYear, rMonth, rDay, token, rHour,
              rMinute, rSecond, rMillisec);
      outStrDate = buff;
      retVal = true;
      break;

    case DDD_DD_MMM_YYYY_HHMMSS_GMT_FORMAT:
      // DateDelimiterToken insensitive. Standard format defined. Network request last modified
      // header.

      const char* weekDayName;
      weekDayName = GetWeekDayNameFromDayIdx(GetDayOfWeek());
      const char* monthName;
      monthName = GetMonthNameFromNumber(rMonth);

      if (weekDayName == NULL || monthName == NULL) return false;

      sprintf(buff, "%s, %02u %s %04u %02u:%02u:%02u GMT", weekDayName, rDay, monthName, rYear,
              rHour, rMinute, rSecond);
      outStrDate = buff;
      retVal = true;
      break;

    default:
      // unknown format
      retVal = false;
      break;
  }

  return retVal;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Returns the number of the month given the month name (3 chars) as ouput parameter.
//! On success returns true, false otherwise.
//!
//! @param      inName               the name (3 chars) of the month e.g. Jan
//!
//! @param     outMonthNumber       the number of the month [1-12]
//!
//! @throw      None
//!
//! @returns    bool     true = success
//!                     false otherwise
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
bool NavDateTime::GetMonthNumberFromName(const char* inName, unsigned int& outMonthNumber) {
  for (size_t i = 0; i < TIME_MONTHS_PER_YEAR; ++i) {
    if (strcmp(inName, Months[i]) == 0) {
      outMonthNumber = i + 1;
      return true;
    }
  }

  return false;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Returns the name (3 chars) of the month which number (1-12) is given by argument.
//! If month number is not in the range [1-12] it returns NULL.
//!
//! @param      inMonthNum     the number of the month
//!
//! @throw      None
//!
//! @returns    the name of the month on success, NULL otherwise
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
const char* NavDateTime::GetMonthNameFromNumber(unsigned int inMonthNum) {
  if (inMonthNum < 1 || inMonthNum > 12) return NULL;

  return Months[inMonthNum - 1];
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//!  Returns the name (3 chars) of the weekday which index (range [0-6]) is given by
//!  argument. If index is not in that range it returns NULL.
//!
//! @param      inDayIdx        the index of the week day
//!
//! @throw      None
//!
//! @returns   the name of the day on success, NULL otherwise
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
const char* NavDateTime::GetWeekDayNameFromDayIdx(unsigned int inDayIdx) {
  if (inDayIdx > 6) return NULL;

  return DaysOfWeek[inDayIdx];
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Facility API to create a formatter in Debugger and improve human readibility.
//! e.g. on Xcode type:
//! (lldb) type summary add --summary-string "{$VAR.ToString()}:s" NavDateTime
//!
///////////////////////////////////////////////////////////////////////////////
NavString NavDateTime::ToString() {
  NavString tmp;
  ToString(tmp, YYYYMMDDTHHMMSSZ_FORMAT);
  return tmp;
}

}  // namespace Navionics
