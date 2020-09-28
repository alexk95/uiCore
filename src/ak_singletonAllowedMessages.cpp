/*
 * ak_singletonAllowedMessages.cpp
 *
 *  Created on: September 16, 2020
 *	Last modified on: September 17, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_singletonAllowedMessages.h"

ak::singletonAllowedMessages * ak::singletonAllowedMessages::my_singleton = nullptr;;

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
	if (_type & ak::core::eReleased) {
		my_released = true;
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
	if (_type & ak::core::eReleased) {
		my_released = false;
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
	if (my_released) { ret.push_back(ak::core::eventType::eReleased); }
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
	if (!my_released) { ret.push_back(ak::core::eventType::eReleased); }
	if (!my_selectionChanged) { ret.push_back(ak::core::eventType::eSelectionChanged); }
	if (!my_stateChanged) { ret.push_back(ak::core::eventType::eStateChanged); }
	//if (!my_textChanged) { ret.push_back(ak::core::eventType::eTextChanged); }
	if (!my_toggeledChecked) { ret.push_back(ak::core::eventType::eToggeledChecked); }
	if (!my_toggeledUnchecked) { ret.push_back(ak::core::eventType::eToggeledUnchecked); }
	if (!my_unknownEvent) { ret.push_back(ak::core::eventType::eUnknownEvent); }
	return ret;
}

bool ak::singletonAllowedMessages::unknownEvent(void) const { return my_unknownEvent; }

bool ak::singletonAllowedMessages::activatedEvent(void) const { return my_activated; }

bool ak::singletonAllowedMessages::changedEvent(void) const { return my_changed; }

bool ak::singletonAllowedMessages::clearedEvent(void) const { return my_cleared; }

bool ak::singletonAllowedMessages::clickedEvent(void) const { return my_clicked; }

bool ak::singletonAllowedMessages::collapsedEvent(void) const { return my_collpased; }

bool ak::singletonAllowedMessages::cursorPositionChangedEvent(void) const { return my_cursorPosotionChanged; }

bool ak::singletonAllowedMessages::destroyedEvent(void) const { return my_destroyed; }

bool ak::singletonAllowedMessages::doubleClickedEvent(void) const { return my_doubleClicked; }

bool ak::singletonAllowedMessages::expandedEvent(void) const { return my_expanded; }

bool ak::singletonAllowedMessages::focusedEvent(void) const { return my_focused; }

bool ak::singletonAllowedMessages::indexChangedEvent(void) const { return my_indexChanged; }

bool ak::singletonAllowedMessages::invalidEntryEvent(void) const { return my_invalidEntry; }

bool ak::singletonAllowedMessages::locationChangedEvent(void) const { return my_locationChanged; }

bool ak::singletonAllowedMessages::releasedEvent(void) const { return my_released; }

bool ak::singletonAllowedMessages::keyPressedEvent(void) const { return my_keyPressed; }

bool ak::singletonAllowedMessages::selectionChangedEvent(void) const { return my_selectionChanged; }

bool ak::singletonAllowedMessages::stateChangedEvent(void) const { return my_stateChanged; }

bool ak::singletonAllowedMessages::textChangedEvent(void) const { return my_textChanged; }

bool ak::singletonAllowedMessages::toggledCheckedEvent(void) const { return my_toggeledChecked; }

bool ak::singletonAllowedMessages::toggledUncheckedEvent(void) const { return my_toggeledUnchecked; }