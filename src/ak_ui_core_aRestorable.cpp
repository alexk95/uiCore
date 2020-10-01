/*
 * ak_ui_core_aRestorable.cpp
 *
 *  Created on: October 01, 2020
 *	Last modified on: October 01, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_core_aRestorable.h>		// Corresponding header

ak::ui::core::aRestorable::aRestorable() {}

ak::ui::core::aRestorable::aRestorable(
	const QString &							_alias
) : my_alias(_alias) {}

ak::ui::core::aRestorable::~aRestorable() {}

void ak::ui::core::aRestorable::setAlias(
	const QString &							_alias
) { my_alias = _alias; }

QString ak::ui::core::aRestorable::alias(void) const { return my_alias; }
