/*
 * ak_uidManager.h
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <mutex>					// mutex

// AK header
#include <ak_globalDataTypes.h>		// UID and ID type

namespace ak {

	//! This class is used to manage UIDs.
	//! This UID manager is incrementing the UID on each getId call.
	class UICORE_API_EXPORT uidManager {
	public:

		//! @brief Default construcor. Initialize the first id with 1
		uidManager();

		//! @brief Deconstructor
		virtual ~uidManager();

		// ############################################################################

		//! @brief Will return the current id and increase it for the next call
		virtual UID getId(void);

		//! @brief Will reset the UID manager (start from the beginning)
		void reset(void);

		//! @brief Will set the current UID to the UID provided
		//! @param _uid The new current _uid
		void setLatestUid(UID _uid);

	private:
		UID				my_currentId;						//! Contains the current id
		std::mutex		my_mutex;							//! Used for thread protection

		// Block copy constructor
		ak::uidManager::uidManager(const uidManager & other) = delete;

		// Block assignment operator
		ak::uidManager & operator = (const uidManager & other) = delete;

	};
}