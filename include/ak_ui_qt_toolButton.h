/*
 * ak_ui_qt_toolButton.h
 *
 *  Created on: November 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aWidget.h>			// Base class

// Qt header
#include <qtoolbutton.h>				// Base class
#include <qstring.h>					// QString
#include <qicon.h>						// QIcon

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			class action;

			class toolButton : public QToolButton, public ui::core::aWidget {
				Q_OBJECT
			public:
				toolButton();

				toolButton(
					const QString &				_text
				);
				
				toolButton(
					const QIcon &				_icon,
					const QString &				_text
				);

				virtual ~toolButton();

				// #######################################################################################################
				
				// Event handling

				//! @brief Emits a key pressend signal a key is pressed
				virtual void keyPressEvent(QKeyEvent * _event) override;

				//! @brief Emits a key released signal a key is released
				virtual void keyReleaseEvent(QKeyEvent * _event) override;

				// #######################################################################################################
				
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void) override { return this; }

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				// #######################################################################################################

				//! @brief Will return the action of this tool button
				action * getAction() const { return my_action; }

			signals:
				void keyPressed(QKeyEvent *);
				void keyReleased(QKeyEvent *);
				void clicked();

			private slots:
				void slotClicked();

			private:

				//! @brief Initializes the components of this toolButton
				void ini(void);

				qt::action *						my_action;

			};

		}
	}
}