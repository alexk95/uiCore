/*
 * ak_notifierForwardMessage.h
 *
 *  Created on: August 02, 2020
 *	Last modified on: September 07, 2020
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
#include <ak_core.h>				// messageType

namespace ak {

	class messenger;

	//! @brief This notifier is used to forward messages by using a new sender id
	class notifierForwardMessage : public ak::notifier {
	public:
		//! @brief Constructor
		//! @param _messenger The messenger used to send the forwarded message
		//! @param _uid The UID which will be used to forward the messages
		//! @param _enableLoopProtection If true, the loop protection will be activated
		notifierForwardMessage(
			ak::messenger *					_messenger,
			ak::UID							_uid,
			bool							_enableLoopProtection = true
		);

		//! Deconstructor
		virtual ~notifierForwardMessage(void);

		// *****************************************************************************************
		// Message IO

		//! @brief Will call the callback function with the provided parameters
		//! @param _senderId The sender ID the message was send from
		//! @param _event The event message
		//! @param _info1 Message addition 1
		//! @param _info2 Message addition 2
		//! @throw ak::Exception to forward exceptions coming from the application core class
		//! @throw ak::Exception if the sender id is equal to the forward id (infinite message loop)
		virtual void notify(
			ak::UID									_senderId,
			ak::core::eventType						_event,
			int										_info1,
			int										_info2
		) override;

		//! @brief Will set the loop protection mode
		//! @param _enable If true, the loop protection will be enabled
		void setLoopProtectionEnabled(
			bool									_enable
		);

		//! @brief Returns true if the loop protection is enabled
		bool loopProtectionEnabled(void) const;

	private:
		ak::UID										my_uid;							//! The UID used to forward the messages
		ak::messenger *								my_messenger;					//! The messenger used to send the messages
		bool										my_loopProtectionEnabled;		//! If true an exception will be thrown if the sender UID is equal to the forward UID

		// Block default constructor
		notifierForwardMessage() = delete;

		// Block copy constructor
		notifierForwardMessage(const notifierForwardMessage & _o) = delete;
	};

}