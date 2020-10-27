/*
 * ak_ui_widget_colorEditButton.h
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qobject.h>						// base class
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

namespace ak {

	// Forward declaration
	class messenger;
	class uidManager;
	
	namespace ui {

		// Forward declaration
		class objectManager;
		class iconManager;
		class colorStyle;

		namespace qt {
			
			class pushButton; class graphicsView;

			class colorEditButton : public QObject, public ak::ui::core::aWidget {
				Q_OBJECT
			public:

				colorEditButton(
					const ak::ui::color &		_color,
					const QString &				_textOverride = QString(""),
					ak::ui::colorStyle *		_colorStyle = nullptr
				);

				virtual ~colorEditButton();
				
				// #############################################################################################################################
				// Base class functions

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
				void SetEnabled(
					bool						_enabled = true
				);

				//! @brief Will set the visible state of this colorEditButton
				void SetVisible(
					bool						_visible = true
				);

				//! @brief Will return the enabled state of this colorEditButton
				bool Enabled() const;

				//! @brief Returns the currently set color
				ak::ui::color color(void) const;

				//! @brief Will override the currently set text to the text provided
				//! @param _text The text to set
				void overrideText(
					const QString &				_text
				);

				//! @brief Will fill the background for the current parent widget
				//! @param _color The color to set
				void fillBackground(
					const ui::color &			_color
				);

			signals:
				void changed(void);

			private slots:
				void slotButtonClicked();

			private:

				ui::qt::graphicsView *			my_view;					//! Graphics view required to display the color
				ak::ui::qt::pushButton *		my_button;					//! The button to change the color
				QHBoxLayout *					my_layout;					//! The layout used to place the widgets

				ak::ui::color					my_color;					//! The currently set color
				
				QWidget *						my_widget;

				//! Block default constructor
				colorEditButton() = delete;

			};

		} // namespace widget
	} // namespace ui
} // namespace ak