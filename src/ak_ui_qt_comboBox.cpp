/*
 * ak_ui_qt_comboBox.cpp
 *
 *  Created on: April 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_comboBox.h>	// corresponding class
#include <ak_exception.h>		// error handling
#include <ak_ui_colorStyle.h>	// color style

ak::ui::qt::comboBox::comboBox(QWidget * _parent)
	: QComboBox(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oComboBox) {}

ak::ui::qt::comboBox::~comboBox() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::comboBox::keyPressEvent(QKeyEvent *_event)
{
	QComboBox::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::comboBox::keyReleaseEvent(QKeyEvent * _event) {
	QComboBox::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::comboBox::widget(void) { return this; }

void ak::ui::qt::comboBox::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // Nullptr provided
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sComboBox));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::comboBox::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::comboBox::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::comboBox::setColorStyle()"); }
}