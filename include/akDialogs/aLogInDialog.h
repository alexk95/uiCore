/*
 * ak_ui_dialog_logIn.h
 *
 *  Created on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#pragma once

// C++ header
#include <vector>
#include <array>
#include <map>

// Qt header
#include <qstring.h>
#include <qpixmap.h>

// AK header
#include <akCore/globalDataTypes.h>
#include <akGui/aPaintable.h>
#include <akGui/aDialog.h>

// Forward declaration
class QWidget;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QToolTip;
class QPaintEvent;
class QPixmap;
class QSpacerItem;

namespace ak {

	class aMessenger;
	class aLineEditWidget;
	class aCheckBoxWidget;
	class aLabelWidget;
	class aPushButtonWidget;
	class aLogInDialogInputField;

	class UICORE_API_EXPORT aLogInDialog : public aDialog, public aPaintable {
		Q_OBJECT
	public:
		//! @brief Constructor
		//! @param _messenger The external messenger used to send event messages
		//! @param _showSavePassword if true the save password checkbox will be displayed
		//! @param _username The initial username
		//! @param _password The initial password
		//! @param _parent The parent widget
		aLogInDialog(
			aMessenger *				_messenger,
			bool						_showSavePassword,
			const QPixmap &				_backgroundImage,
			const QString &				_username = QString(),
			const QString &				_password = QString(),
			QWidget *					_parent = nullptr
		);

		//! @brief Deconstructor
		virtual ~aLogInDialog();

		//! @brief Will set the current color style for this dialog
		virtual void setColorStyle(
			const aColorStyle *			_colorStyle
		) override;

		virtual void paintEvent(QPaintEvent * _event) override;

		void close(
			dialogResult				_result
		);

		ak::ID addCustomInput(
			const QString &				_labelText,
			const QString &				_initialInputText = QString("")
		);


		// #########################################################################################################################

		// Getter

		//! @brief Returns the current username
		QString username(void) const;

		//! @brief Returns the current password
		QString password(void) const;

		//! @brief Will return the text of the custom input with the specified ID
		//! @param _id The ID of the custom input
		QString customInputText(ID _id);

		//! @brief Returns the save password state
		bool savePassword(void) const;

		//! @brief Will display a invalid log in popup message at this dialogs username input
		//! @param _text The text to set as a tooltip
		void showToolTipAtUsername(
			const QString &				_text
		);

		//! @brief Will display a invalid log in popup message at this dialogs password input
		//! @param _text The text to set as a tooltip
		void showToolTipAtPassword(
			const QString &				_text
		);

		void showToolTipAtCustomInput(
			ID							_inputID,
			const QString &				_text
		);

	private slots:
		//! @brief Callback for the log in button
		void slotClicked(void);

		void slotUsernameChanged(const QString & _text);
		void slotPasswordChanged(const QString & _text);

	private:

		void createToolTip(
			QWidget *				_parent,
			const QString &			_text
		) const;


		std::map<ak::ID, aLogInDialogInputField *>	my_customInputFields;
		
		int										my_rowCounter;
		ID										my_currentID;
		bool									my_showSavePassword;

		QString									my_hashedPw;

		aMessenger *							my_messenger;
		
		QPixmap *								my_bgImage;
		QPixmap									my_currentImage;

		QVBoxLayout *							my_mainLayout;
		QSpacerItem *							my_spacer;


		QGridLayout *							my_gridLayout;
		QWidget *								my_gridWidget;

		QHBoxLayout *							my_layout;
		QWidget *								my_layoutWidget;

		aCheckBoxWidget *						my_savePassword;
		aLogInDialogInputField *				my_inputUsername;
		aLogInDialogInputField *				my_inputPassword;
		aPushButtonWidget *						my_buttonLogIn;
		QVBoxLayout *							my_controlLayout;
		QWidget *								my_controlLayoutWidget;

		HashAlgorithm							my_passwordHashAlgorithm;

		aLogInDialog(const aLogInDialog &) = delete;
		aLogInDialog & operator = (const aLogInDialog &) = delete;
	};

	// #########################################################################################################################

	class aLogInDialogInputField {
	public:
		aLogInDialogInputField(
			aLineEditWidget *		_input,
			aLabelWidget *			_label
		);

		aLineEditWidget * getInput(void) const { return my_input; }

		aLabelWidget * getLabel(void) const { return my_label; }

		void clearInput(void);

		QString text(void) const;

		//! @brief Will set the current color style for this combination
		void setColorStyle(
			const aColorStyle *		_colorStyle
		);

	private:
		aLineEditWidget *			my_input;
		aLabelWidget *				my_label;

		aLogInDialogInputField() = delete;
		aLogInDialogInputField(const aLogInDialogInputField &) = delete;
		aLogInDialogInputField & operator = (aLogInDialogInputField &) = delete;
	};

}