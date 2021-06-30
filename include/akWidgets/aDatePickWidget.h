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

#pragma once

 // AK header
#include <akGui/aDialog.h>
#include <akWidgets/aWidget.h>
#include <akCore/globalDataTypes.h>

// QT header
#include <qpushbutton.h>
#include <qdatetime.h>

// Forward declaration
class QHBoxLayout;
class QVBoxLayout;

namespace ak {

	class aPushButtonWidget;
	class aCalendarWidget;

	class UICORE_API_EXPORT aDatePickWidget : public QPushButton, public aWidget {
		Q_OBJECT
	public:

		aDatePickWidget();
		aDatePickWidget(const QDate & _date, dateFormat _dateFormat = dfDDMMYYYY);
		virtual ~aDatePickWidget();

		// #############################################################################################################################

		// Base class functions

		//! @brief Will return the widgets widget to display it
		virtual QWidget * widget(void) override;

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *	_colorStyle
		) override;

		// #############################################################################################################################

		// Getter

		//! @brief Will return the currently selected date
		//! @return Current date
		QDate currentDate(void) const { return my_date; }

		//! @brief Will return the currently selected delimiter
		//! @return Selected delimeter
		QString delimiter(void) const { return my_delimiter; }

		//! @brief Will return the currently selected date format
		//! @return Selected date format
		dateFormat getDateFormat(void) const { return my_dateFormat; }

		// #############################################################################################################################

		// Setter

		//! @brief Will set the currently selected date
		//! @param _date The date to set
		//! @param _refresh If true, the object will be repainted according to the current changes
		void setCurrentDate(const QDate & _date, bool _refresh = true);

		//! @brief Will set the delimiter
		//! @param _delimiter The delimiter to set
		void setDelimiter(const QString & _delimiter, bool _refresh = true);

		//! @brief Will set the date format
		//! @param _delimiter The delimiter to set
		void setDateFormat(dateFormat _dateFormat, bool _refresh = true);


	signals:

		//! @brief Sends a signal, when changes occured
		void changed(void);

	private slots:

		//! @brief When the slot is clicked, the dialog to select a date appears
		void slotClicked(void);

	private:

		//! @brief The date to set
		//! @throw std::exception If the number of the day is a single digit, a 0 will be preprended to the number
		//! @throw std::exception If the number of the month is a single digit, a 0 will be preprended to the number
		//! @throw std::exception If the user lives in different countrys with other date format, the format can be changed
		//! @throw std::exception If the user lives in different countrys with other date delimiter, the delimiter can be changed
		void refreshDate(void);

		QDate				my_date;			//! The date
		dateFormat			my_dateFormat;		//! The date format
		QString				my_delimiter;		//! The date delimiter
	};

	// #################################################################################################################################

	// #################################################################################################################################

	// #################################################################################################################################

	class UICORE_API_EXPORT aDatePickDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:
		aDatePickDialog();
		aDatePickDialog(const QDate & _date);
		virtual ~aDatePickDialog();

		// #############################################################################################################################

		// Base class functions

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *	_colorStyle
		) override;

		// #############################################################################################################################

		// Getter

		//! @brief Will return the currently sected date
		QDate selectedDate(void) const;

	private slots:

		//! @brief When the slot is clicked, the dialog closes with current results
		void slotOkClicked(void);

		//! @brief When the slot is clicked, the dialog is canceled without results
		void slotCancelClicked(void);

	private:

		//! @brief Setups the widget
		void setupWidget(void);

		QVBoxLayout *		my_mainLayout;		//! The main layout of the widget
		QHBoxLayout *		my_inputLayout;		//! The layout of the input
		QWidget *			my_inputWidget;		//! The widget of the input

		aCalendarWidget *	my_calendar;		//! The widget of the calendar
		aPushButtonWidget *	my_buttonOk;		//! The button to confirm the results
		aPushButtonWidget *	my_buttonCancel;	//! The button to cancel the widget
	};
}
