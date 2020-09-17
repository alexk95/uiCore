/*
 * ak_messenger.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <map>						// Map ids and message types
#include <vector>					// Notifiers collection in the map

// AK header
#include "ak_core.h"				// messageType
#include "ak_globalDataTypes.h"		// UID and ID type

namespace ak {

	// Forward declaration
	class notifier;
	class uidManager;

	//! @brief Class used to forward messages to receivers
	//! This class is used to forward a message to receivers.
	//! A message will only be forwared to registered receivers.
	//! Receivers will only get messages from the senders and message types they want to be notified
	class __declspec(dllexport) messenger {
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
			ak::core::messageType			_messageType,
			int								_message,				
			int								_info1 = 0,
			int								_info2 = 0
		);

		// Data exchange
		// ####################################################################################################
		// Sender/receiver organisation

		//!@brief Will register a receiver for a specific type of message from the specified sender
		//!
		//! If a message is received by this object, only receivers with the matching sender id and message type are going to be notified
		//!
		//! @param _senderId The id of the sender the notifier wants to receive the message from
		//! @param _messageType The type of the message which the notifier wants to receive
		//! @param _notifier The notifier which is receiving the message
		void registerReceiver(
			ak::UID							_senderId,
			ak::core::messageType			_messageType,
			ak::notifier *					_notifier
		);

		//!@brief Will return the count of registered notifiers for a specific message type of a specific sender
		//!@param _senderId The sender id
		//!@param _messageType The message type
		int notifierCount(
			ak::UID							_senderId,
			ak::core::messageType			_messageType
		);

		// ####################################################################################################

		//! @brief Will remove all notifiers from the messaging system
		void clearAll(void);

	private:

		ak::uidManager *						my_uidManager;						//! The uid manager this messenger is using

		std::map<
			ak::UID,
			std::map<
				ak::core::messageType,				
				std::vector<ak::notifier *> *
			> *
		>										my_receivers;						//! Map contains all registered receivers

		typedef std::map<
			ak::UID,
			std::map<
				ak::core::messageType,
				std::vector<ak::notifier *> *
			> *
		>::iterator								my_receiversIterator;				//! Iterator used to iterate through the receivers

		typedef std::map<
			ak::core::messageType,
			std::vector<ak::notifier *> *
		>::iterator								my_receiversMessageTypeIterator;	//! Iterator ised to iterate through the messageTypes in a recievers objects

		// Block copy constructor
		messenger(const messenger & other) = delete;

		// Block assignment operator
		messenger & operator = (const messenger & other) = delete;

	};
}