/*
 * ak_ui_colorStyleDefault.cpp
 *
 *  Created on: April 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_colorStyleDefault.h>			// corresponding header
#include <ak_exception.h>						// error handling

ak::ui::colorStyleDefault::colorStyleDefault() {
	setControlsMainBackcolor(ak::ui::color(255, 255, 255));
	setControlsMainForecolor(ak::ui::color(0, 0, 0));
	setControlsErrorForecolor(ak::ui::color(255, 0, 0));
	setWindowMainBackcolor(ak::ui::color(255, 255, 255));
	setWindowMainForecolor(ak::ui::color(0, 0, 0));
	setControlsFocusedColor(ak::ui::color(60, 60, 200));
	setControlsPressedColor(ak::ui::color(60, 60, 200));
	my_alternateBackColor = ak::ui::color(69, 69, 69);
	my_alternateForeColor = ak::ui::color(255, 255, 255);
	my_headerBackColor.setRGBA(80, 80, 80);
	my_headerForeColor.setRGBA(0, 0, 0);
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
