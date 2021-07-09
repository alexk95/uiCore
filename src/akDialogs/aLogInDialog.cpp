/*
 *	File:		aLogInDialog.cpp
 *	Package:	akDialogs
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
	bool										_showSavePassword,
	const QPixmap &								_backgroundImage,
	const QString &								_username,
	const QString &								_hashedPassword,
	QWidget *									_parent
) : aDialog(_parent), aPaintable(otLogInDialog), m_hashedPw(_hashedPassword),
	m_buttonLogIn(nullptr), m_layout(nullptr), m_spacer(nullptr), m_layoutWidget(nullptr), m_gridLayout(nullptr), m_savePassword(nullptr),
	m_mainLayout(nullptr), m_controlLayout(nullptr), m_controlLayoutWidget(nullptr), m_inputPassword(nullptr), m_inputUsername(nullptr),
	m_currentID(ak::invalidID), m_rowCounter(0), m_showSavePassword(_showSavePassword), m_passwordHashAlgorithm(hashSha_256)
{
	// Initialize entries
	m_gridWidget = new QWidget();
	m_gridLayout = new QGridLayout(m_gridWidget);

	// Create username objects
	m_inputUsername = new aLogInDialogInputField(new aLineEditWidget(_username), new aLabelWidget("Username:"));

	// Create password objects
	aLineEditWidget * pwInput = new aLineEditWidget();
	m_inputPassword = new aLogInDialogInputField(pwInput, new aLabelWidget("Password:"));
	pwInput->setEchoMode(QLineEdit::EchoMode::Password);
	if (_hashedPassword.length() > 0) { pwInput->setText("xxxxxxxxxx"); }

	// Add inputs to layout
	m_gridLayout->addWidget(m_inputUsername->getLabel(), m_rowCounter, 0);
	m_gridLayout->addWidget(m_inputUsername->getInput(), m_rowCounter++, 1);
	m_gridLayout->addWidget(m_inputPassword->getLabel(), m_rowCounter, 0);
	m_gridLayout->addWidget(m_inputPassword->getInput(), m_rowCounter++, 1);

	// Check if required to create save password
	if (m_showSavePassword) {
		m_savePassword = new aCheckBoxWidget("Save password");
		m_savePassword->setChecked(true);
		m_gridLayout->addWidget(m_savePassword, m_rowCounter++, 1);
	}

	// Create log in button
	m_buttonLogIn = new aPushButtonWidget("Log-in", this);
	
	// Connect signals
	connect(m_buttonLogIn, &QPushButton::clicked, this, &aLogInDialog::slotClicked);
	connect(m_inputPassword->getInput(), &QLineEdit::textChanged, this, &aLogInDialog::slotPasswordChanged);
	connect(m_inputUsername->getInput(), &QLineEdit::textChanged, this, &aLogInDialog::slotUsernameChanged);

	// Create main layout and display data
	m_controlLayout = new QVBoxLayout(this);

	m_controlLayout->addWidget(m_gridWidget);
	m_controlLayout->addWidget(m_buttonLogIn);

	m_controlLayoutWidget = new QWidget();
	m_controlLayoutWidget->setLayout(m_controlLayout);
	m_controlLayoutWidget->setObjectName("LogInDialogControlLayoutWidget");
	m_controlLayoutWidget->setStyleSheet("#LogInDialogControlLayoutWidget{"
		"background-color:#90000000;"
		"border-radius:10px;"
		"}\n"
		"QLabel{color:#FFFFFF}\n"
		"QCheckBox{color:#FFFFFF}\n");
	m_bgImage = new QPixmap(_backgroundImage);

	
	// Create layout
	m_layoutWidget = new QWidget();
	m_layout = new QHBoxLayout(m_layoutWidget);
	m_layout->addStretch(1);
	m_layout->addWidget(m_controlLayoutWidget, 0, Qt::AlignmentFlag::AlignCenter);
	m_layout->addStretch(1);

	// Create spacer
	m_mainLayout = new QVBoxLayout();
	m_mainLayout->addStretch(1);
	m_mainLayout->addWidget(m_layoutWidget, 0, Qt::AlignmentFlag::AlignBottom);

	setLayout(m_mainLayout);
	
	setWindowTitle("Welcome");
	hideInfoButton();

	resize(m_bgImage->size());
}

ak::aLogInDialog::~aLogInDialog() {
	A_OBJECT_DESTROYING

	delete m_inputPassword;
	delete m_inputUsername;

	for (auto itm : m_customInputFields) {
		delete itm.second;
	}
	m_customInputFields.clear();

	if (m_savePassword != nullptr) { delete m_savePassword; }

	if (m_buttonLogIn != nullptr) {
		disconnect(m_buttonLogIn, SIGNAL(clicked()), this, SLOT(slotClicked()));
		delete m_buttonLogIn;
	}

	if (m_gridLayout != nullptr) { delete m_gridLayout; }
	if (m_gridWidget != nullptr) { delete m_gridWidget; }

	if (m_controlLayoutWidget != nullptr) { delete m_controlLayoutWidget; }

}

void ak::aLogInDialog::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;

	setStyleSheet(m_colorStyle->toStyleSheet(cafBackgroundColorWindow | cafForegroundColorWindow));
	
	m_buttonLogIn->setColorStyle(m_colorStyle);

	m_inputUsername->setColorStyle(m_colorStyle);
	m_inputPassword->setColorStyle(m_colorStyle);

	m_controlLayoutWidget->setStyleSheet("#LogInDialogControlLayoutWidget{"
		"background-color:#90000000;"
		"border-radius:10px;"
		"}\n"
		"QLabel{color:#FFFFFF}\n"
		"QCheckBox{color:#FFFFFF}\n");

}

void ak::aLogInDialog::paintEvent(QPaintEvent *pe)
{
	QPainter paint(this);
	m_currentImage = m_bgImage->scaled(width(), height(), Qt::KeepAspectRatioByExpanding);
	QPoint centerOfWidget = rect().center();
	QRect rectOfPixmap = m_currentImage.rect();
	rectOfPixmap.moveCenter(centerOfWidget);
	paint.drawPixmap(rectOfPixmap.topLeft(), m_currentImage);
}

ak::ID ak::aLogInDialog::addCustomInput(
	const QString &						_labelText,
	const QString &						_initialInputText
) {
	// Reset grid layout
	assert(m_gridLayout != nullptr);
	delete m_gridLayout;
	m_gridLayout = nullptr;
	m_gridLayout = new QGridLayout(m_gridWidget);
	m_rowCounter = 0;
	
	// Create new item
	aLogInDialogInputField * newItem = new aLogInDialogInputField(new aLineEditWidget(_initialInputText), new aLabelWidget(_labelText));
	m_customInputFields.insert_or_assign(++m_currentID, newItem);
	
	// Place all widgets
	for (auto itm : m_customInputFields) {
		m_gridLayout->addWidget(itm.second->getLabel(), m_rowCounter, 0);
		m_gridLayout->addWidget(itm.second->getInput(), m_rowCounter++, 1);
	}

	m_gridLayout->addWidget(m_inputUsername->getLabel(), m_rowCounter, 0);
	m_gridLayout->addWidget(m_inputUsername->getInput(), m_rowCounter++, 1);

	m_gridLayout->addWidget(m_inputPassword->getLabel(), m_rowCounter, 0);
	m_gridLayout->addWidget(m_inputPassword->getInput(), m_rowCounter++, 1);

	if (m_showSavePassword) {
		m_gridLayout->addWidget(m_savePassword, m_rowCounter++, 1);
	}

	return m_currentID;
}

// #######################################################################################################################################################################

// Getter

QString ak::aLogInDialog::username(void) const { return m_inputUsername->text(); }

QString ak::aLogInDialog::password(void) const {
	if (m_hashedPw.length() > 0) { return m_hashedPw; }
	return hashString(m_inputPassword->text(), m_passwordHashAlgorithm);
}

QString ak::aLogInDialog::customInputText(ak::ID _id) {
	auto itm = m_customInputFields.find(_id);
	assert(itm != m_customInputFields.end());	// Invalid id
	return itm->second->text();
}

bool ak::aLogInDialog::savePassword(void) const {
	if (m_savePassword == nullptr) { return false; }
	else { return m_savePassword->isChecked(); }
}

void ak::aLogInDialog::showToolTipAtUsername(
	const QString &							_text
) { createToolTip(m_inputUsername->getInput(), _text); }

void ak::aLogInDialog::showToolTipAtPassword(
	const QString &							_text
) {  }

void ak::aLogInDialog::showToolTipAtCustomInput(
	ak::ID									_inputID,
	const QString &							_text
) {
	auto itm = m_customInputFields.find(_inputID);
	assert(itm != m_customInputFields.end());	// Invalid id
	createToolTip(itm->second->getInput(), _text);
}

// #######################################################################################################################################################################

void ak::aLogInDialog::slotClicked(void) {
	QString pw(m_inputPassword->text());
	QString user(m_inputUsername->text());
	
	// Check if something was entered
	if (user.length() == 0) {
		createToolTip(m_inputUsername->getInput(), "Please enter a username");
		return;
	}
	if (pw.length() == 0) {
		createToolTip(m_inputPassword->getInput(), "Please enter a password");
		return;
	}

	emit logInRequested();
}

void ak::aLogInDialog::slotUsernameChanged(const QString & _text) {
	if (m_hashedPw.length() > 0) { m_hashedPw.clear(); m_inputPassword->clearInput(); }
}

void ak::aLogInDialog::slotPasswordChanged(const QString &	_text) {
	if (m_hashedPw.length() > 0) {
		m_hashedPw.clear();
		if (m_inputPassword->text().length() == 0) { return; }
		for (auto c : _text) {
			if (c != 'x') {
				m_inputPassword->getInput()->setText(c);
				return;
			}
		}
		m_inputPassword->getInput()->setText("x");
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
) : m_input(_input), m_label(_label)
{
	assert(m_input != nullptr);
	assert(m_label != nullptr);
	m_label->setBuddy(m_input);
}

void ak::aLogInDialogInputField::clearInput(void) {
	assert(m_input != nullptr);
	m_input->clear();
}

QString ak::aLogInDialogInputField::text(void) const {
	assert(m_input != nullptr);
	return m_input->text();
}

void ak::aLogInDialogInputField::setColorStyle(
	aColorStyle *			_colorStyle
) {
	m_input->setColorStyle(_colorStyle);
	m_label->setColorStyle(_colorStyle);
}
