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
	: QLineEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_isError(false), my_errorIsForeground(true) {}
ak::ui::qt::lineEdit::lineEdit(const QString & _text, QWidget * _parent)
	: QLineEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_isError(false), my_errorIsForeground(true) {}

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
	setErrorState(my_isError);
}

// #######################################################################################################

void ak::ui::qt::lineEdit::setErrorState(bool _error) {
	my_isError = _error;
	if (my_isError)
	{
		if (my_colorStyle != nullptr) {
			QString sheet{ "color: #" };
			if (my_errorIsForeground) { sheet.append(my_colorStyle->getControlsErrorFrontForegroundColor().toHexString()); }
			else { sheet.append(my_colorStyle->getControlsErrorBackForegroundColor().toHexString()); }
			sheet.append("; background-color: #");
			if (my_errorIsForeground) { sheet.append(my_colorStyle->getControlsMainBackgroundColor().toHexString()); }
			else { sheet.append(my_colorStyle->getControlsErrorBackBackgroundColor().toHexString()); }
			sheet.append("; border: 1px solid #").append(my_colorStyle->getControlsBorderColor().toHexString(true));
			sheet.append(";}");
			setStyleSheet(sheet);
		}
		else if (my_errorIsForeground) {
			setStyleSheet("color: #ff0000;");
		}
		else {
			setStyleSheet("color: #000000; background-color: #ff0000;");
		}

	}
	else if (my_colorStyle != nullptr)
	{
		QString Color = my_colorStyle->getControlsBorderColor().toHexString(true);
		setStyleSheet(my_colorStyle->toStyleSheet(core::caForegroundColorControls |
			core::caBackgroundColorControls | core::caBorderColorControls, "", "border: 1px solid #" + Color + ";"));
	}
	else {
		setStyleSheet("");
	}
}

void ak::ui::qt::lineEdit::setErrorStateIsForeground(bool _isForeground) {
	if (_isForeground == my_errorIsForeground) { return; }	// Ignore if did not change
	my_errorIsForeground = _isForeground;
	setErrorState(my_isError);		// Repaint
}