/*
 * ak_ui_qt_action.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_action.h>			// corresponding header
#include <qwidget.h>
#include <ak_ui_colorStyle.h>

ak::ui::qt::action::action(QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{}

ak::ui::qt::action::action(const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{
	setText(_text);
}
ak::ui::qt::action::action(const QIcon & _icon, const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{
	setText(_text); setIcon(_icon);
}

ak::ui::qt::action::~action() {}

void ak::ui::qt::action::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {

	}
	else {

	}
}

QToolButton::ToolButtonPopupMode ak::ui::qt::action::popupMode(void) { return my_popupMode; }