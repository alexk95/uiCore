/*
 * ak_ui_dialog_logIn.h
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#pragma once

// C++ header
#include <vector>
#include <array>

// Qt header
#include <qstring.h>

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

	class messenger;

	namespace ui {
		namespace dialog {

			class logIn : public ak::ui::core::aDialog, public ak::ui::core::aPaintable {
				
				Q_OBJECT

			public:
				//! @brief Constructor
				//! @param _messenger The external messenger used to send event messages
				//! @param _showSavePassword if true the save password checkbox will be displayed
				//! @param _username The initial username
				//! @param _password The initial password
				//! @param _parent The parent widget
				logIn(
					ak::messenger *								_messenger,
					bool										_showSavePassword,
					const QString &								_username = QString(),
					const QString &								_password = QString(),
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

				//! @brief Will display a invalid log in popup message at this dialog
				void showInvalidLogIn(void);

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

				struct structInput
				{
					QLabel *		label;
					QLineEdit *		edit;
				};

				QString				my_hashedPw;

				ak::messenger *		my_messenger;

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