/*
 * ExampleNotifier.cpp
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <ExampleNotifier.h>		// Corresponding header
#include <Example.h>				// Callback class

#include <akCore/aException.h>

ExampleNotifier::ExampleNotifier(
	Example *				_callbackClass
) {
	if (_callbackClass == nullptr) { throw ak::aException("Callback class is nullptr", "ExampleNotifier::ExampleNotifier()"); }
	my_callbackClass = _callbackClass;
}

ExampleNotifier::~ExampleNotifier(void) {}

// *****************************************************************************************
// Message IO

void ExampleNotifier::notify(
	ak::UID					_senderId,
	ak::eventType			_event,
	int						_info1,
	int						_info2
) {
	try { my_callbackClass->eventCallback(_senderId, _event, _info1, _info2); }
	// Just forward the error message. A message box will be displayed automatically when an exception is thrown inside a notify function.
	// The error message will contain the error message and a "callStack"
	catch (const ak::aException & e) { throw ak::aException(e, "ExampleNotifier::notify()"); }
	catch (const std::exception & e) { throw ak::aException(e.what(), "ExampleNotifier::notify()"); }
	catch (...) { throw ak::aException("Unknown error", "ExampleNotifier::notify()"); }
}