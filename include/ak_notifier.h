/*
 * ak_notifer.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_core.h"					// messageType
#include "ak_globalDataTypes.h"			// UID and ID type
#include "ak_ui_core_aObject.h"			// Base class

namespace ak {
	
	//! This abstract class is used to provide the receiver functionallity used in the messaging class.
	class __declspec(dllexport) notifier : public ak::ui::core::aObject {
	public:
		//! @brief Will initialize the notifier
		notifier();

		//! @brief Will notify this receiver that a message was received by a sender.
		//! @param _senderId The id of the sender the message was sent from
		//! @param _messageType The type of the message
		//! @param _message The message
		//! @param _info1 Message addition 1
		//! @param _info2 Message addition 2
		//! @throw std::exception on any error occuring during the execution
		virtual void notify(
			ak::UID							_senderId,
			ak::core::messageType			_messageType,
			int								_message,
			int								_info1,
			int								_info2
		) = 0;

		//! @brief Will enable this notifier
		void enable(void);

		//! @brief Will disable this notifier
		void disable(void);

		//! @brief Will return true if this notifier is enabled
		bool isEnabled(void) const;

	protected:
		bool								my_isEnabled;		//! If false, messages will be ignored
	};
}