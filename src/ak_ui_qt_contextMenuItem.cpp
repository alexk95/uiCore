/*
 * ak_ui_qt_contextMenuItem.cpp
 *
 *  Created on: November 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_ui_qt_contextMenuItem.h>		// Corresponding header

ak::ui::qt::contextMenuItem::contextMenuItem(
	const QString &				_text,
	ui::core::contextMenuRole	_role
) : QAction(_text), ui::core::aObject(ui::core::objectType::oAction), my_role(_role), my_id(ak::invalidID)
{}

ak::ui::qt::contextMenuItem::contextMenuItem(
	const QIcon &				_icon,
	const QString &				_text,
	ui::core::contextMenuRole	_role
) : QAction(_icon, _text), ui::core::aObject(ui::core::objectType::oAction), my_role(_role), my_id(ak::invalidID)
{}

ak::ui::qt::contextMenuItem::~contextMenuItem() {}

ak::ui::core::contextMenuRole ak::ui::qt::contextMenuItem::role(void) const { return my_role; }

void ak::ui::qt::contextMenuItem::setId(
	ak::ID						_id
) { my_id = _id; }

ak::ID ak::ui::qt::contextMenuItem::id(void) const { return my_id; }