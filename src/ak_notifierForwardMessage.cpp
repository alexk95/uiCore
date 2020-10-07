/*
 * ak_notifierForwardMessage.cpp
 *
 *  Created on: August 02, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_notifierForwardMessage.h>	// corresponding header
#include <ak_exception.h>				// error handling
#include <ak_messenger.h>				// messenger

ak::notifierForwardMessage::notifierForwardMessage(
	ak::messenger *					_messenger,
	ak::UID							_uid,
	bool							_enableLoopProtection
) {
	try {
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger", ak::Exception::exceptionType::Nullptr); }
		if (_uid == ak::invalidUID) { throw ak::Exception("UID may not be 'invalidUID'", "Check UID", ak::Exception::exceptionType::Undefined); }
		my_loopProtectionEnabled = _enableLoopProtection;
		my_messenger = _messenger;
		my_uid = _uid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierForwardMessage::notifierForwardMessage()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierForwardMessage::notifierForwardMessage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierForwardMessage::notifierForwardMessage()"); }
}

ak::notifierForwardMessage::~notifierForwardMessage(void) {}

void ak::notifierForwardMessage::notify(
	ak::UID									_senderId,
	ak::core::eventType						_event,
	int										_info1,
	int										_info2
) {
	try {
		if (!isEnabled()) { return; }
		// Check for possible loop (if enabled)
		if (_senderId == my_uid && my_loopProtectionEnabled) { throw ak::Exception("Sender id is equal to the forwarding id", "Infinite loop protection"); }
		// Forward the message with a new UID
		my_messenger->sendMessage(my_uid, _event, _info1, _info2);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierForwardMessage::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierForwardMessage::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierForwardMessage::notify()"); }
}

void ak::notifierForwardMessage::setLoopProtectionEnabled(
	bool									_enable
) { my_loopProtectionEnabled = _enable; }

bool ak::notifierForwardMessage::loopProtectionEnabled(void) const { return my_loopProtectionEnabled; }