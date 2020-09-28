/*
 * ak_notifierTreeFilter.h
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include "ak_core.h"				// messageType
#include "ak_uidMangager.h"			// UID type
#include "ak_notifier.h"			// base class

namespace ak {

	namespace ui { namespace widget { class tree; } }

	//! @brief This notifier is used to notify a tree about changes in the corresponding filter textedit
	class notifierTreeFilter : public ak::notifier {
	public:

		//! @brief Constructor, sets the tree which is using this notifier
		//! @param _tree The tree using this notifier
		//! @throw ak::Exception if the provided tree was a nullptr
		notifierTreeFilter(
			ak::ui::widget::tree *		_tree
		);

		//! @brief Deconstructor
		virtual ~notifierTreeFilter();

		//! @brief Will call the callback function with the provided parameters
		//! @param _senderId The sender ID the message was send from
		//! @param _event The event message
		//! @param _info1 Message addition 1
		//! @param _info2 Message addition 2
		//! @throw ak::Exception on any error during the filter handler functions in the tree
		virtual void notify(
			ak::UID						_senderId,
			ak::core::eventType			_event,
			int							_info1,
			int							_info2
		) override;

	private:
		ak::ui::widget::tree *			my_tree;			//! The tree which is using this notifier

	};


} // namespace ak
