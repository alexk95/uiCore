/*
 * ak_ui_qt_graphicsView.h
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// AK header
#include <ak_ui_core_aWidget.h>
#include <ak_globalDataTypes.h>

// Qt header
#include <qgraphicsview.h>		// Base class

class QResizeEvent;

namespace ak {
	namespace ui {
		namespace qt {

			class UICORE_API_EXPORT graphicsView : public QGraphicsView, ui::core::aWidget {
				Q_OBJECT
			public:
				graphicsView();
				virtual ~graphicsView();

				virtual QWidget * widget(void) override;

				virtual void setColorStyle(
					const ak::ui::colorStyle *					_colorStyle
				) override;

				virtual void resizeEvent(QResizeEvent *event) override;

				void setHeightForWidthActive(
					bool				_active
				);

			private:
				bool				my_heightForWidth;
			};

		}
	}
}