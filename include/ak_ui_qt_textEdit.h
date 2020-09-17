/*
 * ak_ui_qt_textEdit.h
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// Qt header
#include <qtextedit.h>				// base class
//#include <qscrollbar.h>
#include <qevent.h>					// QKeyEvent

// AK header
#include "ak_ui_core_aWidget.h"		// base class

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			//! @brief This class combines the functionallity of a QTextEdit and a sim::object
			class textEdit : public QTextEdit, public ak::ui::core::aWidget
			{
				Q_OBJECT
			public:
				//! @brief Constructor
				//! @param _parent The parent QWidget for this textEdit
				textEdit(QWidget * _parent = (QWidget *) nullptr);

				//! @brief Constructor
				//! @param _text The initial text for this text edit
				//! @param _parent The parent QWidget for this textEdit
				textEdit(const QString & _text, QWidget * _parent = (QWidget *) nullptr);

				//! @brief Deconstructor
				virtual ~textEdit();

				// #######################################################################################################
				// Event handling
				// #######################################################################################################

				//! @brief Emits a returnPressed signal if the return key is pressed
				void keyPressEvent(QKeyEvent *event);

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw sim::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

				//! @brief Will set the auto scroll to bottom option.
				//! @param _autoScroll If true, the object will automatically scroll to the bottom on change
				void setAutoScrollToBottom(
					bool							_autoScroll
				);

				//! @brief Returns the auto scroll to bottom option
				bool autoScrollToBottom(void) const;

				//! @brief Will perform the auto scroll to bottom
				void performAutoScrollToBottom(void);

			signals:
				void returnPressed(void);

			private:
				bool							my_autoScrollToBottom;		//! If true, the textbox will automatically scroll down on text change
			};
		}
	}
}