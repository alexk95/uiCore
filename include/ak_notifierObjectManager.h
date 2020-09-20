/*
 * ak_notifierObjectManager.h
 *
 *  Created on: July 16, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_notifier.h"			// base class
#include "ak_globalDataTypes.h"		// UID and ID type
#include "ak_core.h"				// messageType

namespace ak {

	namespace ui { class objectManager; }

	//! @brief This notifier is used to notify the core application about incoming events and errors
	class notifierObjectManager : public ak::notifier {
	public:
		//! @brief Constructor
		//! @param _ui The UI_test class where messages will be formwarded to
		//! @throw ak::Exception when the provided UI_test class was a nullptr
		notifierObjectManager(
			ak::ui::objectManager *					_manager
		);

		//! Deconstructor
		virtual ~notifierObjectManager(void);

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
			ak::UID									_senderId,
			ak::core::messageType					_messageType,
			int										_message,
			int										_info1,
			int										_info2
		) override;

	private:
		ak::ui::objectManager *						my_manager;			//! The manager used in this class

	};

} // namespace ak