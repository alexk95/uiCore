/*
 * ak_ui_qt_table.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_qt_table.h"			// Corresponding class
#include "ak_ui_colorStyle.h"		// colorStyle
#include "ak_exception.h"			// error handling

ak::ui::qt::table::table(QWidget * _parent)
: QTableWidget(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}
ak::ui::qt::table::table(int _rows, int _columns, QWidget * _parent)
: QTableWidget(_rows, _columns, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}

ak::ui::qt::table::~table() {}

// #######################################################################################################
// Event handling

void ak::ui::qt::table::keyPressEvent(QKeyEvent *_event)
{
	QTableWidget::keyPressEvent(_event);
	emit keyPressed(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::table::widget(void) { return this; }

void ak::ui::qt::table::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style", ak::Exception::exceptionType::Nullptr); }
		my_colorStyle = _colorStyle;
		this->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTable));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::table::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::table::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::table::setColorStyle()"); }
}
