/*
 * ak_messenger.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_messenger.h"		// Corresponding class
#include "ak_exception.h"		// Error handling
#include "ak_notifier.h"		// Notifier class
#include "ak_uidMangager.h"		// UID manager

ak::messenger::messenger() {
	try {
		my_uidManager = nullptr;
		my_uidManager = new ak::uidManager();
		if (my_uidManager == nullptr) { throw ak::Exception("Failed to create", "Create UID manager"); }
	} 
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::messenger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::messenger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::messenger()"); }
}

ak::messenger::~messenger() {}

// ####################################################################################################

void ak::messenger::sendMessage(
	ak::UID						_senderId,
	ak::core::messageType					_messageType,
	int										_message,
	int										_info1,
	int										_info2
) {
	try {
		// Find recievers for the senders UID
		my_receiversIterator key = my_receivers.find(_senderId);
		if (key != my_receivers.end()) {
			// Find receivers for this message type
			my_receiversMessageTypeIterator messageType = key->second->find(_messageType);
			if (messageType != key->second->end()) {
				// Get notifiers
				if (messageType->second != nullptr) {
					ak::notifier * current_notifer;
					// Go trough all notifiers
					for (int i = 0; i < messageType->second->size(); i++) {
						current_notifer = messageType->second->at(i);
						// Check if notifier exists and is active
						if (current_notifer != nullptr) {
							if (current_notifer->isEnabled()) {
								current_notifer->notify(_senderId, _messageType, _message, _info1, _info2);
							}
						}
					}
				}
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::sendMessage()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::sendMessage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::sendMessage()"); }
}

// #############################################################################
// id management

void ak::messenger::registerReceiver(
	ak::UID						_senderId,
	ak::core::messageType					_messageType,
	ak::notifier *							_notifier
) {
	try {

		if (_notifier == nullptr) { throw ak::Exception("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) {
			_notifier->setUid(my_uidManager->getId());
		}

		my_receiversIterator key = my_receivers.find(_senderId);

		if (key == my_receivers.end()) {
			// Create new vector to store the notifier classes
			std::vector<ak::notifier *> *		collection = nullptr;
			collection = new (std::nothrow) std::vector<ak::notifier *>;
			if (collection == nullptr) { throw ak::Exception("Memory allocation failed", "Allocate collection at ID does not exist"); }
			// Create new map for the specified id
			std::map<ak::core::messageType, std::vector<ak::notifier *> *> * new_map = nullptr;
			new_map = new (std::nothrow) std::map<ak::core::messageType, std::vector<ak::notifier *> *>;
			if (new_map == nullptr) {
				delete collection;
				throw ak::Exception("Memory allocation failed", "Allocate map at ID does not exist");
			}
			// Add new item
			collection->push_back(_notifier);
			new_map->insert_or_assign(_messageType, collection);
			my_receivers.insert_or_assign(_senderId, new_map);
		} else {
			my_receiversMessageTypeIterator messageType = key->second->find(_messageType);
			if (messageType == key->second->end()) {
				// Create new vector to store the notifier classes
				std::vector<ak::notifier *> *		collection = nullptr;
				collection = new (std::nothrow) std::vector<ak::notifier *>;
				if (collection == nullptr) { throw ak::Exception("Memory allocation failed", "Allocate collection at ID does not exist"); }
				// Add new item
				collection->push_back(_notifier);
				key->second->insert_or_assign(_messageType, collection);
			} else {
				messageType->second->push_back(_notifier);
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::registerReceiver()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::registerReceiver()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::registerReceiver()"); }
}

int ak::messenger::notifierCount(
	ak::UID						_senderId,
	ak::core::messageType					_messageType
) {
	int v = 0;
	my_receiversIterator key = my_receivers.find(_senderId);
	if (key != my_receivers.end()) {
		my_receiversMessageTypeIterator messageType = key->second->find(_messageType);
		if (messageType != key->second->end()) { v = messageType->second->size(); }
	}
	return v;
}

void ak::messenger::clearAll(void) {
	for (my_receiversIterator itm = my_receivers.begin(); itm != my_receivers.end(); itm++) {
		itm->second->clear();
		delete itm->second;
	}
	my_receivers.clear();
}