/*
 * ak_notifierStaticEvent.h
 *
 *	Created on: August 16, 2020
 *	Last modified on: September 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
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
		) override;

	private:

		eventCallback *				my_callback;

		// Block default constructor
		notifierStaticEvent() = delete;

		// Block copy constructor
		notifierStaticEvent(const notifierStaticEvent & _o) = delete;

	};
}	// namespace ak