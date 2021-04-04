/*
 * ak_ui_qt_tabView.h
 *
 *  Created on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <vector>

// Qt header
#include <qtabwidget.h>

// AK header
#include <ak_ui_core_aWidget.h>
#include <ak_globalDataTypes.h>

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;
		namespace widget { class tabView; }

		namespace qt{

			class UICORE_API_EXPORT tabView : public QTabWidget, public core::aWidget {
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
				virtual QWidget * widget(void) override;

				//! @brief Will set the objects color style
				//! @param _colorStyle The color style to set
				//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;
				
			private:
				friend ak::ui::widget::tabView;
			};

		} // namespace qt
	} // namespace ui
} // namespace ak