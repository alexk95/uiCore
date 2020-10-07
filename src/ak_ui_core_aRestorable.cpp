/*
 * ak_ui_core_aRestorable.cpp
 *
 *  Created on: October 01, 2020
 *	Last modified on: October 01, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_core_aRestorable.h>		// Corresponding header

ak::ui::core::aRestorable::aRestorable() {}

ak::ui::core::aRestorable::aRestorable(
	const QString &							_alias
) : my_alias(_alias) {}

ak::ui::core::aRestorable::~aRestorable() {}

QString ak::ui::core::aRestorable::alias(void) const { return my_alias; }
