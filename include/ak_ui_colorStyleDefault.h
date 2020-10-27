/*
 * ak_ui_colorStyleDefault.h
 *
 *  Created on: April 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // Qt header
#include <qstring.h>			// QString

// AK header
#include <ak_ui_colorStyle.h>	// base class and stylableObject type

namespace ak {
	namespace ui {

		//! @brief This class provides the default bright color style
		class colorStyleDefault : public ak::ui::colorStyle {
		public:
			//! @brief Constructor
			colorStyleDefault();

			//! @brief Deconstructor
			virtual ~colorStyleDefault();

			// ##############################################################
			// Getter

			//! @brief Will return the name of this styleSheet
			virtual QString getColorStyleName(void) const override;

			//! @brief Will return a stylesheet that can be applied
			//! If the color areas foreground and background are provided the stylesheet will look something like this:
			//!			color:#00ffd0; background-color:#ffffff;
			//! A prefix can be added to specify the widget
			//! When _prefix = "QLabel{" then the stylehseet will now look like this
			//!			QLabel{color:#00ffd0; background-color:#ffffff;
			//! Now a suffix would make sense.
			//! When _suffix = "}\n" then the stylesheet will now look like this
			//!			QLabel{color:#00ffd0; background-color:#ffffff;}\n
			//!
			//! @param _colorAreas The ares to add to the stylesheet
			//! @param _prefix The prefix to add to the stylesheet
			//! @param _suffix The suffix to add to the stylesheet
			virtual QString toStyleSheet(
				ui::core::colorAreaFlag			_colorAreas,
				const QString &					_prefix = QString(),
				const QString &					_suffix = QString()
			) const override;
		};
	} // namespace ui
} // namespace ak