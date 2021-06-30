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
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader | cafBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

}

void ak::aColorStyleSwitchWidget::setCurrentIsBright(bool _isBright) {
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