/*
 * ak_notifierTreeFilter.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_notifierTreeFilter.h"		// corresponding class
#include "ak_exception.h"				// error handling
#include "ak_ui_widget_tree.h"			// tree

ak::notifierTreeFilter::notifierTreeFilter(
	ak::ui::widget::tree *				_tree
) {
	try {
		if (_tree == nullptr) { throw ak::Exception("Is nullptr", "Check tree"); }
		my_tree = _tree;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierTreeFilter::notifierTreeFilter()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierTreeFilter::notifierTreeFilter()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierTreeFilter::notifierTreeFilter()"); }
}

ak::notifierTreeFilter::~notifierTreeFilter() {}

void ak::notifierTreeFilter::notify(
	ak::UID						_senderId,
	ak::core::messageType		_messageType,
	int							_message,
	int							_info1,
	int							_info2
) {
	try {
		// Check if the message is an event message
		if (_messageType == ak::core::messageType::mEvent) {
			// Check the event type
			switch (_message)
			{
			case ak::core::eventType::eTextChanged: my_tree->performFilterTextChanged(); break;
			case ak::core::eventType::eReturnPressed: my_tree->performFilterEnterPressed(); break;
			default: break;
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierTreeFilter::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierTreeFilter::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierTreeFilter::notify()"); }
}