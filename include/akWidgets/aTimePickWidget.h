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

namespace ak {

	class aLabelWidget;
	class aPushButtonWidget;
	class aSpinBoxWidget;

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
			aColorStyle *	_colorStyle
		) override;

		// #############################################################################################################################

		// Getter

		//! @brief Will return the currently selected time
		//! @return QTime of the selected time
		QTime currentTime(void) const { return my_time; }

		//! @brief Will return the currently selected time format
		//! @return The selected time format
		timeFormat getTimeFormat(void) const { return my_timeFormat; }

		//! @brief Will return the minute step
		int minuteStep(void) const { return m_minuteStep; }

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
		
		//! @brief Will set the provided step length as a minute step
		void setMinuteStep(int _step);

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
		int						m_minuteStep;		//! The step size for a single minute step
	};

	// #################################################################################################################################

	// #################################################################################################################################

	// #################################################################################################################################

	class UICORE_API_EXPORT aTimePickDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:

		aTimePickDialog(aTimePickWidget * _owner, timeFormat _timeFormat = tfHHMM);
		aTimePickDialog(const QTime & _time, aTimePickWidget * _owner, timeFormat _timeFormat = tfHHMM);
		virtual ~aTimePickDialog();

		// #############################################################################################################################

		// Base Class function

		//! @brief Will set the objects color style
		//! @param _colorStyle The color style to set
		//! @throw ak::Exception if the provided color style is a nullptr or failed to repaint the object
		virtual void setColorStyle(
			aColorStyle *	_colorStyle
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

		aTimePickWidget *			m_owner;			//! The owner of this time pick dialog

		//! @brief Setups the widget
		void setupWidget(timeFormat _timeFormat);

		timeFormat					m_timeFormat;		//! The current time format of the time pick dialog

		QVBoxLayout *				my_mainLayout;		//! The main layout of the widget

		QHBoxLayout *				my_inputLayout;		//! The layout of the input
		QWidget *					my_inputWidget;		//! The widget of the input

		QHBoxLayout *				my_buttonLayout;	//! The layout of the button
		QWidget *					my_buttonWidget;	//! The widget of the button

		aSpinBoxWidget *			my_hourInput;		//! The spinbox to select the hour
		aSpinBoxWidget *			my_minInput;		//! The button to select the minutes
		aSpinBoxWidget *			m_secInput;			//! The spinbox to select the seconds
		aSpinBoxWidget *			m_msecInput;		//! The spinbox to select the milliseconds

		aPushButtonWidget *			my_buttonOk;		//! The button to confirm the results
		aPushButtonWidget *			my_buttonCancel;	//! The button to cancel the widget

	};
}
