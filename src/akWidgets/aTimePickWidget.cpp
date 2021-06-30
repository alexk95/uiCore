/*
 *	File:		aTimePickWidget.h
 *	Package:	akWidgets
 *
 *  Created on: April 29, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <akWidgets/aTimePickWidget.h>

#include <akGui/aColorStyle.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aSpinBoxWidget.h>

// QT header
#include <qlayout.h>
#include <qtextformat.h>

ak::aTimePickWidget::aTimePickWidget()
	: aWidget(otTimePicker), my_time{ QTime::currentTime() }, my_timeFormat{ tfHHMM }
{
	refreshTime();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aTimePickWidget::aTimePickWidget(const QTime & _time, timeFormat _timeFormat)
	: aWidget(otTimePicker), my_time{ _time }, my_timeFormat{ _timeFormat }
{
	refreshTime();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aTimePickWidget::~aTimePickWidget() {

}

// #############################################################################################################################

QWidget * ak::aTimePickWidget::widget(void) { return this; }

void ak::aTimePickWidget::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorButton |
		cafBackgroundColorButton, "QPushButton{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QPushButton:hover:!pressed{", "}\n"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QPushButton:pressed{", "}\n"));

	this->setStyleSheet(sheet);
}

// #############################################################################################################################

// Setter

void ak::aTimePickWidget::setCurrentTime(const QTime & _time, bool _refresh) {
	my_time = _time;
	if (_refresh) { refreshTime(); }
}

void ak::aTimePickWidget::setTimeFormat(timeFormat _timeFormat, bool _refresh) {
	my_timeFormat = _timeFormat;
	if (_refresh) { refreshTime(); }
}

void ak::aTimePickWidget::setMinuteStep(int _step) {
	m_minuteStep = _step;

	if (m_minuteStep > 1) {
		int min = my_time.minute();
		if (min % m_minuteStep != 0) {
			min = (min / m_minuteStep) * m_minuteStep;
		}
		my_time.setHMS(my_time.hour(), min, my_time.second(), my_time.msec());
	}
}

// #############################################################################################################################

// Slots

void ak::aTimePickWidget::slotClicked(void) {
	aTimePickDialog t{ my_time , this, my_timeFormat };

	if (my_colorStyle != nullptr) { t.setColorStyle(my_colorStyle); }

	if (t.showDialog() == ak::dialogResult::resultOk) {
		my_time = t.selectedTime();
		refreshTime();
		emit changed();
	}
}


// #############################################################################################################################

void ak::aTimePickWidget::refreshTime(void) {
	QString h;
	QString m;
	QString s;
	QString ms;

	h = QString::number(my_time.hour());
	if (my_time.minute() < 10) {
		m = "0" + QString::number(my_time.minute());
	}
	else {
		m = QString::number(my_time.minute());
	}
	if (my_time.second() < 10) {
		s = "0" + QString::number(my_time.second());
	}
	else {
		s = "0" + QString::number(my_time.second());
	}

	if (my_time.msec() < 10) {
		ms = "000" + QString::number(my_time.msec());
	} else if (my_time.msec() < 100) {
		ms = "00" + QString::number(my_time.msec());
	} else if (my_time.msec() < 1000) {
		ms = "0" + QString::number(my_time.msec());
	}
	else {
		ms = QString::number(my_time.msec());
	}

	QString msg;

	switch (my_timeFormat) {
	case ak::tfHHMM:
		msg.append(h).append(":").append(m); break;
	case ak::tfHHMMSS:
		msg.append(h).append(":").append(m).append(":").append(s); break;
	case ak::tfHHMMSSMMMM:
		msg.append(h).append(":").append(m).append(":").append(s).append(".").append(ms); break;
	}

	setText(msg);

}

// #################################################################################################################################

// #################################################################################################################################

// #############################################################################################################################

ak::aTimePickDialog::aTimePickDialog(aTimePickWidget * _owner, timeFormat _timeFormat)
	: ak::aPaintable(otTimePickerDialog), m_timeFormat(_timeFormat), m_owner(_owner)
{
	setupWidget(m_timeFormat);

}

ak::aTimePickDialog::aTimePickDialog(const QTime & _time, aTimePickWidget * _owner, timeFormat _timeFormat)
	: ak::aPaintable(otTimePickerDialog), m_timeFormat(_timeFormat), m_owner(_owner)
{
	setupWidget(m_timeFormat);

	switch (m_timeFormat)
	{
	case ak::tfHHMMSSMMMM:
		m_msecInput->setValue(_time.msec());
	case ak::tfHHMMSS:
		m_secInput->setValue(_time.second());
	case ak::tfHHMM:
		my_minInput->setValue(_time.minute());
		my_hourInput->setValue(_time.hour());
		break;
	default:
		assert(0);	// Not implemented time format
		break;
	}

	
}

ak::aTimePickDialog::~aTimePickDialog() {
}

// #############################################################################################################################

void ak::aTimePickDialog::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_buttonCancel->setColorStyle(my_colorStyle);
	my_buttonOk->setColorStyle(my_colorStyle);

	my_hourInput->setColorStyle(my_colorStyle);
	my_minInput->setColorStyle(my_colorStyle);

	if (m_secInput != nullptr) {
		m_secInput->setColorStyle(my_colorStyle);
	}
	if (m_msecInput != nullptr) {
		m_msecInput->setColorStyle(my_colorStyle);
	}

	QString sheet(my_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow |
		cafDefaultBorderWindow | cafBorderColorWindow, "#time_picker_dialog_main {", "}"));

	setStyleSheet(sheet);
}

// #################################################################################################################################

// Getter

QTime ak::aTimePickDialog::selectedTime(void) const {
	switch (m_timeFormat)
	{
	case ak::tfHHMM: return QTime(my_hourInput->value(), my_minInput->value());
	case ak::tfHHMMSS: return QTime(my_hourInput->value(), my_minInput->value(), m_secInput->value());
	case ak::tfHHMMSSMMMM: return QTime(my_hourInput->value(), my_minInput->value(), m_secInput->value(), m_msecInput->value());
	default:
		assert(0);
		return QTime(my_hourInput->value(), my_minInput->value());
	}
}

// #################################################################################################################################

// Event Handling
void ak::aTimePickDialog::slotOkClicked(void) {
	my_result = resultOk;
	close();
}
void ak::aTimePickDialog::slotCancelClicked(void) {
	my_result = resultCancel;
	close();
}

// Private functions

void ak::aTimePickDialog::setupWidget(timeFormat _timeFormat) {

	// Create Layouts
	my_mainLayout = new QVBoxLayout{ this };

	my_inputWidget = new QWidget;
	my_inputLayout = new QHBoxLayout{ my_inputWidget };

	my_buttonWidget = new QWidget;
	my_buttonLayout = new QHBoxLayout{ my_buttonWidget };

	my_mainLayout->addWidget(my_inputWidget);
	my_mainLayout->addWidget(my_buttonWidget);

	// Create Controls

	my_hourInput = new aSpinBoxWidget{ 0 };
	my_hourInput->setMaximum(23);
	my_hourInput->setSuffix("h");

	my_minInput = new aSpinBoxWidget(0);
	my_minInput->setMaximum(59);
	if (m_owner != nullptr) {
		my_minInput->SetStepLength(m_owner->minuteStep());
	}
	my_minInput->setSuffix("min");

	my_inputLayout->addWidget(my_hourInput);
	my_inputLayout->addWidget(my_minInput);

	if (m_timeFormat == tfHHMMSS || m_timeFormat == tfHHMMSSMMMM) {
		m_secInput = new aSpinBoxWidget(0);
		m_secInput->setMaximum(59);
		m_secInput->setSuffix("sec");
		my_inputLayout->addWidget(m_secInput);
	}
	else {
		m_secInput = nullptr;
	}

	if (m_timeFormat == tfHHMMSSMMMM) {
		m_msecInput = new aSpinBoxWidget(0);
		m_msecInput->setMaximum(999);
		m_msecInput->setSuffix("msec");
		my_inputLayout->addWidget(m_msecInput);
	}
	else {
		m_msecInput = nullptr;
	}

	my_buttonOk = new aPushButtonWidget{ "Ok" };
	my_buttonCancel = new aPushButtonWidget{ "Cancel" };

	// Add Controls To Layouts
	my_buttonLayout->addWidget(my_buttonOk);
	my_buttonLayout->addWidget(my_buttonCancel);

	connect(my_buttonOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
	connect(my_buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));

	// Setup Dialog
	Qt::WindowFlags f;
	f.setFlag(Qt::Window);
	f.setFlag(Qt::FramelessWindowHint);
	setWindowFlags(f);

	setObjectName("time_picker_dialog_main");
}

