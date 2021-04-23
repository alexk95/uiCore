/*
 * ak_ui_qt_radioButton.h
 *
 *  Created on: April 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qradiobutton.h>			// Base class
#include <qstring.h>				// QString

// AK header
#include <ak_ui_core_aWidget.h>		// Base class
#include <ak_globalDataTypes.h>

namespace ak {
	namespace ui {

		class colorStyle;

		namespace qt {

			class UICORE_API_EXPORT radioButton : public QRadioButton, public ui::core::aWidget
			{
				Q_OBJECT
			public:
				radioButton(QWidget * _parent = nullptr);

				radioButton(const QString & _text, QWidget * _parent = nullptr);

				virtual ~radioButton();

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

			private:
				radioButton(const radioButton &) = delete;
				radioButton & operator = (const radioButton &) = delete;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak
