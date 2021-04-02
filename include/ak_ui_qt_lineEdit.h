/*
 * ak_ui_qt_lineEdit.h
 *
 *  Created on: March 10, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_globalDataTypes.h>
#include <ak_ui_core_aWidget.h>		// Base class

// Qt header
#include <qlineedit.h>				// Base class

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
			class UICORE_API_EXPORT lineEdit : public QLineEdit, public ak::ui::core::aWidget
			{
				Q_OBJECT
			public:
				//! @brief Constructor
				//! @param _parent The parent QWidget for this textEdit
				lineEdit(QWidget * _parent = (QWidget *) nullptr);

				//! @brief Constructor
				//! @param _text The initial text for this text edit
				//! @param _parent The parent QWidget for this textEdit
				lineEdit(const QString & _text, QWidget * _parent = (QWidget *) nullptr);

				//! @brief Deconstructor
				virtual ~lineEdit();

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

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);

			private:
				bool							my_autoScrollToBottom;		//! If true, the textbox will automatically scroll down on text change

				lineEdit(const lineEdit &) = delete;
				lineEdit & operator = (const lineEdit &) = delete;

			};
		}
	}
}