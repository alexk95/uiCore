/*
 * ak_ui_qt_checkBox.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include "ak_ui_qt_checkBox.h"			// corresponding header
#include "ak_ui_colorStyle.h"			// colorStyle
#include "ak_exception.h"				// error handling

ak::ui::qt::checkBox::checkBox(QWidget * _parent)
: QCheckBox(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oCheckBox) {}
ak::ui::qt::checkBox::checkBox(const QString & _text, QWidget * _parent)
: QCheckBox(_text, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oCheckBox) {}

ak::ui::qt::checkBox::~checkBox() {}

QWidget * ak::ui::qt::checkBox::widget(void) { return this; }

void ak::ui::qt::checkBox::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try { 
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sCheckBox));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::checkBox::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::checkBox::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::checkBox::setColorStyle()"); }
}