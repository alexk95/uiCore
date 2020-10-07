/*
 * ak_ui_qt_timer.cpp
 *
 *  Created on: September 19, 2020
 *	Last modified on: September 30, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_qt_timer.h>		// Corresponding header

ak::ui::qt::timer::timer()
	: QTimer(), ak::ui::core::aObject(ak::ui::core::objectType::oTimer)
{}

ak::ui::qt::timer::~timer() {}