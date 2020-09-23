/*
 * ak_ui_qt_textEdit.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_qt_textEdit.h"	// corresponding class
#include "ak_exception.h"		// error handling
#include "ak_ui_colorStyle.h"	// colorStyle
#include <qscrollbar.h>

ak::ui::qt::textEdit::textEdit(QWidget * _parent)
: QTextEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(true) {}
ak::ui::qt::textEdit::textEdit(const QString & _text, QWidget * _parent)
: QTextEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTextEdit), my_autoScrollToBottom(true) {}

ak::ui::qt::textEdit::~textEdit() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::textEdit::keyPressEvent(QKeyEvent *_event)
{
	QTextEdit::keyPressEvent(_event);
	emit keyPressed(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::textEdit::widget(void) { return this; }

void ak::ui::qt::textEdit::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTextEdit));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::textEdit::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::textEdit::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::textEdit::setColorStyle()"); }
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