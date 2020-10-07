/*
 * ak_ui_dialog_logIn.h
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// C++ header
#include <vector>
#include <array>

// AK header
#include <ak_ui_core_aPaintable.h>
#include <ak_ui_core_aDialog.h>

// Forward declaration
class QWidget;
class QLabel;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QToolTip;

namespace ak {
	namespace ui {
		namespace dialog {

			class logIn : public ak::ui::core::aDialog, public ak::ui::core::aPaintable {
				
				Q_OBJECT

			public:
				//! @brief Constructor
				//! @param _validLogIns The valid possible combinations of username and password (Must provide at least one entry)
				//! @param _showSavePassword if true the save password checkbox will be displayed
				//! @param _username The initial username
				//! @param _password The initial password
				//! @param _maxLogInAttempts If a value greater than 0 is provided, this value will represent the maximum possible log in attempts before the dialog will close automatically
				//! @param _usernameCaseSensitive If true the username awill be checked case sensitive
				//! @param _parent The parent widget
				logIn(
					const std::vector<std::array<QString, 2>> &	_validLogIns,
					bool										_showSavePassword,
					const QString &								_username,
					const QString &								_password,
					int											_maxLogInAttempts = 0,
					bool										_usernameCaseSensitive = false,
					QWidget *									_parent = nullptr
				);

				//! @brief Deconstructor
				virtual ~logIn();

				//! @brief Will set the current color style for this dialog
				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;

				// ####################################################################

				// Getter

				//! @brief Returns the current username
				QString username(void) const;

				//! @brief Returns the current password
				QString password(void) const;

				//! @brief Returns the save password state
				bool savePassword(void) const;

			private slots:
				//! @brief Callback for the log in button
				void slotClicked(void);

			private:

				void createToolTip(
					QWidget *				_parent,
					const QString &			_text
				) const;

				int										my_logInAttempt;			//! The current log in attempt
				int										my_maxLogInAttempts;		//! The maximum ammount of log in attempts
				bool									my_usernameCaseSensitive;	//! If true the username will be checked case sensitive
				std::vector<std::array<QString, 2>>		my_validLogIns;				//! Contains all valid log in combinations
				QToolTip *								my_toolTip;

				struct structInput
				{
					QLabel *		label;
					QLineEdit *		edit;
				};

				QGridLayout *		my_gridLayout;
				QWidget *			my_gridWidget;

				QCheckBox *			my_savePassword;
				structInput			my_inputUsername;
				structInput			my_inputPassword;
				QPushButton *		my_buttonLogIn;
				QVBoxLayout *		my_layout;

				logIn(const logIn &) = delete;
				logIn & operator = (const logIn &) = delete;
			};

		}
	}
}