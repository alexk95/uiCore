/*
 *	File:		aMessenger.cpp
 *	Package:	akCore
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akCore/aMessenger.h>
#include <akCore/aNotifier.h>
#include <akCore/aUidMangager.h>

ak::aMessenger::aMessenger() : my_isEnabled(true) {
	my_uidManager = nullptr;
	my_uidManager = new ak::aUidManager();
}

ak::aMessenger::~aMessenger() {}

// ####################################################################################################

void ak::aMessenger::sendMessage(
	UID					_senderId,
	eventType				_event,
	int						_info1,
	int						_info2
) {
	if (!my_isEnabled) { return; }
	try {
		// Find recievers for the senders UID
		auto uidItem = my_uidReceivers.find(_senderId);
		if (uidItem != my_uidReceivers.end()) {
			// Go trough all notifiers
			for (auto n : *uidItem->second) {
				if (n->isEnabled()) {
					n->notify(_senderId, _event, _info1, _info2);
				}
			}
		}

		// Find receivers for the send event type
		auto eventItem = my_eventReceivers.find(_event);
		if (eventItem != my_eventReceivers.end()) {
			// Go trough all notifiers
			for (auto n : *eventItem->second) {
				if (n->isEnabled()) { n->notify(_senderId, _event, _info1, _info2); }
			}
		}

		for (auto r : my_allMessageReceivers) { if (r->isEnabled()) { r->notify(_senderId, _event, _info1, _info2); } }

	}
	catch (const aException & e) { throw aException(e, "ak::aMessenger::sendMessage()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aMessenger::sendMessage()"); }
	catch (...) { throw aException("Unknown error", "ak::aMessenger::sendMessage()"); }
}

// #############################################################################
// id management

ak::UID ak::aMessenger::registerUidReceiver(
	UID					_senderId,
	aNotifier *			_notifier
) {
	try {

		if (_notifier == nullptr) { throw aException("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) {
			_notifier->setUid(my_uidManager->getId());
		}

		auto itm = my_uidReceivers.find(_senderId);

		if (itm == my_uidReceivers.end()) {
			// Create new vector to store the notifier classes
			std::vector<aNotifier *> * collection = nullptr;
			collection = new (std::nothrow) std::vector<aNotifier *>();
			if (collection == nullptr) { throw aException("Memory allocation failed", "Allocate collection at ID does not exist"); }
			collection->push_back(_notifier);
			my_uidReceivers.insert_or_assign(_senderId, collection);
		} else {
			itm->second->push_back(_notifier);
		}
		return _notifier->uid();
	}
	catch (const aException & e) { throw aException(e, "ak::aMessenger::registerUidReceiver()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aMessenger::registerUidReceiver()"); }
	catch (...) { throw aException("Unknown error", "ak::aMessenger::registerUidReceiver()"); }
}

ak::UID ak::aMessenger::registerEventTypeReceiver(
	eventType				_eventType,
	aNotifier *					_notifier
) {
	try {

		if (_notifier == nullptr) { throw aException("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) {
			_notifier->setUid(my_uidManager->getId());
		}

		auto itm = my_eventReceivers.find(_eventType);

		if (itm == my_eventReceivers.end()) {
			// Create new vector to store the notifier classes
			std::vector<aNotifier *> * collection = nullptr;
			collection = new (std::nothrow) std::vector<aNotifier *>();
			if (collection == nullptr) { throw aException("Memory allocation failed", "Allocate collection at ID does not exist"); }
			collection->push_back(_notifier);
			my_eventReceivers.insert_or_assign(_eventType, collection);
		}
		else { itm->second->push_back(_notifier); }
		return _notifier->uid();
	}
	catch (const aException & e) { throw aException(e, "ak::aMessenger::registerEventTypeReceiver()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aMessenger::registerEventTypeReceiver()"); }
	catch (...) { throw aException("Unknown error", "ak::aMessenger::registerEventTypeReceiver()"); }
}

ak::UID ak::aMessenger::registerNotifierForAllMessages(
	aNotifier *					_notifier
) {
	try {
		if (_notifier == nullptr) { throw aException("Is nullptr", "Check notifier"); }
		if (_notifier->uid() == ak::invalidUID) { _notifier->setUid(my_uidManager->getId()); }
		my_allMessageReceivers.push_back(_notifier);
		return _notifier->uid();
	}
	catch (const aException & e) { throw aException(e, "ak::aMessenger::registerNotifierForAllMessages()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aMessenger::registerNotifierForAllMessages()"); }
	catch (...) { throw aException("Unknown error", "ak::aMessenger::registerNotifierForAllMessages()"); }
}

int ak::aMessenger::uidNotifierCount(
	UID					_senderId
) {
	int v = 0;
	auto key = my_uidReceivers.find(_senderId);
	if (key != my_uidReceivers.end()) { v = key->second->size(); }
	return v;
}

int ak::aMessenger::eventNotifierCount(
	eventType				_event
) {
	int v = 0;
	auto key = my_eventReceivers.find(_event);
	if (key != my_eventReceivers.end()) { v = key->second->size(); }
	return v;
}

void ak::aMessenger::clearAll(void) { my_uidReceivers.clear();
	my_eventReceivers.clear(); my_allMessageReceivers.clear(); }