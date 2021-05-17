/*
 *	File:		aColorEditButtonWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aColorEditButtonWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aGraphicsWidget.h>

// Qt header
#include <qlayout.h>						// QHBoxLayout
#include <qcolordialog.h>					// QColorDialog

#define MY_OBJECTNAME "//NOTE_change"
#define TYPE_COLORAREA aColorAreaFlag

ak::aColorEditButtonWidget::aColorEditButtonWidget(
	const aColor &						_color,
	const QString &						_textOverride,
	aColorStyle *						_colorStyle
)
	: aWidget(otColorEditButton),
	my_button(nullptr),
	my_layout(nullptr),
	my_view(nullptr)
{
	// Check arguments

	my_widget = new QWidget();
	my_widget->setContentsMargins(QMargins(0, 0, 0, 0));
	my_widget->setObjectName(MY_OBJECTNAME);

	// Create layout
	my_layout = new QHBoxLayout(my_widget);
	my_layout->setContentsMargins(QMargins(0, 0, 0, 0));

	// Create graphics view
	my_view = new aGraphicsWidget();
	my_view->setHeightForWidthActive(true);
	my_view->setMaximumWidth(30);
	my_view->setMaximumHeight(30);

	// Create pushbutton
	my_button = new aPushButtonWidget();
	my_button->setContentsMargins(QMargins(0, 0, 0, 0));

	my_layout->addWidget(my_view);
	my_layout->addWidget(my_button);
	//my_layout->setStretch(1, 1);

	// Set the current color
	setColor(_color);

	// Override the color text if required
	if (_textOverride.length() > 0) { overrideText(_textOverride); }

	if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

	connect(my_button, &QPushButton::clicked, this, &aColorEditButtonWidget::slotButtonClicked);
}

ak::aColorEditButtonWidget::~aColorEditButtonWidget() {
	A_OBJECT_DESTROYING

	if (my_button != nullptr) { delete my_button; my_button = nullptr; }
	if (my_layout != nullptr) { delete my_layout; my_layout = nullptr; }
	if (my_view != nullptr) { delete my_view; my_view = nullptr; }
}

QWidget * ak::aColorEditButtonWidget::widget(void) { return my_widget; }

void ak::aColorEditButtonWidget::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_button->setColorStyle(my_colorStyle);
}

// #############################################################################################################################

void ak::aColorEditButtonWidget::setColor(
	const aColor &							_color
) {
	my_color = _color;
	my_view->setStyleSheet(my_color.toHexString(false, "background-color:#"));
	my_button->setText(my_color.toRGBString(","));
}

void ak::aColorEditButtonWidget::SetEnabled(
	bool											_enabled
) { my_button->setEnabled(_enabled); }

void ak::aColorEditButtonWidget::SetVisible(
	bool											_visible
) { my_button->setVisible(_visible); }

bool ak::aColorEditButtonWidget::Enabled() const { return my_button->isEnabled(); }

ak::aColor ak::aColorEditButtonWidget::color(void) const { return my_color; }

void ak::aColorEditButtonWidget::overrideText(
	const QString &								_text
) {
	my_button->setText(_text);
}

void ak::aColorEditButtonWidget::fillBackground(
	const aColor &			_color
) {
	/*QString sheet("#" MY_OBJECTNAME "{background-color:#");
	sheet.append(_color.toHexString(true));
	sheet.append(";}\n");
	*/
	QString sheet("background-color:#");
	sheet.append(_color.toHexString(true));
	sheet.append(";}\n");
	my_widget->setAutoFillBackground(true);
	my_widget->setStyleSheet(sheet);
}

void ak::aColorEditButtonWidget::setPushButtonStyleSheet(
	const QString &				_sheet
) {
	my_button->setStyleSheet(_sheet);
}

void ak::aColorEditButtonWidget::slotButtonClicked() {
	// Show color dialog
	QColorDialog dia(my_color.toQColor());
	if (dia.exec() == 1) {
		aColor newColor(dia.currentColor());
		if (newColor != my_color) {
			setColor(newColor);
			// Send changed message
			emit changed();
		}
	}
}