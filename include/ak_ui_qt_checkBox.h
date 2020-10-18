/*
 * ak_ui_qt_checkBox.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// Qt header
#include <qcheckbox.h>				// base class
#include <qstring.h>				// QString

// AK header
#include <ak_ui_core_aWidget.h>		// base class
#include <ak_ui_core_aRestorable.h>	// base class

#include <rapidjson/document.h>		// RJSON document

#include <ak_globalDataTypes.h>

// Forward declaration
class QKeyEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			//! @brief This class combines the functionallity of a QCheckBox and a ak::object
			class checkBox : public QCheckBox, public ak::ui::core::aWidget, public ak::ui::core::aRestorable
			{
				Q_OBJECT
			public:
				//! @brief Constructor
				//! @param _parent The parent QWidget for this CheckBox
				checkBox(
					QWidget *								_parent = (QWidget *) nullptr
				);

				//! @brief Constructor
				//! @param _text The initial text of the CheckBox
				//! @param _parent The parent QWidget for this CheckBox
				checkBox(
					const QString &							_text,
					QWidget *								_parent = (QWidget *) nullptr
				);

				//! @brief Deconstructor
				virtual ~checkBox();

				// #######################################################################################################
				
				// Event handling

				//! @brief Emits a key pressend signal a key is pressed
				virtual void keyPressEvent(
					QKeyEvent *								_event
				) override;

				//! @brief Emits a key released signal a key is released
				virtual void keyReleaseEvent(
					QKeyEvent *								_event
				) override;

				// #######################################################################################################
				
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *					_colorStyle
				) override;

				//! @brief Will set the alias for this object
				//! @param _alias The alias to set
				//! @throw ak::Exception if the provided alias length is 0
				virtual void setAlias(
					const QString &							_alias
				) override;

				//! @brief Will create a rapidjson::Value representing this objects current state
				//! The value looks like this:
				//!	     { "Alias":"[ObjectAlias]","Type":"[ObjectType]","Settings":{...} }
				virtual void addObjectSettingsToValue(
					rapidjson::Value &						_array,
					rapidjson::Document::AllocatorType &	_allocator
				) override;

				//! @brief Will restore the settings from the provided JSON value which must have an object type
				//! The value looks like this:
				//!	     { { "[SettingsName]":"[SettingsValue]",... } }
				//! @param _settings The settings to restore
				virtual void restoreSettings(
					const rapidjson::Value &				_settings
				) override;

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);

			};
		} // namespace qt
	} // namespace ui
} // namespace ak
