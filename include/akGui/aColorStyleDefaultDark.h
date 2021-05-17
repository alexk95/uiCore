/*
 *	File:		aColorStyleDefaultDark.h
 *	Package:	akGui
 *
 *  Created on: April 08, 2020
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
#include <akCore/globalDataTypes.h>
#include <akGui/aColorStyle.h>			// Base class

namespace ak {

	//! @brief This class provides the default dark color style
	class UICORE_API_EXPORT aColorStyleDefaultDark : public aColorStyle {
	public:
		//! @brief Constructor
		aColorStyleDefaultDark();

		//! @brief Deconstructor
		virtual ~aColorStyleDefaultDark();

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
			colorAreaFlag			_colorAreas,
			const QString &			_prefix = QString(),
			const QString &			_suffix = QString()
		) const override;

	};
} // namespace ak