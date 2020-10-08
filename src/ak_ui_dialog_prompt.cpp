/*
 * ak_ui_dialog_prompt.cpp
 *
 *  Created on: October 07, 2020
 *	Last modified on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_dialog_prompt.h>	// Corresponding header
#include <ak_ui_colorStyle.h>

// Qt header
#include <qwidget.h>				// QWidget
#include <qpushbutton.h>			// QPushButton
#include <qlabel.h>					// QLabel
#include <qlayout.h>				// QLayout

ak::ui::dialog::prompt::prompt(
	const QString &				_message,
	const QString &				_title,
	ak::ui::core::promptType	_type,
	QWidget *					_parent
) : ui::core::aPaintable(ui::core::oPrompt), ui::core::aDialog(_parent),
	my_button1(nullptr), my_button2(nullptr), my_button3(nullptr), my_buttonsLayout(nullptr),
	my_buttonsWidget(nullptr), my_infoLayout(nullptr), my_infoWidget(nullptr), my_layout(nullptr)
{
	// Create Buttons
	my_buttonsWidget = new QWidget;
	my_buttonsLayout = new QHBoxLayout(my_buttonsWidget);

	switch (_type)
	{
	case ak::ui::core::promptOk:
		my_button1 = new QPushButton("Ok");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotOk()));
		my_buttonsLayout->addWidget(my_button1);
		break;
	case ak::ui::core::promptYesNo:
		my_button1 = new QPushButton("Yes");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotYes()));
		my_button2 = new QPushButton("No");
		connect(my_button2, SIGNAL(clicked()), this, SLOT(slotNo()));
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptYesNoCancel:
		my_button1 = new QPushButton("Yes");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotYes()));
		my_button2 = new QPushButton("No");
		connect(my_button2, SIGNAL(clicked()), this, SLOT(slotNo()));
		my_button3 = new QPushButton("Cancel");
		connect(my_button3, SIGNAL(clicked()), this, SLOT(slotCancel()));
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	case ak::ui::core::promptOkCancel:
		my_button1 = new QPushButton("Ok");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotOk()));
		my_button2 = new QPushButton("Cancel");
		connect(my_button2, SIGNAL(clicked()), this, SLOT(slotCancel()));
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptRetryCancel:
		my_button1 = new QPushButton("Retry");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotRetry()));
		my_button2 = new QPushButton("Cancel");
		connect(my_button2, SIGNAL(clicked()), this, SLOT(slotCancel()));
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptIgnoreRetryCancel:
		my_button1 = new QPushButton("Ignore");
		connect(my_button1, SIGNAL(clicked()), this, SLOT(slotIgnore()));
		my_button2 = new QPushButton("Retry");
		connect(my_button2, SIGNAL(clicked()), this, SLOT(slotRetry()));
		my_button3 = new QPushButton("Cancel");
		connect(my_button3, SIGNAL(clicked()), this, SLOT(slotCancel()));
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	default:
		assert(0);	// Unknown type
		break;
	}

	// Create info
	my_infoWidget = new QWidget;
	my_infoLayout = new QHBoxLayout(my_infoWidget);
	my_label = new QLabel(_message);
	my_label->setWordWrap(true);
	my_infoLayout->addWidget(my_label);

	// Create main layout
	my_layout = new QVBoxLayout(this);
	my_layout->addWidget(my_infoWidget);
	my_layout->addWidget(my_buttonsWidget);

	setWindowTitle(_title);

	// Hide info button
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

}

ak::ui::dialog::prompt::~prompt() {
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

void ak::ui::dialog::prompt::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr);
	my_colorStyle = _colorStyle;

	setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sMainWindow));

	if (my_buttonsWidget != nullptr) { my_button1->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget)); }
	if (my_infoWidget != nullptr) { my_button1->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget)); }

	if (my_button1 != nullptr) { my_button1->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sPushButton)); }
	if (my_button2 != nullptr) { my_button2->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sPushButton)); }
	if (my_button3 != nullptr) { my_button3->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sPushButton)); }

	if (my_label != nullptr) { my_button1->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sLabel)); }

}

void ak::ui::dialog::prompt::slotOk(void) { my_result = ui::core::dialogResult::resultOk; close(); }

void ak::ui::dialog::prompt::slotCancel(void) { my_result = ui::core::dialogResult::resultCancel; close(); }

void ak::ui::dialog::prompt::slotYes(void) { my_result = ui::core::dialogResult::resultYes; close(); }

void ak::ui::dialog::prompt::slotNo(void) { my_result = ui::core::dialogResult::resultNo; close(); }

void ak::ui::dialog::prompt::slotRetry(void) { my_result = ui::core::dialogResult::resultRetry; close(); }

void ak::ui::dialog::prompt::slotIgnore(void) { my_result = ui::core::dialogResult::resultIgnore; close(); }
