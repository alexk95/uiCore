/*
 * ak_aSingletonAllowedMessages.cpp
 *
 *  Created on: September 16, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aSingletonAllowedMessages.h>

ak::aSingletonAllowedMessages * ak::aSingletonAllowedMessages::my_singleton = nullptr;

ak::aSingletonAllowedMessages::aSingletonAllowedMessages(void)
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

ak::aSingletonAllowedMessages * ak::aSingletonAllowedMessages::instance(void) {
	if (my_singleton == nullptr) {
		my_singleton = new aSingletonAllowedMessages;
	}
	return my_singleton;
}

void ak::aSingletonAllowedMessages::setFlag(eventType _type) {
	if (_type & etUnknownEvent) {
		my_unknownEvent = true;
	}
	if (_type & etActivated) {
		my_activated = true;
	}
	if (_type & etChanged) {
		my_changed = true;
	}
	if (_type & etCleared) {
		my_cleared = true;
	}
	if (_type & etClicked) {
		my_clicked = true;
	}
	if (_type & etClosing) {
		my_closeRequested = true;
	}
	if (_type & etCollpased) {
		my_collpased = true;
	}
	if (_type & etCursorPosotionChanged) {
		my_cursorPosotionChanged = true;
	}
	if (_type & etDestroyed) {
		my_destroyed = true;
	}
	if (_type & etDoubleClicked) {
		my_doubleClicked = true;
	}
	if (_type & etExpanded) {
		my_expanded = true;
	}
	if (_type & etFocused) {
		my_focused = true;
	}
	if (_type & etInvalidEntry) {
		my_invalidEntry = true;
	}
	if (_type & etIndexChanged) {
		my_indexChanged = true;
	}
	if (_type & etLocationChanged) {
		my_locationChanged = true;
	}
	if (_type & etKeyPressed) {
		my_keyPressed = true;
	}
	if (_type & etSelectionChanged) {
		my_selectionChanged = true;
	}
	if (_type & etStateChanged) {
		my_stateChanged = true;
	}
	/*if (_type & etTextChanged) {
		my_textChanged = true;
	}*/
	if (_type & etToggeledChecked) {
		my_toggeledChecked = true;
	}
	if (_type & etToggeledUnchecked) {
		my_toggeledUnchecked = true;
	}
}

void ak::aSingletonAllowedMessages::removeFlag(
	eventType			_type
) {
	if (_type & etUnknownEvent) {
		my_unknownEvent = false;
	}
	if (_type & etActivated) {
		my_activated = false;
	}
	if (_type & etChanged) {
		my_changed = false;
	}
	if (_type & etCleared) {
		my_cleared = false;
	}
	if (_type & etClicked) {
		my_clicked = false;
	}
	if (_type & etClosing) {
		my_closeRequested = false;
	}
	if (_type & etCollpased) {
		my_collpased = false;
	}
	if (_type & etCursorPosotionChanged) {
		my_cursorPosotionChanged = false;
	}
	if (_type & etDestroyed) {
		my_destroyed = false;
	}
	if (_type & etDoubleClicked) {
		my_doubleClicked = false;
	}
	if (_type & etExpanded) {
		my_expanded = false;
	}
	if (_type & etFocused) {
		my_focused = false;
	}
	if (_type & etInvalidEntry) {
		my_invalidEntry = false;
	}
	if (_type & etIndexChanged) {
		my_indexChanged = false;
	}
	if (_type & etLocationChanged) {
		my_locationChanged = false;
	}
	if (_type & etKeyPressed) {
		my_keyPressed = false;
	}
	if (_type & etSelectionChanged) {
		my_selectionChanged = false;
	}
	if (_type & etStateChanged) {
		my_stateChanged = false;
	}
	/*if (_type & etTextChanged) {
		my_textChanged = false;
	}*/
	if (_type & etToggeledChecked) {
		my_toggeledChecked = false;
	}
	if (_type & etToggeledUnchecked) {
		my_toggeledUnchecked = false;
	}
}

std::vector<ak::eventType> ak::aSingletonAllowedMessages::enabledMessages(void) const {
	std::vector<eventType> ret;
	if (my_activated) { ret.push_back(etActivated); }
	if (my_changed) { ret.push_back(etChanged); }
	if (my_cleared) { ret.push_back(etCleared); }
	if (my_clicked) { ret.push_back(etClicked); }
	if (my_closeRequested) { ret.push_back(etClosing); }
	if (my_collpased) { ret.push_back(etCollpased); }
	if (my_cursorPosotionChanged) { ret.push_back(etCursorPosotionChanged); }
	if (my_destroyed) { ret.push_back(etDestroyed); }
	if (my_doubleClicked) { ret.push_back(etDoubleClicked); }
	if (my_expanded) { ret.push_back(etExpanded); }
	if (my_focused) { ret.push_back(etFocused); }
	if (my_indexChanged) { ret.push_back(etIndexChanged); }
	if (my_invalidEntry) { ret.push_back(etInvalidEntry); }
	if (my_keyPressed) { ret.push_back(etKeyPressed); }
	if (my_locationChanged) { ret.push_back(etLocationChanged); }
	if (my_selectionChanged) { ret.push_back(etSelectionChanged); }
	if (my_stateChanged) { ret.push_back(etStateChanged); }
	if (my_toggeledChecked) { ret.push_back(etToggeledChecked); }
	if (my_toggeledUnchecked) { ret.push_back(etToggeledUnchecked); }
	if (my_unknownEvent) { ret.push_back(etUnknownEvent); }
	return ret;
}

std::vector<ak::eventType> ak::aSingletonAllowedMessages::disabledMessages(void) const {
	std::vector<eventType> ret;
	if (!my_activated) { ret.push_back(etActivated); }
	if (!my_changed) { ret.push_back(etChanged); }
	if (!my_cleared) { ret.push_back(etCleared); }
	if (!my_clicked) { ret.push_back(etClicked); }
	if (!my_closeRequested) { ret.push_back(etClosing); }
	if (!my_collpased) { ret.push_back(etCollpased); }
	if (!my_cursorPosotionChanged) { ret.push_back(etCursorPosotionChanged); }
	if (!my_destroyed) { ret.push_back(etDestroyed); }
	if (!my_doubleClicked) { ret.push_back(etDoubleClicked); }
	if (!my_expanded) { ret.push_back(etExpanded); }
	if (!my_focused) { ret.push_back(etFocused); }
	if (!my_indexChanged) { ret.push_back(etIndexChanged); }
	if (!my_invalidEntry) { ret.push_back(etInvalidEntry); }
	if (!my_keyPressed) { ret.push_back(etKeyPressed); }
	if (!my_locationChanged) { ret.push_back(etLocationChanged); }
	if (!my_selectionChanged) { ret.push_back(etSelectionChanged); }
	if (!my_stateChanged) { ret.push_back(etStateChanged); }
	if (!my_toggeledChecked) { ret.push_back(etToggeledChecked); }
	if (!my_toggeledUnchecked) { ret.push_back(etToggeledUnchecked); }
	if (!my_unknownEvent) { ret.push_back(etUnknownEvent); }
	return ret;
}
