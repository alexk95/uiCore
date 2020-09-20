/*
 * ak_ui_qt_tabView.h
 *
 *  Created on: September 18, 2020
 *	Last modified on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <vector>

// Qt header
#include <qtabwidget.h>

// AK header
#include <ak_ui_core_aWidget.h>

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt{

			class tabView : public QTabWidget, public core::aWidget {
			public:
				
				//! @brief Constructor
				//! @param _colorStyle The initial color style to set
				//! @param _parent The parent widget
				tabView(
					colorStyle *			_colorStyle = (colorStyle *) nullptr,
					QWidget *				_parent = (QWidget *) nullptr
				);

				virtual ~tabView();

				// #######################################################################################################
				// Base class functions

				//! @brief Will return the widgets widget to display it
				virtual QWidget * widget(void);

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				);

			private:
				
			};

		} // namespace qt
	} // namespace ui
} // namespace ak