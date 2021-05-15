/*
 * ak_exception.cpp
 *
 *  Created on: January 29, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>

ak::aException::aException()
{
	my_where = "exception_where";
	my_what = "exception_what";
	my_out = "ex : exception";
	my_type = Undefined;
}

ak::aException::aException(
	const char *	 				_exWhat,
	const char *				 	_exWhere,
	ak::aException::exceptionType				_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }


ak::aException::aException(
	const std::string &				_exWhat,
	const char *					_exWhere,
	exceptionType					_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::aException::aException(
	const std::string &				_exWhat,
	const std::string &				_exWhere,
	exceptionType					_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::aException::aException(
	const char *					_exWhat,
	const std::string &				_exWhere,
	exceptionType					_exType
) : my_what(_exWhat), my_where(_exWhere), my_type(_exType) { buildOut(); }

ak::aException::aException(
	const ak::aException &			_other
) : my_what(_other.getWhat()),
	my_where(_other.getWhere()),
	my_type(_other.getType())
{
	buildOut();
	my_type = _other.getType();
}

ak::aException::aException(
	const ak::aException & 			_other,
	const char * 					_exWhere
) : my_what(_other.getWhat()),
	my_where(_other.getWhere()),
	my_type(_other.getType())
{
	my_where += AK_CSTR_NL "@ ";
	my_where += _exWhere;
	buildOut();
	my_type = _other.getType();
}

ak::aException::~aException() throw() {}

const char * ak::aException::what(void) const throw() { return my_out.c_str(); }

std::string ak::aException::getWhat(void) const { return my_what; }

std::string ak::aException::getWhere(void) const {return my_where; }

ak::aException::exceptionType ak::aException::getType(void) const { return my_type; }

void ak::aException::buildOut(void) {
	my_out = my_what;
	my_out += AK_CSTR_NL "@ ";
	my_out += my_where;
}
