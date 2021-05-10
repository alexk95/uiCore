/*
 * ak_singletonAllowedMessages.cpp
 *
 *  Created on: September 16, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_singletonAllowedMessages.h>

ak::singletonAllowedMessages * ak::singletonAllowedMessages::my_singleton = nullptr;

ak::singletonAllowedMessages::singletonAllowedMessages(void)
	: my_unknownEvent(true),
	my_activated(true),
	my_changed(true),
	my_cleared(true),
	my_clicked(true),
	my_collpased(true),
	my_cursorPosotionChanged(true),
	my_destroyed(true),
	my_doubleClicked(true),
	my_expanded(true),
	my_focused(true),
	my_invalidEntry(true),
	my_indexChanged(true),
	my_locationChanged(true),
	my_released(true),
	my_keyPressed(true),
	my_selectionChanged(true),
	my_stateChanged(true),
	my_textChanged(true),
	my_toggeledChecked(true),
	my_toggeledUnchecked(true)
{}

ak::singletonAllowedMessages * ak::singletonAllowedMessages::instance(void) {
	if (my_singleton == nullptr) {
		my_singleton = new singletonAllowedMessages();
	}
	return my_singleton;
}

void ak::singletonAllowedMessages::setFlag(ak::core::eventType		_type) {
	if (_type & ak::core::eUnknownEvent) {
		my_unknownEvent = true;
	}
	if (_type & ak::core::eActivated) {
		my_activated = true;
	}
	if (_type & ak::core::eChanged) {
		my_changed = true;
	}
	if (_type & ak::core::eCleared) {
		my_cleared = true;
	}
	if (_type & ak::core::eClicked) {
		my_clicked = true;
	}
	if (_type & ak::core::eClosing) {
		my_closeRequested = true;
	}
	if (_type & ak::core::eCollpased) {
		my_collpased = true;
	}
	if (_type & ak::core::eCursorPosotionChanged) {
		my_cursorPosotionChanged = true;
	}
	if (_type & ak::core::eDestroyed) {
		my_destroyed = true;
	}
	if (_type & ak::core::eDoubleClicked) {
		my_doubleClicked = true;
	}
	if (_type & ak::core::eExpanded) {
		my_expanded = true;
	}
	if (_type & ak::core::eFocused) {
		my_focused = true;
	}
	if (_type & ak::core::eInvalidEntry) {
		my_invalidEntry = true;
	}
	if (_type & ak::core::eIndexChanged) {
		my_indexChanged = true;
	}
	if (_type & ak::core::eLocationChanged) {
		my_locationChanged = true;
	}
	if (_type & ak::core::eKeyPressed) {
		my_keyPressed = true;
	}
	if (_type & ak::core::eSelectionChanged) {
		my_selectionChanged = true;
	}
	if (_type & ak::core::eStateChanged) {
		my_stateChanged = true;
	}
	/*if (_type & ak::core::eTextChanged) {
		my_textChanged = true;
	}*/
	if (_type & ak::core::eToggeledChecked) {
		my_toggeledChecked = true;
	}
	if (_type & ak::core::eToggeledUnchecked) {
		my_toggeledUnchecked = true;
	}
}

void ak::singletonAllowedMessages::removeFlag(
	ak::core::eventType			_type
) {
	if (_type & ak::core::eUnknownEvent) {
		my_unknownEvent = false;
	}
	if (_type & ak::core::eActivated) {
		my_activated = false;
	}
	if (_type & ak::core::eChanged) {
		my_changed = false;
	}
	if (_type & ak::core::eCleared) {
		my_cleared = false;
	}
	if (_type & ak::core::eClicked) {
		my_clicked = false;
	}
	if (_type & ak::core::eClosing) {
		my_closeRequested = false;
	}
	if (_type & ak::core::eCollpased) {
		my_collpased = false;
	}
	if (_type & ak::core::eCursorPosotionChanged) {
		my_cursorPosotionChanged = false;
	}
	if (_type & ak::core::eDestroyed) {
		my_destroyed = false;
	}
	if (_type & ak::core::eDoubleClicked) {
		my_doubleClicked = false;
	}
	if (_type & ak::core::eExpanded) {
		my_expanded = false;
	}
	if (_type & ak::core::eFocused) {
		my_focused = false;
	}
	if (_type & ak::core::eInvalidEntry) {
		my_invalidEntry = false;
	}
	if (_type & ak::core::eIndexChanged) {
		my_indexChanged = false;
	}
	if (_type & ak::core::eLocationChanged) {
		my_locationChanged = false;
	}
	if (_type & ak::core::eKeyPressed) {
		my_keyPressed = false;
	}
	if (_type & ak::core::eSelectionChanged) {
		my_selectionChanged = false;
	}
	if (_type & ak::core::eStateChanged) {
		my_stateChanged = false;
	}
	/*if (_type & ak::core::eTextChanged) {
		my_textChanged = false;
	}*/
	if (_type & ak::core::eToggeledChecked) {
		my_toggeledChecked = false;
	}
	if (_type & ak::core::eToggeledUnchecked) {
		my_toggeledUnchecked = false;
	}
}

