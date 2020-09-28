/*
 * ak_notiferPropertyGrid.h
 *
 *  Created on: March 29, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_notifier.h"			// base class
#include "ak_globalDataTypes.h"		// UID and ID type
#include "ak_core.h"				// message type

namespace ak {

	// Forward declaration
	namespace ui { namespace widget { class propertyGrid; } }

	//! @brief This notifier is used internally in the property grid
	//! It notifies the property grid that a change in the corresponding table occured
	class notifierPropertyGrid : public notifier {
	public:

		//! @brief Constructor
		//! @param _propertyGrid The property grid using this notifier
		//! @param _tableUid The UID of the table the property grid is managing 
		//! @throw ak::Exception if the provided propetyGrid was a nullptr
		notifierPropertyGrid(
			ak::ui::widget::propertyGrid *			_propertyGrid,
			ak::UID									_tableUid
		);

		//! @brief Will notify this receiver that a message was received from a sender.
		//! @param _senderId The id of the sender the message was sent from
		//! @param _event The event message
		//! @param _info1 Message addition 1
		//! @param _info2 Message addition 2
		//! @throw ak::Exception if any error occured on a function call
		virtual void notify(
			ak::UID									_senderId,
			ak::core::eventType						_event,
			int										_info1,
			int										_info2
		);

	protected:
		bool										my_isEnabled;			//! If true, this notifier is enabled

		ak::ui::widget::propertyGrid *				my_propertyGrid;		//! The property grid which is using this notifier
		ak::UID										my_tableUid;			//! The UID of the corresponding table
	};

} // namespace ak
