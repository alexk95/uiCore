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

ak::ui::qt::lineEdit::lineEdit(QWidget * _parent)
	: QLineEdit(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_autoScrollToBottom(false) {}
ak::ui::qt::lineEdit::lineEdit(const QString & _text, QWidget * _parent)
	: QLineEdit(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oLineEdit), my_autoScrollToBottom(false) {}

ak::ui::qt::lineEdit::~lineEdit() {}

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
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sLineEdit));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::lineEdit::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::lineEdit::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::lineEdit::setColorStyle()"); }
}

// #######################################################################################################

