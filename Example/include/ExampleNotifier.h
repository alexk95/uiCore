/*
 * ExampleNotifier.h
 *
 *  Created on: September 15, 2020
 *	Last modified on: September 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

#include <ak_globalDataTypes.h>		// UID type
#include <ak_notifier.h>			// Base class

class Example;

//! @brief This notifier is used to notify the core application about incoming events and errors from the uiAPI
class ExampleNotifier : public ak::notifier {
public:
	
	//! @brief Constructor
	//! @param _callbackClass The object that is using this notifier
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
		ak::core::eventType		_event,
		int						_info1,
		int						_info2
	) override;

private:
	Example *					my_callbackClass;	//! The object that is called when an event occurs
};