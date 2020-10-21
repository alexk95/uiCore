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

#include <qgraphicsview.h>

class QResizeEvent;

namespace ak {
	namespace ui {
		namespace qt {

			class graphicsView : public QGraphicsView {
				Q_OBJECT
			public:
				graphicsView();
				virtual ~graphicsView();

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