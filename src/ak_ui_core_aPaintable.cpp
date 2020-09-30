/*
 * ak_ui_core_aPaintable.cpp
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_core_aPaintable.h>		// corresponding header
#include <ak_ui_colorStyle.h>			// color Style

ak::ui::core::aPaintable::aPaintable(
	ak::ui::core::objectType		_type,
	ak::ui::colorStyle *			_colorStyle,
	ak::UID							_uid,
	int								_references
) : ak::ui::core::aObject(_type, _uid, _references), my_colorStyle(_colorStyle) {}

ak::ui::core::aPaintable::~aPaintable() {}

bool ak::ui::core::aPaintable::isPaintableType(void) const { return true; }