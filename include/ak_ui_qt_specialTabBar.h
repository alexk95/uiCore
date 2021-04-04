/*
 * ak_ui_qt_specialTabBar.h
 *
 *  Created on: April 04, 2021
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
#include <qtabbar.h>

// AK header
#include <ak_ui_core_aObject.h>
#include <ak_globalDataTypes.h>
#include <ak_ui_color.h>

// Forward declaration
class QWidget;

namespace ak {
	namespace ui {

		// Forward declaration
		class colorStyle;

		namespace qt {

			class UICORE_API_EXPORT specialTabBar : public QTabBar, public core::aObject {
				Q_OBJECT
			public:
				specialTabBar();
				virtual ~specialTabBar();

				virtual void paintEvent(QPaintEvent * _event) override;

				void clearColors(bool _repaint = true);

				void clearColor(int _index, bool _repaint = true);

				void addColor(int _index, color _color, bool _repaint = true);

				void setRepaintBlocked(bool _blocked = true) { my_repaintIsBlocked = _blocked; }

			private:
				
				std::map<int, color>		my_colors;

				bool						my_repaintIsBlocked;

				specialTabBar(const specialTabBar&) = delete;
				specialTabBar & operator = (const specialTabBar&) = delete;

			};


		} // namespace qt
	} // namespace ui
} // namespace ak