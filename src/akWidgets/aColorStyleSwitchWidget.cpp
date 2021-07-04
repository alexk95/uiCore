/*
 *	File:		aColorStyleSwitchWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: June 30, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akWidgets/aColorStyleSwitchWidget.h>
#include <akGui/aColorStyle.h>
#include <akGui/aColorStyleDefault.h>
#include <akGui/aColorStyleDefaultDark.h>
#include <akAPI/uiAPI.h>

#include <qaction.h>

ak::aColorStyleSwitchWidget::aColorStyleSwitchWidget(
	const QString &			_brightModeTitle,
	const QString &			_darkModeTitle,
	const QIcon &			_brightModeIcon,
	const QIcon &			_darkModeIcon,
	bool					_isBright
)	: aToolButtonWidget(_brightModeIcon, _brightModeTitle), m_isBright(_isBright), m_brightModeIcon(_brightModeIcon), 
	m_brightModeTitle(_brightModeTitle), m_darkModeIcon(_darkModeIcon), m_darkModeTitle(_darkModeTitle)
{
	m_action = new QAction(this);

	if (m_isBright) {
		setText(m_darkModeTitle);
		setIcon(m_darkModeIcon);
	}
	else {
		setText(m_brightModeTitle);
		setIcon(m_brightModeIcon);
	}

	connect(this, &aToolButtonWidget::clicked, this, &aColorStyleSwitchWidget::slotClicked);
}

ak::aColorStyleSwitchWidget::~aColorStyleSwitchWidget() {

}

QWidget * ak::aColorStyleSwitchWidget::widget(void) { return this; }

void ak::aColorStyleSwitchWidget::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;

	QString sheet(m_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader | cafBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

	if (m_isAutoSetColorStyle) {
		if (m_colorStyle->getColorStyleName() == aColorStyleDefault::colorStyleName()) {
			setCurrentIsBright(true);
		}
		else if (m_colorStyle->getColorStyleName() == aColorStyleDefaultDark::colorStyleName()) {
			setCurrentIsBright(false);
		}
		else {
			assert(0);	// The colorStyleSwitchWidget should only be used with the two default color styles provided with the uiCore
		}
	}

}

void ak::aColorStyleSwitchWidget::setCurrentIsBright(bool _isBright) {
	if (m_isBright == _isBright) { return; }
	m_isBright = _isBright;
	if (m_isBright) {
		setText(m_darkModeTitle);
		setIcon(m_darkModeIcon);
	}
	else {
		setText(m_brightModeTitle);
		setIcon(m_brightModeIcon);
	}
}

void ak::aColorStyleSwitchWidget::slotClicked(bool _checked) {
	m_isBright = !m_isBright;
	
	if (m_isBright) {
		setText(m_darkModeTitle);
		setIcon(m_darkModeIcon);
		if (m_isAutoSetColorStyle) { uiAPI::setDefaultColorStyle(); }
	}
	else {
		setText(m_brightModeTitle);
		setIcon(m_brightModeIcon);
		if (m_isAutoSetColorStyle) { uiAPI::setDefaultDarkColorStyle(); }
	}

	emit changed();
}