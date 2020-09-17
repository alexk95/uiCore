/*
 * ak_notifierStaticEvent.h
 *
 *	Created on: August 16, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_notifier.h"			// base class
#include "ak_globalDataTypes.h"		// UID and ID type
#include "ak_core.h"				// message type

namespace ak {

	class notifierStaticEvent : public notifier {
	public:
		__declspec(dllexport) typedef void eventCallback(ak::UID, int, int, int);

		notifierStaticEvent(
			eventCallback *	callback
		);

		virtual void notify(
			ak::UID							_senderId,
			ak::core::messageType			_messageType,
			int								_message,
			int								_info1,
			int								_info2
		);

	private:

		eventCallback *				my_callback;

		// Block default constructor
		notifierStaticEvent() = delete;

		// Block copy constructor
		notifierStaticEvent(const notifierStaticEvent & _o) = delete;

	};
}	// namespace ak