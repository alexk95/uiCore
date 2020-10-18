/*
 * ak_ui_qt_pushButton.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_pushButton.h>		// corresponding class
#include <ak_ui_colorStyle.h>			// colorStyle

// Qt header
#include <qevent.h>

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
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sPushButton));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::pushButton::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::pushButton::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::pushButton::setColorStyle()"); }
}
