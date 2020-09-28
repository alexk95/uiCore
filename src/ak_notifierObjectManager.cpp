/*
 * ak_notifierObjectManager.cpp
 *
 *  Created on: July 16, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_notifierObjectManager.h"	// corresponding header
#include "ak_exception.h"				// error handling
#include "ak_ui_objectManager.h"		// objectManager

ak::notifierObjectManager::notifierObjectManager(
	ak::ui::objectManager *					_manager
) {
	try {
		if (_manager == nullptr) { throw ak::Exception("Is nullptr", "Check manager", ak::Exception::exceptionType::Nullptr); }
		my_manager = _manager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierObjectManager::notifierObjectManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierObjectManager::notifierObjectManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierObjectManager::notifierObjectManager()"); }
}

ak::notifierObjectManager::~notifierObjectManager(void) {}

// *****************************************************************************************

// Message IO

void ak::notifierObjectManager::notify(
	ak::UID									_senderId,
	ak::core::eventType						_event,
	int										_info1,
	int										_info2
) {
	try {
		if (_event == ak::core::eventType::eDestroyed) {
			assert(isEnabled()); // Message received even if notifier was disabled
			my_manager->creatorDestroyed(_senderId);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierObjectManager::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierObjectManager::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierObjectManager::notify()"); }
}
