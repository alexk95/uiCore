/*
 * ak_ui_qt_label.cpp
 *
 *  Created on: October 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_label.h>			// Corresponding header
#include <ak_ui_colorStyle.h>		// ColorStyle

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::label::label(QWidget * _parent)
	: QLabel(_parent), ui::core::aWidget(ui::core::objectType::oLabel)
{}

ak::ui::qt::label::label(const QString & _text, QWidget * _parent)
	: QLabel(_text, _parent), ui::core::aWidget(ui::core::objectType::oLabel)
{}

ak::ui::qt::label::~label() { A_OBJECT_DESTROYING }

QWidget * ak::ui::qt::label::widget(void) { return this; }

void ak::ui::qt::label::setColorStyle(
	const ak::ui::colorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
			TYPE_COLORAREA::caBackgroundColorTransparent, "#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
			TYPE_COLORAREA::caBackgroundColorTransparent));
	}
}