/*
 * ak_ui_qt_comboBox.cpp
 *
 *  Created on: April 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_comboBox.h>	// corresponding class
#include <ak_exception.h>		// error handling
#include <ak_ui_colorStyle.h>	// color style

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::comboBox::comboBox(QWidget * _parent)
	: QComboBox(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oComboBox) {}

ak::ui::qt::comboBox::~comboBox() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::ui::qt::comboBox::keyPressEvent(QKeyEvent *_event)
{
	QComboBox::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::comboBox::keyReleaseEvent(QKeyEvent * _event) {
	QComboBox::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::comboBox::widget(void) { return this; }

void ak::ui::qt::comboBox::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
			TYPE_COLORAREA::caBackgroundColorControls, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
			TYPE_COLORAREA::caBackgroundColorControls));
	}
}