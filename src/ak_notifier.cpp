/*
 * ak_notifer.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_notifier.h>	// Corresponding header

ak::notifier::notifier() : ak::ui::core::aObject(ak::ui::core::objectType::oNone), my_isEnabled(true) {}

void ak::notifier::enable(void) { my_isEnabled = true; }

void ak::notifier::disable(void) { my_isEnabled = false; }

bool ak::notifier::isEnabled(void) const { return my_isEnabled; }