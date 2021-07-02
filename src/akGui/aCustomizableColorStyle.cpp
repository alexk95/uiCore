/*
 *	File)) {		aCustomizableColorStyle.cpp
 *	Package)) {	akGui
 *
 *  Created on)) { July 02, 2021
 *	Author)) { Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akGui/aCustomizableColorStyle.h>
#include <akCore/aFlags.h>

QString ak::aCustomizableColorStyle::toStyleSheet(
	colorAreaFlag				_colorAreas,
	const QString &				_prefix,
	const QString &				_suffix
) {
	aFlags<colorAreaFlag> f(_colorAreas);
	QString ret{ _prefix };

	if (f.flagIsSet(cafBackgroundColorWindow)) {
		ret.append(getSheet(cafBackgroundColorWindow));
	}
	if (f.flagIsSet(cafForegroundColorWindow)) {
		ret.append(getSheet(cafForegroundColorWindow));
	}
	if (f.flagIsSet(cafBorderColorWindow)) {
		ret.append(getSheet(cafBorderColorWindow));
	}
	if (f.flagIsSet(cafBackgroundColorHeader)) {
		ret.append(getSheet(cafBackgroundColorHeader));
	}
	if (f.flagIsSet(cafForegroundColorHeader)) {
		ret.append(getSheet(cafForegroundColorHeader));
	}
	if (f.flagIsSet(cafBorderColorHeader)) {
		ret.append(getSheet(cafBorderColorHeader));
	}
	if (f.flagIsSet(cafBackgroundColorControls)) {
		ret.append(getSheet(cafBackgroundColorControls));
	}
	if (f.flagIsSet(cafForegroundColorControls)) {
		ret.append(getSheet(cafForegroundColorControls));
	}
	if (f.flagIsSet(cafBorderColorControls)) {
		ret.append(getSheet(cafBorderColorControls));
	}
	if (f.flagIsSet(cafDefaultBorderControls)) {
		ret.append(getSheet(cafDefaultBorderControls));
	}
	if (f.flagIsSet(cafDefaultBorderWindow)) {
		ret.append(getSheet(cafDefaultBorderWindow));
	}
	if (f.flagIsSet(cafBackgroundColorAlternate)) {
		ret.append(getSheet(cafBackgroundColorAlternate));
	}
	if (f.flagIsSet(cafForegroundColorError)) {
		ret.append(getSheet(cafForegroundColorError));
	}
	if (f.flagIsSet(cafBackgroundColorFocus)) {
		ret.append(getSheet(cafBackgroundColorFocus));
	}
	if (f.flagIsSet(cafBackgroundColorSelected)) {
		ret.append(getSheet(cafBackgroundColorSelected));
	}
	if (f.flagIsSet(cafForegroundColorFocus)) {
		ret.append(getSheet(cafForegroundColorFocus));
	}
	if (f.flagIsSet(cafForegroundColorSelected)) {
		ret.append(getSheet(cafForegroundColorSelected));
	}
	if (f.flagIsSet(cafBackgroundColorTransparent)) {
		ret.append(getSheet(cafBackgroundColorTransparent));
	}
	if (f.flagIsSet(cafImagesTree)) {
		ret.append(getSheet(cafImagesTree));
	}
	if (f.flagIsSet(cafImagesDock)) {
		ret.append(getSheet(cafImagesDock));
	}
	if (f.flagIsSet(cafDefaultBorderHeader)) {
		ret.append(getSheet(cafDefaultBorderHeader));
	}
	if (f.flagIsSet(cafBackgroundColorDialogWindow)) {
		ret.append(getSheet(cafBackgroundColorDialogWindow));
	}
	if (f.flagIsSet(cafForegroundColorDialogWindow)) {
		ret.append(getSheet(cafForegroundColorDialogWindow));
	}
	if (f.flagIsSet(cafBackgroundColorButton)) {
		ret.append(getSheet(cafBackgroundColorButton));
	}
	if (f.flagIsSet(cafForegroundColorButton)) {
		ret.append(getSheet(cafForegroundColorButton));
	}
	if (f.flagIsSet(cafTabToolBarGroupSeperatorLine)) {
		ret.append(getSheet(cafTabToolBarGroupSeperatorLine));
	}
	ret.append(_suffix);
	return ret;
}

void ak::aCustomizableColorStyle::setSheet(colorAreaFlag _area, const QString & _sheet) {
	m_sheets.insert_or_assign(_area, _sheet);
}

QString ak::aCustomizableColorStyle::getSheet(colorAreaFlag _area) {
	auto itm = m_sheets.find(_area);
	if (itm == m_sheets.end()) {
		assert(0);
		return "";
	}
	return itm->second;
}

void ak::aCustomizableColorStyle::removeSheet(colorAreaFlag _area) {
	m_sheets.erase(_area);
}