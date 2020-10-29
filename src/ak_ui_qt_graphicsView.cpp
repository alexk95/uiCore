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

ak::ui::qt::graphicsView::graphicsView()
	: ui::core::aWidget(ui::core::objectType::oGraphicsView)
{}

ak::ui::qt::graphicsView::~graphicsView() { A_OBJECT_DESTROYING }

QWidget * ak::ui::qt::graphicsView::widget(void) { return this; }

void ak::ui::qt::graphicsView::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	assert(0); // Not implemented yet
}

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