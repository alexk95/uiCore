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
	: aWidget(otDatePicker), my_date{ QDate::currentDate() }, my_dateFormat{ dfDDMMYYYY }, my_delimiter{ "." }
{
	refreshDate();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aDatePickWidget::aDatePickWidget(const QDate & _date, dateFormat _dateFormat)
	: aWidget(otDatePicker), my_date{ _date }, my_dateFormat{ _dateFormat }, my_delimiter{ "." }
{
	refreshDate();

	connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));
}

ak::aDatePickWidget::~aDatePickWidget() {

}

// #############################################################################################################################

QWidget * ak::aDatePickWidget::widget(void) { return this; }

void ak::aDatePickWidget::setColorStyle(
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

void ak::aDatePickWidget::setCurrentDate(const QDate & _date, bool _refresh) {
	my_date = _date;
	if (_refresh) { refreshDate(); }
}

void ak::aDatePickWidget::setDelimiter(const QString & _delimiter, bool _refresh) {
	my_delimiter = _delimiter;
	if (_refresh) { refreshDate(); }
}

void ak::aDatePickWidget::setDateFormat(dateFormat _dateFormat, bool _refresh) {
	my_dateFormat = _dateFormat;
	if (_refresh) { refreshDate(); }
}

// #############################################################################################################################

// Slots

void ak::aDatePickWidget::slotClicked(void) {
	aDatePickDialog d{ my_date };
	if (my_colorStyle != nullptr) { d.setColorStyle(my_colorStyle); }

	if (d.showDialog() == ak::dialogResult::resultOk) {
		my_date = d.selectedDate();
		refreshDate();
		emit changed();
	}
}

// #################################################################################################################################

void ak::aDatePickWidget::refreshDate(void) {
	QString y{ QString::number(my_date.year()) };
	QString d;
	QString m;
	if (my_date.day() < 10) { d = "0" + QString::number(my_date.day()); }
	else { d = QString::number(my_date.day()); }

	if (my_date.month() < 10) { m = "0" + QString::number(my_date.month()); }
	else { m = QString::number(my_date.month()); }

	QString msg;

	switch (my_dateFormat)
	{
	case ak::dfDDMMYYYY:
		msg.append(d).append(my_delimiter).append(m).append(my_delimiter).append(y); break;
	case ak::dfMMDDYYYY:
		msg.append(m).append(my_delimiter).append(d).append(my_delimiter).append(y); break;
	case ak::dfYYYYMMDD:
		msg.append(y).append(my_delimiter).append(m).append(my_delimiter).append(d); break;
	case ak::dfYYYYDDMM:
		msg.append(y).append(my_delimiter).append(d).append(my_delimiter).append(m); break;
	default:
		assert(0);	// Unknown format
		break;
	}

	setText(msg);
}

// #################################################################################################################################

// #################################################################################################################################

// #################################################################################################################################

ak::aDatePickDialog::aDatePickDialog()
	: ak::aPaintable(otDatePickerDialog)
{
	setupWidget();

}

ak::aDatePickDialog::aDatePickDialog(const QDate & _date)
	: ak::aPaintable(otDatePickerDialog)
{
	setupWidget();
	my_calendar->setSelectedDate(_date);
}

ak::aDatePickDialog::~aDatePickDialog() {

}

// #############################################################################################################################

void ak::aDatePickDialog::setColorStyle(
	const aColorStyle *	_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_buttonCancel->setColorStyle(my_colorStyle);
	my_buttonOk->setColorStyle(my_colorStyle);

	QString sheet(my_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow |
		cafDefaultBorderWindow | cafBorderColorWindow, "#date_picker_dialog_main {", "}"));

	setStyleSheet(sheet);
	my_calendar->setColorStyle(my_colorStyle);
}

// #############################################################################################################################

// Getter

QDate ak::aDatePickDialog::selectedDate(void) const {
	return my_calendar->selectedDate();
}

// #############################################################################################################################

// Slots

void ak::aDatePickDialog::slotOkClicked(void) {
	my_result = resultOk;
	close();
}

void ak::aDatePickDialog::slotCancelClicked(void) {
	my_result = resultCancel;
	close();
}

// #############################################################################################################################

// Private functions

void ak::aDatePickDialog::setupWidget(void) {
	// Create layouts
	my_mainLayout = new QVBoxLayout{ this };

	my_inputWidget = new QWidget;
	my_inputLayout = new QHBoxLayout{ my_inputWidget };
	my_inputWidget->setContentsMargins(0, 0, 0, 0);

	// Create controls
	my_calendar = new aCalendarWidget;
	my_buttonOk = new aPushButtonWidget{ "Ok" };
	my_buttonCancel = new aPushButtonWidget{ "Cancel" };
	my_calendar->setOnlyCurrentMonthVisible(true);

	// Add controls to layout
	my_inputLayout->addWidget(my_buttonOk);
	my_inputLayout->addWidget(my_buttonCancel);

	my_mainLayout->addWidget(my_calendar);
	my_mainLayout->addWidget(my_inputWidget);

	// Setup dialog
	Qt::WindowFlags f;
	f.setFlag(Qt::Window);
	f.setFlag(Qt::FramelessWindowHint);
	setWindowFlags(f);

	setObjectName("date_picker_dialog_main");
	my_calendar->setObjectName("date_picker_dialog_calendar");
	my_buttonOk->setObjectName("date_picker_dialog_ok");
	my_buttonCancel->setObjectName("date_picker_dialog_cancel");

	// Connect signals
	connect(my_buttonOk, SIGNAL(clicked()), this, SLOT(slotOkClicked()));
	connect(my_buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancelClicked()));
}