/*
 * ak_messenger.h
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <map>						// Map ids and message types
#include <vector>					// Notifiers collection in the map

// AK header
#include <ak_core.h>				// eventType
#include <ak_globalDataTypes.h>		// UID and ID type

namespace ak {

	// Forward declaration
	class notifier;
	class uidManager;

	//! @brief Class used to forward messages to receivers
	//! This class is used to forward a message to receivers.
	//! A message will only be forwared to registered receivers.
	//! Receivers will only get messages from the senders and message types they want to be notified
	class UICORE_API messenger {
	public:

		//! @brief Default constructor
		messenger();

		//! @brief Deconstructor
		virtual ~messenger();

		// ####################################################################################################
		// Data exchange

		//!	@brief Will forward the message to all registered receivers
		//!	The sender id will be compared with the registered receivers in the receivers map.
		//!	If a receivers was found, the message Type will be compared with the registered message types at the found receivers.
		//!	If also the message type was found, the message will be forwareded to all receivers in the vector at the registered message type map entry.
		//! @param _senderId The id of the object the message is coming from 
		//! @param _messageType The type of the message which is transmitted
		//! @param _message The message to be transmitted
		//! @param _info1 The massage info 1
		//! @param _info2 The message info 2
		void sendMessage(
			ak::UID							_senderId,
			ak::core::eventType				_event,
			int								_info1 = 0,
			int								_info2 = 0
		);

		// Data exchange
		// ####################################################################################################
		// Sender/receiver organisation

		//! @brief Will register a receiver for a the specified sender
		//! If a message is received by this object, only receivers with the matching sender UID
		//! @param _senderId The id of the sender the notifier wants to receive the message from
		//! @param _messageType The type of the message which the notifier wants to receive
		//! @param _notifier The notifier which is receiving the message
		ak::UID registerUidReceiver(
			ak::UID							_senderId,
			ak::notifier *					_notifier
		);

		//! @brief Will register a receiver for a specified sender
		//! @param _eventType The event type for what to register this receiver
		//! @param _notifier The notifier to register
		ak::UID registerEventTypeReceiver(
			ak::core::eventType				_eventType,
			ak::notifier *					_notifier
		);

		//! @brief Will register a receiver for every single message send
		//! @param _notifier The notifier to register
		ak::UID registerNotifierForAllMessages(
			ak::notifier *					_notifier
		);

		//!@brief Will return the count of registered notifiers for a specific sender
		//!@param _senderId The sender UID
		int uidNotifierCount(
			ak::UID							_senderUID
		);

		//!@brief Will return the count of registered notifiers for a specific sender
		//!@param _event The event type
		int eventNotifierCount(
			ak::core::eventType				_event
		);

		// ####################################################################################################

		//! @brief Will remove all notifiers from the messaging system
		void clearAll(void);

		//! @brief Will enable the messenger
		void enable(void) { my_isEnabled = true; }

		//! @brief Will disable the messenger
		void disable(void) { my_isEnabled = false; }

	private:

		ak::uidManager *						my_uidManager;						//! The uid manager this messenger is using

		bool									my_isEnabled;						//! If true, messages will be send to the notifiers

		std::map<
			ak::UID,
			std::vector<ak::notifier *> *
		>										my_uidReceivers;					//! Map contains all registered receivers

		typedef std::map<
			ak::UID,
			std::vector<ak::notifier *> *
		>::iterator								my_uidReceiversIterator;			//! Iterator used to iterate through the receivers

		std::map<
			ak::core::eventType,
			std::vector<ak::notifier *> *
		>										my_eventReceivers;					//! Map contains all registered event type receivers

		typedef std::map<
			ak::core::eventType,
			std::vector<ak::notifier *> *
		>::iterator								my_eventReceiversIterator;			//! Iterator used to iterate through the UID receivers

		std::vector<ak::notifier *>				my_allMessageReceivers;

		// Block copy constructor
		messenger(const messenger & other) = delete;

		// Block assignment operator
		messenger & operator = (const messenger & other) = delete;

	};
}