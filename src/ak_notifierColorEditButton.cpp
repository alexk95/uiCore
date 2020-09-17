/*
 * ak_notifierColorEditButton.cpp
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_notifierColorEditButton.h"			// corresponding header
#include "ak_ui_widget_colorEditButton.h"		// colorEditButton
#include "ak_exception.h"						// error handling

ak::notifierColorEditButton::notifierColorEditButton(
	ak::ui::widget::colorEditButton *		_btn
)
	: my_button(_btn)
{
	try {
		if (_btn == nullptr) { throw ak::Exception("Is nullptr", "Check button"); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierColorEditButton::notifierColorEditButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierColorEditButton::notifierColorEditButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierColorEditButton::notifierColorEditButton()"); }
}

ak::notifierColorEditButton::~notifierColorEditButton(void) {}

void ak::notifierColorEditButton::notify(
	ak::UID									_senderId,
	ak::core::messageType					_messageType,
	int										_message,
	int										_info1,
	int										_info2
) {
	try {
		if (isEnabled()) {
			if (_messageType == ak::core::messageType::mEvent) {
				my_button->widgetEvent(_senderId, (ak::core::eventType)_message, _info1, _info2);
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierColorEditButton::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierColorEditButton::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierColorEditButton::notify()"); }
}