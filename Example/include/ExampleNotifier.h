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

#pragma once

#include <ak_globalDataTypes.h>		// UID type
#include <ak_notifier.h>			// Base class

class Example;

//! @brief This notifier is used to notify the core application about incoming events and errors
class ExampleNotifier : public ak::notifier {
public:
	
	//! @brief Constructor
	ExampleNotifier(
		Example *				_callbackClass
	);

	//! Deconstructor
	virtual ~ExampleNotifier(void);

	// *****************************************************************************************
	// Message IO

	//! @brief Will call the callback function with the provided parameters
	//! @param _senderId The sender ID the message was send from
	//! @param _message The message
	//! @param _messageType The type of the messsage
	//! @param _info1 Message addition 1
	//! @param _info2 Message addition 2
	//! @throw ak::Exception to forward exceptions coming from the application core class
	virtual void notify(
		ak::UID					_senderId,
		ak::core::messageType	_messageType,
		int						_message,
		int						_info1,
		int						_info2
	) override;

private:
	Example *					my_callbackClass;
};