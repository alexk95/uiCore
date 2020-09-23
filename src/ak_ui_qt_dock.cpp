/*
 * ak_ui_qt_dock.cpp
 *
 *  Created on: August 10, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <ak_ui_qt_dock.h>			// corresponding class
#include <ak_exception.h>			// error handling
#include <ak_ui_colorStyle.h>		// colorStyle
#include <qevent.h>					// QKeyEvent

ak::ui::qt::dock::dock(
	const QString &				_title,
	ak::ui::colorStyle *		_colorStyle,
	QWidget *					_parent,
	Qt::WindowFlags				_flags
) 
	: ak::ui::core::aWidget(ak::ui::core::objectType::oDock, _colorStyle),
	QDockWidget(_title, _parent, _flags)
{}

ak::ui::qt::dock::~dock() {}

// #######################################################################################################

QWidget * ak::ui::qt::dock::widget(void) { return this; }

void ak::ui::qt::dock::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check color style"); }
		my_colorStyle = _colorStyle;
		setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sDock));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::dock::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::dock::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::dock::setColorStyle()"); }
}