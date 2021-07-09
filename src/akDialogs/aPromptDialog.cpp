/*
 *	File:		aPromptDialog.cpp
 *	Package:	akDialogs
 *
 *  Created on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // Corresponding header
#include <akDialogs/aPromptDialog.h>

// AK GUI header
#include <akGui/aColorStyle.h>

// AK Widgets header
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aPushButtonWidget.h>

// Qt header
#include <qwidget.h>				// QWidget
#include <qlayout.h>				// QLayout
#include <qevent.h>

ak::aPromptDialog::aPromptDialog(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	QWidget *					_parent
) : aPaintable(otPrompt), aDialog(_parent),
	m_button1(nullptr), m_button2(nullptr), m_button3(nullptr), m_buttonsLayout(nullptr),
	m_buttonsWidget(nullptr), m_infoLayout(nullptr), m_infoWidget(nullptr), m_layout(nullptr), m_iconLabel(nullptr)
{
	setupDialog(_message, _title, _type, m_currentIcon);
}

ak::aPromptDialog::aPromptDialog(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QIcon &				_icon,
	QWidget *					_parent
) : aPaintable(otPrompt), aDialog(_parent),
	m_button1(nullptr), m_button2(nullptr), m_button3(nullptr), m_buttonsLayout(nullptr),
	m_buttonsWidget(nullptr), m_infoLayout(nullptr), m_infoWidget(nullptr), m_layout(nullptr), m_iconLabel(nullptr), m_currentIcon(_icon)
{
	setupDialog(_message, _title, _type, m_currentIcon);
}

ak::aPromptDialog::~aPromptDialog() {
	A_OBJECT_DESTROYING

	if (m_label != nullptr) { delete m_label; }

	if (m_infoLayout != nullptr) { delete m_layout; }
	if (m_infoWidget != nullptr) { delete m_infoWidget; }

	if (m_button1 != nullptr) { delete m_button1; }
	if (m_button2 != nullptr) { delete m_button2; }
	if (m_button3 != nullptr) { delete m_button3; }

	if (m_buttonsLayout != nullptr) { delete m_buttonsLayout; }
	if (m_buttonsWidget != nullptr) { delete m_buttonsWidget; }

	//if (m_layout != nullptr) { delete m_layout; }
}

void ak::aPromptDialog::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	m_colorStyle = _colorStyle;
	QString sheet(m_colorStyle->toStyleSheet(cafBackgroundColorDialogWindow | cafForegroundColorDialogWindow, "QDialog {", "}"));
	setStyleSheet(sheet);

	if (m_button1 != nullptr) { m_button1->setColorStyle(m_colorStyle); }
	if (m_button2 != nullptr) { m_button2->setColorStyle(m_colorStyle); }
	if (m_button3 != nullptr) { m_button3->setColorStyle(m_colorStyle); }

	if (m_label != nullptr) { m_label->setColorStyle(m_colorStyle); }

}

// ##############################################################################################################

// Protected functions

void ak::aPromptDialog::resizeEvent(QResizeEvent * _event) {
	QDialog::resizeEvent(_event);
}

// ##############################################################################################################

// Slots

void ak::aPromptDialog::slotOk(void) { Close(resultOk); }

void ak::aPromptDialog::slotCancel(void) { Close(resultCancel); }

void ak::aPromptDialog::slotYes(void) { Close(resultYes); }

void ak::aPromptDialog::slotNo(void) { Close(resultNo); }

void ak::aPromptDialog::slotRetry(void) { Close(resultRetry); }

void ak::aPromptDialog::slotIgnore(void) { Close(resultIgnore); }

// ##############################################################################################################

// Private functions

void ak::aPromptDialog::setupDialog(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QIcon &				_icon
) {
	m_type = _type;

	// Create Buttons
	m_buttonsWidget = new QWidget;
	m_buttonsLayout = new QHBoxLayout(m_buttonsWidget);

	// Create label layout
	m_infoWidget = new QWidget;
	m_infoLayout = new QHBoxLayout(m_infoWidget);

	// Create info label
	m_label = new aLabelWidget(_message);
	m_label->setWordWrap(true);

	switch (m_type)
	{
	case promptOk:
		m_button1 = new aPushButtonWidget("Ok");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		m_buttonsLayout->addWidget(m_button1);
		break;
	case promptYesNo:
		m_button1 = new aPushButtonWidget("Yes");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotYes);
		m_button2 = new aPushButtonWidget("No");
		connect(m_button2, &QPushButton::clicked, this, &aPromptDialog::slotNo);
		m_buttonsLayout->addWidget(m_button1);
		m_buttonsLayout->addWidget(m_button2);
		break;
	case promptYesNoCancel:
		m_button1 = new aPushButtonWidget("Yes");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotYes);
		m_button2 = new aPushButtonWidget("No");
		connect(m_button2, &QPushButton::clicked, this, &aPromptDialog::slotNo);
		m_button3 = new aPushButtonWidget("Cancel");
		connect(m_button3, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		m_buttonsLayout->addWidget(m_button1);
		m_buttonsLayout->addWidget(m_button2);
		m_buttonsLayout->addWidget(m_button3);
		break;
	case promptOkCancel:
		m_button1 = new aPushButtonWidget("Ok");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		m_button2 = new aPushButtonWidget("Cancel");
		connect(m_button2, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		m_buttonsLayout->addWidget(m_button1);
		m_buttonsLayout->addWidget(m_button2);
		break;
	case promptRetryCancel:
		m_button1 = new aPushButtonWidget("Retry");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotRetry);
		m_button2 = new aPushButtonWidget("Cancel");
		connect(m_button2, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		m_buttonsLayout->addWidget(m_button1);
		m_buttonsLayout->addWidget(m_button2);
		break;
	case promptIgnoreRetryCancel:
		m_button1 = new aPushButtonWidget("Ignore");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotIgnore);
		m_button2 = new aPushButtonWidget("Retry");
		connect(m_button2, &QPushButton::clicked, this, &aPromptDialog::slotRetry);
		m_button3 = new aPushButtonWidget("Cancel");
		connect(m_button3, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		m_buttonsLayout->addWidget(m_button1);
		m_buttonsLayout->addWidget(m_button2);
		m_buttonsLayout->addWidget(m_button3);
		break;
	case promptIconLeft:
		m_button1 = new aPushButtonWidget("Ok");
		connect(m_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		m_buttonsLayout->addWidget(m_button1);
		m_iconLabel = new aLabelWidget;
		m_iconLabel->setPixmap(_icon.pixmap(m_label->size().height(), m_label->size().height()));
		m_infoLayout->addWidget(m_iconLabel);
		break;
	default:
		assert(0);	// Unknown type
		break;
	}

	m_infoLayout->addWidget(m_label);

	// Create main layout
	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_infoWidget);
	m_layout->addWidget(m_buttonsWidget);

	setWindowTitle(_title);
	hideInfoButton();
}