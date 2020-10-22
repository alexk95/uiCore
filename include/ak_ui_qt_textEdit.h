/*
 * ak_ui_qt_textEdit.h
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
#include <qtextedit.h>				// base class

// AK header
#include <ak_ui_core.h>				// Key type
#include <ak_ui_core_aWidget.h>		// base class
#include <ak_ui_core_aRestorable.h>	// base class

// Forward declaration
class QKeyEvent;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			// Forward declaration
			class dock;

			//! @brief This class combines the functionallity of a QTextEdit and a ak::ui::core::aWidget
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
				
				//! @brief Emits a returnPressed signal if the return key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				virtual void keyReleaseEvent(QKeyEvent *event) override;

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

				// #######################################################################################################

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
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);

			private slots:
				void slotChanged();

			private:
				bool							my_autoScrollToBottom;		//! If true, the textbox will automatically scroll down on text change
				
				textEdit(const textEdit &) = delete;
				textEdit & operator = (const textEdit &) = delete;

			};
		}
	}
}