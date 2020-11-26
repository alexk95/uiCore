/*
 * ak_mutex.h
 *
 *  Created on: November 25, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

#include <mutex>		// Base class

namespace ak {

	class mutex : public std::mutex {
	public:
		mutex();
		virtual ~mutex();

		void Lock(void);

		void Unlock(void);

		bool TryLock(void);

		bool isLocked(void) const { return my_isLocked; }


	private:

		bool		my_isLocked;

		mutex(const mutex &) = delete;

	};

}