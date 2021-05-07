/*
 * ak_ui_core_aPaintable.cpp
 *
 *  Created on: July 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_core_aPaintable.h>		// corresponding header
#include <ak_ui_colorStyle.h>			// color Style

ak::ui::core::aPaintable::aPaintable(
	ak::ui::core::objectType		_type,
	const ak::ui::colorStyle *		_colorStyle,
	ak::UID							_uid
) : ak::ui::core::aObject(_type, _uid), my_colorStyle(_colorStyle) {}

ak::ui::core::aPaintable::~aPaintable() {}

bool ak::ui::core::aPaintable::isPaintableType(void) const { return true; }