/*
 * ak_ui_qt_comboButtonItem.cpp
 *
 *  Created on: March 19, 2020
 *	Last modified on: August 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_comboButtonItem.h>		// corresponding class
#include <ak_ui_core.h>						// objectType

ak::ui::qt::comboButtonItem::comboButtonItem(
	QObject * _parent
) : QAction(_parent), ak::ui::core::aObject(ak::ui::core::objectType::oComboButtonItem) {}

ak::ui::qt::comboButtonItem::comboButtonItem(
	const QString & _text,
	QObject * _parent
) : QAction(_text, _parent), ak::ui::core::aObject(ak::ui::core::objectType::oComboButtonItem) {}

ak::ui::qt::comboButtonItem::comboButtonItem(
	const QIcon & _icon,
	const QString & _text,
	QObject * _parent
) : QAction(_icon, _text, _parent), ak::ui::core::aObject(ak::ui::core::objectType::oComboButtonItem) {}

ak::ui::qt::comboButtonItem::comboButtonItem(
	const ak::ui::qt::comboButtonItem &	_other
) : ak::ui::core::aObject(ak::ui::core::objectType::oComboButtonItem) { setIcon(_other.icon()); setIconText(_other.iconText()); }

ak::ui::qt::comboButtonItem & ak::ui::qt::comboButtonItem::operator = (const ak::ui::qt::comboButtonItem & _other) {
	setIcon(_other.icon());
	setIconText(_other.iconText());
	return *this;
}

ak::ui::qt::comboButtonItem::~comboButtonItem() {}
