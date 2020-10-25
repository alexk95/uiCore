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

// Qt header
#include <qstring.h>
#include <qpixmap.h>

// AK header
#include <ak_ui_core_aPaintable.h>
#include <ak_ui_core_aDialog.h>

// Forward declaration
class QWidget;
class QLabel;
class QCheckBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QToolTip;
class QPaintEvent;
class QPixmap;
class QSpacerItem;

namespace ak {

	class messenger;

	namespace ui {

		namespace qt { class lineEdit; }

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
				/*logIn(
					ak::messenger *								_messenger,
					bool										_showSavePassword,
					const QString &								_username = QString(),
					const QString &								_password = QString(),
					QWidget *									_parent = nullptr
				);
				*/

				//! @brief Constructor
				//! @param _messenger The external messenger used to send event messages
				//! @param _showSavePassword if true the save password checkbox will be displayed
				//! @param _username The initial username
				//! @param _password The initial password
				//! @param _parent The parent widget
				logIn(
					ak::messenger *								_messenger,
					bool										_showSavePassword,
					const QPixmap &								_backgroundImage,
					const QString &								_username = QString(),
					const QString &								_password = QString(),
					QWidget *									_parent = nullptr
				);

				//! @brief Deconstructor
				virtual ~logIn();

				//! @brief Will set the current color style for this dialog
				virtual void setColorStyle(
					const ak::ui::colorStyle *			_colorStyle
				) override;

				virtual void paintEvent(QPaintEvent *pe) override;

				void close(
					ui::core::dialogResult				_result
				);

				// ####################################################################

				// Getter

				//! @brief Returns the current username
				QString username(void) const;

				//! @brief Returns the current password
				QString password(void) const;

				//! @brief Returns the save password state
				bool savePassword(void) const;

				//! @brief Will display a invalid log in popup message at this dialogs username input
				//! @param _text The text to set as a tooltip
				void showToolTipAtUsername(
					const QString &							_text
				);

				//! @brief Will display a invalid log in popup message at this dialogs password input
				//! @param _text The text to set as a tooltip
				void showToolTipAtPassword(
					const QString &							_text
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

				struct structInput
				{
					QLabel *				label;
					ak::ui::qt::lineEdit *	edit;
				};

				QString				my_hashedPw;

				ak::messenger *		my_messenger;

				QPixmap *			my_bgImage;
				QPixmap				my_currentImage;

				QVBoxLayout *		my_mainLayout;
				QSpacerItem *		my_spacer;


				QGridLayout *		my_gridLayout;
				QWidget *			my_gridWidget;

				QHBoxLayout *		my_layout;
				QWidget *			my_layoutWidget;

				QCheckBox *			my_savePassword;
				structInput			my_inputUsername;
				structInput			my_inputPassword;
				QPushButton *		my_buttonLogIn;
				QVBoxLayout *		my_controlLayout;
				QWidget *			my_controlLayoutWidget;

				logIn(const logIn &) = delete;
				logIn & operator = (const logIn &) = delete;
			};

		}
	}
}