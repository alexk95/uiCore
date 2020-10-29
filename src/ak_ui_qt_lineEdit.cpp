/*
 * ak_ui_qt_lineEdit.cpp
 *
 *  Created on: October 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_ui_qt_lineEdit.h>		// corresponding header
#include <ak_exception.h>			// Error handling
#include <ak_ui_colorStyle.h>		// colorStyle

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::lineEdit::lineEdit(QWidget * _parent)
	: QLineEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_autoScrollToBottom(false) {}
ak::ui::qt::lineEdit::lineEdit(const QString & _text, QWidget * _parent)
	: QLineEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_autoScrollToBottom(false) {}

ak::ui::qt::lineEdit::~lineEdit() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::ui::qt::lineEdit::keyPressEvent(QKeyEvent *_event)
{
	QLineEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::ui::qt::lineEdit::keyReleaseEvent(QKeyEvent *_event) {
	QLineEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::lineEdit::widget(void) { return this; }

void ak::ui::qt::lineEdit::setColorStyle(
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

// #######################################################################################################

