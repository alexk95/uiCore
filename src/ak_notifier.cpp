/*
 * ak_notifer.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_notifier.h>	// Corresponding header

ak::notifier::notifier() : ak::ui::core::aObject(ak::ui::core::objectType::oNone), my_isEnabled(true) {}

void ak::notifier::enable(void) { my_isEnabled = true; }

void ak::notifier::disable(void) { my_isEnabled = false; }

bool ak::notifier::isEnabled(void) const { return my_isEnabled; }