/*
 * ak_ui_qt_table.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_table.h>			// Corresponding class
#include <ak_ui_colorStyle.h>		// colorStyle
#include <ak_exception.h>			// error handling

#include <qheaderview.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::table::table(QWidget * _parent)
: QTableWidget(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}
ak::ui::qt::table::table(int _rows, int _columns, QWidget * _parent)
: QTableWidget(_rows, _columns, _parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTable) {}

ak::ui::qt::table::~table() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Event handling

void ak::ui::qt::table::keyPressEvent(QKeyEvent *_event)
{
	QTableWidget::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::table::keyReleaseEvent(QKeyEvent * _event) {
	QTableWidget::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

QWidget * ak::ui::qt::table::widget(void) { return this; }

void ak::ui::qt::table::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	this->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls | TYPE_COLORAREA::caBackgroundColorAlternate));
	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader | TYPE_COLORAREA::caBackgroundColorHeader,
		"QHeaderView{border: none;", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader |
		TYPE_COLORAREA::caBackgroundColorHeader |
		TYPE_COLORAREA::caDefaultBorderHeader | TYPE_COLORAREA::caBorderColorHeader
		,
		"QHeaderView::section{", "}"));
	this->horizontalHeader()->setStyleSheet(sheet);
	this->verticalHeader()->setStyleSheet(sheet);
}
