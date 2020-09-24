/*
 * ak_notiferPropertyGrid.cpp
 *
 *  Created on: March 29, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_notifierPropertyGrid.h"	// corresponding header
#include "ak_exception.h"				// error handling
#include "ak_ui_widget_propertyGrid.h"	// propertyGrid

ak::notifierPropertyGrid::notifierPropertyGrid(
	ak::ui::widget::propertyGrid *			_propertyGrid,
	ak::UID									_tableUid
) : my_isEnabled(true)
{
	try {
		if (_propertyGrid == nullptr) { throw ak::Exception("Is nullptr", "Check propertyGrid"); }
		my_propertyGrid = _propertyGrid;
		my_tableUid = _tableUid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierPropertyGrid::notifierPropertyGrid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierPropertyGrid::notifierPropertyGrid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierPropertyGrid::notifierPropertyGrid()"); }

}

void ak::notifierPropertyGrid::notify(
	ak::UID							_senderId,
	ak::core::messageType			_messageType,
	int								_message,
	int								_info1,
	int								_info2
) {
	try {
		// Check if the notifier is enabled
		if (my_isEnabled) {
			// Check if the message type is an event
			if (_messageType == ak::core::messageType::mEvent) {
				// Check if the sender is the table
				if (_senderId == my_tableUid) {
					// Check if the message comes from the second column of the table
					if (_info2 == 1) {
						my_propertyGrid->raiseCellEvent(_info1, (ak::core::eventType)_message);
					}
					else if (_info2 == 0) {
						if (_senderId == my_tableUid) {
							// Check if the event was a clicked event
							if (_message == ak::core::eventType::eClicked || _message == ak::core::eventType::eDoubleClicked ||
								_message == ak::core::eventType::eActivated) {
								my_propertyGrid->focusRow(_info1);
							}
						}
						else {
							// Forward the event to the property grid event handler (raise widget event function)
							my_propertyGrid->raiseWidgetEvent(_senderId, (ak::core::eventType)_message);
						}
					}
					else if (_message == ak::core::eKeyPressed) { my_propertyGrid->keyPressedEvent((ak::ui::core::keyType) _info2); }
					else { assert(0); } // Invalid column (table is expected to have only two columns
				}
				else {
					// Forward the event to the property grid event handler (raise widget event function)
					my_propertyGrid->raiseWidgetEvent(_senderId, (ak::core::eventType)_message);
				}
				
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::notifierPropertyGrid::notify()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::notifierPropertyGrid::notify()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::notifierPropertyGrid::notify()"); }

}