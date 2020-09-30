/*
 * ak_ui_qt_timer.cpp
 *
 *  Created on: September 19, 2020
 *	Last modified on: September 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_qt_timer.h>		// Corresponding header

ak::ui::qt::timer::timer()
	: QTimer(), ak::ui::core::aObject(ak::ui::core::objectType::oTimer)
{}

ak::ui::qt::timer::~timer() {}