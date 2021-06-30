/*
 *	File:		aSpinBoxWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: June 17, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2021 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akWidgets/aSpinBoxWidget.h>
#include <akGui/aColorStyle.h>

ak::aSpinBoxWidget::aSpinBoxWidget(QWidget * _parent)
	: QSpinBox(_parent), aWidget(otSpinBox), m_stepLength(1), m_value(0)
{ setValue(0); }

ak::aSpinBoxWidget::aSpinBoxWidget(int _value, QWidget * _parent) 
	: QSpinBox(_parent), aWidget(otSpinBox), m_stepLength(1), m_value(_value)
{ setValue(_value); }

ak::aSpinBoxWidget::~aSpinBoxWidget(void) {}

// #############################################################################################################################

// Base class functions

QWidget * ak::aSpinBoxWidget::widget(void) {
	return this;
}

void ak::aSpinBoxWidget::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	if (_colorStyle == nullptr) {
		assert(0);
		return;
	}
	my_colorStyle = _colorStyle;
	
	QString sheet;
	sheet = my_colorStyle->toStyleSheet(cafBackgroundColorControls | cafForegroundColorControls);
}

void ak::aSpinBoxWidget::SetStepLength(int _stepLength) {
	m_stepLength = _stepLength;
	slotValueChanged("");
}

void ak::aSpinBoxWidget::slotValueChanged(const QString & _v) {
	if (m_stepLength > 1) {
		if (value() % m_stepLength != 0) {
			setValue((value() / m_stepLength) * m_stepLength);
			return;
		}
	}
	if (value() != m_value) {
		m_value = value();
		emit valueChangedA();
	}
}