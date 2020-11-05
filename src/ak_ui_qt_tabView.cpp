/*
 * ak_ui_qt_tabView.cpp
 *
 *  Created on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_tabView.h>		// Corresponding header
#include <ak_exception.h>			// Error handling
#include <ak_ui_colorStyle.h>

// Qt header
#include <qwidget.h>				// QWidget

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::tabView::tabView(
	colorStyle *			_colorStyle,
	QWidget *				_parent
) : QTabWidget(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTabView, _colorStyle)
{}

ak::ui::qt::tabView::~tabView() { A_OBJECT_DESTROYING }

// #######################################################################################################
// Base class functions

QWidget * ak::ui::qt::tabView::widget(void) { return this; }

void ak::ui::qt::tabView::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	/*QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls, "QWidget{", "}\n"));*/
	QString sheet;
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls | TYPE_COLORAREA::caBackgroundColorControls //|
		//TYPE_COLORAREA::caBorderColorControls
		, "QTabWidget{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabWidget::pane{", "border: 1px solid #"));
	if (!sheet.isEmpty()) {
		QString Color(my_colorStyle->getHeaderBackgroundColor().toHexString(true));
		sheet.append(Color);
		sheet.append(";}\n");
	}
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabWidget::tab-bar{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caForegroundColorHeader,
		"QTabBar::tab{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorFocus | TYPE_COLORAREA::caForegroundColorFocus,
		"QTabBar::tab:hover{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorSelected | TYPE_COLORAREA::caForegroundColorSelected,
		"QTabBar::tab:selected QTabBar::tab::pressed{", "}"));
	this->setStyleSheet(sheet);
}