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
	my_controlsMainBackcolor.setRGBA(204, 204, 204);
	my_controlsMainForecolor.setRGBA(0, 0, 0);
	my_controlsErrorForecolor.setRGBA(255, 0, 0);
	my_controlsPressedBackColor.setRGBA(60, 200, 60);
	my_controlsPressedForeColor.setRGBA(0, 0, 0);
	my_controlsFocusBackColor.setRGBA(64, 128, 200);
	my_controlsFocusForeColor.setRGBA(255, 255, 255);
	my_controlsBorderColor.setRGBA(0, 0, 0);

	my_windowMainBackcolor.setRGBA(255, 255, 255);
	my_windowMainForecolor.setRGBA(0, 0, 0);

	my_alternateBackColor.setRGBA(69, 69, 69);
	my_alternateForeColor.setRGBA(255, 255, 255);

	my_headerBackColor.setRGBA(218, 218, 218);
	my_headerForeColor.setRGBA(0, 0, 0);
}

ak::ui::colorStyleDefault::~colorStyleDefault() {}

QString ak::ui::colorStyleDefault::getColorStyleName(void) const { return QString("Default"); }

QString ak::ui::colorStyleDefault::toStyleSheet(
	ui::core::colorAreaFlag			_colorAreas,
	const QString &					_prefix,
	const QString &					_suffix
) const { return ""; }

QString ak::ui::colorStyleDefault::getFilePath(
	const QString &			_fileName
) const {
	return "";
}