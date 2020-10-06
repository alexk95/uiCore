/*
 * ak_ui_dialog_logIn.h
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#pragma once

// AK header
#include <ak_ui_core_aPaintable.h>
#include <ak_ui_core_aDialog.h>

class QWidget;
class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

namespace ak {
	namespace ui {
		namespace dialog {

			class logIn : public ak::ui::core::aDialog, public ak::ui::core::aPaintable {
				
				Q_OBJECT

			public:
				logIn(
					QWidget *						_parent = nullptr
				);

				virtual ~logIn();

				virtual void setColorStyle(
					ak::ui::colorStyle *			_colorStyle
				) override;

				// ####################################################################

				// Setter

				void setUsername(
					const QString &		_username
				);

				void setPassword(
					const QString &		_password
				);

				// ####################################################################

				// Getter

				QString username(void) const;

				QString password(void) const;

			private slots:
				void slotClicked(void);

			private:

				struct structEntry {
					QWidget *		widget;
					QHBoxLayout *	layout;
					QLabel *		label;
					QLineEdit *		lineEdit;
				};

				structEntry			my_username;
				structEntry			my_password;
				QPushButton *		my_buttonLogIn;
				QVBoxLayout *		my_layout;

				logIn(const logIn &) = delete;
				logIn & operator = (const logIn &) = delete;
			};

		}
	}
}