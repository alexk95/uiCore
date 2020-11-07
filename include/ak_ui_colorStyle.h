/*
 * ak_ui_colorStyle.h
 *
 *  Created on: April 09, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <vector>				// vector<>

// Qt header
#include <qstring.h>			// QString

// AK header
#include <ak_ui_color.h>		// color
#include <ak_exception.h>		// error handling
#include <ak_ui_core.h>			// color area flag

namespace ak {
	namespace ui {

		//! @brief This class is used for storing color information used to paint the UI
		//! This class is an abstract class. It defines the main functions used by the UI_manager and all its childs
		//! This object provides the main foreground color and background color for the window and the controls
		class UICORE_API colorStyle {
		public:

			//! @brief Will return the name of this styleSheet
			virtual QString getColorStyleName(void) const = 0;

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
			) const = 0;

			//! @brief Returns the currently set window main foreground color
			ak::ui::color getWindowMainForecolor(void) const;
			
			//! @brief Returns the currently set window main background color
			ak::ui::color getWindowMainBackcolor(void) const;
			
			//! @brief Returns the currently set controls main foreground color
			ak::ui::color getControlsMainForecolor(void) const;
			
			//! @brief Returns the currently set controls main background color
			ak::ui::color getControlsMainBackcolor(void) const;
			
			//! @brief Returns the currently set controls mian error foreground color
			ak::ui::color getControlsErrorForecolor(void) const;

			//! @brief Returns the currently set controls focus background color
			ak::ui::color getControlsFocusedBackColor(void) const;

			//! @brief Returns the currently set controls focus color
			ak::ui::color getControlsFocusedForeColor(void) const;

			//! @brief Returns the currently set controls pressed color
			ak::ui::color getControlsPressedForeColor(void) const { return my_controlsPressedForeColor; }

			//! @brief Returns the currently set controls pressed color
			ak::ui::color getControlsPressedBackColor(void) const { return my_controlsPressedBackColor; }

			//! @brief Returns the currently set alternate fore color
			ak::ui::color getAlternateForeColor(void) const;

			//! @brief Returns the currently set alternate back color
			ak::ui::color getAlternateBackColor(void) const;

			//! @brief Returns the currently set header foreground color
			ak::ui::color getHeaderForegroundColor(void) const;

			//! @brief Returns the currently set header background color
			ak::ui::color getHeaderBackgroundColor(void) const;

			//! @brief Returns the currently set controls border color
			ui::color getControlsBorderColor(void) const { return my_controlsBorderColor; }

			//! @brief Will set the provided directories to the colorStyle
			//! @param _directory The new search directory to add
			void setDirectories(
				const std::vector<QString> &	_directories
			);

			//! @brief Returns the filename for the requested file by searching the set directories
			//! @param _fileName The filename only of the requsted file (subdirectories have to be provided)
			//! @throw ak::Exception if the file does not exist (with exception type = FileNotFound)
			virtual QString getFilePath(
				const QString &			_fileName
			) const;

		protected:

			ak::ui::color				my_windowMainForecolor;			//! The currently set window main foreground color
			ak::ui::color				my_windowMainBackcolor;			//! The currently set window main background color
			ak::ui::color				my_controlsMainForecolor;		//! The currently set controls main foreground color
			ak::ui::color				my_controlsMainBackcolor;		//! The currently set controls main background color
			ak::ui::color				my_controlsErrorForecolor;		//! The currently set controls error foreground color
			ak::ui::color				my_controlsFocusBackColor;			//! The currently set controls focus back color
			ak::ui::color				my_controlsFocusForeColor;			//! The currently set controls focus fore color
			ak::ui::color				my_controlsPressedBackColor;		//! The currently set controls pressed color
			ak::ui::color				my_controlsPressedForeColor;		//! The currently set controls pressed color
			ak::ui::color				my_alternateBackColor;
			ak::ui::color				my_alternateForeColor;
			ak::ui::color				my_headerBackColor;
			ak::ui::color				my_headerForeColor;
			ak::ui::color				my_headerBorderColor;
			ak::ui::color				my_controlsBorderColor;

			std::vector<QString>		my_directories;					//! Contains all directories to scan for incons
		};

	} // namespace ui
} // namespace ak