/*
 * ak_ui_qt_aSpecialTabBar.cpp
 *
 *  Created on: April 04, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akGui/aSpecialTabBar.h>

#include <qstylepainter.h>
#include <QStyleOptionTab>

ak::aSpecialTabBar::aSpecialTabBar() : aObject{ otSpecialTabBar }, my_repaintIsBlocked{ false } {}

ak::aSpecialTabBar::~aSpecialTabBar() { A_OBJECT_DESTROYING }

void ak::aSpecialTabBar::paintEvent(QPaintEvent * _event) {

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

void ak::aSpecialTabBar::clearColors(bool _repaint) {
	my_colors.clear();
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::aSpecialTabBar::clearColor(int _index, bool _repaint) {
	my_colors.erase(_index);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::aSpecialTabBar::addColor(int _index, const aColor & _color, bool _repaint) {
	my_colors.insert_or_assign(_index, _color);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}