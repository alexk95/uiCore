/*
 *	File:		aCalendarWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: April 30, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <akWidgets/aCalendarWidget.h>
#include <akGui/aColorStyle.h>

// Qt header
#include <qtextformat.h>

ak::aCalendarWidget::aCalendarWidget()
	: aWidget(otNone), my_onlyCurrentMonthVisible(false)
{

}

ak::aCalendarWidget::~aCalendarWidget() {

}

// #############################################################################################################################

// Base class functions

QWidget * ak::aCalendarWidget::widget(void) { return this; }

void foo(QStringList & _lst, const QObjectList & _obj) {
	for (auto o : _obj) {
		_lst.push_back(o->objectName());
		//foo(_lst, o->children());
	}
}

void ak::aCalendarWidget::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	QStringList l;
	foo(l, children());

	my_colorStyle = _colorStyle;
	assert(my_colorStyle != nullptr);	// Nullptr provided

	QString sheet(my_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow,
		"#qt_aCalendarWidget_aCalendarWidgetview {", "}\n"));

	setStyleSheet(sheet);

	// Style the aCalendarWidget header since it can not be styled by using qss
	QBrush b(_colorStyle->getHeaderBackgroundColor().toQColor());
	QTextCharFormat format;
	format.setBackground(b);
	format.setForeground(_colorStyle->getHeaderForegroundColor().toQColor());
	QFont font;
	font.setWeight(14);
	font.setBold(false);
	format.setFont(font);
	setHeaderTextFormat(format);
}

// #############################################################################################################################

// Protected functions

void ak::aCalendarWidget::paintCell(
	QPainter *			_painter,
	const QRect &		_rect,
	const QDate &		_date
) const {
	if (my_onlyCurrentMonthVisible) {
		if (_date.month() == monthShown()) {
			QCalendarWidget::paintCell(_painter, _rect, _date);
		}
	}
	else { QCalendarWidget::paintCell(_painter, _rect, _date); }
}

// #############################################################################################################################	