/*
 * ak_ui_qt_aContextMenuItem.cpp
 *
 *  Created on: November 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akGui/aContextMenuItem.h>

ak::aContextMenuItem::aContextMenuItem(
	const QString &				_text,
	contextMenuRole				_role
) : QAction(_text), aObject(otAction), my_role(_role), my_id(invalidID)
{}

ak::aContextMenuItem::aContextMenuItem(
	const QIcon &				_icon,
	const QString &				_text,
	contextMenuRole				_role
) : QAction(_icon, _text), aObject(otAction), my_role(_role), my_id(invalidID)
{}

ak::aContextMenuItem::~aContextMenuItem() { A_OBJECT_DESTROYING }

ak::contextMenuRole ak::aContextMenuItem::role(void) const { return my_role; }

void ak::aContextMenuItem::setId(
	ak::ID						_id
) { my_id = _id; }

ak::ID ak::aContextMenuItem::id(void) const { return my_id; }