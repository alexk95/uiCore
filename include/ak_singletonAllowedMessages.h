/*
 * ak_singletonAllowedMessages.h
 *
 *  Created on: September 16; 2020
 *	Last modified on: September 16; 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_core.h>

// C++ header
#include <vector>

namespace ak {
	
	class singletonAllowedMessages
	{
	public:

		singletonAllowedMessages(const singletonAllowedMessages &other) = delete;

		void operator=(const singletonAllowedMessages &) = delete;
		
		static singletonAllowedMessages * instance();

		// ###################################################################################

		// Setter

		void setFlag(ak::core::eventType		_type);

		void removeFlag(ak::core::eventType		_type);

		// ###################################################################################

		// Getter

		std::vector<ak::core::eventType> enabledMessages(void) const;

		std::vector<ak::core::eventType> disabledMessages(void) const;

		bool unknownEvent(void) const;

		bool activatedEvent(void) const;

		bool changedEvent(void) const;

		bool clearedEvent(void) const;

		bool clickedEvent(void) const;

		bool collapsedEvent(void) const;

		bool cursorPositionChangedEvent(void) const;

		bool destroyedEvent(void) const;

		bool doubleClickedEvent(void) const;

		bool expandedEvent(void) const;

		bool focusedEvent(void) const;

		bool invalidEntryEvent(void) const;

		bool indexChangedEvent(void) const;

		bool keyPressedEvent(void) const;

		bool locationChangedEvent(void) const;

		bool releasedEvent(void) const;

		bool selectionChangedEvent(void) const;

		bool stateChangedEvent(void) const;

		bool textChangedEvent(void) const;

		bool toggledCheckedEvent(void) const;

		bool toggledUncheckedEvent(void) const;

	protected:
		singletonAllowedMessages();

		static singletonAllowedMessages*		my_singleton;

		bool						my_unknownEvent;
		bool						my_activated;
		bool						my_changed;
		bool						my_cleared;
		bool						my_clicked;
		bool						my_collpased;
		bool						my_cursorPosotionChanged;
		bool						my_destroyed;
		bool						my_doubleClicked;
		bool						my_expanded;
		bool						my_focused;
		bool						my_invalidEntry;
		bool						my_indexChanged;
		bool						my_locationChanged;
		bool						my_released;
		bool						my_keyPressed;
		bool						my_selectionChanged;
		bool						my_stateChanged;
		bool						my_textChanged;
		bool						my_toggeledChecked;
		bool						my_toggeledUnchecked;
			
	};

}