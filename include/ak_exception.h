/*
 * ak_exception.h
 *
 *  Created on: January 29, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <exception>			// Base class
#include <string>				// std::string

// AK header
#include <ak_globalDataTypes.h>	// UICORE_API

namespace ak {

	//! The ak::Exception class can be used as an universal exception which may used to provide nicely readable exception messages.
	//! The provided exception message 'ex_what' is representing the exception message
	//! The provided where message 'ex_where' is representing the location where this exception is thrown from.
	//!		If the exception will be catched by a try block, this exception can be thrown to the next higher level by providing
	//!		just the new location. At the beginning of each provided location a '@ ' will be added
	//!		A ak::Exception.what() message can look like this:
	//!			Exception Message
	//!			@ Inner function
	//!			@ Outter function
	//!			@ Initial calling function
	class UICORE_API Exception : public std::exception {
	public:

		//! Used to determine the type of the exception
		enum exceptionType {
			Undefined = 0,
			Arithmetic,
			ArrayOutOfBounds,
			FileNotFound,
			IO,
			Nullptr,
			NumberFormat,
			InvalidInput,
			UnknownCommand,
			BadAlloc
		};

		//! @brief Default constructor. Will initialize the type with undefined
		Exception();

		//! @brief Assignment constructor. Will set the internal variables
		//! @param _exWhat The exception message
		//! @param _exWhere The location where the exception was thrown from (e.g. function name)
		//! @param _exType The type of the exception
		Exception(
			const char *				_exWhat,
			const char *				_exWhere,
			exceptionType				_exType = exceptionType::Undefined
		);

		//! @brief Assignment constructor. Will set the internal variables
		//! @param _exWhat The exception message
		//! @param _exWhere The location where the exception was thrown from (e.g. function name)
		//! @param _exType The type of the exception
		Exception(
			const std::string &			_exWhat,
			const char *				_exWhere,
			exceptionType				_exType = exceptionType::Undefined
		);

		//! @brief Assignment constructor. Will set the internal variables
		//! @param _exWhat The exception message
		//! @param _exWhere The location where the exception was thrown from (e.g. function name)
		//! @param _exType The type of the exception
		Exception(
			const std::string &			_exWhat,
			const std::string &			_exWhere,
			exceptionType				_exType = exceptionType::Undefined
		);

		//! @brief Assignment constructor. Will set the internal variables
		//! @param _exWhat The exception message
		//! @param _exWhere The location where the exception was thrown from (e.g. function name)
		//! @param _exType The type of the exception
		Exception(
			const char *				_exWhat,
			const std::string &			_exWhere,
			exceptionType				_exType = exceptionType::Undefined
		);

		//! @brief Copy constructor. Will copy all information from the other exception.
		//! @param _other The other exception
		Exception(
			const Exception &			_other
		);

		//! @brief Forward constructor. Will copy all information from the other exception and append the new location
		//! Will copy all the information contained in the other exception. Afterwards the new where location will be appended.
		//! @brief _other The other exception the information will be copied from
		//! @brief _exWhere The new location message which will be appended
		Exception(
			const Exception &			_other,
			const char *				_exWhere
		);

		//! @brief Deconstructor
		virtual ~Exception() throw();

		//!	@brief Will return the exception and location text
		virtual const char * what() const throw() override;

		//!	@brief Returns a copy of the what message which represents the exception text
		std::string getWhat(void) const;

		//! @brief Return a copy of the what message which represents the location
		std::string getWhere(void) const;

		//! @brief Returns a copy of the exception type		
		exceptionType getType(void) const;

	protected:
		std::string						my_what;								//! Contains the exception message
		std::string						my_where;								//! Contains the location desription
		exceptionType					my_type;								//! Contains the exception type which is initialized as undefined

		void buildOut(void);													//! Used internally to build the output string

	private:
		std::string						my_out;									//! Contains the output message which will be returned in the what() function
	};

} // namespace ak
