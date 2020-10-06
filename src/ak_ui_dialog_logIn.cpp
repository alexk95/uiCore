/*
 * ak_ui_dialog_logIn.cpp
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_dialog_logIn.h>		// corresponding header
#include <ak_ui_colorStyle.h>
#include <ak_ui_core.h>

// Qt header
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>

ak::ui::dialog::logIn::logIn(
	QWidget *						_parent
)	: ui::core::aDialog(_parent), my_buttonLogIn(nullptr), my_layout(nullptr),
	ak::ui::core::aPaintable(ui::core::objectType::oLogInDialog)
{
	// Initialize entries
	my_username.label = nullptr;
	my_username.layout = nullptr;
	my_username.lineEdit = nullptr;
	my_username.widget = nullptr;

	my_password.label = nullptr;
	my_password.layout = nullptr;
	my_password.lineEdit = nullptr;
	my_password.widget = nullptr;

	// Create username objects
	my_username.widget = new QWidget(this);
	my_username.layout = new QHBoxLayout(my_username.widget);
	my_username.label = new QLabel("Password:");
	my_username.lineEdit = new QLineEdit();
	my_username.label->setBuddy(my_username.lineEdit);
	my_username.layout->addWidget(my_username.label);
	my_username.layout->addWidget(my_username.lineEdit);
	my_username.widget->setLayout(my_username.layout);

	// Create password objects
	my_password.widget = new QWidget(this);
	my_password.layout = new QHBoxLayout(my_password.widget);
	my_password.label = new QLabel("Password:");
	my_password.lineEdit = new QLineEdit();
	my_password.lineEdit->setEchoMode(QLineEdit::EchoMode::Password);
	my_password.label->setBuddy(my_password.lineEdit);
	my_password.layout->addWidget(my_password.label);
	my_password.layout->addWidget(my_password.lineEdit);
	my_password.widget->setLayout(my_password.layout);

	my_buttonLogIn = new QPushButton("Log-in", this);
	connect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));

	my_layout = new QVBoxLayout(this);
	my_layout->addWidget(my_username.widget);
	my_layout->addWidget(my_password.widget);
	my_layout->addWidget(my_buttonLogIn);
	
	setLayout(my_layout);

}

ak::ui::dialog::logIn::~logIn() {
	if (my_username.label != nullptr) { delete my_username.label; }
	if (my_username.lineEdit != nullptr) { delete my_username.lineEdit; }
	if (my_username.layout != nullptr) { delete my_username.layout; }
	if (my_username.widget != nullptr) { delete my_username.widget; }

	if (my_password.label != nullptr) { delete my_password.label; }
	if (my_password.lineEdit != nullptr) { delete my_password.lineEdit; }
	if (my_password.layout != nullptr) { delete my_password.layout; }
	if (my_password.widget != nullptr) { delete my_password.widget; }

	disconnect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));
	if (my_buttonLogIn != nullptr) { delete my_buttonLogIn; }

	if (my_layout != nullptr) { delete my_layout; }

}

void ak::ui::dialog::logIn::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sMainWindow));

	my_buttonLogIn->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sPushButton));
	my_password.widget->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sWidget));
	my_password.label->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLabel));
	my_password.lineEdit->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLineEdit));

	my_username.widget->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sWidget));
	my_username.label->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLabel));
	my_username.lineEdit->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::sLineEdit));

}

// ####################################################################

// Setter

void ak::ui::dialog::logIn::setUsername(
	const QString &		_username
) { my_username.lineEdit->setText(_username); }

void ak::ui::dialog::logIn::setPassword(
	const QString &		_password
) { my_password.lineEdit->setText(_password); }

// ####################################################################

// Getter

QString ak::ui::dialog::logIn::username(void) const { return my_username.lineEdit->text(); }

QString ak::ui::dialog::logIn::password(void) const { return my_password.lineEdit->text(); }

void ak::ui::dialog::logIn::slotClicked(void) { my_result = ak::ui::core::resultOk; close(); }