/*
 * ak_ui_qt_radioButton.cpp
 *
 *  Created on: April 22, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <ak_ui_qt_radioButton.h>	// Corresponding header
#include <ak_ui_colorStyle.h>		// ColorStyle

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::radioButton::radioButton(QWidget * _parent)
	: QRadioButton(_parent), ui::core::aWidget(ui::core::objectType::oRadioButton)
{}

ak::ui::qt::radioButton::radioButton(const QString & _text, QWidget * _parent)
	: QRadioButton(_text, _parent), ui::core::aWidget(ui::core::objectType::oRadioButton)
{}

ak::ui::qt::radioButton::~radioButton() { A_OBJECT_DESTROYING }

QWidget * ak::ui::qt::radioButton::widget(void) { return this; }

void ak::ui::qt::radioButton::setColorStyle(
	const ak::ui::colorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorTransparent));
	
}