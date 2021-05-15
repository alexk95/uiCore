/*
 * ak_ui_dialog_aLogInDialog.cpp
 *
 *  Created on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// Corresponding header
#include <akDialogs/aLogInDialog.h>

// AK Core header
#include <akCore/akCore.h>
#include <akCore/aMessenger.h>

// AK GUI header
#include <akGui/aColorStyle.h>

// AK Widgets header
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aCheckBoxWidget.h>

// Qt header
#include <qcryptographichash.h>		// Hashing the password
#include <qwidget.h>				// QWidget
#include <qlayout.h>				// QLayout
#include <qtooltip.h>				// QToolTip
#include <qpainter.h>

ak::aLogInDialog::aLogInDialog(
	aMessenger *								_messenger,
	bool										_showSavePassword,
	const QPixmap &								_backgroundImage,
	const QString &								_username,
	const QString &								_hashedPassword,
	QWidget *									_parent
) : aDialog(_parent), aPaintable(otLogInDialog), my_hashedPw(_hashedPassword),
	my_buttonLogIn(nullptr), my_layout(nullptr), my_spacer(nullptr), my_layoutWidget(nullptr), my_gridLayout(nullptr), my_savePassword(nullptr),
	my_messenger(_messenger), my_mainLayout(nullptr), my_controlLayout(nullptr), my_controlLayoutWidget(nullptr), my_inputPassword(nullptr), my_inputUsername(nullptr),
	my_currentID(ak::invalidID), my_rowCounter(0), my_showSavePassword(_showSavePassword), my_passwordHashAlgorithm(hashSha_256)
{
	assert(my_messenger != nullptr);

	// Initialize entries
	my_gridWidget = new QWidget();
	my_gridLayout = new QGridLayout(my_gridWidget);

	// Create username objects
	my_inputUsername = new aLogInDialogInputField(new aLineEditWidget(_username), new aLabelWidget("Username:"));

	// Create password objects
	aLineEditWidget * pwInput = new aLineEditWidget();
	my_inputPassword = new aLogInDialogInputField(pwInput, new aLabelWidget("Password:"));
	pwInput->setEchoMode(QLineEdit::EchoMode::Password);
	if (_hashedPassword.length() > 0) { pwInput->setText("xxxxxxxxxx"); }

	// Add inputs to layout
	my_gridLayout->addWidget(my_inputUsername->getLabel(), my_rowCounter, 0);
	my_gridLayout->addWidget(my_inputUsername->getInput(), my_rowCounter++, 1);
	my_gridLayout->addWidget(my_inputPassword->getLabel(), my_rowCounter, 0);
	my_gridLayout->addWidget(my_inputPassword->getInput(), my_rowCounter++, 1);

	// Check if required to create save password
	if (my_showSavePassword) {
		my_savePassword = new aCheckBoxWidget("Save password");
		my_savePassword->setChecked(true);
		my_gridLayout->addWidget(my_savePassword, my_rowCounter++, 1);
	}

	// Create log in button
	my_buttonLogIn = new aPushButtonWidget("Log-in", this);
	
	// Connect signals
	connect(my_buttonLogIn, &QPushButton::clicked, this, &aLogInDialog::slotClicked);
	connect(my_inputPassword->getInput(), &QLineEdit::textChanged, this, &aLogInDialog::slotPasswordChanged);
	connect(my_inputUsername->getInput(), &QLineEdit::textChanged, this, &aLogInDialog::slotUsernameChanged);

	// Create main layout and display data
	my_controlLayout = new QVBoxLayout(this);

	my_controlLayout->addWidget(my_gridWidget);
	my_controlLayout->addWidget(my_buttonLogIn);

	my_controlLayoutWidget = new QWidget();
	my_controlLayoutWidget->setLayout(my_controlLayout);
	my_controlLayoutWidget->setObjectName("LogInDialogControlLayoutWidget");
	my_controlLayoutWidget->setStyleSheet("#LogInDialogControlLayoutWidget{"
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

ak::aLogInDialog::~aLogInDialog() {
	A_OBJECT_DESTROYING

	delete my_inputPassword;
	delete my_inputUsername;

	for (auto itm : my_customInputFields) {
		delete itm.second;
	}
	my_customInputFields.clear();

	if (my_savePassword != nullptr) { delete my_savePassword; }

	if (my_buttonLogIn != nullptr) {
		disconnect(my_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));
		delete my_buttonLogIn;
	}

	if (my_gridLayout != nullptr) { delete my_gridLayout; }
	if (my_gridWidget != nullptr) { delete my_gridWidget; }

	if (my_controlLayoutWidget != nullptr) { delete my_controlLayoutWidget; }

}

void ak::aLogInDialog::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	setStyleSheet(my_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow));
	
	my_buttonLogIn->setColorStyle(my_colorStyle);

	my_inputUsername->setColorStyle(my_colorStyle);
	my_inputPassword->setColorStyle(my_colorStyle);

	my_controlLayoutWidget->setStyleSheet("#LogInDialogControlLayoutWidget{"
		"background-color:#90000000;"
		"border-radius:10px;"
		"}\n"
		"QLabel{color:#FFFFFF}\n"
		"QCheckBox{color:#FFFFFF}\n");

}

void ak::aLogInDialog::paintEvent(QPaintEvent *pe)
{
	QPainter paint(this);
	my_currentImage = my_bgImage->scaled(width(), height(), Qt::KeepAspectRatioByExpanding);
	QPoint centerOfWidget = rect().center();
	QRect rectOfPixmap = my_currentImage.rect();
	rectOfPixmap.moveCenter(centerOfWidget);
	paint.drawPixmap(rectOfPixmap.topLeft(), my_currentImage);
}

void ak::aLogInDialog::close(
	dialogResult				_result
) { setResult(_result); QDialog::close(); }

ak::ID ak::aLogInDialog::addCustomInput(
	const QString &						_labelText,
	const QString &						_initialInputText
) {
	// Reset grid layout
	assert(my_gridLayout != nullptr);
	delete my_gridLayout;
	my_gridLayout = nullptr;
	my_gridLayout = new QGridLayout(my_gridWidget);
	my_rowCounter = 0;
	
	// Create new item
	aLogInDialogInputField * newItem = new aLogInDialogInputField(new aLineEditWidget(_initialInputText), new aLabelWidget(_labelText));
	my_customInputFields.insert_or_assign(++my_currentID, newItem);
	
	// Place all widgets
	for (auto itm : my_customInputFields) {
		my_gridLayout->addWidget(itm.second->getLabel(), my_rowCounter, 0);
		my_gridLayout->addWidget(itm.second->getInput(), my_rowCounter++, 1);
	}

	my_gridLayout->addWidget(my_inputUsername->getLabel(), my_rowCounter, 0);
	my_gridLayout->addWidget(my_inputUsername->getInput(), my_rowCounter++, 1);

	my_gridLayout->addWidget(my_inputPassword->getLabel(), my_rowCounter, 0);
	my_gridLayout->addWidget(my_inputPassword->getInput(), my_rowCounter++, 1);

	if (my_showSavePassword) {
		my_gridLayout->addWidget(my_savePassword, my_rowCounter++, 1);
	}

	return my_currentID;
}

// #######################################################################################################################################################################

// Getter

QString ak::aLogInDialog::username(void) const { return my_inputUsername->text(); }

QString ak::aLogInDialog::password(void) const {
	if (my_hashedPw.length() > 0) { return my_hashedPw; }
	return hashString(my_inputPassword->text(), my_passwordHashAlgorithm);
}

QString ak::aLogInDialog::customInputText(ak::ID _id) {
	auto itm = my_customInputFields.find(_id);
	assert(itm != my_customInputFields.end());	// Invalid id
	return itm->second->text();
}

bool ak::aLogInDialog::savePassword(void) const {
	if (my_savePassword == nullptr) { return false; }
	else { return my_savePassword->isChecked(); }
}

void ak::aLogInDialog::showToolTipAtUsername(
	const QString &							_text
) { createToolTip(my_inputUsername->getInput(), _text); }

void ak::aLogInDialog::showToolTipAtPassword(
	const QString &							_text
) {  }

void ak::aLogInDialog::showToolTipAtCustomInput(
	ak::ID									_inputID,
	const QString &							_text
) {
	auto itm = my_customInputFields.find(_inputID);
	assert(itm != my_customInputFields.end());	// Invalid id
	createToolTip(itm->second->getInput(), _text);
}

// #######################################################################################################################################################################

void ak::aLogInDialog::slotClicked(void) {
	QString pw(my_inputPassword->text());
	QString user(my_inputUsername->text());
	
	// Check if something was entered
	if (user.length() == 0) {
		createToolTip(my_inputUsername->getInput(), "Please enter a username");
		return;
	}
	if (pw.length() == 0) {
		createToolTip(my_inputPassword->getInput(), "Please enter a password");
		return;
	}

	my_messenger->sendMessage(my_uid, etClicked);

}

void ak::aLogInDialog::slotUsernameChanged(const QString & _text) {
	if (my_hashedPw.length() > 0) { my_hashedPw.clear(); my_inputPassword->clearInput(); }
}

void ak::aLogInDialog::slotPasswordChanged(const QString &	_text) {
	if (my_hashedPw.length() > 0) {
		my_hashedPw.clear();
		if (my_inputPassword->text().length() == 0) { return; }
		for (auto c : _text) {
			if (c != 'x') {
				my_inputPassword->getInput()->setText(c);
				return;
			}
		}
		my_inputPassword->getInput()->setText("x");
	}
}

// #######################################################################################################################################################################

void ak::aLogInDialog::createToolTip(
	QWidget *				_parent,
	const QString &			_text
) const {
	QToolTip::showText(_parent->mapToGlobal(QPoint(0, 0)), _text, nullptr, QRect(), 3000);
}

// #######################################################################################################################################################################

ak::aLogInDialogInputField::aLogInDialogInputField(
	aLineEditWidget *				_input,
	aLabelWidget *					_label
) : my_input(_input), my_label(_label)
{
	assert(my_input != nullptr);
	assert(my_label != nullptr);
	my_label->setBuddy(my_input);
}

void ak::aLogInDialogInputField::clearInput(void) {
	assert(my_input != nullptr);
	my_input->clear();
}

QString ak::aLogInDialogInputField::text(void) const {
	assert(my_input != nullptr);
	return my_input->text();
}

void ak::aLogInDialogInputField::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	my_input->setColorStyle(_colorStyle);
	my_label->setColorStyle(_colorStyle);
}
