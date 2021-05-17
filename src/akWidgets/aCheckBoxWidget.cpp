/*
 *	File:		aCheckBoxWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aCheckBoxWidget.h>

// Qt header
#include <qevent.h>								// QKeyEvent

// C++ header
#include <string>

ak::aCheckBoxWidget::aCheckBoxWidget(
	QWidget *								_parent
) : QCheckBox(_parent), aWidget(otCheckBox) {}

ak::aCheckBoxWidget::aCheckBoxWidget(
	const QString &							_text,
	QWidget *								_parent
) : QCheckBox(_text, _parent), aWidget(otCheckBox) {}

ak::aCheckBoxWidget::~aCheckBoxWidget() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Event handling

void ak::aCheckBoxWidget::keyPressEvent(
	QKeyEvent *								_event
) {
	QCheckBox::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::aCheckBoxWidget::keyReleaseEvent(
	QKeyEvent *								_event
) {
	QCheckBox::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::aCheckBoxWidget::widget(void) { return this; }

void ak::aCheckBoxWidget::setColorStyle(
	const aColorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls | cafBackgroundColorControls, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls | cafBackgroundColorControls));
	}
}
