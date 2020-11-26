/*
 * ak_mutex.cpp
 *
 *  Created on: November 25, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_mutex.h>

ak::mutex::mutex() : my_isLocked(false) {}

ak::mutex::~mutex() {}

void ak::mutex::Lock(void) { lock(); my_isLocked = true; }

void ak::mutex::Unlock(void) { unlock(); my_isLocked = false; }

bool ak::mutex::TryLock(void) {
	if (try_lock()) { my_isLocked = true; return true; }
	else { return false; }
}