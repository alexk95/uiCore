/*
 * ak_ui_dialog_logIn.cpp
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

// AK header
#include <ak_ui_dialog_logIn.h>		// corresponding header
#include <ak_ui_colorStyle.h>
#include <ak_ui_core.h>
#include <ak_messenger.h>			// messenger

// Qt header
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qtooltip.h>

//#include <qcryptographichash.h>


ak::ui::dialog::logIn::logIn(
	ak::messenger *								_messenger,
	bool										_showSavePassword,
	const QString &								_username,
	const QString &								_password,
	QWidget *									_parent
)	: ui::core::aDialog(_parent), ak::ui::core::aPaintable(ui::core::objectType::oLogInDialog),
	my_buttonLogIn(nullptr), my_layout(nullptr), my_gridLayout(nullptr), my_savePassword(nullptr), my_messenger(_messenger)
{
	assert(my_messenger != nullptr);

	// Initialize entries
	my_inputPassword.label = nullptr;
	my_inputPassword.edit = nullptr;

	my_inputUsername.label = nullptr;
	my_inputUsername.edit = nullptr;

	my_gridWidget = new QWidget();
	my_gridLayout = new QGridLayout(my_gridWidget);

	int rowCounter = 0;

	// Create username objects
	my_inputUsername.label = new QLabel("Username:");
	my_inputUsername.edit = new QLineEdit(_username);
	my_inputUsername.label->setBuddy(my_inputUsername.edit);

	// Create password objects
	my_inputPassword.label = new QLabel("Password:");
	my_inputPassword.edit = new QLineEdit(_password);
	my_inputPassword.edit->setEchoMode(QLineEdit::EchoMode::Password);
	my_inputPassword.label->setBuddy(my_inputPassword.edit);

	// Add inputs to layout
	my_gridLayout->addWidget(my_inputUsername.label, rowCounter, 0);
	my_gridLayout->addWidget(my_inputUsername.edit, rowCounter++, 1);
	my_gridLayout->addWidget(my_inputPassword.label, rowCounter, 0);
	my_gridLayout->addWidget(my_inputPassword.edit, rowCounter++, 1);

	// Check if required to create save password
	if (_showSavePassword) {
		my_savePassword = new QCheckBox("Save password");
		my_gridLayout->addWidget(my_savePassword, rowCounter++, 1);
	}

	// Create log in button
	my_buttonLogIn = new QPushButton("Log-in", this);
	connect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));

	// Create main layout and display data
	my_layout = new QVBoxLayout(this);

	my_layout->addWidget(my_gridWidget);
	my_layout->addWidget(my_buttonLogIn);
	
	setLayout(my_layout);

	setWindowTitle("Welcome");
	// Hide info button
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

ak::ui::dialog::logIn::~logIn() {
	if (my_inputPassword.label != nullptr) { delete my_inputPassword.label; }
	if (my_inputPassword.edit != nullptr) { delete my_inputPassword.edit; }

	if (my_inputUsername.label != nullptr) { delete my_inputUsername.label; }
	if (my_inputUsername.edit != nullptr) { delete my_inputUsername.edit; }

	if (my_savePassword != nullptr) { delete my_savePassword; }

	if (my_buttonLogIn != nullptr) {
		disconnect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));
		delete my_buttonLogIn;
	}

	if (my_gridLayout != nullptr) { delete my_gridLayout; }
	if (my_gridWidget != nullptr) { delete my_gridWidget; }

	if (my_layout != nullptr) { delete my_layout; }

}

void ak::ui::dialog::logIn::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sMainWindow));

	my_buttonLogIn->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sPushButton));
	my_gridWidget->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sWidget));

	my_inputUsername.label->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLabel));
	my_inputUsername.edit->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLineEdit));

	my_inputPassword.label->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLabel));
	my_inputPassword.edit->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLineEdit));

}

// ####################################################################

// Getter

QString ak::ui::dialog::logIn::username(void) const { return my_inputUsername.edit->text(); }

QString ak::ui::dialog::logIn::password(void) const { return my_inputPassword.edit->text(); }

bool ak::ui::dialog::logIn::savePassword(void) const {
	if (my_savePassword == nullptr) { return false; }
	else { return my_savePassword->isChecked(); }
}

void ak::ui::dialog::logIn::slotClicked(void) {
	assert(my_inputPassword.edit != nullptr);
	assert(my_inputUsername.edit != nullptr);

	QString pw(my_inputPassword.edit->text());
	QString user(my_inputUsername.edit->text());
	
	// Check if something was entered
	if (user.length() == 0) {
		createToolTip(my_inputUsername.edit, "Please enter a username");
		return;
	}
	if (pw.length() == 0) {
		createToolTip(my_inputPassword.edit, "Please enter a password");
		return;
	}

	my_messenger->sendMessage(my_uid, ak::core::eventType::eClicked);

}

void ak::ui::dialog::logIn::createToolTip(
	QWidget *				_parent,
	const QString &			_text
) const {
	QToolTip::showText(_parent->mapToGlobal(QPoint(0, 0)), _text, nullptr, QRect(), 3000);
}