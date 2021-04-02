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
#include <qstylepainter.h>
#include <QStyleOptionTab>

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
		"QTabBar::tab:selected{", "}"));
	
	this->setStyleSheet(sheet);
}

// #######################################################################################################

// #######################################################################################################

// #######################################################################################################

ak::ui::qt::tabViewTabBar::tabViewTabBar(color _defaultColor)
	: my_defaultColor{ _defaultColor }, my_repaintIsBlocked{ false }
{}

ak::ui::qt::tabViewTabBar::~tabViewTabBar() {}

void ak::ui::qt::tabViewTabBar::paintEvent(QPaintEvent * _event) {

	QStylePainter painter(this);
	QStyleOptionTab opt;

	for (int i = 0; i < count(); i++)
	{
		initStyleOption(&opt, i);
		auto itm = my_colors.find(i);
		if (itm != my_colors.end()) {
			opt.palette.setColor(QPalette::Button, itm->second.toQColor());
		}
		painter.drawControl(QStyle::CE_TabBarTabShape, opt);
		painter.drawControl(QStyle::CE_TabBarTabLabel, opt);
	}

}

void ak::ui::qt::tabViewTabBar::clearColors(bool _repaint) {
	my_colors.clear();
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::ui::qt::tabViewTabBar::clearColor(int _index, bool _repaint) {
	my_colors.erase(_index);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::ui::qt::tabViewTabBar::addColor(int _index, color _color, bool _repaint) {
	my_colors.insert_or_assign(_index, _color);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}