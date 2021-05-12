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
//! @date   08 Sep 2006
//!
//! SYSTEM: NAVIONICS
//!
//! COPYRIGHT:  Copyright NAVIONICS S.p.A 2006
//!
//!
//! FUNCTION:   Holds the declaration of class NavTimeSpan
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
//!     <td> 26 Oct 2006        </td>
//!     <td> M.Fiorentini/F.Ilario  </td>
//!     <td> Revisiting of the overall class against
//!     code standard and test validation   </td>
//! </tr>
//! <tr>
//!     <td> 08 Nov 2006        </td>
//!     <td> M.Fiorentini/F.Ilario  </td>
//!     <td> Added operators *      </td>
//! </tr>
//! <tr>
//!     <td> 15 Dec 2006            </td>
//!     <td> F.Ilario               </td>
//!     <td> Added OneMilliSecond constant  </td>
//! </tr>
//! <tr>
//!     <td> 18 Dec 2006            </td>
//!     <td> F.Ilario               </td>
//!     <td> Removed volatile keyword to all methods in order
//!         to bypass a GNU_4 compiler bug.
//!         For performance test only added a duplicated
//!         assignment operator that assign from not volatile
//!         object to volatile object.
//!     </td>
//! </tr>
//! </table>
///////////////////////////////////////////////////////////////////////////
#ifndef NAVIONICS_SYSTEM_TIMESPAN_H
#define NAVIONICS_SYSTEM_TIMESPAN_H

#include <NavString.h>
#include <NavArithmeticException.h>

//! Number of seconds per minute
#define SECONDS_PER_MINUTE 60

//! Number of minutes per hour
#define MINUTES_PER_HOUR 60

//! Number of hours per day
#define HOURS_PER_DAY 24

//! Number of seconds per hour
#define SECONDS_PER_HOUR (SECONDS_PER_MINUTE * MINUTES_PER_HOUR)

//! Number of seconds per day
#define SECONDS_PER_DAY (SECONDS_PER_HOUR * HOURS_PER_DAY)

//! Number of minutes per day
#define MINUTES_PER_DAY (HOURS_PER_DAY * MINUTES_PER_HOUR)

//! Number of milliseconds per second
#define MILLISECONDS_PER_SECOND 1000

//! Number of milliseconds per minute
#define MILLISECONDS_PER_MINUTE (MILLISECONDS_PER_SECOND * SECONDS_PER_MINUTE)

//! Number of milliseconds per hour
#define MILLISECONDS_PER_HOUR (MILLISECONDS_PER_SECOND * SECONDS_PER_HOUR)

//! Scaling factor between higher and lower part components of a time span object
//! the lower part uses only 16 bits while higher uses 32 bits both
//! expressed in millseconds.
#define TIME_SPAN_SCALE 0x10000

//! Maximum time span range allowed.
//! It is 150 years in milliseconds / TIME_SPAN_SCALE
#define TIME_SPAN_MAX 72180175

//! Minimum time span range allowed.
#define TIME_SPAN_MIN (-TIME_SPAN_MAX)

namespace Navionics {

struct STimeComponents {
  bool IsNegative;
  unsigned int m_uIDay;
  unsigned int m_uIHour;
  unsigned int m_uIMinute;
  unsigned int m_uISecond;
  unsigned int m_uMilliSecond;
};

////////////////////////////////////////////////////////////////////////////
//! @class NavTimeSpan
//! NavTimeSpan class provides functions and operators to manipulate
//! time interval.
//! A time span value is composed by an higher part of 32 bits
//! and a lower part of 16 bits. The total 48 bits are expressed
//! in milliseconds.
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
//! @see SECONDS_PER_MINUTE
//! @see MINUTES_PER_HOUR
//! @see HOURS_PER_DAY
//! @see SECONDS_PER_HOUR
//! @see SECONDS_PER_DAY
//! @see MINUTES_PER_DAY
//! @see MILLISECONDS_PER_SECOND
//! @see MILLISECONDS_PER_MINUTE
//! @see MILLISECONDS_PER_HOUR
//! @see TIME_SPAN_SCALE
//! @see TIME_SPAN_MAX
//! @see TIME_SPAN_MIN
//! @see NavTimeSpan operator *
////////////////////////////////////////////////////////////////////////////
class NavTimeSpan {
 public:
  //! @property OneMilliSecond
  //! represents a time span of one millisecond
  static const NavTimeSpan OneMilliSecond;
  //! @property OneSecond
  //! represents a time span of one second
  static const NavTimeSpan OneSecond;
  //! @property OneMinute
  //! represents a time span of one minute
  static const NavTimeSpan OneMinute;
  //! @property OneHour
  //! represents a time span of one hour
  static const NavTimeSpan OneHour;
  //! @property OneDay
  //! represents a time span of one day
  static const NavTimeSpan OneDay;

  NavTimeSpan(void);

  explicit NavTimeSpan(int IzzSecond);

  NavTimeSpan(int IzzSecond, unsigned int uIzzMilliSecond);

  NavTimeSpan(const NavTimeSpan& rTimeSpan);

  NavTimeSpan operator-(void) const;

  NavTimeSpan operator-(const NavTimeSpan& rTimeSpan) const;

  const NavTimeSpan& operator-=(const NavTimeSpan& rTimeSpan);

  NavTimeSpan operator+(const NavTimeSpan& rTimeSpan) const;

  const NavTimeSpan& operator+=(const NavTimeSpan& rTimeSpan);

  NavTimeSpan operator*(const int& Multiplier) const;

  const NavTimeSpan& operator*=(const int& Multiplier);

  bool operator==(const NavTimeSpan& rTimeSpan) const;

  bool operator!=(const NavTimeSpan& rTimeSpan) const;

  bool operator>(const NavTimeSpan& rTimeSpan) const;

  bool operator>=(const NavTimeSpan& rTimeSpan) const;

  bool operator<(const NavTimeSpan& rTimeSpan) const;

  bool operator<=(const NavTimeSpan& rTimeSpan) const;

  const NavTimeSpan& operator=(const NavTimeSpan& rTimeSpan);

  int GetTotalMilliSeconds(void) const;

  int GetTotalSeconds(void) const;

  int GetTotalMinutes(void) const;

  int GetTotalHours(void) const;

  void GetTimeComponents(STimeComponents& rSTime) const;

  bool IsZero(void) const;

  bool IsNotZero(void) const;

  bool IsNegative(void) const;

  bool IsPositive(void) const;

  NavTimeSpan Abs(void) const;

  // returns the number of byte written. The buffer is externally allocated
  unsigned int Serialize(unsigned char* io_buffer) const;

  // returns the number of bytes to store the class
  unsigned int GetSerializedSize() const;

  // returns the number of bytes used.
  unsigned int Deserialize(const unsigned char* i_buffer);

 protected:
  //! @property m_IHighPart
  //! Holds higher part of time span value (32 bits are used)
  int m_IHighPart;

  //! @property m_uILowPart
  //! Holds lower part of time span value (16 bits are used)
  unsigned int m_uILowPart;
};

NavTimeSpan operator*(const int& Multiplier, const NavTimeSpan& rTimeSpan);

void multiply_32_for_32(unsigned int FirstOperand, unsigned int SecondOperand,
                        volatile unsigned int& ResultHighPart,
                        volatile unsigned int& ResultLowPart);

void Divide_by_60(int value, volatile int& quotient, volatile int& remainder);

void Divide_by_1000(int value, volatile int& quotient, volatile int& remainder);
}  // namespace Navionics

#endif  // NAVIONICS_SYSTEM_TIMESPAN_H
