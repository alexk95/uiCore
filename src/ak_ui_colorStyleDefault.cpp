/*
 * ak_ui_colorStyleDefault.cpp
 *
 *  Created on: April 26, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_colorStyleDefault.h>			// corresponding header
#include <ak_exception.h>						// error handling

ak::ui::colorStyleDefault::colorStyleDefault() {
	setControlsMainBackcolor(ak::ui::color(80, 80, 80));
	setControlsMainForecolor(ak::ui::color(0, 0, 0));
	setControlsErrorForecolor(ak::ui::color(255, 0, 0));
	setWindowMainBackcolor(ak::ui::color(240, 240, 240));
	setWindowMainForecolor(ak::ui::color(0, 0, 0));
}

ak::ui::colorStyleDefault::~colorStyleDefault() {}

QString ak::ui::colorStyleDefault::getColorStyleName(void) const { return QString("Default"); }

QString ak::ui::colorStyleDefault::getStylesheet(
	const std::vector<ak::ui::colorStyle::styleableObject>	_items
) const {
	return QString();

}

QString ak::ui::colorStyleDefault::getStylesheet(
	ak::ui::colorStyle::styleableObject					_object
) const { return QString(); }
