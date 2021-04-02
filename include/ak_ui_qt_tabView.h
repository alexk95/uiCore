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
#include <qtabbar.h>

// AK header
#include <ak_ui_core_aWidget.h>
#include <ak_globalDataTypes.h>
#include <ak_ui_color.h>

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

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
				
			};

			// #######################################################################################################

			// #######################################################################################################

			// #######################################################################################################

			class UICORE_API_EXPORT tabViewTabBar : QTabBar {
				Q_OBJECT
			public:
				tabViewTabBar(color _defaultColor);
				virtual ~tabViewTabBar();

				virtual void paintEvent(QPaintEvent * _event) override;

				void clearColors(bool _repaint = true);

				void clearColor(int _index, bool _repaint = true);

				void addColor(int _index, color _color, bool _repaint = true);

				void setRepaintBlocked(bool _blocked = true) { my_repaintIsBlocked = _blocked; }

			private:
				color						my_defaultColor;

				std::map<int, color>		my_colors;

				bool						my_repaintIsBlocked;

				tabViewTabBar() = delete;
				tabViewTabBar(const tabViewTabBar&) = delete;
				tabViewTabBar & operator = (const tabViewTabBar&) = delete;

			};


		} // namespace qt
	} // namespace ui
} // namespace ak