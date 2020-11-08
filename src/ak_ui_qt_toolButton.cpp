/*
 * ak_ui_qt_toolButton.cpp
 *
 *  Created on: November 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_toolButton.h>		// Corresponding header
#include <ak_ui_qt_action.h>			// action
#include <ak_ui_colorStyle.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::toolButton::toolButton()
	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action();
	ini();
}

ak::ui::qt::toolButton::toolButton(
	const QString &				_text
)	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action(_text);
	ini();
}

ak::ui::qt::toolButton::toolButton(
	const QIcon &				_icon,
	const QString &				_text
)	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action(_icon, _text);
	ini();
}

ak::ui::qt::toolButton::~toolButton() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Event handling

void ak::ui::qt::toolButton::keyPressEvent(QKeyEvent *_event)
{
	QToolButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::toolButton::keyReleaseEvent(QKeyEvent * _event) {
	QToolButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

// Base class function

void ak::ui::qt::toolButton::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus |
		TYPE_COLORAREA::caBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected |
		TYPE_COLORAREA::caBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader |
		TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caBorderColorHeader,
		"QToolTip{", "border: 1px;}"));


	setStyleSheet(sheet);
}

// #######################################################################################################

void ak::ui::qt::toolButton::SetToolTip(
	const QString &						_text
) {
	setToolTip(_text);
	setWhatsThis(_text);
	my_action->setToolTip(_text);
	my_action->setWhatsThis(_text);
}

QString ak::ui::qt::toolButton::ToolTip(void) const { return toolTip(); }

// #######################################################################################################

// Slots

void ak::ui::qt::toolButton::slotClicked() { emit clicked(); }

// #######################################################################################################

// Private member

void ak::ui::qt::toolButton::ini(void) {
	setDefaultAction(my_action);
	setToolTip("");
	setWhatsThis("");
	my_action->setToolTip("");
	my_action->setWhatsThis("");
	connect(my_action, SIGNAL(triggered(bool)), this, SLOT(slotClicked()));
}
