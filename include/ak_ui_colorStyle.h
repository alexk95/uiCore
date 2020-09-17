/*
 * ak_ui_colorStyle.h
 *
 *  Created on: April 09, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <vector>				// vector<>

// Qt header
#include <qstring.h>			// QString

// AK header
#include "ak_ui_color.h"		// color
#include "ak_exception.h"		// error handling

namespace ak {
	namespace ui {

		//! @brief This class is used for storing color information used to paint the UI
		//! This class is an abstract class. It defines the main functions used by the UI_manager and all its childs
		//! This object provides the main foreground color and background color for the window and the controls
		class __declspec(dllexport) colorStyle {
		public:

			//! Contains all item types paintable with a colorStyle which can be created with the ui_manager
			enum styleableObject {
				sCheckBox,
				sComboBox,
				sComboButton,
				sDock,
				sLabel,
				sMainWindow,
				sMdiArea,
				sMdiSubArea,
				sMenu,
				sObjectHeader,
				sObjectTabBar,
				sObjectTabView,
				sProgressBar,
				sPushButton,
				sStatusBar,
				sTabBar,
				sTable,
				sTabWidget,
				sTabWidgetPage,
				sTextEdit,
				sToolBar,
				sToolButton,
				sTree
			};

			//! @brief Will return the name of this styleSheet
			virtual QString getColorStyleName(void) const = 0;

			//! @brief Creates and returns a QStylesheet for all provided objects
			virtual QString getStylesheet(
				const std::vector<styleableObject>	_objects
			) const = 0;

			//! @brief Creates an returns a QStylesheet for the provided object
			virtual QString getStylesheet(
				styleableObject					_object
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

			//! @brief Set the window main foreground color
			//! @param _color The color to set
			void setWindowMainForecolor(
				const ak::ui::color &		_color
			);
			
			//! @brief Set the window main background color
			//! @param _color The color to set
			void setWindowMainBackcolor(
				const ak::ui::color &		_color
			);
			
			//! @brief Set the control main foreground color
			//! @param _color The color to set
			void setControlsMainForecolor(
				const ak::ui::color &		_color
			);

			//! @brief Set the control main background color
			//! @param _color The color to set
			void setControlsMainBackcolor(
				const ak::ui::color &		_color
			);

			//! @brief Will set the error forecolor for the controls
			//! @param _color The color to set
			void setControlsErrorForecolor(
				const ak::ui::color &		_color
			);
		
			//! @brief Will add the provided directory to the colorStyle
			//! @param _directory The new search directory to add
			//! @throw ak::Exception if the provided directory does not exist
			void addDirectory(
				const QString &			_directory
			);

			//! @brief Will remove the provided directory from the colorStyle, returns false if directory does not exist before
			//! @param _throwException If true, an exception will be thrown if the provided directory does not exist
			//! @throw ak::Exception if the provided directory was not added before
			bool removeDirectory(
				const QString &			_directory,
				bool					_throwException = false
			);

			//! @brief Will set the provided directories to the colorStyle
			//! @param _directory The new search directory to add
			//! @throw ak::Exception if one of the provided directories deos not exist
			void setDirectories(
				const std::vector<QString> &	_directories
			);

		protected:

			//! @brief Returns the filename for the requested file by scanning the current directories
			//! Returns an empty String if the reqsted file does not exist in any of the subdirectories
			//! @param _fileName The filename only of the requsted file (subdirectories have to be provided)
			//! @throw ak::Exception if the file check failed
			QString getFilePath(
				const QString &			_fileName
			) const;

			ak::ui::color				my_windowMainForecolor;			//! The currently set window main foreground color
			ak::ui::color				my_windowMainBackcolor;			//! The currently set window main background color
			ak::ui::color				my_controlsMainForecolor;		//! The currently set controls main foreground color
			ak::ui::color				my_controlsMainBackcolor;		//! The currently set controls main background color
			ak::ui::color				my_controlsErrorForecolor;		//! The currently set controls error foreground color

			std::vector<QString>	my_directories;					//! Contains all directories to scan for incons
		};

	} // namespace ui
} // namespace ak