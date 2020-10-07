/*
 * ak_exception.cpp
 *
 *  Created on: January 29, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_exception.h>		// Corresponding class

ak::Exception::Exception()
{
	my_where = "exception_where";
	my_what = "exception_what";
	my_out = "ex : exception";
	my_type = Undefined;
}

ak::Exception::Exception(
	const char *	 				_exWhat,
	const char *				 	_exWhere,
	ak::Exception::exceptionType				_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }


ak::Exception::Exception(
	const std::string &				_exWhat,
	const char *					_exWhere,
	ak::Exception::exceptionType				_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::Exception::Exception(
	const std::string &				_exWhat,
	const std::string &				_exWhere,
	ak::Exception::exceptionType				_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::Exception::Exception(
	const char *				_exWhat,
	const std::string &			_exWhere,
	ak::Exception::exceptionType			_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::Exception::Exception(
	const ak::Exception &			_other
) : my_what(_other.getWhat()),
	my_where(_other.getWhere()),
	my_type(_other.getType())
{
	buildOut();
	my_type = _other.getType();
}

ak::Exception::Exception(
	const ak::Exception & 			_other,
	const char * 					_exWhere
) : my_what(_other.getWhat()),
	my_where(_other.getWhere()),
	my_type(_other.getType())
{
	my_where += "\n@ ";
	my_where += _exWhere;
	buildOut();
	my_type = _other.getType();
}

ak::Exception::~Exception() throw() {}

const char * ak::Exception::what(void) const throw() { return my_out.c_str(); }

std::string ak::Exception::getWhat(void) const { return my_what; }

std::string ak::Exception::getWhere(void) const {return my_where; }

ak::Exception::exceptionType ak::Exception::getType(void) const { return my_type; }

void ak::Exception::buildOut(void) {
	my_out = my_what;
	my_out += "\n@ ";
	my_out += my_where;
}
