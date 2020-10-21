/*
 * ak_ui_qt_pushButton.h
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

 // Qt header
#include <qpushbutton.h>			// base class
#include <qstring>					// QString
#include <qicon.h>					// QIcon

// AK header
#include <ak_ui_core_aWidget.h>		// base class

// Forward declaration
class QKeyEvent;

namespace ak {
	namespace ui {
		
		// Forward declaration
		class colorStyle;

		namespace qt {

			//! @brief This class combines the functionallity of a QPushButton and a ak::ui::core::aWidget
			class pushButton : public QPushButton, public ak::ui::core::aWidget
			{
				Q_OBJECT
			public:

				//! @brief Constructor
				//! @param _parent The parent QWidget for this push button
				pushButton(QWidget * _parent = (QWidget *) nullptr);

				//! @brief Constructor
				//! @param _text The initial text of this push button
				//! @param _parent The parent QWidget for this push button
				pushButton(const QString & _text, QWidget * _parent = (QWidget *) nullptr);

				//! @brief Constructor
				//! @param _icon The initial icon of this push button
				//! @param _text The initial text of this push button
				//! @param _parent The parent QWidget for this push button
				pushButton(const QIcon & _icon, const QString & _text, QWidget * _parent = (QWidget *) nullptr);

				//! @brief Deconstructor
				virtual ~pushButton();

				// #######################################################################################################
				// Event handling

				//! @brief Emits a key pressend signal a key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				//! @brief Emits a key released signal a key is released
				virtual void keyReleaseEvent(QKeyEvent * _event) override;

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;
				
			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);

			};
		
		} // namespace qt
	} // namespace ui
} // namespace ak