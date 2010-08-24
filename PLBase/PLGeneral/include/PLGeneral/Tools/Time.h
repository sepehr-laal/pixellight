/*********************************************************\
 *  File: Time.h                                         *
 *
 *  Copyright (C) 2002-2010 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


#ifndef __PLGENERAL_TIME_H__
#define __PLGENERAL_TIME_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGeneral/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGeneral {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Time and date class
*
*  @note
*    - If the year is between [1970..January 19, 2038] it's Unix/POSIX (32 bit) compatible
*    - If the year is between [1980..January 1, 2108] it's DOS compatible
*/
class Time {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Month of year enumeration
		*/
		enum EMonth {
			January = 1,	/**< January */
			February,		/**< February */
			March,			/**< March */
			April,			/**< April */
			May,			/**< May */
			June,			/**< June */
			July,			/**< July */
			August,			/**< August */
			September,		/**< September */
			October,		/**< October */
			November,		/**< November */
			December		/**< December */
		};

		/**
		*  @brief
		*    Day of week enumeration
		*/
		enum EDay {
			Sunday = 0,	/**< Sunday */
			Monday,		/**< Monday */
			Tuesday,	/**< Tuesday */
			Wednesday,	/**< Wednesday */
			Thursday,	/**< Thursday */
			Friday,		/**< Friday */
			Saturday	/**< Saturday */
		};


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		PLGENERAL_API static const Time   Null;					/**< Year=0 Month=January Day=1 DayOfWeek=Sunday Hour=0 Minute=0 Second=0 Millisecond=0 */
		PLGENERAL_API static const Time   UnixEpochStart;		/**< Year=1970 Month=January Day=1 DayOfWeek=Thursday Hour=0 Minute=0 Second=0 Millisecond=0 */
		PLGENERAL_API static const Time   DOSEpochStart;		/**< Year=1980 Month=January Day=1 DayOfWeek=Tuesday Hour=0 Minute=0 Second=0 Millisecond=0 */
		PLGENERAL_API static const String ShortMonthName[12];	/**< Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec */
		PLGENERAL_API static const String ShortDayName[7];		/**< Sun, Mon, Tue, Wed, Thu, Fri, Sat */


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether or not the given year is a leap year
		*
		*  @param[in] nYear
		*    Year to check
		*
		*  @return
		*    'true' if the the given year is a leap year, else 'false'
		*/
		PLGENERAL_API static bool IsLeapYear(uint32 nYear);

		/**
		*  @brief
		*    Returns the number of days in a month
		*
		*  @param[in] nMonth
		*    Month to check
		*  @param[in] nYear
		*    Year to check (0 == ignore, and assume no leap year)
		*
		*  @return
		*    Number of days in the given month
		*/
		PLGENERAL_API static PLGeneral::uint8 GetDaysPerMonth(EMonth nMonth, uint32 nYear = 0);

