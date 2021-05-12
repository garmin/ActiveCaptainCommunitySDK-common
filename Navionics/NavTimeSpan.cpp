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

#include "navionics.h"

#include "NavTimeSpan.h"

// TODO -- This will be fixed in a future change.
#ifdef _MSC_VER
#pragma warning(disable : 4146)
#endif

namespace Navionics {

//! represents a time span of one millisecond
const NavTimeSpan NavTimeSpan::OneMilliSecond(0, 1);

//! Constant representing a time span of one second
const NavTimeSpan NavTimeSpan::OneSecond(1);

//! Constant representing a time span of one minute
const NavTimeSpan NavTimeSpan::OneMinute(SECONDS_PER_MINUTE);

//! Constant representing a time span of one hour
const NavTimeSpan NavTimeSpan::OneHour(SECONDS_PER_HOUR);

//! Constant representing a time span of one day
const NavTimeSpan NavTimeSpan::OneDay(SECONDS_PER_DAY);

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Default Constructor. Initializes the members with zero.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan::NavTimeSpan(void) {
  m_IHighPart = 0;
  m_uILowPart = 0;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Constructor to initialize NavTimeSpan object using seconds
//! given as parameter. No Overflow is possible.
//!
//! @param      IzzSecond number of seconds to be set
//!
//! @throw      None
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan::NavTimeSpan(int IzzSecond) {
  int wkIquot;
  int wkIrem;

  if (IzzSecond >= 0) {
    wkIquot = (unsigned int)(IzzSecond / TIME_SPAN_SCALE);
    wkIrem = (unsigned int)(IzzSecond % TIME_SPAN_SCALE);
  } else {
    wkIquot = -(int)((unsigned int)((-IzzSecond) / TIME_SPAN_SCALE));
    wkIrem = -(int)((unsigned int)((-IzzSecond) % TIME_SPAN_SCALE));
  }

  if (wkIrem < 0) {
    --wkIquot;
    wkIrem += TIME_SPAN_SCALE;
  }

  m_IHighPart = wkIquot * MILLISECONDS_PER_SECOND;
  m_uILowPart = wkIrem * MILLISECONDS_PER_SECOND;
  m_IHighPart += m_uILowPart / TIME_SPAN_SCALE;
  m_uILowPart %= TIME_SPAN_SCALE;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Constructor to initialize NavTimeSpan object using
//! seconds and milliseconds given as parameters.
//! No Overflow is possible.
//!
//! @param      IzzSecond number of seconds
//! @param      uIzzMilliSecond number of milliseconds
//!
//! @throw      INVALID_ARGUMENT If milliseconds exceed its range
//!         (i.e. grater than 999).
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan::NavTimeSpan(int IzzSecond, unsigned int uIzzMilliSecond) {
  int wkIquot;
  int wkIrem;

  if (uIzzMilliSecond >= MILLISECONDS_PER_SECOND) {
    throw make_arithmetic_exception(INVALID_ARGUMENT);
  }

  if (IzzSecond >= 0) {
    wkIquot = (unsigned int)(IzzSecond / TIME_SPAN_SCALE);
    wkIrem = (unsigned int)(IzzSecond % TIME_SPAN_SCALE);
  } else {
    wkIquot = -(int)((unsigned int)((-IzzSecond) / TIME_SPAN_SCALE));
    wkIrem = -(int)((unsigned int)((-IzzSecond) % TIME_SPAN_SCALE));
  }

  if (wkIrem < 0) {
    --wkIquot;
    wkIrem += TIME_SPAN_SCALE;
  }

  // Does Not Cause an Overflow
  m_IHighPart = wkIquot * MILLISECONDS_PER_SECOND;
  m_uILowPart = wkIrem * MILLISECONDS_PER_SECOND + uIzzMilliSecond;
  m_IHighPart += m_uILowPart / TIME_SPAN_SCALE;
  m_uILowPart %= TIME_SPAN_SCALE;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Copy Constructor which takes a const parameter.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        NA
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan::NavTimeSpan(const NavTimeSpan& rTimeSpan) {
  m_IHighPart = rTimeSpan.m_IHighPart;
  m_uILowPart = rTimeSpan.m_uILowPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Unary - operator to toggle the sign of the members
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        NavTimeSpan resulting object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavTimeSpan::operator-(void) const {
  NavTimeSpan wkObjTimeSpan;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  if (m_uILowPart > 0) {
    wkObjTimeSpan.m_IHighPart = -m_IHighPart - 1;
    wkObjTimeSpan.m_uILowPart = TIME_SPAN_SCALE - m_uILowPart;
  } else {
    wkObjTimeSpan.m_IHighPart = -m_IHighPart;
    wkObjTimeSpan.m_uILowPart = 0;
  }

  return wkObjTimeSpan;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Subtracts the given NavTimeSpan object from the current one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan resulting object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavTimeSpan::operator-(const NavTimeSpan& rTimeSpan) const {
  NavTimeSpan result;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_uILowPart >= rTimeSpan.m_uILowPart) {
    result.m_uILowPart = m_uILowPart - rTimeSpan.m_uILowPart;

    result.m_IHighPart = m_IHighPart - rTimeSpan.m_IHighPart;
  } else {
    result.m_uILowPart = m_uILowPart + (TIME_SPAN_SCALE - rTimeSpan.m_uILowPart);

    result.m_IHighPart = m_IHighPart - rTimeSpan.m_IHighPart - 1;
  }
  if (result.m_IHighPart > TIME_SPAN_MAX || result.m_IHighPart < TIME_SPAN_MIN - 1 ||
      (result.m_IHighPart == TIME_SPAN_MIN - 1 && result.m_uILowPart == 0)) {
    throw make_arithmetic_exception(OUT_OF_RANGE);
  }

  return result;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Subtracts the given NavTimeSpan object from the current one changing it.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan reference to changed current object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
const NavTimeSpan& NavTimeSpan::operator-=(const NavTimeSpan& rTimeSpan) {
  return *this = *this - rTimeSpan;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Adds the given NavTimeSpan object from the current one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan resulting object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavTimeSpan::operator+(const NavTimeSpan& rTimeSpan) const {
  NavTimeSpan result;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_uILowPart >= TIME_SPAN_SCALE - rTimeSpan.m_uILowPart) {
    result.m_uILowPart = m_uILowPart - (TIME_SPAN_SCALE - rTimeSpan.m_uILowPart);

    result.m_IHighPart = m_IHighPart + rTimeSpan.m_IHighPart + 1;
  } else {
    result.m_uILowPart = m_uILowPart + rTimeSpan.m_uILowPart;

    result.m_IHighPart = m_IHighPart + rTimeSpan.m_IHighPart;
  }

  if (result.m_IHighPart > TIME_SPAN_MAX || result.m_IHighPart < TIME_SPAN_MIN - 1 ||
      (result.m_IHighPart == TIME_SPAN_MIN - 1 && result.m_uILowPart == 0)) {
    throw make_arithmetic_exception(OUT_OF_RANGE);
  }

  return result;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Adds the given NavTimeSpan object from the current one changing it.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan reference to changed current object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
const NavTimeSpan& NavTimeSpan::operator+=(const NavTimeSpan& rTimeSpan) {
  return *this = *this + rTimeSpan;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Multiply the current NavTimeSpan object by a given number as right operand.
//! This is useful to calculate a multiple of a NavTimeSpan object
//! with an operator syntax.
//! For instance we can build NavTimeSpan object like:
//! OneDay * number_of_days
//! OneHour * number_of_hours
//!
//! @param      Multiplier  reference to the multiplier
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan resulting object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavTimeSpan::operator*(const int& Multiplier) const {
  NavTimeSpan result;
  bool Negative;
  unsigned int operand;
  unsigned int carry;
  unsigned int high_part;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  Negative = IsNegative();
  if (Negative)
    result = -*this;
  else
    result = *this;

  if (Multiplier < 0) {
    // Modular properties of unsigned arithmetic
    // ensure that the result is correct for all
    // possible values of operand
    operand = -static_cast<unsigned int>(Multiplier);
    Negative = !Negative;
  } else
    operand = static_cast<unsigned int>(Multiplier);

  multiply_32_for_32(operand, static_cast<unsigned int>(result.m_IHighPart), carry, high_part);

  if (carry > 0) throw make_arithmetic_exception(OUT_OF_RANGE);

  multiply_32_for_32(operand, result.m_uILowPart, carry, operand);

  if (carry > TIME_SPAN_MAX / TIME_SPAN_SCALE) throw make_arithmetic_exception(OUT_OF_RANGE);

  carry = carry * TIME_SPAN_SCALE + operand / TIME_SPAN_SCALE;

  result.m_uILowPart = operand % TIME_SPAN_SCALE;

  if (high_part > TIME_SPAN_MAX - carry) throw make_arithmetic_exception(OUT_OF_RANGE);

  result.m_IHighPart = int(high_part + carry);

  return Negative ? -result : result;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Multiply and change the current NavTimeSpan object by a given number.
//!
//! @param      Multiplier  reference to the multiplier
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan reference to changed current object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
const NavTimeSpan& NavTimeSpan::operator*=(const int& Multiplier) {
  *this = *this * Multiplier;
  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is equal to the given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the two objects are equal
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator==(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  return m_IHighPart == rTimeSpan.m_IHighPart && m_uILowPart == rTimeSpan.m_uILowPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is not equal to the given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the two objects are not equal
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator!=(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  return m_IHighPart != rTimeSpan.m_IHighPart || m_uILowPart != rTimeSpan.m_uILowPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is greater than given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator>(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_IHighPart == rTimeSpan.m_IHighPart) return m_uILowPart > rTimeSpan.m_uILowPart;

  return m_IHighPart > rTimeSpan.m_IHighPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is greater than or equal to
//! the given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator>=(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_IHighPart == rTimeSpan.m_IHighPart) return m_uILowPart >= rTimeSpan.m_uILowPart;

  return m_IHighPart >= rTimeSpan.m_IHighPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is less than given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator<(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_IHighPart == rTimeSpan.m_IHighPart) return m_uILowPart < rTimeSpan.m_uILowPart;

  return m_IHighPart < rTimeSpan.m_IHighPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks if current NavTimeSpan object is less than or equal to
//! the given one.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::operator<=(const NavTimeSpan& rTimeSpan) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  if (m_IHighPart == rTimeSpan.m_IHighPart) return m_uILowPart <= rTimeSpan.m_uILowPart;

  return m_IHighPart <= rTimeSpan.m_IHighPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Assignment Operator which takes a const parameter.
//!
//! @param      rTimeSpan   reference to self class object
//!
//! @throw      None
//!
//! @returns        NavTimeSpan reference to current object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
const NavTimeSpan& NavTimeSpan::operator=(const NavTimeSpan& rTimeSpan) {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  NavAssert(rTimeSpan.m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(rTimeSpan.m_IHighPart >= TIME_SPAN_MIN ||
            (rTimeSpan.m_IHighPart == TIME_SPAN_MIN - 1 && rTimeSpan.m_uILowPart > 0));

  m_IHighPart = rTimeSpan.m_IHighPart;
  m_uILowPart = rTimeSpan.m_uILowPart;

  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Retrieves the total number of milliseconds.
//!
//! @param      None
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        int number of milliseconds
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
int NavTimeSpan::GetTotalMilliSeconds(void) const {
  if (m_IHighPart >= 0) {
    // Check for overflow
    if (m_IHighPart > (INT_MAX - static_cast<int>(m_uILowPart)) / TIME_SPAN_SCALE) {
      throw make_arithmetic_exception(OUT_OF_RANGE);
    }

  } else {
    // Check for underflow
    if (-static_cast<unsigned int>(m_IHighPart) >
        (-static_cast<unsigned int>(INT_MIN) + static_cast<int>(m_uILowPart)) / TIME_SPAN_SCALE)

    {
      throw make_arithmetic_exception(OUT_OF_RANGE);
    }
  }

  return m_IHighPart * TIME_SPAN_SCALE + m_uILowPart;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Retrieves the total number of seconds. The number returned
//! is truncated.
//!
//! @param      None
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        int number of seconds
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
int NavTimeSpan::GetTotalSeconds(void) const {
  int wkIresult;
  int wkIquot;
  int wkIrem;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  Divide_by_1000(m_IHighPart, wkIresult, wkIrem);

  Divide_by_1000(wkIrem * TIME_SPAN_SCALE + int(m_uILowPart), wkIquot, wkIrem);

  if (wkIresult == 0) return wkIquot;

  if (wkIresult > 0) {
    // Check for overflow
    NavAssert(wkIquot >= 0);
    if (wkIresult > (INT_MAX - wkIquot) / TIME_SPAN_SCALE) {
      throw make_arithmetic_exception(OUT_OF_RANGE);
    }
  } else {
    if (wkIquot == 0 && wkIrem > 0) wkIquot = 1;

    // Check for underflow
    if (-static_cast<unsigned int>(wkIresult) >
        (-static_cast<unsigned int>(INT_MIN) - static_cast<unsigned int>(-wkIquot)) /
            TIME_SPAN_SCALE) {
      throw make_arithmetic_exception(OUT_OF_RANGE);
    }
  }

  return wkIresult * TIME_SPAN_SCALE + wkIquot;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Retrieves the total number of minutes. The number returned
//! is truncated.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        int number of minutes
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
int NavTimeSpan::GetTotalMinutes(void) const {
  int wkIresult;
  int wkIquot;
  int wkIrem;
  int wkIrem2;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  Divide_by_1000(int(m_IHighPart), wkIquot, wkIrem);

  Divide_by_60(wkIquot, wkIquot, wkIrem2);

  wkIrem += wkIrem2 * MILLISECONDS_PER_SECOND;

  wkIresult = wkIquot * TIME_SPAN_SCALE;

  if (wkIrem >= 0) {
    return wkIresult +
           ((unsigned int)(wkIrem)*TIME_SPAN_SCALE + m_uILowPart) / MILLISECONDS_PER_MINUTE;
  }

  return wkIresult -
         int(((unsigned int)(-wkIrem) * TIME_SPAN_SCALE - m_uILowPart) / MILLISECONDS_PER_MINUTE);
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Retrieves the total number of hours. The number returned
//! is truncated.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        GNS_SINT    number of hours
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
int NavTimeSpan::GetTotalHours(void) const {
  int wkIquot;
  int wkIrem;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  wkIquot = GetTotalMinutes();

  Divide_by_60(wkIquot, wkIquot, wkIrem);
  return wkIquot;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks whether the NavTimeSpan is zero.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::IsZero(void) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  return m_IHighPart == 0 && m_uILowPart == 0;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks whether the NavTimeSpan is not zero.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::IsNotZero(void) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  return m_IHighPart != 0 || m_uILowPart != 0;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks whether the NavTimeSpan is negative.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::IsNegative(void) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  return m_IHighPart < 0;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Checks whether the NavTimeSpan is positive.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        bool        true if the condition is satisfied
//!                 false otherwise
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
bool NavTimeSpan::IsPositive(void) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  return m_IHighPart > 0 || (m_IHighPart == 0 && m_uILowPart > 0);
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Calculates the absolute value.
//!
//! @param      None
//!
//! @throw      None
//!
//! @returns        NavTimeSpan object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan NavTimeSpan::Abs(void) const {
  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  if (m_IHighPart < 0) return -NavTimeSpan(*this);

  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Multiply the current NavTimeSpan object by a given number as left operand.
//! This is useful to calculate a multiple of a NavTimeSpan object
//! with an operator syntax.
//! For instance we can build NavTimeSpan object like:
//! number_of_days * OneDay
//! number_of_hours * OneHour
//!
//! @param      Multiplier  reference to the multiplier
//! @param      rTimeSpan   reference to NavTimeSpan object
//!
//! @throw      OUT_OF_RANGE    if an overflow or underflow is detected
//!
//! @returns        NavTimeSpan resulting object
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
NavTimeSpan operator*(const int& Multiplier, const NavTimeSpan& rTimeSpan) {
  return rTimeSpan * Multiplier;
}

//////////////////////////////////////////////////////////////////////////////
//! \n\b Description\n
//! Retrieves the time interval components: Day, Hour, Minute,
//! Second, MilliSecond and also whether the sign is negative or positive.
//!
//! @param      rSTime  reference to STimeComponents structure.
//!
//! @throw      None
//!
//! @returns        None
//!
//! \b Thread_Safety:   NA
//!
//////////////////////////////////////////////////////////////////////////////
void NavTimeSpan::GetTimeComponents(STimeComponents& rSTime) const {
  unsigned int wkuIMinute;
  unsigned int wkuIMilliSecond;
  unsigned int wkuIHighPart;
  unsigned int wkuILowPart;

  NavAssert(m_IHighPart <= TIME_SPAN_MAX);
  NavAssert(m_IHighPart >= TIME_SPAN_MIN || (m_IHighPart == TIME_SPAN_MIN - 1 && m_uILowPart > 0));

  rSTime.IsNegative = (m_IHighPart < 0);
  if (rSTime.IsNegative) {
    wkuIHighPart = (unsigned int)(-m_IHighPart);

    if (m_uILowPart > 0) {
      --wkuIHighPart;
      wkuILowPart = TIME_SPAN_SCALE - m_uILowPart;
    } else
      wkuILowPart = 0;
  } else {
    wkuIHighPart = (unsigned int)(m_IHighPart);
    wkuILowPart = m_uILowPart;
  }

  wkuIMinute = (wkuIHighPart / MILLISECONDS_PER_MINUTE) * TIME_SPAN_SCALE;

  wkuIMilliSecond = ((wkuIHighPart % MILLISECONDS_PER_MINUTE) * TIME_SPAN_SCALE) + wkuILowPart;

  wkuIMinute += wkuIMilliSecond / MILLISECONDS_PER_MINUTE;

  rSTime.m_uISecond = (wkuIMilliSecond / MILLISECONDS_PER_SECOND) % SECONDS_PER_MINUTE;
  rSTime.m_uMilliSecond = wkuIMilliSecond % MILLISECONDS_PER_SECOND;
  rSTime.m_uIDay = wkuIMinute / MINUTES_PER_DAY;
  rSTime.m_uIHour = (wkuIMinute %= MINUTES_PER_DAY) / MINUTES_PER_HOUR;
  rSTime.m_uIMinute = wkuIMinute % MINUTES_PER_HOUR;
}

unsigned int NavTimeSpan::Serialize(unsigned char* io_buffer) const {
  for (size_t i = 0; i < 4; ++i) {
    *io_buffer = static_cast<unsigned char>((m_IHighPart >> i * 8) & 0xff);
    ++io_buffer;
  }

  *io_buffer = static_cast<unsigned char>(m_uILowPart & 0xff);
  ++io_buffer;
  *io_buffer = static_cast<unsigned char>((m_uILowPart >> 8) & 0xff);

  return 6;
}

unsigned int NavTimeSpan::GetSerializedSize() const { return 6; }

unsigned int NavTimeSpan::Deserialize(const unsigned char* i_buffer) {
  m_IHighPart = 0;
  for (size_t i = 0; i < 4; ++i) {
    m_IHighPart |= ((unsigned int)(*i_buffer)) << (8 * i);
    ++i_buffer;
  }

  m_uILowPart = (unsigned int)(*i_buffer);
  ++i_buffer;
  m_uILowPart |= (((unsigned int)(*i_buffer)) << 8);

  return 6;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Multiply two unsigned int values, yielding a 64 bits result as a pair of
//! unsigned int
//!
//! @param      FirstOperand:   32 bits value to be multiplied
//! @param      SecondOperand:  32 bits value to be multiplied
//! @param      ResultHighPart: higher 32 bits of result
//! @param      ResultLowPart:  lower 32 bits of result
//!
//! @throw      None
//!
//! @returns        None
//!
//! \b Thread_Safety:   Yes
//!
//! Note:   to increase performances, it is advisable to enable the use of
//!     the unsigned long long int type, if possible or to code the
//!     function in assembler, for CPUs that have a 32 * 32 -> 64
//!     multiplication instruction.
//!
///////////////////////////////////////////////////////////////////////////////
inline void multiply_32_for_32(unsigned int FirstOperand, unsigned int SecondOperand,
                               volatile unsigned int& ResultHighPart,
                               volatile unsigned int& ResultLowPart) {
  //! This version is generally faster, but it relies upon the
  //! unsigned long long type, that is an extension to C++ (fairly common,
  //! but not yet ISO standard)

  unsigned long long int value;

  value = static_cast<unsigned long long int>(FirstOperand) *
          static_cast<unsigned long long int>(SecondOperand);
  ResultHighPart =
      static_cast<unsigned int>(value / (static_cast<unsigned long long int>(UINT_MAX) + 1));
  ResultLowPart =
      static_cast<unsigned int>(value % (static_cast<unsigned long long int>(UINT_MAX) + 1));
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Divide by 60 in an efficient way; both quotient and remainder are
//! computed; the remainder has the same sign of the value
//!
//! @param      value:      the int value
//! @param      quotient:   reference to the quotient
//! @param      remainder:  reference to the remainder
//!
//! @throw      None
//!
//! @returns        the number of bits
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
inline void Divide_by_60(int value, volatile int& quotient, volatile int& remainder) {
  if (value < 0)
    // Modular properties of unsigned arithmetic
    // ensure that the result is correct for all
    // possible values of operand
    quotient = -static_cast<int>((-static_cast<int>(value)) / 60);
  else
    quotient = static_cast<int>(static_cast<unsigned int>(value) / 60);

  remainder = value - quotient * 60;
}

///////////////////////////////////////////////////////////////////////////////
//! \n\b Description \n
//! Divide by 1000 in an efficient way; both quotient and remainder are
//! computed; the remainder has the same sign of the value
//!
//! @param      value:      the int value
//! @param      quotient:   reference to the quotient
//! @param      remainder:  reference to the remainder
//!
//! @throw      None
//!
//! @returns        the number of bits
//!
//! \b Thread_Safety:   Yes
//!
///////////////////////////////////////////////////////////////////////////////
inline void Divide_by_1000(int value, volatile int& quotient, volatile int& remainder) {
  if (value < 0)
    // Modular properties of unsigned arithmetic
    // ensure that the result is correct for all
    // possible values of operand
    quotient = -static_cast<int>((-static_cast<unsigned int>(value)) / 1000);
  else
    quotient = static_cast<int>(static_cast<unsigned int>(value) / 1000);
  remainder = value - quotient * 1000;
}

}  // namespace Navionics
