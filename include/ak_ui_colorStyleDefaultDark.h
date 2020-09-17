/*
 * ak_ui_colorStyleDefaultDark.h
 *
 *  Created on: April 08, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qstring.h>			// QString

// AK header
#include "ak_ui_colorStyle.h"	// base class and stylableObject type

namespace ak {
	namespace ui {

		//! @brief This class provides the default dark color style
		class colorStyleDefaultDark : public ak::ui::colorStyle {
		public:
			//! @brief Constructor
			colorStyleDefaultDark();
			
			//! @brief Deconstructor
			~colorStyleDefaultDark();

			// ##############################################################
			// Getter

			//! @brief Will return the name of this styleSheet
			virtual QString getColorStyleName(void) const;

			//! @brief Creates an returns a QStylesheet for all provided objects
			//! @param _items The items to create a stylesheet string for
			//! @throw sim::Exception if one of the image files needed does not exist
			virtual QString getStylesheet(
				const std::vector<styleableObject>	_items
			) const;

			//! @brief Creates an returns a QStylesheet for the provided object
			//! @param _object The object to create a stylesheet string for
			//! @throw sim::Exception if one of the image files needed does not exist
			virtual QString getStylesheet(
				styleableObject					_object
			) const;
		};
	} // namespace ui
} // namespace sim