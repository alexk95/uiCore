/*
 * ak_notifierStaticEvent.h
 *
 *	Created on: August 16, 2020
 *	Last modified on: September 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_notifier.h>			// base class
#include <ak_globalDataTypes.h>		// UID and ID type
#include <ak_core.h>				// message type

namespace ak {

	class UICORE_API notifierStaticEvent : public notifier {
	public:
		typedef void eventCallback(ak::UID, ak::core::eventType, int, int);

		notifierStaticEvent(
			eventCallback *	callback
		);

		virtual void notify(
			ak::UID							_senderId,
			ak::core::eventType				_event,
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