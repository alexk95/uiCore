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

#define m_OBJECTNAME "//NOTE_change"
#define TYPE_COLORAREA aColorAreaFlag

ak::aColorEditButtonWidget::aColorEditButtonWidget(
	const aColor &						_color,
	const QString &						_textOverride,
	aColorStyle *						_colorStyle
)
	: aWidget(otColorEditButton),
	m_button(nullptr),
	m_layout(nullptr),
	m_view(nullptr)
{
	// Check arguments

	m_widget = new QWidget();
	m_widget->setContentsMargins(QMargins(0, 0, 0, 0));
	m_widget->setObjectName(m_OBJECTNAME);

	// Create layout
	m_layout = new QHBoxLayout(m_widget);
	m_layout->setContentsMargins(QMargins(0, 0, 0, 0));

	// Create graphics view
	m_view = new aGraphicsWidget();
	m_view->setHeightForWidthActive(true);
	m_view->setMaximumWidth(30);
	m_view->setMaximumHeight(30);

	// Create pushbutton
	m_button = new aPushButtonWidget();
	m_button->setContentsMargins(QMargins(0, 0, 0, 0));

	m_layout->addWidget(m_view);
	m_layout->addWidget(m_button);
	//m_layout->setStretch(1, 1);

	// Set the current color
	setColor(_color);

	// Override the color text if required
	if (_textOverride.length() > 0) { overrideText(_textOverride); }

	if (m_colorStyle != nullptr) { setColorStyle(m_colorStyle); }

	connect(m_button, &QPushButton::clicked, this, &aColorEditButtonWidget::slotButtonClicked);
}

ak::aColorEditButtonWidget::~aColorEditButtonWidget() {
	A_OBJECT_DESTROYING

	if (m_button != nullptr) { delete m_button; m_button = nullptr; }
	if (m_layout != nullptr) { delete m_layout; m_layout = nullptr; }
	if (m_view != nullptr) { delete m_view; m_view = nullptr; }
}

QWidget * ak::aColorEditButtonWidget::widget(void) { return m_widget; }

void ak::aColorEditButtonWidget::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;
	m_button->setColorStyle(m_colorStyle);
}

// #############################################################################################################################

void ak::aColorEditButtonWidget::setColor(
	const aColor &							_color
) {
	m_color = _color;
	m_view->setStyleSheet(m_color.toHexString(false, "background-color:#"));
	m_button->setText(m_color.toRGBString(","));
}

void ak::aColorEditButtonWidget::SetEnabled(
	bool											_enabled
) { m_button->setEnabled(_enabled); }

void ak::aColorEditButtonWidget::SetVisible(
	bool											_visible
) { m_button->setVisible(_visible); }

bool ak::aColorEditButtonWidget::Enabled() const { return m_button->isEnabled(); }

ak::aColor ak::aColorEditButtonWidget::color(void) const { return m_color; }

void ak::aColorEditButtonWidget::overrideText(
	const QString &								_text
) {
	m_button->setText(_text);
}

void ak::aColorEditButtonWidget::fillBackground(
	const aColor &			_color
) {
	/*QString sheet("#" m_OBJECTNAME "{background-color:#");
	sheet.append(_color.toHexString(true));
	sheet.append(";}\n");
	*/
	QString sheet("background-color:#");
	sheet.append(_color.toHexString(true));
	sheet.append(";}\n");
	m_widget->setAutoFillBackground(true);
	m_widget->setStyleSheet(sheet);
}

void ak::aColorEditButtonWidget::setPushButtonStyleSheet(
	const QString &				_sheet
) {
	m_button->setStyleSheet(_sheet);
}

void ak::aColorEditButtonWidget::slotButtonClicked() {
	// Show color dialog
	QColorDialog dia(m_color.toQColor());
	if (dia.exec() == 1) {
		aColor newColor(dia.currentColor());
		if (newColor != m_color) {
			setColor(newColor);
			// Send changed message
			emit changed();
		}
	}
}