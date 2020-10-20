/*
 * ak_ui_qt_action.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_action.h>			// corresponding header
#include <qwidget.h>
#include <ak_ui_colorStyle.h>

ak::ui::qt::action::action(QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QWidgetAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{}

ak::ui::qt::action::action(const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QWidgetAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{
	setText(_text);
}
ak::ui::qt::action::action(const QIcon & _icon, const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QWidgetAction(_parent), my_popupMode(_popupMode), ak::ui::core::aPaintable(ak::ui::core::objectType::oAction), my_widget(nullptr)
{
	setText(_text); setIcon(_icon);
}

ak::ui::qt::action::~action() {}

QWidget * ak::ui::qt::action::createWidget(QWidget *parent) {
	my_widget = QWidgetAction::createWidget(parent);
	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }
	return my_widget;
}
void ak::ui::qt::action::deleteWidget(QWidget *widget) {
	QWidgetAction::deleteWidget(widget);
	my_widget = nullptr;
}

void ak::ui::qt::action::setColorStyle(
	const ak::ui::colorStyle *					_colorStyle
) {
	if (my_widget == nullptr) { return; }
	my_widget->setStyleSheet(_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sWidgetAll));
}

QToolButton::ToolButtonPopupMode ak::ui::qt::action::popupMode(void) { return my_popupMode; }