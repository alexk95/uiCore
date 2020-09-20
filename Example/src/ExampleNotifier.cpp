/*
	The following copyright affects the following files in the uiCore repository:
	.../uiCore/Example/Example.vcxproj
	.../uiCore/Example/include/Example.h
	.../uiCore/Example/src/Example.cpp
	.../uiCore/Example/src/main.cpp

	Copyright (c) 2020 Alexander Küster

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, without conditions:

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <ExampleNotifier.h>		// Corresponding header
#include <ak_exception.h>			// Error handling
#include <Example.h>				// Callback class

ExampleNotifier::ExampleNotifier(
	Example *				_callbackClass
) {
	if (_callbackClass == nullptr) { throw ak::Exception("Callback class is nullptr", "ExampleNotifier::ExampleNotifier()"); }
	my_callbackClass = _callbackClass;
}

ExampleNotifier::~ExampleNotifier(void) {}

// *****************************************************************************************
// Message IO

void ExampleNotifier::notify(
	ak::UID					_senderId,
	ak::core::messageType	_messageType,
	int						_message,
	int						_info1,
	int						_info2
) {
	try {
		if (_messageType == ak::core::messageType::mEvent) {
			my_callbackClass->eventCallback(_senderId, (ak::core::eventType)_message, _info1, _info2);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ExampleNotifier::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ExampleNotifier::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ExampleNotifier::notify()"); }
}