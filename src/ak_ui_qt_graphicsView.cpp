/*
 * ak_ui_qt_graphicsView.cpp
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_ui_qt_graphicsView.h>
#include <qevent.h>

ak::ui::qt::graphicsView::graphicsView() {}

ak::ui::qt::graphicsView::~graphicsView() {}

void ak::ui::qt::graphicsView::resizeEvent(QResizeEvent *_event) {
	if (my_heightForWidth) {
		if (height() != width()) { resize(height(), height()); }
	}
	QGraphicsView::resizeEvent(_event);
}

void ak::ui::qt::graphicsView::setHeightForWidthActive(
	bool				_active
) {
	my_heightForWidth = _active;
}