/*
 * ak_globalDataTypes.h
 *
 *  Created on: September 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

#if  defined(_WIN32) || defined (_WIN64)
#define OS_WINDOWS 1
#define UICORE_API __declspec(dllexport)
#elif defined(__unix__) || defined (unix) || defined(__unix)
#define OS_UNIX 1
#endif

namespace ak {

	//! @brief The UID datatype used for objects
	typedef unsigned long long UID;

	//! @brief The ID datatype used for items
	typedef int	ID;

	//! @brief Represents the invalid UID for an object
	const UID invalidUID = 0;

	//! @brief Represents the invalid ID for an item
	const ID invalidID = -1;

}
