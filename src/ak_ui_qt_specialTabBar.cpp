/*
 * ak_ui_qt_specialTabBar.cpp
 *
 *  Created on: April 04, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_ui_qt_specialTabBar.h>

#include <qstylepainter.h>
#include <QStyleOptionTab>

ak::ui::qt::specialTabBar::specialTabBar()
	: core::aObject{ core::objectType::oSpecialTabBar }, my_repaintIsBlocked{ false }
{}

ak::ui::qt::specialTabBar::~specialTabBar() {}

void ak::ui::qt::specialTabBar::paintEvent(QPaintEvent * _event) {

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

void ak::ui::qt::specialTabBar::clearColors(bool _repaint) {
	my_colors.clear();
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::ui::qt::specialTabBar::clearColor(int _index, bool _repaint) {
	my_colors.erase(_index);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}

void ak::ui::qt::specialTabBar::addColor(int _index, color _color, bool _repaint) {
	my_colors.insert_or_assign(_index, _color);
	if (_repaint && !my_repaintIsBlocked) { repaint(); }
}