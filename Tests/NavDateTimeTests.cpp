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
    @brief Regression tests for NavDateTime

    Copyright 2019 by Garmin Ltd. or its subsidiaries.
*/

#define DBG_MODULE "Acdb"
#define DBG_TAG "NavDateTimeTests"

#include <map>
#include "NavDateTime.h"
#include "TF_pub.h"

namespace Acdb {
namespace Test {
using namespace Navionics;

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test NavDateTime default constructor.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.constructor.default") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime expected{TIME_ORIGIN, 1, 1, 0, 0, 0, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime actual;

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Default constructor");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test NavDateTime date (day/month/year) constructor.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.constructor.date") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int expectedDay = 23;
  const unsigned int expectedMonth = 5;
  const unsigned int expectedYear = 2018;
  const unsigned int expectedHour = 0;
  const unsigned int expectedMinute = 0;
  const unsigned int expectedSecond = 0;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime actual{expectedDay, expectedMonth, expectedYear};

  unsigned int actualDay;
  unsigned int actualMonth;
  unsigned int actualYear;
  unsigned int actualHour;
  unsigned int actualMinute;
  unsigned int actualSecond;

  actual.GetDate(actualDay, actualMonth, actualYear);
  actual.GetTimeOfDay(actualHour, actualMinute, actualSecond);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expectedYear == actualYear, "Date constructor, year");
  TF_assert_msg(state, expectedMonth == actualMonth, "Date constructor, month");
  TF_assert_msg(state, expectedDay == actualDay, "Date constructor, day");
  TF_assert_msg(state, expectedHour == actualHour, "Date constructor, hour");
  TF_assert_msg(state, expectedMinute == actualMinute, "Date constructor, minute");
  TF_assert_msg(state, expectedSecond == actualSecond, "Date constructor, second");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test NavDateTime constructor with date/time parameters.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.constructor.full") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int expectedDay = 23;
  const unsigned int expectedMonth = 5;
  const unsigned int expectedYear = 2018;
  const unsigned int expectedHour = 9;
  const unsigned int expectedMinute = 30;
  const unsigned int expectedSecond = 1;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime actual{
      expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond, 0};

  unsigned int actualDay;
  unsigned int actualMonth;
  unsigned int actualYear;
  unsigned int actualHour;
  unsigned int actualMinute;
  unsigned int actualSecond;

  actual.GetDate(actualDay, actualMonth, actualYear);
  actual.GetTimeOfDay(actualHour, actualMinute, actualSecond);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expectedYear == actualYear, "Full constructor, year");
  TF_assert_msg(state, expectedMonth == actualMonth, "Full constructor, month");
  TF_assert_msg(state, expectedDay == actualDay, "Full constructor, day");
  TF_assert_msg(state, expectedHour == actualHour, "Full constructor, hour");
  TF_assert_msg(state, expectedMinute == actualMinute, "Full constructor, minute");
  TF_assert_msg(state, expectedSecond == actualSecond, "Full constructor, second");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test comparison operators.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.comparison") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime navDateTime{2018, 5, 23, 9, 30, 1, 0};
  NavDateTime navDateTimeEqual{2018, 5, 23, 9, 30, 1, 0};
  NavDateTime navDateTimeGreater{2018, 5, 23, 9, 31, 1, 0};
  NavDateTime navDateTimeLess{2018, 5, 23, 9, 29, 1, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------

  // Operator ==
  TF_assert_msg(state, (navDateTime == navDateTime), "Comparison ==");
  TF_assert_msg(state, (navDateTimeEqual == navDateTime), "Comparison ==");
  TF_assert_msg(state, (navDateTimeGreater == navDateTime) == false, "Comparison ==");
  TF_assert_msg(state, (navDateTimeLess == navDateTime) == false, "Comparison ==");

  // Operator !=
  TF_assert_msg(state, (navDateTime != navDateTime) == false, "Comparison !=");
  TF_assert_msg(state, (navDateTimeEqual != navDateTime) == false, "Comparison !=");
  TF_assert_msg(state, (navDateTimeGreater != navDateTime), "Comparison !=");
  TF_assert_msg(state, (navDateTimeLess != navDateTime), "Comparison !=");

  // Operator >
  TF_assert_msg(state, (navDateTime > navDateTime) == false, "Comparison >");
  TF_assert_msg(state, (navDateTimeEqual > navDateTime) == false, "Comparison >");
  TF_assert_msg(state, (navDateTimeGreater > navDateTime), "Comparison >");
  TF_assert_msg(state, (navDateTimeLess > navDateTime) == false, "Comparison >");

  // Operator >=
  TF_assert_msg(state, (navDateTime >= navDateTime), "Comparison >=");
  TF_assert_msg(state, (navDateTimeEqual >= navDateTime), "Comparison >=");
  TF_assert_msg(state, (navDateTimeGreater >= navDateTime), "Comparison >=");
  TF_assert_msg(state, (navDateTimeLess >= navDateTime) == false, "Comparison >=");

  // Operator <
  TF_assert_msg(state, (navDateTime < navDateTime) == false, "Comparison <");
  TF_assert_msg(state, (navDateTimeEqual < navDateTime) == false, "Comparison <");
  TF_assert_msg(state, (navDateTimeGreater < navDateTime) == false, "Comparison <");
  TF_assert_msg(state, (navDateTimeLess < navDateTime), "Comparison <");

  // Operator <=
  TF_assert_msg(state, (navDateTime <= navDateTime), "Comparison <=");
  TF_assert_msg(state, (navDateTimeEqual <= navDateTime), "Comparison <=");
  TF_assert_msg(state, (navDateTimeGreater <= navDateTime) == false, "Comparison <=");
  TF_assert_msg(state, (navDateTimeLess <= navDateTime), "Comparison <=");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test setting a date in NavDateTime from string.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.conversion.from_string") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime expectedDate{2018, 5, 23, 0, 0, 0, 0};
  NavDateTime expectedDateHour{2018, 5, 23, 9, 0, 0, 0};
  NavDateTime expectedDateTime{2018, 5, 23, 9, 30, 1, 0};
  NavDateTime expectedDateTimeMs{2018, 5, 23, 9, 30, 1, 123};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  // Date only
  NavDateTime actualDate;
  NavDateTime actualDate2;
  NavDateTime actualDate3;
  NavDateTime actualDate4;

  actualDate.FromString("2018-05-23", YYYYMMDD_FORMAT);
  actualDate2.FromString("2018-23-05", YYYYDDMM_FORMAT);
  actualDate3.FromString("05-23-2018", MMDDYYYY_FORMAT);
  actualDate4.FromString("23-05-2018", DDMMYYYY_FORMAT);

  // Date and hours
  NavDateTime actualDateHour;

  actualDateHour.FromString("2018-05-23-09", YYYYMMDDHH_FORMAT);

  // Date and time (no milliseconds)
  NavDateTime actualDateTime;
  NavDateTime actualDateTime2;
  NavDateTime actualDateTime3;

  actualDateTime.FromString("Wed, 23 May 2018 09:30:01 GMT", DDD_DD_MMM_YYYY_HHMMSS_GMT_FORMAT);
  actualDateTime2.FromString("2018-05-23T09:30:01Z", YYYYMMDDTHHMMSSZ_FORMAT);
  actualDateTime3.FromString("2018-05-23 09:30:01", YYYYMMDD_HHMMSS_FORMAT);

  // Date and time (milliseconds)
  NavDateTime actualDateTimeMs;
  NavDateTime actualDateTimeMs2;

  actualDateTimeMs.FromString("2018%YMay%t23%d9%h30%n1%s123%l", NO_SPECIFIED_FORMAT);
  actualDateTimeMs2.FromString("2018-05-23T09:30:01.123Z", YYYYMMDD_HHMMSS_MMM_FORMAT);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expectedDate == actualDate, "From string");
  TF_assert_msg(state, expectedDate == actualDate2, "From string");
  TF_assert_msg(state, expectedDate == actualDate3, "From string");
  TF_assert_msg(state, expectedDate == actualDate4, "From string");

  TF_assert_msg(state, expectedDateHour == actualDateHour, "From string");

  TF_assert_msg(state, expectedDateTime == actualDateTime, "From string");
  TF_assert_msg(state, expectedDateTime == actualDateTime2, "From string");
  TF_assert_msg(state, expectedDateTime == actualDateTime3, "From string");

  TF_assert_msg(state, expectedDateTimeMs == actualDateTimeMs, "From string");
  TF_assert_msg(state, expectedDateTimeMs == actualDateTimeMs2, "From string");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test setting a date in NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.conversion.to_string") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime dateTime{2018, 5, 23, 9, 30, 1, 0};

  std::map<DateStringType, std::string> expected{
      {YYYYMMDD_FORMAT, "2018-05-23"},
      {YYYYMMDDHH_FORMAT, "2018-05-23-09"},
      {YYYYDDMM_FORMAT, "2018-23-05"},
      {MMDDYYYY_FORMAT, "05-23-2018"},
      {DDMMYYYY_FORMAT, "23-05-2018"},
      {YYYYMMDD_HHMMSS_FORMAT, "2018-05-23 09:30:01"},
      {YYYYMMDDTHHMMSSZ_FORMAT, "2018-05-23T09:30:01Z"},
      {YYYYMMDD_HHMMSS_MMM_FORMAT, "2018-05-23-09:30:01.000"},
      {DDD_DD_MMM_YYYY_HHMMSS_GMT_FORMAT, "Wed, 23 May 2018 09:30:01 GMT"}};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  for (std::map<DateStringType, std::string>::iterator it = expected.begin(); it != expected.end();
       ++it) {
    std::string actual;
    dateTime.ToString(actual, it->first, DATE_DELIMITER_DASH);

    TF_assert_msg(state, it->second == actual, "Get date string");
  }
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test setting a date in NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.getters.get_date_time") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int expectedDay = 21;
  const unsigned int expectedMonth = 5;
  const unsigned int expectedYear = 2018;
  const unsigned int expectedHour = 9;
  const unsigned int expectedMinute = 14;
  const unsigned int expectedSecond = 31;

  NavDateTime dateTime{
      expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  unsigned int actualDay = 0;
  unsigned int actualMonth = 0;
  unsigned int actualYear = 0;
  unsigned int actualHour = 0;
  unsigned int actualMinute = 0;
  unsigned int actualSecond = 0;

  dateTime.GetDate(actualDay, actualMonth, actualYear);
  dateTime.GetTimeOfDay(actualHour, actualMinute, actualSecond);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expectedDay == actualDay, "Get date and time, day");
  TF_assert_msg(state, expectedMonth == actualMonth, "Get date and time, month");
  TF_assert_msg(state, expectedYear == actualYear, "Get date and time, year");
  TF_assert_msg(state, expectedHour == actualHour, "Get date and time, hour");
  TF_assert_msg(state, expectedMinute == actualMinute, "Get date and time, minute");
  TF_assert_msg(state, expectedSecond == actualSecond, "Get date and time, second");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test getting day of week.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.info.get_day_of_week") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int expected = 2;  // Wednesday

  NavDateTime dateTime{2018, 5, 23, 9, 30, 1, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  unsigned int actual = dateTime.GetDayOfWeek();

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Get day of week");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test same day check.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.info.is_same_day") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime dateTime{2018, 5, 23, 9, 30, 1, 0};
  NavDateTime dateTimeSame{2018, 5, 23, 23, 0, 0, 0};
  NavDateTime dateTimeNotSame{2019, 5, 23, 23, 0, 0, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, dateTime.IsSameDay(dateTimeSame), "Same day");
  TF_assert_msg(state, !dateTime.IsSameDay(dateTimeNotSame), "Not same day");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test leap year check.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.info.is_leap_year") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int leapYear = 2020;
  const unsigned int notLeapYear = 2018;

  // Edge case: years divisible by 100 aren't leap years unless they are also divisible by 400.
  const unsigned int leapYear2 = 2000;
  const unsigned int notLeapYear2 = 2100;

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, NavDateTime::IsLeapYear(leapYear), "Leap year");
  TF_assert_msg(state, NavDateTime::IsLeapYear(leapYear2), "Leap year");
  TF_assert_msg(state, !NavDateTime::IsLeapYear(notLeapYear), "Not leap year");
  TF_assert_msg(state, !NavDateTime::IsLeapYear(notLeapYear2), "Not leap year");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test adding a duration to a NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.math.add_duration") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int day = 21;
  const unsigned int month = 5;
  const unsigned int year = 2018;
  const unsigned int hour = 9;
  const unsigned int minute = 14;
  const unsigned int second = 31;

  NavDateTime expected{2018, 5, 23, 9, 30, 1, 0};

  const NavTimeSpan duration{2 * SECONDS_PER_DAY + 15 * SECONDS_PER_MINUTE + 30};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime dateTime{year, month, day, hour, minute, second, 0};

  // operator + (NavDateTime + duration)
  NavDateTime actual = dateTime + duration;

  // operator +=
  NavDateTime actual2 = dateTime;
  actual2 += duration;

  // operator + (duration + NavDateTime)
  NavDateTime actual3 = duration + dateTime;

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Add duration +");
  TF_assert_msg(state, expected == actual2, "Add duration +=");
  TF_assert_msg(state, expected == actual3, "Add duration + (duration LHS)");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test subtracting a NavDateTime from a NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.math.subtract_datetime") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  NavDateTime dateTime1{2018, 5, 23, 9, 30, 1, 0};
  NavDateTime dateTime2{2018, 5, 21, 9, 14, 31, 0};

  const NavTimeSpan expected{2 * SECONDS_PER_DAY + 15 * SECONDS_PER_MINUTE + 30};
  const NavTimeSpan expected2{-1 * (2 * SECONDS_PER_DAY + 15 * SECONDS_PER_MINUTE + 30)};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavTimeSpan actual = dateTime1 - dateTime2;
  NavTimeSpan actual2 = dateTime2 - dateTime1;

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Subtract datetime");
  TF_assert_msg(state, expected2 == actual2, "Subtract datetime 2");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test subtracting a duration from a NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.math.subtract_duration") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int day = 23;
  const unsigned int month = 5;
  const unsigned int year = 2018;
  const unsigned int hour = 9;
  const unsigned int minute = 30;
  const unsigned int second = 1;

  NavDateTime expected{2018, 5, 21, 9, 14, 31, 0};

  const NavTimeSpan duration{2 * SECONDS_PER_DAY + 15 * SECONDS_PER_MINUTE + 30};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime dateTime{year, month, day, hour, minute, second, 0};

  // operator -
  NavDateTime actual = dateTime - duration;

  // operator -=
  NavDateTime actual2 = dateTime;
  actual2 -= duration;

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Subtract duration -");
  TF_assert_msg(state, expected == actual2, "Subtract duration -=");
}

//----------------------------------------------------------------
//!
//!   @public
//!   @detail
//!         Test setting a date in NavDateTime.
//!
//----------------------------------------------------------------
TF_TEST("navdatetime.setters.set_date_time") {
  // ----------------------------------------------------------
  // Arrange
  // ----------------------------------------------------------
  const unsigned int expectedDay = 21;
  const unsigned int expectedMonth = 5;
  const unsigned int expectedYear = 2018;
  const unsigned int expectedHour = 9;
  const unsigned int expectedMinute = 14;
  const unsigned int expectedSecond = 31;

  NavDateTime expected{
      expectedYear, expectedMonth, expectedDay, expectedHour, expectedMinute, expectedSecond, 0};

  // ----------------------------------------------------------
  // Act
  // ----------------------------------------------------------
  NavDateTime actual;

  actual.SetDate(expectedDay, expectedMonth, expectedYear);
  actual.SetTimeOfDay(expectedHour, expectedMinute, expectedSecond);

  // ----------------------------------------------------------
  // Assert
  // ----------------------------------------------------------
  TF_assert_msg(state, expected == actual, "Set date and time");
}

}  // end of namespace Test
}  // end of namespace Acdb
