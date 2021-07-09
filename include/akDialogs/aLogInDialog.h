/*
 *	File:		aLogInDialog.h
 *	Package:	akDialogs
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
			aColorStyle *			_colorStyle
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

	signals:
		void logInRequested(void);

	private:

		void createToolTip(
			QWidget *				_parent,
			const QString &			_text
		) const;


		std::map<ak::ID, aLogInDialogInputField *>	m_customInputFields;
		
		int										m_rowCounter;
		ID										m_currentID;
		bool									m_showSavePassword;

		QString									m_hashedPw;

		QPixmap *								m_bgImage;
		QPixmap									m_currentImage;

		QVBoxLayout *							m_mainLayout;
		QSpacerItem *							m_spacer;


		QGridLayout *							m_gridLayout;
		QWidget *								m_gridWidget;

		QHBoxLayout *							m_layout;
		QWidget *								m_layoutWidget;

		aCheckBoxWidget *						m_savePassword;
		aLogInDialogInputField *				m_inputUsername;
		aLogInDialogInputField *				m_inputPassword;
		aPushButtonWidget *						m_buttonLogIn;
		QVBoxLayout *							m_controlLayout;
		QWidget *								m_controlLayoutWidget;

		HashAlgorithm							m_passwordHashAlgorithm;

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

		aLineEditWidget * getInput(void) const { return m_input; }

		aLabelWidget * getLabel(void) const { return m_label; }

		void clearInput(void);

		QString text(void) const;

		//! @brief Will set the current color style for this combination
		void setColorStyle(
			aColorStyle *		_colorStyle
		);

	private:
		aLineEditWidget *			m_input;
		aLabelWidget *				m_label;

		aLogInDialogInputField() = delete;
		aLogInDialogInputField(const aLogInDialogInputField &) = delete;
		aLogInDialogInputField & operator = (aLogInDialogInputField &) = delete;
	};

}