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
#include <akCore/akCore.h>
#include <akDialogs/aCustomizableInputDialog.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aDatePickWidget.h>
#include <akWidgets/aTimePickWidget.h>
#include <akWidgets/aCheckBoxWidget.h>

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
		delete input.second.first;
		delete input.second.second;
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
		input.second.first->setColorStyle(_style);
		input.second.second->setColorStyle(_style);
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
	m_buttonLayout->addWidget(nBtn);
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

ak::UID ak::aCustomizableInputDialog::addDateInput(const QString& _label, const aDate& _initialDate, dateFormat _dateFormat) {
	aDatePickWidget * nInput = new aDatePickWidget(_initialDate, _dateFormat);
	addCustomInput(_label, nInput);
	connect(nInput, &aDatePickWidget::changed, this, &aCustomizableInputDialog::slotInputChanged);
	return nInput->uid();
}

ak::UID ak::aCustomizableInputDialog::addTimeInput(const QString& _label, const aTime& _initialTime, timeFormat _timeFormat) {
	aTimePickWidget * nInput = new aTimePickWidget(_initialTime, _timeFormat);
	addCustomInput(_label, nInput);
	connect(nInput, &aTimePickWidget::changed, this, &aCustomizableInputDialog::slotInputChanged);
	return nInput->uid();
}

ak::UID ak::aCustomizableInputDialog::addCheckInput(const QString& _label, bool _initiallyChecked) {
	aCheckBoxWidget * nInput = new aCheckBoxWidget;
	nInput->setChecked(_initiallyChecked);
	addCustomInput(_label, nInput);
	connect(nInput, &aCheckBoxWidget::stateChanged, this, &aCustomizableInputDialog::slotInputChanged);
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
	m_inputs.insert_or_assign(_widget->uid(), std::pair<aLabelWidget *, aWidget *>(nLabel, _widget));

	// Add to layout
	m_inputsLayout->addWidget(nLabel, m_currentInputRow, 0);
	m_inputsLayout->addWidget(_widget->widget(), m_currentInputRow++, 1);

	return _widget->uid();
}

// ################################################################################################################

// Getter

QString ak::aCustomizableInputDialog::getText(UID _inputUID) {
	auto control = m_inputs.find(_inputUID);
	if (control == m_inputs.end()) {
		assert(0);
		return "";
	}
	aLineEditWidget * input = dynamic_cast<aLineEditWidget *>(control->second.second);
	if (input) {
		return input->text();
	}
	else {
		assert(0);
		return "";
	}
}

QString ak::aCustomizableInputDialog::getPassword(UID _inputUID, HashAlgorithm _hashAlgorithm) {
	auto control = m_inputs.find(_inputUID);
	if (control == m_inputs.end()) {
		assert(0);
		return "";
	}
	aLineEditWidget * input = dynamic_cast<aLineEditWidget *>(control->second.second);
	if (input) {
		return hashString(input->text(), _hashAlgorithm);
	}
	else {
		assert(0);
		return "";
	}
}

ak::aDate ak::aCustomizableInputDialog::getDate(UID _inputUID) {
	auto control = m_inputs.find(_inputUID);
	if (control == m_inputs.end()) {
		assert(0);
		return aDate();
	}
	aDatePickWidget * input = dynamic_cast<aDatePickWidget *>(control->second.second);
	if (input) {
		return input->currentDate();
	}
	else {
		assert(0);
		return aDate();
	}
}

ak::aTime ak::aCustomizableInputDialog::getTime(UID _inputUID) {
	auto control = m_inputs.find(_inputUID);
	if (control == m_inputs.end()) {
		assert(0);
		return aTime();
	}
	aTimePickWidget * input = dynamic_cast<aTimePickWidget *>(control->second.second);
	if (input) {
		return input->currentTime();
	}
	else {
		assert(0);
		return aTime();
	}
}

bool ak::aCustomizableInputDialog::getChecked(UID _inputUID) {
	auto control = m_inputs.find(_inputUID);
	if (control == m_inputs.end()) {
		assert(0);
		return false;
	}
	aCheckBoxWidget * input = dynamic_cast<aCheckBoxWidget *>(control->second.second);
	if (input) {
		return input->isChecked();
	}
	else {
		assert(0);
		return false;
	}
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