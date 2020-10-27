/*
 * ak_ui_qt_pushButton.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_pushButton.h>		// corresponding class
#include <ak_ui_colorStyle.h>			// colorStyle

// Qt header
#include <qevent.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::pushButton::pushButton(QWidget * _parent)
: QPushButton(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oPushButton) {}
ak::ui::qt::pushButton::pushButton(const QString & _text, QWidget * _parent)
: QPushButton(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oPushButton) {}
ak::ui::qt::pushButton::pushButton(const QIcon & _icon, const QString & _text, QWidget * _parent)
: QPushButton(_icon, _text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oPushButton) {}

ak::ui::qt::pushButton::~pushButton() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::pushButton::keyPressEvent(QKeyEvent *_event)
{
	QPushButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::pushButton::keyReleaseEvent(QKeyEvent * _event) {
	QPushButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::pushButton::widget(void) { return this; }

void ak::ui::qt::pushButton::setColorStyle(
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
