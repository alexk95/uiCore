/*
 *	File:		aDate.h
 *	Package:	akCore
 *
 *  Created on: July 07, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <akCore/akCore.h>

#include <string>
#include <qstring.h>
#include <qdatetime.h>

namespace ak {

	class aDate {
	public:
		aDate();
		aDate(int _d, int _m, int _y);
		aDate(const aDate & _other);
		virtual ~aDate();

		// ############################################################################################################

		// Getter

		//! @brief Will return the year
		int year(void) const { return m_y; }

		//! @brief Will return the month
		int month(void) const { return m_m; }

		//! @brief Will return the day
		int day(void) const { return m_d; }

		//! @brief Will return a String representation of this date
		//! @param _delimiter The delimiter that is used to seperate the day, month and year
		std::string toString(const std::string _delimiter = "-", dateFormat _dateFormat = dfDDMMYYYY) const;

		//! @brief Will return a String representation of this date
		//! @param _delimiter The delimiter that is used to seperate the day, month and year
		std::wstring toWString(const std::wstring _delimiter = L"-", dateFormat _dateFormat = dfDDMMYYYY) const;

		//! @brief Will return a String representation of this date
		//! @param _delimiter The delimiter that is used to seperate the day, month and year
		QString toQString(const QString _delimiter = "-", dateFormat _dateFormat = dfDDMMYYYY) const;

		//! @brief Will return a QDate object representing this object
		QDate toQDate(void) const;

		// ############################################################################################################

		// Setter

		//! @brief Will set the year
		//! @param _y The year to set
		void setYear(int _y) { m_y = _y; }

		//! @brief Will set the month
		//! @param _m The month to set
		void setMonth(int _m) { m_m = _m; }

		//! @brief Will set the day
		//! @param _d The day to set
		void setDay(int _d) { m_d = _d; }

		//! @brief Will set the current date
		//! @param _y The year to set
		//! @param _m The month to set
		//! @param _d The day to set
		void set(int _d, int _m, int _y);

		// ############################################################################################################

		// Static parser

		//! @brief Will parse the provided string and create a new date object
		//! @param _string The input string
		//! @param _delimiter The delimiter that seperates the day, month and year
		static aDate parseString(const char * _string, const char * _delimiter = "-", dateFormat _dateFormat = dfDDMMYYYY);

		//! @brief Will parse the provided string and create a new date object
		//! @param _string The input string
		//! @param _delimiter The delimiter that seperates the day, month and year
		static aDate parseString(const wchar_t * _string, const wchar_t * _delimiter = L"-", dateFormat _dateFormat = dfDDMMYYYY);

		//! @brief Will parse the provided string and create a new date object
		//! @param _string The input string
		//! @param _delimiter The delimiter that seperates the day, month and year
		static aDate parseString(const std::string & _string, const std::string & _delimiter = "-", dateFormat _dateFormat = dfDDMMYYYY);

		//! @brief Will parse the provided string and create a new date object
		//! @param _string The input string
		//! @param _delimiter The delimiter that seperates the day, month and year
		static aDate parseString(const std::wstring & _string, const std::wstring & _delimiter = L"-", dateFormat _dateFormat = dfDDMMYYYY);

		//! @brief Will parse the provided string and create a new date object
		//! @param _string The input string
		//! @param _delimiter The delimiter that seperates the day, month and year
		static aDate parseString(const QString & _string, const QString & _delimiter = "-", dateFormat _dateFormat = dfDDMMYYYY);

		// ############################################################################################################

		// Operators

		aDate & operator = (const aDate & _other);
		bool operator == (const aDate & _other);
		bool operator != (const aDate & _other);
		bool operator < (const aDate & _other);
		bool operator > (const aDate & _other);
		bool operator <= (const aDate & _other);
		bool operator >= (const aDate & _other);

	private:
		int		m_y;
		int		m_m;
		int		m_d;
	};

}