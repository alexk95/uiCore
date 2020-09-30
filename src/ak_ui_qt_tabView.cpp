/*
 * ak_ui_qt_tabView.cpp
 *
 *  Created on: September 18, 2020
 *	Last modified on: September 18, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_qt_tabView.h>		// Corresponding header
#include <ak_exception.h>			// Error handling
#include <ak_ui_colorStyle.h>

// Qt header
#include <qwidget.h>				// QWidget

ak::ui::qt::tabView::tabView(
	colorStyle *			_colorStyle,
	QWidget *				_parent
) : QTabWidget(_parent), ak::ui::core::aWidget(ak::ui::core::objectType::oTabView, _colorStyle)
{}

ak::ui::qt::tabView::~tabView() {}

// #######################################################################################################
// Base class functions

QWidget * ak::ui::qt::tabView::widget(void) { return this; }

void ak::ui::qt::tabView::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style"); }
		my_colorStyle = _colorStyle;
		setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTabWidget));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::tabView::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::tabView::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::tabView::setColorStyle()"); }
}