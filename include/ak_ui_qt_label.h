/*
 * ak_ui_qt_label.h
 *
 *  Created on: October 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// Qt header
#include <qlabel.h>					// Base class
#include <qstring.h>				// QString

// AK header
#include <ak_ui_core_aWidget.h>		// Base class

class QWidget;

namespace ak {
	namespace ui {
		namespace qt {

			class label : public QLabel, ui::core::aWidget {
			public:
				label(QWidget * _parent = nullptr);
				label(const QString & _text, QWidget * _parent = nullptr);
				virtual ~label();

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
				label() = delete;
				label(const label &) = delete;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak
