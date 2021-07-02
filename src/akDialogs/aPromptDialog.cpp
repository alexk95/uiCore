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
	my_button1(nullptr), my_button2(nullptr), my_button3(nullptr), my_buttonsLayout(nullptr),
	my_buttonsWidget(nullptr), my_infoLayout(nullptr), my_infoWidget(nullptr), my_layout(nullptr), my_iconLabel(nullptr)
{
	setupDialog(_message, _title, _type, my_currentIcon);
}

ak::aPromptDialog::aPromptDialog(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QIcon &				_icon,
	QWidget *					_parent
) : aPaintable(otPrompt), aDialog(_parent),
	my_button1(nullptr), my_button2(nullptr), my_button3(nullptr), my_buttonsLayout(nullptr),
	my_buttonsWidget(nullptr), my_infoLayout(nullptr), my_infoWidget(nullptr), my_layout(nullptr), my_iconLabel(nullptr), my_currentIcon(_icon)
{
	setupDialog(_message, _title, _type, my_currentIcon);
}

ak::aPromptDialog::~aPromptDialog() {
	A_OBJECT_DESTROYING

	if (my_label != nullptr) { delete my_label; }

	if (my_infoLayout != nullptr) { delete my_layout; }
	if (my_infoWidget != nullptr) { delete my_infoWidget; }

	if (my_button1 != nullptr) { delete my_button1; }
	if (my_button2 != nullptr) { delete my_button2; }
	if (my_button3 != nullptr) { delete my_button3; }

	if (my_buttonsLayout != nullptr) { delete my_buttonsLayout; }
	if (my_buttonsWidget != nullptr) { delete my_buttonsWidget; }

	//if (my_layout != nullptr) { delete my_layout; }
}

void ak::aPromptDialog::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(cafBackgroundColorDialogWindow | cafForegroundColorDialogWindow, "QDialog {", "}"));
	setStyleSheet(sheet);

	if (my_button1 != nullptr) { my_button1->setColorStyle(my_colorStyle); }
	if (my_button2 != nullptr) { my_button2->setColorStyle(my_colorStyle); }
	if (my_button3 != nullptr) { my_button3->setColorStyle(my_colorStyle); }

	if (my_label != nullptr) { my_label->setColorStyle(my_colorStyle); }

}

// ##############################################################################################################

// Protected functions

void ak::aPromptDialog::resizeEvent(QResizeEvent * _event) {
	QDialog::resizeEvent(_event);
}

// ##############################################################################################################

// Slots

void ak::aPromptDialog::slotOk(void) { my_result = resultOk; close(); }

void ak::aPromptDialog::slotCancel(void) { my_result = resultCancel; close(); }

void ak::aPromptDialog::slotYes(void) { my_result = resultYes; close(); }

void ak::aPromptDialog::slotNo(void) { my_result = resultNo; close(); }

void ak::aPromptDialog::slotRetry(void) { my_result = resultRetry; close(); }

void ak::aPromptDialog::slotIgnore(void) { my_result = resultIgnore; close(); }

// ##############################################################################################################

// Private functions

void ak::aPromptDialog::setupDialog(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QIcon &				_icon
) {
	my_type = _type;

	// Create Buttons
	my_buttonsWidget = new QWidget;
	my_buttonsLayout = new QHBoxLayout(my_buttonsWidget);

	// Create label layout
	my_infoWidget = new QWidget;
	my_infoLayout = new QHBoxLayout(my_infoWidget);

	// Create info label
	my_label = new aLabelWidget(_message);
	my_label->setWordWrap(true);

	switch (my_type)
	{
	case promptOk:
		my_button1 = new aPushButtonWidget("Ok");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		my_buttonsLayout->addWidget(my_button1);
		break;
	case promptYesNo:
		my_button1 = new aPushButtonWidget("Yes");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotYes);
		my_button2 = new aPushButtonWidget("No");
		connect(my_button2, &QPushButton::clicked, this, &aPromptDialog::slotNo);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case promptYesNoCancel:
		my_button1 = new aPushButtonWidget("Yes");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotYes);
		my_button2 = new aPushButtonWidget("No");
		connect(my_button2, &QPushButton::clicked, this, &aPromptDialog::slotNo);
		my_button3 = new aPushButtonWidget("Cancel");
		connect(my_button3, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	case promptOkCancel:
		my_button1 = new aPushButtonWidget("Ok");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		my_button2 = new aPushButtonWidget("Cancel");
		connect(my_button2, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case promptRetryCancel:
		my_button1 = new aPushButtonWidget("Retry");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotRetry);
		my_button2 = new aPushButtonWidget("Cancel");
		connect(my_button2, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case promptIgnoreRetryCancel:
		my_button1 = new aPushButtonWidget("Ignore");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotIgnore);
		my_button2 = new aPushButtonWidget("Retry");
		connect(my_button2, &QPushButton::clicked, this, &aPromptDialog::slotRetry);
		my_button3 = new aPushButtonWidget("Cancel");
		connect(my_button3, &QPushButton::clicked, this, &aPromptDialog::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	case promptIconLeft:
		my_button1 = new aPushButtonWidget("Ok");
		connect(my_button1, &QPushButton::clicked, this, &aPromptDialog::slotOk);
		my_buttonsLayout->addWidget(my_button1);
		my_iconLabel = new aLabelWidget;
		my_iconLabel->setPixmap(_icon.pixmap(my_label->size().height(), my_label->size().height()));
		my_infoLayout->addWidget(my_iconLabel);
		break;
	default:
		assert(0);	// Unknown type
		break;
	}

	my_infoLayout->addWidget(my_label);

	// Create main layout
	my_layout = new QVBoxLayout(this);
	my_layout->addWidget(my_infoWidget);
	my_layout->addWidget(my_buttonsWidget);

	setWindowTitle(_title);

	// Hide info button
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}