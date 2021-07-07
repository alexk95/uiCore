/*
 *	File:		aMSSQLWrapper.cpp
 *	Package:	akDB
 *
 *  Created on: July 07, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akDB/aMSSQLWrapper.h>

#include <cassert>
#include <sstream>
#include <codecvt>

ak::aMSSQLWrapper::aMSSQLWrapper()
	: m_environmentHandle(nullptr), m_connectionHandle(nullptr), m_lastQueryHandle(nullptr),
	m_isConnected(false)
{}

ak::aMSSQLWrapper::aMSSQLWrapper(
	const std::string &		_ipAddress,
	const std::string &		_port,
	const std::string &		_userName,
	const std::string &		_password
) : m_environmentHandle(nullptr), m_connectionHandle(nullptr), m_lastQueryHandle(nullptr),
	m_isConnected(false)
{
	if (!connect(_ipAddress, _port, _userName, _password)) {
		assert(0);
		throw std::exception("Failed to initialize connection to Database");
	}
}

ak::aMSSQLWrapper::~aMSSQLWrapper() {
	disconnect();
}

bool ak::aMSSQLWrapper::connect(
	const std::string &		_ipAddress,
	const std::string &		_port,
	const std::string &		_userName,
	const std::string &		_password
) {
	return connect(convertString(_ipAddress), convertString(_port),
		convertString(_userName), convertString(_password));
}

bool ak::aMSSQLWrapper::connect(
	const std::wstring &	_ipAddress,
	const std::wstring &	_port,
	const std::wstring &	_userName,
	const std::wstring &	_password
) {
	std::wstring logInInfo{ L"DRIVER={SQL Server};SERVER=" };
	logInInfo.append(_ipAddress);
	logInInfo.append(L", ");
	logInInfo.append(_port);
	logInInfo.append(L"; UID=");
	logInInfo.append(_userName);

	logInInfo.append(L"; PWD=");
	logInInfo.append(_password);
	logInInfo.append(L";");

	return connect(logInInfo);
}

bool ak::aMSSQLWrapper::connect(
	const std::string &	_connectionString
) {
	return connect(convertString(_connectionString));
}

bool ak::aMSSQLWrapper::connect(
	const std::wstring &	_connectionString
) {
	if (m_isConnected) {
		assert(0);
		return true;
	}

	//define handles and variables
	SQLHANDLE sqlStmtHandle;
	SQLWCHAR retconstring[1024];

	//initializations
	sqlStmtHandle = NULL;
	//allocations
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_environmentHandle)) { return false; }
	if (SQL_SUCCESS != SQLSetEnvAttr(m_environmentHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) { return false; }
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, m_environmentHandle, &m_connectionHandle)) { return false; }

	// Try to connect
	switch (SQLDriverConnect(m_connectionHandle, NULL, (SQLWCHAR*)_connectionString.c_str(),
		SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT))
	{
	case SQL_SUCCESS:
		break;
	case SQL_SUCCESS_WITH_INFO:
		break;
	case SQL_INVALID_HANDLE:
		throw std::exception("Invalid handle");
	case SQL_ERROR:
		throw std::exception("SQL error");
	default:
		assert(0); // That should not happen
		throw std::exception("Unknown error");
		break;
	}
	return true;
}

void ak::aMSSQLWrapper::disconnect(void) {
	deallocateHandles();
}

SQLHANDLE ak::aMSSQLWrapper::executeQuery(const std::wstring & _query) {
	freeLastQueryHandle();

	// Allocates the statement
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, m_connectionHandle, &m_lastQueryHandle)) {
		throw std::exception("An error occured while allocating the statement handle");
	}

	// Executes a preparable statement
	auto result = SQLExecDirectW(m_lastQueryHandle, (SQLWCHAR*)_query.c_str(), SQL_NTS);
	if (SQL_SUCCESS != result) {
		//Clear memory
		SQLFreeHandle(SQL_HANDLE_STMT, m_lastQueryHandle);
		throw std::exception("Failed to execute query");
	}
	else {
		return m_lastQueryHandle;
	}
}

// ####################################################################################################################################

// Helper functions

std::wstring ak::aMSSQLWrapper::getSQLConnectionError(void) {
	return getSQLError(SQL_HANDLE_DBC, m_connectionHandle);
}

std::wstring ak::aMSSQLWrapper::getSQLEnvironmentError(void) {
	return getSQLError(SQL_HANDLE_ENV, m_environmentHandle);
}

std::wstring ak::aMSSQLWrapper::getSQLError(unsigned int _handleType, const SQLHANDLE & _handle)
{
	SQLWCHAR SQLState[1024];
	SQLWCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(_handleType, _handle, 1, SQLState, NULL, message, 1024, NULL)) {
		// Returns the current values of multiple fields of a diagnostic record that contains error, warning, and status information
		std::wstringstream ss;
		ss << L"SQL state: " << SQLState << L", SQL driver message: " << message << L"\n";
		return ss.str();
	}
	else {
		return L"Unknown SQL error";
	}
}

std::wstring ak::aMSSQLWrapper::convertString(const std::string & _str) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	return myconv.from_bytes(_str);
}

void ak::aMSSQLWrapper::freeLastQueryHandle(void) {
	freeHandle(m_lastQueryHandle, SQL_HANDLE_STMT);
}

void ak::aMSSQLWrapper::freeHandle(SQLHANDLE & _handle, unsigned int _handleType) {
	if (_handle != nullptr) {
		SQLFreeHandle(_handleType, _handle);
		_handle = nullptr;
	}
}

// ####################################################################################################################################

// Private functions

void ak::aMSSQLWrapper::deallocateHandles(void) {
	freeLastQueryHandle();

	if (m_connectionHandle != NULL) {
		// Free the resources and disconnect
		SQLDisconnect(m_connectionHandle);
		freeHandle(m_connectionHandle, SQL_HANDLE_DBC);
		freeHandle(m_environmentHandle, SQL_HANDLE_ENV);
	}
	m_isConnected = false;
}