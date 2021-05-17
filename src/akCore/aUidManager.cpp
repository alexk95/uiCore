/*
 *	File:		aUidManager.cpp
 *	Package:	akCore
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aUidMangager.h>
#include <akCore/aException.h>

ak::aUidManager::aUidManager() : my_currentId(1) {}

ak::aUidManager::~aUidManager() {}

// ###########################################################

ak::UID ak::aUidManager::getId(void) {
	my_mutex.lock();
	ak::UID id = my_currentId++;
	if (id == 0xffffffffffffffff) {
		assert(0);
		my_mutex.unlock();
		throw aException("Max UID reached!", "aUidManager::getId()", aException::ArrayOutOfBounds);
	}
	my_mutex.unlock();
	return id;
}

void ak::aUidManager::reset(void) {
	my_mutex.lock();
	my_currentId = 1;
	my_mutex.unlock();
}

void ak::aUidManager::setLatestUid(ak::UID _uid) {
	my_mutex.lock();
	my_currentId = _uid;
	my_mutex.unlock();
}