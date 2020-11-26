/*
 * ak_messenger.cpp
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_messenger.h>		// Corresponding class
#include <ak_exception.h>		// Error handling
#include <ak_notifier.h>		// Notifier class
#include <ak_uidMangager.h>		// UID manager

ak::messenger::messenger() : my_isEnabled(true) {
	my_uidManager = nullptr;
	my_uidManager = new ak::uidManager();
}

ak::messenger::~messenger() {}

// ####################################################################################################

void ak::messenger::sendMessage(
	ak::UID					_senderId,
	ak::core::eventType		_event,
	int						_info1,
	int						_info2
) {
	if (!my_isEnabled) { return; }
	try {
		// Find recievers for the senders UID
		my_uidReceiversIterator uidItem = my_uidReceivers.find(_senderId);
		if (uidItem != my_uidReceivers.end()) {
			// Go trough all notifiers
			for (auto n : *uidItem->second) {
				if (n->isEnabled()) {
					n->notify(_senderId, _event, _info1, _info2);
				}
			}
		}

		// Find receivers for the send event type
		my_eventReceiversIterator eventItem = my_eventReceivers.find(_event);
		if (eventItem != my_eventReceivers.end()) {
			// Go trough all notifiers
			for (auto n : *eventItem->second) {
				if (n->isEnabled()) { n->notify(_senderId, _event, _info1, _info2); }
			}
		}

		for (auto r : my_allMessageReceivers) { if (r->isEnabled()) { r->notify(_senderId, _event, _info1, _info2); } }

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::sendMessage()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::sendMessage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::sendMessage()"); }
}

// #############################################################################
// id management

ak::UID ak::messenger::registerUidReceiver(
	ak::UID					_senderId,
	ak::notifier *			_notifier
) {
	try {

		if (_notifier == nullptr) { throw ak::Exception("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) {
			_notifier->setUid(my_uidManager->getId());
		}

		my_uidReceiversIterator itm = my_uidReceivers.find(_senderId);

		if (itm == my_uidReceivers.end()) {
			// Create new vector to store the notifier classes
			std::vector<ak::notifier *> * collection = nullptr;
			collection = new (std::nothrow) std::vector<ak::notifier *>();
			if (collection == nullptr) { throw ak::Exception("Memory allocation failed", "Allocate collection at ID does not exist"); }
			collection->push_back(_notifier);
			my_uidReceivers.insert_or_assign(_senderId, collection);
		} else {
			itm->second->push_back(_notifier);
		}
		return _notifier->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::registerUidReceiver()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::registerUidReceiver()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::registerUidReceiver()"); }
}

ak::UID ak::messenger::registerEventTypeReceiver(
	ak::core::eventType				_eventType,
	ak::notifier *					_notifier
) {
	try {

		if (_notifier == nullptr) { throw ak::Exception("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) {
			_notifier->setUid(my_uidManager->getId());
		}

		my_eventReceiversIterator itm = my_eventReceivers.find(_eventType);

		if (itm == my_eventReceivers.end()) {
			// Create new vector to store the notifier classes
			std::vector<ak::notifier *> * collection = nullptr;
			collection = new (std::nothrow) std::vector<ak::notifier *>();
			if (collection == nullptr) { throw ak::Exception("Memory allocation failed", "Allocate collection at ID does not exist"); }
			collection->push_back(_notifier);
			my_eventReceivers.insert_or_assign(_eventType, collection);
		}
		else { itm->second->push_back(_notifier); }
		return _notifier->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::registerEventTypeReceiver()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::registerEventTypeReceiver()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::registerEventTypeReceiver()"); }
}

ak::UID ak::messenger::registerNotifierForAllMessages(
	ak::notifier *					_notifier
) {
	try {
		if (_notifier == nullptr) { throw ak::Exception("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) { _notifier->setUid(my_uidManager->getId()); }
		my_allMessageReceivers.push_back(_notifier);
		return _notifier->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::messenger::registerNotifierForAllMessages()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::messenger::registerNotifierForAllMessages()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::messenger::registerNotifierForAllMessages()"); }
}

int ak::messenger::uidNotifierCount(
	ak::UID					_senderId
) {
	int v = 0;
	my_uidReceiversIterator key = my_uidReceivers.find(_senderId);
	if (key != my_uidReceivers.end()) { v = key->second->size(); }
	return v;
}

int ak::messenger::eventNotifierCount(
	ak::core::eventType				_event
) {
	int v = 0;
	my_eventReceiversIterator key = my_eventReceivers.find(_event);
	if (key != my_eventReceivers.end()) { v = key->second->size(); }
	return v;
}

void ak::messenger::clearAll(void) { my_uidReceivers.clear();
	my_eventReceivers.clear(); my_allMessageReceivers.clear(); }
