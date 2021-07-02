/*
 *	File:		aGraphicsWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akWidgets/aGraphicsWidget.h>
#include <qevent.h>

ak::aGraphicsWidget::aGraphicsWidget()
	: aWidget(otGraphicsView)
{}

ak::aGraphicsWidget::~aGraphicsWidget() { A_OBJECT_DESTROYING }

QWidget * ak::aGraphicsWidget::widget(void) { return this; }

void ak::aGraphicsWidget::setColorStyle(
	aColorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	assert(0); // Not implemented yet
}

void ak::aGraphicsWidget::resizeEvent(QResizeEvent *_event) {
	if (my_heightForWidth) {
		if (height() != width()) { resize(height(), height()); }
	}
	QGraphicsView::resizeEvent(_event);
}

void ak::aGraphicsWidget::setHeightForWidthActive(
	bool				_active
) {
	my_heightForWidth = _active;
}