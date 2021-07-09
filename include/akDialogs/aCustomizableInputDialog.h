/*
 *	File:		aCustomizableInputDialog.h
 *	Package:	akDialogs
 *
 *  Created on: July 09, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// uiCore header
#include <akCore/globalDataTypes.h>
#include <akGui/aDialog.h>
#include <akGui/aPaintable.h>

// C++ header
#include <vector>

class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;

namespace ak {
	class aWidget;
	class aPushButtonWidget;
	class aLabelWidget;
	class aLineEditWidget;
	class aColorStyle;

	class UICORE_API_EXPORT aCustomizableInputDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:
		aCustomizableInputDialog(QWidget * _parent = (QWidget *) nullptr);
		virtual ~aCustomizableInputDialog();

		//! @brief Will set the current color style for this dialog and all controls
		//! @param _style The color style to set
		virtual void setColorStyle(aColorStyle * _style) override;

		// #################################################################################

		// Item adder

		//! @brief Will add a new button to the dialog
		//! Will return the buttons local UID
		//! @param _buttonText The button text
		UID addButton(const QString& _buttonText);

		//! @brief Will add a new text input
		//! Will return the inputs local UID
		//! @param _label The label text
		//! @param _initialText The initial text of the text edit
		//! @param _placeholder The placeholder text
		//! @param _toolTip The tooltip text
		UID addTextInput(const QString& _label, const QString& _initialText, const QString& _placeholder = QString(), const QString& _toolTip = QString());

		//! @brief Will add a new text input as a password input
		//! Will return the inputs local UID
		//! @param _label The label text
		//! @param _initialText The initial text of the text edit
		//! @param _placeholder The placeholder text
		//! @param _toolTip The tooltip text
		UID addPasswordInput(const QString& _label, const QString& _initialText, const QString& _placeholder = QString(), const QString& _toolTip = QString());

		//! @brief Will add the custom widget
		//! Will return the inputs local UID.
		//! The dialog will take over the widget.
		//! @param _label The label text
		//! @param _widget The widget to add
		UID addCustomInput(const QString& _label, aWidget * _widget);

	signals:
		void buttonClicked(UID _buttonUID);
		void editFinished(UID _editUID);

	private slots:
		void slotButtonClicked(void);
		void slotInputChanged(void);

	private:

		std::vector<aPushButtonWidget *>		m_buttons;			//! All buttons that are placed in this dialog
		std::vector<std::pair<aLabelWidget *, 
			aWidget *>>							m_inputs;			//! All inputs that are placed in this dialog

		UID										m_currentUid;		//! The current item UID
		int										m_currentInputRow;	//! The current row index

		QVBoxLayout *							m_centralLayout;	//! The central layout of the dialog

		QWidget *								m_inputsLayoutW;	//! The layout widget for the inputs
		QGridLayout *							m_inputsLayout;		//! The inputs layout

		QWidget *								m_buttonLayoutW;	//! The layout widget for the buttons
		QHBoxLayout *							m_buttonLayout;		//! The buttons layout
			
		aCustomizableInputDialog(aCustomizableInputDialog&) = delete;
		aCustomizableInputDialog& operator = (aCustomizableInputDialog&) = delete;

	};

}