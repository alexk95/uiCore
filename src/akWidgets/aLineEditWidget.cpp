/*
 *	File:		aLineEditWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: October 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <akCore/aException.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aLineEditWidget.h>

ak::aLineEditWidget::aLineEditWidget(QWidget * _parent)
	: QLineEdit(_parent), aWidget(otLineEdit), my_isError(false), my_errorIsForeground(true) {
	connect(this, &QLineEdit::editingFinished, this, &aLineEditWidget::slotEditingFinished);
}
ak::aLineEditWidget::aLineEditWidget(const QString & _text, QWidget * _parent)
	: QLineEdit(_text, _parent), aWidget(otLineEdit), my_isError(false), my_errorIsForeground(true), my_text(_text) {
	connect(this, &QLineEdit::editingFinished, this, &aLineEditWidget::slotEditingFinished);
}

ak::aLineEditWidget::~aLineEditWidget() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::aLineEditWidget::keyPressEvent(QKeyEvent *_event)
{
	QLineEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::aLineEditWidget::keyReleaseEvent(QKeyEvent *_event) {
	QLineEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::aLineEditWidget::widget(void) { return this; }

void ak::aLineEditWidget::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	setErrorState(my_isError);
}

// #######################################################################################################

void ak::aLineEditWidget::setErrorState(bool _error) {
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
		setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
			cafBackgroundColorControls | cafBorderColorControls, "", "border: 1px solid #" + Color + ";"));
	}
	else {
		setStyleSheet("");
	}
}

void ak::aLineEditWidget::setErrorStateIsForeground(bool _isForeground) {
	if (_isForeground == my_errorIsForeground) { return; }	// Ignore if did not change
	my_errorIsForeground = _isForeground;
	setErrorState(my_isError);		// Repaint
}

// #######################################################################################################

// Slots

void ak::aLineEditWidget::slotEditingFinished() {
	if (text() != my_text) { my_text = text(); emit finishedChanges(); }
}