/*
 *	File:		aSingletonAllowedMessages.h
 *	Package:	akCore
 *
 *  Created on: September 16; 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <akCore/globalDataTypes.h>
#include <akCore/akCore.h>

// C++ header
#include <vector>

namespace ak {
	
	class UICORE_API_EXPORT aSingletonAllowedMessages
	{
	public:
		
		aSingletonAllowedMessages(const aSingletonAllowedMessages &other) = delete;

		aSingletonAllowedMessages & operator=(const aSingletonAllowedMessages &) = delete;
		
		static aSingletonAllowedMessages * instance();

		// ###################################################################################

		// Setter

		void setFlag(eventType		_type);

		void removeFlag(eventType	_type);

		// ###################################################################################

		// Getter

		std::vector<eventType> enabledMessages(void) const;

		std::vector<ak::eventType> disabledMessages(void) const;

		bool unknownEvent(void) const { return my_unknownEvent; }

		bool activatedEvent(void) const { return my_activated; }

		bool changedEvent(void) const { return my_changed; }

		bool clearedEvent(void) const { return my_cleared; }

		bool clickedEvent(void) const { return my_clicked; }

		bool closeRequestedEvent(void) const { return my_closeRequested; }

		bool collapsedEvent(void) const { return my_collpased; }

		bool cursorPositionChangedEvent(void) const { return my_cursorPosotionChanged; }

		bool destroyedEvent(void) const { return my_destroyed; }

		bool doubleClickedEvent(void) const { return my_doubleClicked; }

		bool expandedEvent(void) const { return my_expanded; }

		bool focusedEvent(void) const { return my_focused; }

		bool invalidEntryEvent(void) const { return my_invalidEntry; }

		bool indexChangedEvent(void) const { return my_indexChanged; }

		bool keyPressedEvent(void) const { return my_keyPressed; }

		bool locationChangedEvent(void) const { return my_locationChanged; }

		bool releasedEvent(void) const { return my_released; }

		bool selectionChangedEvent(void) const { return my_selectionChanged; }

		bool stateChangedEvent(void) const { return my_stateChanged; }

		bool textChangedEvent(void) const { return my_textChanged; }

		bool toggledCheckedEvent(void) const { return my_toggeledChecked; }

		bool toggledUncheckedEvent(void) const { return my_toggeledUnchecked; }

	protected:
		aSingletonAllowedMessages();

		static aSingletonAllowedMessages *	my_singleton;

		bool						my_unknownEvent;
		bool						my_activated;
		bool						my_changed;
		bool						my_cleared;
		bool						my_clicked;
		bool						my_closeRequested;
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