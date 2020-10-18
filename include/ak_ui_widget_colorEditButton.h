/*
 * ak_ui_widget_colorEditButton.h
 *
 *  Created on: August 05, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// Qt header
#include <qstring.h>

// AK header
#include <ak_ui_core_aWidgetManager.h>		// base class
#include <ak_core.h>						// eventType
#include <ak_ui_core.h>						// objectType
#include <ak_globalDataTypes.h>				// UID and ID type
#include <ak_ui_color.h>					// color

// Forward declaration
class QHBoxLayout;
class QWidget;
class QGraphicsView;

namespace ak {

	// Forward declaration
	class messenger;
	class notifierColorEditButton;
	class uidManager;
	
	namespace ui {

		// Forward declaration
		class objectManager;
		class signalLinker;
		class iconManager;
		class colorStyle;
		namespace qt { class pushButton; }

		namespace widget {
			
			class colorEditButton : public ak::ui::core::aWidgetManager {
			public:

				colorEditButton(
					ak::messenger *				_messenger,
					ak::uidManager *			_uidManager,
					const ak::ui::color &		_color,
					const QString &				_textOverride = QString(""),
					ak::ui::colorStyle *		_colorStyle = nullptr
				);

				virtual ~colorEditButton();
				
				// #############################################################################################################################
				// ak::widget Functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *	_colorStyle
				) override;

				// #############################################################################################################################

				//! @brief Will set the widgets
				void setColor(
					const ak::ui::color &		_color
				);

				//! @brief Will set the enabled state of this colorEditButton
				void setEnabled(
					bool						_enabled = true
				);

				//! @brief Will set the visible state of this colorEditButton
				void setVisible(
					bool						_visible = true
				);

				//! @brief Will return the enabled state of this colorEditButton
				bool enabled() const;

				//! @brief Returns the currently set color
				ak::ui::color color(void) const;

				//! @brief Will override the currently set text to the text provided
				//! @param _text The text to set
				void overrideText(
					const QString &				_text
				);

				//! @brief Will perform actions for the provided widget event
				//! @param _sender The sender of the event
				//! @param _eventType The type of the event
				//! @param _info1 Additional information 1
				//! @param _info2 Additional information 2
				void widgetEvent(
					ak::UID						_sender,
					ak::core::eventType			_eventType,
					int							_info1 = 0,
					int							_info2 = 0
				);

			private:

				QGraphicsView *					my_view;					//! Graphics view required to display the color
				ak::ui::qt::pushButton *		my_button;					//! The button to change the color
				QHBoxLayout *					my_layout;					//! The layout used to place the widgets
				QWidget *						my_widget;					//! The central widget of the color edit button

				ak::messenger *					my_externalMessenger;		//! The external messenger used to send notifications
				ak::uidManager *				my_externalUidManager;		//! The external UID manager
				ak::notifierColorEditButton *	my_notifier;				//! The notifier used for the internal messaging system
				ak::ui::color					my_color;					//! The currently set color
				ui::signalLinker *				my_signalLinker;			//! Signal linker used to conenct the pushbutton callback

				//! Block default constructor
				colorEditButton() = delete;

			};

		} // namespace widget
	} // namespace ui
} // namespace ak