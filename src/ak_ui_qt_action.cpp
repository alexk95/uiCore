/*
 * ak_ui_qt_action.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include "ak_ui_qt_action.h"			// corresponding header

ak::ui::qt::action::action(QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_parent), my_popupMode(_popupMode), ak::ui::core::aObject(ak::ui::core::objectType::oAction) {}
ak::ui::qt::action::action(const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_text, _parent), my_popupMode(_popupMode), ak::ui::core::aObject(ak::ui::core::objectType::oAction) {}
ak::ui::qt::action::action(const QIcon & _icon, const QString & _text, QToolButton::ToolButtonPopupMode _popupMode, QObject * _parent)
	: QAction(_icon, _text, _parent), my_popupMode(_popupMode), ak::ui::core::aObject(ak::ui::core::objectType::oAction) {}

ak::ui::qt::action::~action() {}

QToolButton::ToolButtonPopupMode ak::ui::qt::action::popupMode(void) { return my_popupMode; }