/*
 * ak_uidManager.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_uidMangager.h>			// corresponding class

ak::uidManager::uidManager() : my_currentId(1) {}

ak::uidManager::~uidManager() {}

// ###########################################################

ak::UID ak::uidManager::getId(void) {
	my_mutex.lock();
	ak::UID id = my_currentId++;
	my_mutex.unlock();
	return id;
}

void ak::uidManager::reset(void) {
	my_mutex.lock();
	my_currentId = 1;
	my_mutex.unlock();
}

void ak::uidManager::setLatestUid(ak::UID _uid) {
	my_mutex.lock();
	my_currentId = _uid;
	my_mutex.unlock();
}