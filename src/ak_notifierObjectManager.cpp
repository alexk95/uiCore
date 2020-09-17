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

//! Deconstructor
ak::notifierObjectManager::~notifierObjectManager(void) {}

// *****************************************************************************************
// Message IO

//! @brief Will call the callback function with the provided parameters
//! @param _senderId The sender ID the message was send from
//! @param _message The message
//! @param _messageType The type of the messsage
//! @param _info1 Message addition 1
//! @param _info2 Message addition 2
//! @throw ak::Exception to forward exceptions coming from the application core class
void ak::notifierObjectManager::notify(
	ak::UID									_senderId,
	ak::core::messageType					_messageType,
	int										_message,
	int										_info1,
	int										_info2
) {
	try {
		if (_messageType == ak::core::messageType::mEvent) {
			if ((ak::core::eventType) _message == ak::core::eventType::eDestroyed) {
				assert(isEnabled()); // Message received even if notifier was disabled
				my_manager->creatorDestroyed(_senderId);
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierObjectManager::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierObjectManager::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierObjectManager::notify()"); }
}