std::vector<ak::core::eventType> ak::singletonAllowedMessages::enabledMessages(void) const {
	std::vector<ak::core::eventType> ret;
	if (my_activated) { ret.push_back(ak::core::eventType::eActivated); }
	if (my_changed) { ret.push_back(ak::core::eventType::eChanged); }
	if (my_cleared) { ret.push_back(ak::core::eventType::eCleared); }
	if (my_clicked) { ret.push_back(ak::core::eventType::eClicked); }
	if (my_closeRequested) { ret.push_back(ak::core::eventType::eClosing); }
	if (my_collpased) { ret.push_back(ak::core::eventType::eCollpased); }
	if (my_cursorPosotionChanged) { ret.push_back(ak::core::eventType::eCursorPosotionChanged); }
	if (my_destroyed) { ret.push_back(ak::core::eventType::eDestroyed); }
	if (my_doubleClicked) { ret.push_back(ak::core::eventType::eDoubleClicked); }
	if (my_expanded) { ret.push_back(ak::core::eventType::eExpanded); }
	if (my_focused) { ret.push_back(ak::core::eventType::eFocused); }
	if (my_indexChanged) { ret.push_back(ak::core::eventType::eIndexChanged); }
	if (my_invalidEntry) { ret.push_back(ak::core::eventType::eInvalidEntry); }
	if (my_keyPressed) { ret.push_back(ak::core::eventType::eKeyPressed); }
	if (my_locationChanged) { ret.push_back(ak::core::eventType::eLocationChanged); }
	if (my_selectionChanged) { ret.push_back(ak::core::eventType::eSelectionChanged); }
	if (my_stateChanged) { ret.push_back(ak::core::eventType::eStateChanged); }
	//if (my_textChanged) { ret.push_back(ak::core::eventType::eTextChanged); }
	if (my_toggeledChecked) { ret.push_back(ak::core::eventType::eToggeledChecked); }
	if (my_toggeledUnchecked) { ret.push_back(ak::core::eventType::eToggeledUnchecked); }
	if (my_unknownEvent) { ret.push_back(ak::core::eventType::eUnknownEvent); }
	return ret;
}

std::vector<ak::core::eventType> ak::singletonAllowedMessages::disabledMessages(void) const {
	std::vector<ak::core::eventType> ret;
	if (!my_activated) { ret.push_back(ak::core::eventType::eActivated); }
	if (!my_changed) { ret.push_back(ak::core::eventType::eChanged); }
	if (!my_cleared) { ret.push_back(ak::core::eventType::eCleared); }
	if (!my_clicked) { ret.push_back(ak::core::eventType::eClicked); }
	if (!my_closeRequested) { ret.push_back(ak::core::eventType::eClosing); }
	if (!my_collpased) { ret.push_back(ak::core::eventType::eCollpased); }
	if (!my_cursorPosotionChanged) { ret.push_back(ak::core::eventType::eCursorPosotionChanged); }
	if (!my_destroyed) { ret.push_back(ak::core::eventType::eDestroyed); }
	if (!my_doubleClicked) { ret.push_back(ak::core::eventType::eDoubleClicked); }
	if (!my_expanded) { ret.push_back(ak::core::eventType::eExpanded); }
	if (!my_focused) { ret.push_back(ak::core::eventType::eFocused); }
	if (!my_indexChanged) { ret.push_back(ak::core::eventType::eIndexChanged); }
	if (!my_invalidEntry) { ret.push_back(ak::core::eventType::eInvalidEntry); }
	if (!my_keyPressed) { ret.push_back(ak::core::eventType::eKeyPressed); }
	if (!my_locationChanged) { ret.push_back(ak::core::eventType::eLocationChanged); }
	if (!my_selectionChanged) { ret.push_back(ak::core::eventType::eSelectionChanged); }
	if (!my_stateChanged) { ret.push_back(ak::core::eventType::eStateChanged); }
	//if (!my_textChanged) { ret.push_back(ak::core::eventType::eTextChanged); }
	if (!my_toggeledChecked) { ret.push_back(ak::core::eventType::eToggeledChecked); }
	if (!my_toggeledUnchecked) { ret.push_back(ak::core::eventType::eToggeledUnchecked); }
	if (!my_unknownEvent) { ret.push_back(ak::core::eventType::eUnknownEvent); }
	return ret;
}
