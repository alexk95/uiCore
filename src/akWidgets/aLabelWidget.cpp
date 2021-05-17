/*
 *	File:		aLabelWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: October 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColorStyle.h>
#include <akWidgets/aLabelWidget.h>

ak::aLabelWidget::aLabelWidget(QWidget * _parent)
	: QLabel(_parent), aWidget(otLabel)
{}

ak::aLabelWidget::aLabelWidget(const QString & _text, QWidget * _parent)
	: QLabel(_text, _parent), aWidget(otLabel)
{}

ak::aLabelWidget::~aLabelWidget() { A_OBJECT_DESTROYING }

QWidget * ak::aLabelWidget::widget(void) { return this; }

void ak::aLabelWidget::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
			cafBackgroundColorTransparent, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
			cafBackgroundColorTransparent));
	}
}