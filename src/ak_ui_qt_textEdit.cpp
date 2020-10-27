/*
 * ak_ui_qt_textEdit.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_textEdit.h>	// corresponding class
#include <ak_exception.h>		// error handling
#include <ak_ui_colorStyle.h>	// colorStyle

// Qt header
#include <qscrollbar.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::textEdit::textEdit(QWidget * _parent)
: QTextEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}
ak::ui::qt::textEdit::textEdit(const QString & _text, QWidget * _parent)
: QTextEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(false)
{
	connect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

ak::ui::qt::textEdit::~textEdit()
{
	disconnect(this, SIGNAL(textChanged()), this, SLOT(slotChanged()));
}

// #######################################################################################################
// Event handling

void ak::ui::qt::textEdit::keyPressEvent(QKeyEvent *_event)
{
	QTextEdit::keyPressEvent(_event); emit keyPressed(_event);
}

void ak::ui::qt::textEdit::keyReleaseEvent(QKeyEvent *_event) {
	QTextEdit::keyReleaseEvent(_event); emit keyReleased(_event);
}

void ak::ui::qt::textEdit::slotChanged() {
	performAutoScrollToBottom();
}

// #######################################################################################################

QWidget * ak::ui::qt::textEdit::widget(void) { return this; }

void ak::ui::qt::textEdit::setColorStyle(
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

void ak::ui::qt::textEdit::setAutoScrollToBottom(
	bool							_autoScroll
) { my_autoScrollToBottom = _autoScroll; }

bool ak::ui::qt::textEdit::autoScrollToBottom(void) const { return my_autoScrollToBottom; }

void ak::ui::qt::textEdit::performAutoScrollToBottom(void) {
	if (my_autoScrollToBottom) {
		//ensureCursorVisible();
		QScrollBar * bar = verticalScrollBar();
		if (bar->isVisible()) { bar->setValue(bar->maximum());}
	}
}