		/**
		*  @brief
		*    Calculates the day of a week
		*
		*  @param[in] nDayOfMonth
		*    Day of month
		*  @param[in] nMonth
		*    Month
		*  @param[in] nYear
		*    Year
		*
		*  @return
		*    The day of a week
		*/
		PLGENERAL_API static EDay CalculateDayOfWeek(uint8 nDayOfMonth, EMonth nMonth, uint16 nYear);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGENERAL_API Time();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] nYear
		*    Year
		*  @param[in] nMonth
		*    Month [January..December]
		*  @param[in] nDayOfMonth
		*    Day of the month [1..31]
		*  @param[in] nDayOfWeek
		*    Day of the week [Sunday..Saturday]
		*  @param[in] nHour
		*    Hour [0..23]
		*  @param[in] nMinute
		*    Minute [0..59]
		*  @param[in] nSecond
		*    Second [0..59]
		*  @param[in] nMillisecond
		*    Millisecond [0..999]
		*/
		PLGENERAL_API Time(uint16 nYear, EMonth nMonth, uint8 nDayOfMonth, EDay nDayOfWeek, uint8 nHour, uint8 nMinute, uint8 nSecond, uint16 nMillisecond);

		/**
		*  @brief
		*    Destructor
		*/
		PLGENERAL_API ~Time();

		/**
		*  @brief
		*    Get the year
		*
		*  @return
		*    Year
		*/
		PLGENERAL_API uint16 GetYear() const;

		/**
		*  @brief
		*    Set the year
		*
		*  @param[in] nYear
		*    Year
		*/
		PLGENERAL_API void SetYear(uint16 nYear);

		/**
		*  @brief
		*    Get the month
		*
		*  @return
		*    Month [January..December]
		*/
		PLGENERAL_API EMonth GetMonth() const;

		/**
		*  @brief
		*    Set the month
		*
		*  @param[in] nMonth
		*    Month [January..December]
		*/
		PLGENERAL_API void SetMonth(EMonth nMonth);

		/**
		*  @brief
		*    Get the day of the month
		*
		*  @return
		*    Day of the month [1..31]
		*/
		PLGENERAL_API uint8 GetDayOfMonth() const;

		/**
		*  @brief
		*    Set the day of the month
		*
		*  @param[in] nDayOfMonth
		*    Day of the month [1..31]
		*/
		PLGENERAL_API void SetDayOfMonth(uint8 nDayOfMonth);

		/**
		*  @brief
		*    Get the day of the week
		*
		*  @return
		*    Day of the week [Sunday..Saturday]
		*/
		PLGENERAL_API EDay GetDayOfWeek() const;

		/**
		*  @brief
		*    Set the day of the week
		*
		*  @param[in] nDayOfWeek
		*    Day of the week [Sunday..Saturday]
		*/
		PLGENERAL_API void SetDayOfWeek(EDay nDayOfWeek);

		/**
		*  @brief
		*    Get the hour
		*
		*  @return
		*    Hour [0..23]
		*/
		PLGENERAL_API uint8 GetHour() const;

		/**
		*  @brief
		*    Set the hour
		*
		*  @param[in] nHour
		*    Hour [0..23]
		*/
		PLGENERAL_API void SetHour(uint8 nHour);

		/**
		*  @brief
		*    Get the minute
		*
		*  @return
		*    Minute [0..59]
		*/
		PLGENERAL_API uint8 GetMinute() const;

		/**
		*  @brief
		*    Set the minute
		*
		*  @param[in] nMinute
		*    Minute [0..59]
		*/
		PLGENERAL_API void SetMinute(uint8 nMinute);

		/**
		*  @brief
		*    Get the second
		*
		*  @return
		*    Second [0..59]
		*/
		PLGENERAL_API uint8 GetSecond() const;

		/**
		*  @brief
		*    Set the second
		*
		*  @param[in] nSecond
		*    Second [0..59]
		*/
		PLGENERAL_API void SetSecond(uint8 nSecond);

		/**
		*  @brief
		*    Get the millisecond
		*
		*  @return
		*    Millisecond [0..999]
		*/
		PLGENERAL_API uint16 GetMillisecond() const;

		/**
		*  @brief
		*    Set the millisecond
		*
		*  @param[in] nMillisecond
		*    Millisecond [0..999]
		*/
		PLGENERAL_API void SetMillisecond(uint16 nMillisecond);

		/**
		*  @brief
		*    Returns a string representation of the time
		*
		*  @return
		*    String representation (for example: Sun Sep 16 01:03:52 1973)
		*/
		PLGENERAL_API String ToString() const;

		/**
		*  @brief
		*    Converts a date from Unix/POSIX format
		*
		*  @param[in] nUnixDate
		*    Date in Unix format
		*/
		PLGENERAL_API void SetUnixDate(uint32 nUnixDate);

		/**
		*  @brief
		*    Converts a date from DOS format
		*
		*  @param[in] nDOSDate
		*    Date in DOS format
		*/
		PLGENERAL_API void SetDOSDate(uint32 nDOSDate);

		//[-------------------------------------------------------]
		//[ Comparison                                            ]
		//[-------------------------------------------------------]
		PLGENERAL_API bool operator ==(const Time &cTime) const;
		PLGENERAL_API bool operator !=(const Time &cTime) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint16 m_nYear;			/**< Years */
		EMonth m_nMonth;		/**< Month of the year - [January..December] */
		uint8  m_nDayOfMonth;	/**< Day of the month - [1..31] */
		EDay   m_nDayOfWeek;	/**< Day of the week - [Sunday..Saturday] */
		uint8  m_nHour;			/**< Hours since midnight - [0..23] */
		uint8  m_nMinute;		/**< Minutes after the hour - [0..59] */
		uint8  m_nSecond;		/**< Seconds after the minute - [0..59] */
		uint16 m_nMillisecond;	/**< Milliseconds after the second - [0..999] */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGeneral


#endif // __PLGENERAL_TIME_H__
