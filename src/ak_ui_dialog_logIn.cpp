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
#include <qcryptographichash.h>		// Hashing the password
#include <qwidget.h>				// QWidget
#include <qlabel.h>					// QLabel
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qcheckbox.h>
#include <qtooltip.h>

#include <qpainter.h>

ak::ui::dialog::logIn::logIn(
	ak::messenger *								_messenger,
	bool										_showSavePassword,
	const QPixmap &								_backgroundImage,
	const QString &								_username,
	const QString &								_hashedPassword,
	QWidget *									_parent
)	: ui::core::aDialog(_parent), ak::ui::core::aPaintable(ui::core::objectType::oLogInDialog), my_hashedPw(_hashedPassword),
	my_buttonLogIn(nullptr), my_layout(nullptr), my_spacer(nullptr), my_layoutWidget(nullptr), my_gridLayout(nullptr), my_savePassword(nullptr),
	my_messenger(_messenger), my_mainLayout(nullptr), my_controlLayout(nullptr), my_controlLayoutWidget(nullptr)
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
	my_inputPassword.edit = new QLineEdit();
	my_inputPassword.edit->setEchoMode(QLineEdit::EchoMode::Password);
	if (_hashedPassword.length() > 0) { my_inputPassword.edit->setText("xxxxxxxxxx"); }
	my_inputPassword.label->setBuddy(my_inputPassword.edit);

	// Add inputs to layout
	my_gridLayout->addWidget(my_inputUsername.label, rowCounter, 0);
	my_gridLayout->addWidget(my_inputUsername.edit, rowCounter++, 1);
	my_gridLayout->addWidget(my_inputPassword.label, rowCounter, 0);
	my_gridLayout->addWidget(my_inputPassword.edit, rowCounter++, 1);

	// Check if required to create save password
	if (_showSavePassword) {
		my_savePassword = new QCheckBox("Save password");
		my_savePassword->setChecked(true);
		my_gridLayout->addWidget(my_savePassword, rowCounter++, 1);
	}

	// Create log in button
	my_buttonLogIn = new QPushButton("Log-in", this);
	
	// Connect signals
	connect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));
	connect(my_inputPassword.edit, SIGNAL(textChanged(const QString &)), this, SLOT(slotPasswordChanged(const QString &)));
	connect(my_inputUsername.edit, SIGNAL(textChanged(const QString &)), this, SLOT(slotUsernameChanged(const QString &)));

	// Create main layout and display data
	my_controlLayout = new QVBoxLayout(this);

	my_controlLayout->addWidget(my_gridWidget);
	my_controlLayout->addWidget(my_buttonLogIn);

	my_controlLayoutWidget = new QWidget();
	my_controlLayoutWidget->setLayout(my_controlLayout);
	my_controlLayoutWidget->setObjectName("ABC");
	my_controlLayoutWidget->setStyleSheet("#ABC{"
		"background-color:#90000000;"
		"border-radius:10px;"

		"}\n"
		"QLabel{color:#FFFFFF}\n"
		"QCheckBox{color:#FFFFFF}\n");
	my_bgImage = new QPixmap(_backgroundImage);

	
	// Create layout
	my_layoutWidget = new QWidget();
	my_layout = new QHBoxLayout(my_layoutWidget);
	my_layout->addStretch(1);
	my_layout->addWidget(my_controlLayoutWidget, 0, Qt::AlignmentFlag::AlignCenter);
	my_layout->addStretch(1);

	// Create spacer
	my_mainLayout = new QVBoxLayout();
	my_mainLayout->addStretch(1);
	my_mainLayout->addWidget(my_layoutWidget, 0, Qt::AlignmentFlag::AlignBottom);

	setLayout(my_mainLayout);
	
	setWindowTitle("Welcome");
	// Hide info button
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	resize(my_bgImage->size());
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

	if (my_controlLayoutWidget != nullptr) { delete my_controlLayoutWidget; }

}

void ak::ui::dialog::logIn::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
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

void ak::ui::dialog::logIn::paintEvent(QPaintEvent *pe)
{
	QPainter paint(this);
	my_currentImage = my_bgImage->scaled(width(), height(), Qt::KeepAspectRatioByExpanding);
	QPoint centerOfWidget = rect().center();
	QRect rectOfPixmap = my_currentImage.rect();
	rectOfPixmap.moveCenter(centerOfWidget);
	paint.drawPixmap(rectOfPixmap.topLeft(), my_currentImage);
}

// ####################################################################

// Getter

QString ak::ui::dialog::logIn::username(void) const { return my_inputUsername.edit->text(); }

QString ak::ui::dialog::logIn::password(void) const {
	if (my_hashedPw.length() > 0) { return my_hashedPw; }
	QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
	QString txt(my_inputPassword.edit->text());
	std::string str(txt.toStdString());
	hash.addData(str.c_str(), str.length());
	QByteArray arr(hash.result());
	QByteArray result(arr.toHex());
	std::string ret(result.toStdString());
	return QString(ret.c_str());
}

bool ak::ui::dialog::logIn::savePassword(void) const {
	if (my_savePassword == nullptr) { return false; }
	else { return my_savePassword->isChecked(); }
}

void ak::ui::dialog::logIn::showInvalidLogIn(void) {
	createToolTip(my_inputPassword.edit, "Invalid username and password combination");
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

void ak::ui::dialog::logIn::slotUsernameChanged(const QString & _text) {
	if (my_hashedPw.length() > 0) { my_hashedPw.clear(); my_inputPassword.edit->clear(); }
}

void ak::ui::dialog::logIn::slotPasswordChanged(const QString &	_text) {
	if (my_hashedPw.length() > 0) {
		my_hashedPw.clear();
		if (my_inputPassword.edit->text().length() == 0) { return; }
		for (auto c : _text) {
			if (c != 'x') {
				my_inputPassword.edit->setText(c); return;
			}
		}
		my_inputPassword.edit->setText("x");
	}
}

void ak::ui::dialog::logIn::createToolTip(
	QWidget *				_parent,
	const QString &			_text
) const {
	QToolTip::showText(_parent->mapToGlobal(QPoint(0, 0)), _text, nullptr, QRect(), 3000);
}