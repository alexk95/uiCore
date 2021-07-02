/*
 *	File:		aColorStyleDefaultDark.cpp
 *	Package:	akGui
 *
 *  Created on: April 08, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColorStyleDefaultDark.h>
#include <akCore/aException.h>

ak::aColorStyleDefaultDark::aColorStyleDefaultDark() {
	my_controlsMainBackcolor.setRGBA(48, 48, 48);
	my_controlsMainForecolor.setRGBA(255, 255, 255);
	my_controlsErrorFrontForecolor.setRGBA(255,0,0);
	my_controlsErrorBackBackcolor.setRGBA(255, 0, 0);
	my_controlsErrorBackForecolor.setRGBA(0, 0, 0);
	my_controlsPressedBackColor.setRGBA(60, 200, 60);
	my_controlsPressedForeColor.setRGBA(0, 0, 0);
	my_controlsFocusBackColor.setRGBA(64, 128, 200);
	my_controlsFocusForeColor.setRGBA(255, 255, 255);
	my_controlsBorderColor.setRGBA(130, 135, 144);

	my_windowMainBackcolor.setRGBA(48, 48, 48);
	my_windowMainForecolor.setRGBA(255, 255, 255);

	my_alternateBackColor.setRGBA(69, 69, 69);
	my_alternateForeColor.setRGBA(255, 255, 255);

	my_headerBackColor.setRGBA(80, 80, 80);
	my_headerForeColor.setRGBA(255, 255, 255);
}

ak::aColorStyleDefaultDark::~aColorStyleDefaultDark() {}

QString ak::aColorStyleDefaultDark::getColorStyleName(void) const { return QString("Default_Dark"); }

/*
#define _borderDefaultControl "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultWindow "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultOsgTabWidget "border-style: outset;border-width: 1px; border-radius: 0px;border-color: #707070;"
#define _borderDefaultTabWidget "border-top: 1px solid #707070;border-bottom: 1px solid #707070;"
#define _borderDefaultMdiSubwindow "border-style: outset;border-width: 1px;"
#define _borderButtonNormal "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #c0c0c0;padding: 2px;"
#define _borderButtonPressed "border-style: inset;border-width: 1px; border-radius: 0px; border-color: #000000;padding: 2px;"
#define _borderButtonFocus "border-style: outset;border-width: 1px; border-radius: 0px; border-color: #5050e0;padding: 2px;"
*/

#define STYLE_COLOR_BACK_ALTERNATE "alternate-background-color:#454545;"
#define STYLE_COLOR_BACK_CONTROLS "background-color: #303030;"
#define STYLE_COLOR_BACK_FOCUS "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #4080e0, stop : 1 #a0a0f0);"
#define STYLE_COLOR_BACK_HEADER "background-color: #505050;"
#define STYLE_COLOR_BACK_SELECTED "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #10b010, stop : 1 #a0f0a0);"
#define STYLE_COLOR_BACK_TRANSPARENT "background-color: transparent;"
#define STYLE_COLOR_BACK_WINDOW "background-color: #303030;"
#define STYLE_COLOR_BACK_DIALOGWINDOW "background-color: #404040;"
#define STYLE_COLOR_BACK_PUSHBUTTON "background-color: #505050;"

#define STYLE_COLOR_FORE_CONTROLS "color: white;"
#define STYLE_COLOR_FORE_ERROR "color: #ff0000;"
#define STYLE_COLOR_FORE_FOCUS "color: white;"
#define STYLE_COLOR_FORE_HEADER "color: white;"
#define STYLE_COLOR_FORE_SELECTED "color: black;"
#define STYLE_COLOR_FORE_WINDOW "color: white;"
#define STYLE_COLOR_FORE_DIALOGWINDOW "color: white;"
#define STYLE_COLOR_FORE_PUSHBUTTON "color: white;"

#define STYLE_COLOR_BORDER_CONTROLS "border-color: #707070;"
#define STYLE_COLOR_BORDER_HEADER "border-color: #707070;"
#define STYLE_COLOR_BORDER_WINDOW "border-color: #707070;"
#define STYLE_COLOR_BORDER_AS_COLOR "color: #707070;"

#define STYLE_DEFAULT_BORDER_CONTROLS "border: 1px solid;"
#define STYLE_DEFAULT_BORDER_HEADER "border: 1px solid;"
#define STYLE_DEFAULT_BORDER_WINDOW "border: 1px solid;"

#define AK_INTERN_ADD_STYLE_IF_FLAG(___flag, ___style) if (flagIsSet(_colorAreas, ___flag)) { ret.append(___style); }

QString ak::aColorStyleDefaultDark::toStyleSheet(
	colorAreaFlag			_colorAreas,
	const QString &					_prefix,
	const QString &					_suffix
) {
	QString ret(_prefix);
	
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorAlternate, STYLE_COLOR_BACK_ALTERNATE);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorControls, STYLE_COLOR_BACK_CONTROLS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorFocus, STYLE_COLOR_BACK_FOCUS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorHeader, STYLE_COLOR_BACK_HEADER);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorSelected, STYLE_COLOR_BACK_SELECTED);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorTransparent, STYLE_COLOR_BACK_TRANSPARENT);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorWindow, STYLE_COLOR_BACK_WINDOW);
 	AK_INTERN_ADD_STYLE_IF_FLAG(cafBorderColorControls, STYLE_COLOR_BORDER_CONTROLS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBorderColorHeader, STYLE_COLOR_BORDER_HEADER);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBorderColorWindow, STYLE_COLOR_BORDER_WINDOW);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafDefaultBorderControls, STYLE_DEFAULT_BORDER_CONTROLS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafDefaultBorderHeader, STYLE_DEFAULT_BORDER_HEADER);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafDefaultBorderWindow, STYLE_DEFAULT_BORDER_WINDOW);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorControls, STYLE_COLOR_FORE_CONTROLS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorError, STYLE_COLOR_FORE_ERROR);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorFocus, STYLE_COLOR_FORE_FOCUS);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorHeader, STYLE_COLOR_FORE_HEADER);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorSelected, STYLE_COLOR_FORE_SELECTED);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorWindow, STYLE_COLOR_FORE_WINDOW);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorDialogWindow, STYLE_COLOR_BACK_DIALOGWINDOW);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafBackgroundColorButton, STYLE_COLOR_BACK_PUSHBUTTON);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorDialogWindow, STYLE_COLOR_FORE_DIALOGWINDOW);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafForegroundColorButton, STYLE_COLOR_FORE_PUSHBUTTON);
	AK_INTERN_ADD_STYLE_IF_FLAG(cafTabToolBarGroupSeperatorLine, STYLE_COLOR_BORDER_AS_COLOR);

	ret.append(_suffix);
	return ret;
}
