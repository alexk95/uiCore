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
class QWidget;
class QSpinBox;

namespace ak {

	class aLabelWidget;
	class aPushButtonWidget;

	class UICORE_API_EXPORT aTimePickWidget : public QPushButton, public aWidget {
		Q_OBJECT
	public:

		aTimePickWidget();
		aTimePickWidget(const QTime & _time, timeFormat _timeFormat);
		virtual ~aTimePickWidget();

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

		//! @brief Will return the currently selected time
		//! @return QTime of the selected time
		QTime currentTime(void) const { return my_time; }

		//! @brief Will return the currently selected time format
		//! @return The selected time format
		timeFormat getTimeFormat(void) const { return my_timeFormat; }

		// #############################################################################################################################

		// Setter

		//! @brief Will set the currently selected time
		//! @param _time The date to set
		//! @param _refresh If true, the object will be repainted according to the current changes
		void setCurrentTime(const QTime & _time, bool _refresh = true);

		//! @brief Will set the time format
		//! @param _timeFormat The time format to set
		//! @param _refresh If true, the object will be repainted according to the current changes
		void setTimeFormat(timeFormat _timeFormat, bool _refresh = true);

	signals:

		//! @brief Sends a signal, when changes occured
		void changed(void);

	private slots:

		//! @brief When the slot is clicked, the dialog to select a date appears
		void slotClicked(void);

	private:

		//! @brief The time to set
		//! @throw std::exception If the number of the minute is a single digit, a 0 will be preprended to the number
		//! @throw std::exception If the number of the second is a single digit, a 0 will be preprended to the number
		//! @throw std::exception If the number of the milli second is a single digit, a 0 will be preprended to the number
		//! @throw std::exception If the user lives in different countrys with other time format, the format can be changed
		void refreshTime(void);

		QTime					my_time;			//! The time
		timeFormat				my_timeFormat;		//! The time format
	};

	// #################################################################################################################################

	// #################################################################################################################################

	// #################################################################################################################################

	class aTimePickDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:

		aTimePickDialog();
		aTimePickDialog(const QTime & _time, timeFormat _timeFormat);
		virtual ~aTimePickDialog();

		// #############################################################################################################################

		// Base Class function

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			const aColorStyle *	_colorStyle
		) override;

		// #############################################################################################################################

		// Getter

		//! @brief Will return the currently sected date
		QTime selectedTime(void) const;

	private slots:

		//! @brief When the slot is clicked, the dialog closes with current results
		void slotOkClicked(void);

		//! @brief When the slot is clicked, the dialog is canceled without results
		void slotCancelClicked(void);

	private:

		//! @brief Setups the widget
		void setupWidget(void);

		QVBoxLayout *				my_mainLayout;		//! The main layout of the widget

		QHBoxLayout *				my_inputLayout;		//! The layout of the input
		QWidget *					my_inputWidget;		//! The widget of the input

		QHBoxLayout *				my_buttonLayout;	//! The layout of the button
		QWidget *					my_buttonWidget;	//! The widget of the button

		aLabelWidget *				my_hourLabel;		//! The label of the hours
		QSpinBox *					my_hourInput;		//! The button to select the hour

		aLabelWidget *				my_minLabel;		//! The label of the minutes
		QSpinBox *					my_minInput;		//! The button to select the minutes

		aPushButtonWidget *			my_buttonOk;		//! The button to confirm the results
		aPushButtonWidget *			my_buttonCancel;	//! The button to cancel the widget

	};
}
