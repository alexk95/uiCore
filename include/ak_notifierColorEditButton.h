/*
 * ak_notifierColorEditButton.h
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_notifier.h"			// base class
#include "ak_core.h"				// messageType
#include "ak_globalDataTypes.h"		// UID and ID type

namespace ak {

	// Forward declaration
	namespace ui { namespace widget { class colorEditButton; } }

	//! @brief This notifier is used to notify the core application about incoming events and errors
	class notifierColorEditButton : public ak::notifier {
	public:
		//! @brief Constructor
		//! @param _btn The color edit button that will get notified by this notifier
		//! @throw ak::Exception when the provided colorEditButton was a nullptr
		notifierColorEditButton(
			ak::ui::widget::colorEditButton *		_btn
		);

		//! Deconstructor
		virtual ~notifierColorEditButton(void);

		// *****************************************************************************************
		// Message IO

		//! @brief Will call the callback function with the provided parameters
		//! @param _senderId The sender ID the message was send from
		//! @param _event The event message
		//! @param _info1 Message addition 1
		//! @param _info2 Message addition 2
		//! @throw ak::Exception to forward exceptions coming from the application core class
		virtual void notify(
			ak::UID									_senderId,
			ak::core::eventType						_event,
			int										_info1,
			int										_info2
		) override;

	private:
		ak::ui::widget::colorEditButton *				my_button;			//! The colorEditButton used in this class

	};

} // namespace ak