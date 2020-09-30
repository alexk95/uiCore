/*
 * ak_notifierStaticEvent.cpp
 *
 *	Created on: August 16, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_notifierStaticEvent.h>		// corresponding header
#include <ak_exception.h>				// error handling

ak::notifierStaticEvent::notifierStaticEvent(
	eventCallback *	_callback
) {
	if (_callback == nullptr) { throw ak::Exception("Callback function is nullptr", "ak::notifierStaticEvent::notifierStaticEvent()"); }
	my_callback = _callback;
}

void ak::notifierStaticEvent::notify(
	ak::UID							_senderId,
	ak::core::eventType				_event,
	int								_info1,
	int								_info2
) {
	try {
		if (my_isEnabled == true) {
			my_callback(_senderId, _event, _info1, _info2);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierStaticEvent::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierStaticEvent::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierStaticEvent::notify()"); }
}