/*
 *	File:		aCustomizableInputDialog.cpp
 *	Package:	akDialogs
 *
 *  Created on: July 09, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// uiCore header
#include <akGui/aColorStyle.h>
#include <akDialogs/aCustomizableInputDialog.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aLineEditWidget.h>

// Qt header
#include <qwidget.h>
#include <qlayout.h>

ak::aCustomizableInputDialog::aCustomizableInputDialog(QWidget * _parent)
	: aPaintable(otNone), aDialog(_parent), m_currentUid(invalidUID), m_currentInputRow(0)
{
	// Create layouts
	m_centralLayout = new QVBoxLayout(this);

	m_inputsLayoutW = new QWidget;
	m_inputsLayout = new QGridLayout(m_inputsLayoutW);

	m_buttonLayoutW = new QWidget;
	m_buttonLayout = new QHBoxLayout(m_buttonLayoutW);

	m_centralLayout->addWidget(m_inputsLayoutW);
	m_centralLayout->addWidget(m_buttonLayoutW);
}

ak::aCustomizableInputDialog::~aCustomizableInputDialog() {
	for (auto btn : m_buttons) {
		delete btn;
	}

	for (auto input : m_inputs) {
		delete input.first;
		delete input.second;
	}

	delete m_buttonLayout;
	delete m_buttonLayoutW;
	delete m_inputsLayout;
	delete m_inputsLayoutW;
	delete m_centralLayout;
}

void ak::aCustomizableInputDialog::setColorStyle(aColorStyle * _style) {
	m_colorStyle = _style;

	setStyleSheet(m_colorStyle->toStyleSheet(cafBackgroundColorDialogWindow | cafForegroundColorDialogWindow, "QDialog {", "}"));

	for (auto btn : m_buttons) {
		btn->setColorStyle(_style);
	}

	for (auto input : m_inputs) {
		input.first->setColorStyle(_style);
		input.second->setColorStyle(_style);
	}
}

// #####################################################################################################################################

// Item adder

ak::UID ak::aCustomizableInputDialog::addButton(const QString& _buttonText) {
	aPushButtonWidget * nBtn = new aPushButtonWidget(_buttonText);
	if (m_colorStyle) { nBtn->setColorStyle(m_colorStyle); }
	nBtn->setUid(++m_currentUid);
	connect(nBtn, &aPushButtonWidget::clicked, this, &aCustomizableInputDialog::slotButtonClicked);
	m_buttons.push_back(nBtn);
	return nBtn->uid();
}

ak::UID ak::aCustomizableInputDialog::addTextInput(const QString& _label, const QString& _initialText, const QString& _placeholder, const QString& _toolTip) {
	aLineEditWidget * nInput = new aLineEditWidget(_initialText);
	nInput->setPlaceholderText(_placeholder);
	nInput->setToolTip(_toolTip);

	addCustomInput(_label, nInput);
	connect(nInput, &aLineEditWidget::finishedChanges, this, &aCustomizableInputDialog::slotInputChanged);

	return nInput->uid();
}

ak::UID ak::aCustomizableInputDialog::addPasswordInput(const QString& _label, const QString& _initialText, const QString& _placeholder, const QString& _toolTip) {
	aLineEditWidget * nInput = new aLineEditWidget(_initialText);
	nInput->setPlaceholderText(_placeholder);
	nInput->setToolTip(_toolTip);
	nInput->setEchoMode(QLineEdit::Password);
	
	addCustomInput(_label, nInput);
	connect(nInput, &aLineEditWidget::finishedChanges, this, &aCustomizableInputDialog::slotInputChanged);

	return nInput->uid();
}

ak::UID ak::aCustomizableInputDialog::addCustomInput(const QString& _label, aWidget * _widget) {
	// Create label and setup controls
	aLabelWidget * nLabel = new aLabelWidget(_label);
	if (m_colorStyle) { 
		nLabel->setColorStyle(m_colorStyle);
		_widget->setColorStyle(m_colorStyle);
	}
	nLabel->setBuddy(_widget->widget());

	// Store data
	_widget->setUid(++m_currentUid);
	m_inputs.push_back(std::pair<aLabelWidget *, aWidget *>(nLabel, _widget));

	// Add to layout
	m_inputsLayout->addWidget(nLabel, m_currentInputRow, 0);
	m_inputsLayout->addWidget(_widget->widget(), m_currentInputRow++, 1);

	return _widget->uid();
}

// #####################################################################################################################################

// Slots

void ak::aCustomizableInputDialog::slotButtonClicked(void) {
	aObject * actualSender = dynamic_cast<aObject *>(sender());
	if (actualSender) {
		emit buttonClicked(actualSender->uid());
	}
	else {
		assert(0);
	}
}

void ak::aCustomizableInputDialog::slotInputChanged(void) {
	aObject * actualSender = dynamic_cast<aObject *>(sender());
	if (actualSender) {
		emit editFinished(actualSender->uid());
	}
	else {
		assert(0);
	}
}