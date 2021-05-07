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
		class UICORE_API_EXPORT colorStyle {
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
				core::colorAreaFlag				_colorAreas,
				const QString &					_prefix = QString(),
				const QString &					_suffix = QString()
			) const = 0;

			//! @brief Returns the currently set window main foreground color
			color getWindowMainForegroundColor(void) const { return my_windowMainForecolor; }
			
			//! @brief Returns the currently set window main background color
			color getWindowMainBackgroundColor(void) const { return my_windowMainBackcolor; }
			
			//! @brief Returns the currently set controls main foreground color
			color getControlsMainForegroundColor(void) const { return my_controlsMainForecolor; }
			
			//! @brief Returns the currently set controls main background color
			color getControlsMainBackgroundColor(void) const { return my_controlsMainBackcolor; }
			
			//! @brief Returns the currently set controls mian error foreground color
			color getControlsErrorFrontForegroundColor(void) const { return my_controlsErrorFrontForecolor; }

			//! @brief Returns the currently set controls mian error foreground color
			color getControlsErrorBackForegroundColor(void) const { return my_controlsErrorBackForecolor; }

			//! @brief Returns the currently set controls mian error foreground color
			color getControlsErrorBackBackgroundColor(void) const { return my_controlsErrorBackBackcolor; }

			//! @brief Returns the currently set controls focus background color
			color getControlsFocusedBackgroundColor(void) const { return my_controlsFocusBackColor; }

			//! @brief Returns the currently set controls focus color
			color getControlsFocusedForegroundColor(void) const { return my_controlsFocusForeColor; }

			//! @brief Returns the currently set controls pressed color
			color getControlsPressedForegroundColor(void) const { return my_controlsPressedForeColor; }

			//! @brief Returns the currently set controls pressed color
			color getControlsPressedBackgroundColor(void) const { return my_controlsPressedBackColor; }

			//! @brief Returns the currently set alternate fore color
			color getAlternateForegroundColor(void) const { return my_alternateForeColor; }

			//! @brief Returns the currently set alternate back color
			color getAlternateBackgroundColor(void) const { return my_alternateBackColor; }

			//! @brief Returns the currently set header foreground color
			color getHeaderForegroundColor(void) const { return my_headerForeColor; }

			//! @brief Returns the currently set header background color
			color getHeaderBackgroundColor(void) const { return my_headerBackColor; }

			//! @brief Returns the currently set controls border color
			color getControlsBorderColor(void) const { return my_controlsBorderColor; }

			//! @brief Will set the provided directories to the colorStyle
			//! @param _directory The new search directory to add
			void setDirectories(
				const std::vector<QString> &	_directories
			) { my_directories = _directories; }

			//! @brief Returns the filename for the requested file by searching the set directories
			//! @param _fileName The filename only of the requsted file (subdirectories have to be provided)
			//! @throw ak::Exception if the file does not exist (with exception type = FileNotFound)
			virtual QString getFilePath(
				const QString &			_fileName
			) const;

		protected:

			color				my_windowMainForecolor;			//! The currently set window main foreground color
			color				my_windowMainBackcolor;			//! The currently set window main background color

			color				my_controlsMainForecolor;		//! The currently set controls main foreground color
			color				my_controlsMainBackcolor;		//! The currently set controls main background color

			color				my_controlsErrorFrontForecolor;	//! The currently set controls error front foreground color
			color				my_controlsErrorBackForecolor;	//! The currently set controls error back foreground color
			color				my_controlsErrorBackBackcolor;	//! The currently set controls error back background color
			color				my_controlsFocusBackColor;		//! The currently set controls focus back color
			color				my_controlsFocusForeColor;		//! The currently set controls focus fore color
			color				my_controlsPressedBackColor;	//! The currently set controls pressed color
			color				my_controlsPressedForeColor;	//! The currently set controls pressed color
			color				my_controlsBorderColor;

			color				my_alternateBackColor;
			color				my_alternateForeColor;

			color				my_headerBackColor;
			color				my_headerForeColor;
			color				my_headerBorderColor;

			std::vector<QString>		my_directories;					//! Contains all directories to scan for incons
		};

	} // namespace ui
} // namespace ak