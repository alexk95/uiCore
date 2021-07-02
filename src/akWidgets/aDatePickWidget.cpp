/*
 *	File:		aDatePickWidget.h
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
#include <akWidgets/aDatePickWidget.h>
#include <akWidgets/aCalendarWidget.h>

#include <akGui/aColorStyle.h>
#include <akWidgets/aPushButtonWidget.h>

// QT header
#include <qlayout.h>
#include <qtextformat.h>

ak::aDatePickWidget::aDatePickWidget()
	: aWidget(otDatePicker), m_date{ QDate::currentDate() }, m_dateFormat{ dfDDMMYYYY }, m_delimiter{ "." }
{
	refreshDate();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aDatePickWidget::aDatePickWidget(const QDate & _date, dateFormat _dateFormat)
	: aWidget(otDatePicker), m_date{ _date }, m_dateFormat{ _dateFormat }, m_delimiter{ "." }
{
	refreshDate();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aDatePickWidget::~aDatePickWidget() {

}

// #############################################################################################################################

QWidget * ak::aDatePickWidget::widget(void) { return this; }

void ak::aDatePickWidget::setColorStyle(
	aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;
	QString sheet(m_colorStyle->toStyleSheet(cafForegroundColorButton |
		cafBackgroundColorButton, "QPushButton{", "}\n"));
	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QPushButton:hover:!pressed{", "}\n"));
	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QPushButton:pressed{", "}\n"));

	this->setStyleSheet(sheet);
}

// #############################################################################################################################

// Setter

void ak::aDatePickWidget::setCurrentDate(const QDate & _date, bool _refresh) {
	m_date = _date;
	if (_refresh) { refreshDate(); }
}

void ak::aDatePickWidget::setDelimiter(const QString & _delimiter, bool _refresh) {
	m_delimiter = _delimiter;
	if (_refresh) { refreshDate(); }
}

void ak::aDatePickWidget::setDateFormat(dateFormat _dateFormat, bool _refresh) {
	m_dateFormat = _dateFormat;
	if (_refresh) { refreshDate(); }
}

// #############################################################################################################################

// Slots

void ak::aDatePickWidget::slotClicked(void) {
	aDatePickDialog d{ m_date, this };
	if (m_colorStyle != nullptr) { d.setColorStyle(m_colorStyle); }

	if (d.showDialog() == ak::dialogResult::resultOk) {
		m_date = d.selectedDate();
		refreshDate();
		emit changed();
	}
}

// #################################################################################################################################

void ak::aDatePickWidget::refreshDate(void) {
	QString y{ QString::number(m_date.year()) };
	QString d;
	QString m;
	if (m_date.day() < 10) { d = "0" + QString::number(m_date.day()); }
	else { d = QString::number(m_date.day()); }

	if (m_date.month() < 10) { m = "0" + QString::number(m_date.month()); }
	else { m = QString::number(m_date.month()); }

	QString msg;

	switch (m_dateFormat)
	{
	case ak::dfDDMMYYYY:
		msg.append(d).append(m_delimiter).append(m).append(m_delimiter).append(y); break;
	case ak::dfMMDDYYYY:
		msg.append(m).append(m_delimiter).append(d).append(m_delimiter).append(y); break;
	case ak::dfYYYYMMDD:
		msg.append(y).append(m_delimiter).append(m).append(m_delimiter).append(d); break;
	case ak::dfYYYYDDMM:
		msg.append(y).append(m_delimiter).append(d).append(m_delimiter).append(m); break;
	default:
		assert(0);	// Unknown format
		break;
	}

	setText(msg);
}

// #################################################################################################################################

// #################################################################################################################################

// #################################################################################################################################

ak::aDatePickDialog::aDatePickDialog(aDatePickWidget * _parent)
	: ak::aPaintable(otDatePickerDialog), aDialog(_parent)
{
	setupWidget();

}

ak::aDatePickDialog::aDatePickDialog(const QDate & _date, aDatePickWidget * _parent)
	: ak::aPaintable(otDatePickerDialog), aDialog(_parent)
{
	setupWidget();
	m_calendar->setSelectedDate(_date);
}

ak::aDatePickDialog::~aDatePickDialog() {

}

// #############################################################################################################################

void ak::aDatePickDialog::setColorStyle(
	aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;
	m_buttonCancel->setColorStyle(m_colorStyle);
	m_buttonOk->setColorStyle(m_colorStyle);

	QString sheet(m_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow |
		cafDefaultBorderWindow | cafBorderColorWindow, "#date_picker_dialog_main {", "}"));

	setStyleSheet(sheet);
	m_calendar->setColorStyle(m_colorStyle);
}

// #############################################################################################################################

// Getter

QDate ak::aDatePickDialog::selectedDate(void) const {
	return m_calendar->selectedDate();
}

// #############################################################################################################################

// Slots

void ak::aDatePickDialog::slotOkClicked(void) {
	m_result = resultOk;
	close();
}

void ak::aDatePickDialog::slotCancelClicked(void) {
	m_result = resultCancel;
	close();
}

// #############################################################################################################################

// Private functions

void ak::aDatePickDialog::setupWidget(void) {
	// Create layouts
	m_mainLayout = new QVBoxLayout{ this };

	m_inputWidget = new QWidget;
	m_inputLayout = new QHBoxLayout{ m_inputWidget };
	m_inputWidget->setContentsMargins(0, 0, 0, 0);

	// Create controls
	m_calendar = new aCalendarWidget;
	m_buttonOk = new aPushButtonWidget{ "Ok" };
	m_buttonCancel = new aPushButtonWidget{ "Cancel" };
	m_calendar->setOnlyCurrentMonthVisible(true);

	// Add controls to layout
	m_inputLayout->addWidget(m_buttonOk);
	m_inputLayout->addWidget(m_buttonCancel);

	m_mainLayout->addWidget(m_calendar);
	m_mainLayout->addWidget(m_inputWidget);

	// Setup dialog
	Qt::WindowFlags f;
	f.setFlag(Qt::Window);
	f.setFlag(Qt::FramelessWindowHint);
	setWindowFlags(f);

	setObjectName("date_picker_dialog_main");
	m_calendar->setObjectName("date_picker_dialog_calendar");
	m_buttonOk->setObjectName("date_picker_dialog_ok");
	m_buttonCancel->setObjectName("date_picker_dialog_cancel");

	// Connect signals
	connect(m_buttonOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
	connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
}