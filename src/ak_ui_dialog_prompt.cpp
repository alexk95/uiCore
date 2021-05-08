/*
 * ak_ui_dialog_prompt.cpp
 *
 *  Created on: October 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_dialog_prompt.h>	// Corresponding header
#include <ak_ui_colorStyle.h>
#include <ak_ui_qt_label.h>
#include <ak_ui_qt_pushButton.h>

// Qt header
#include <qwidget.h>				// QWidget
#include <qlayout.h>				// QLayout
#include <qevent.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::dialog::prompt::prompt(
	const QString &				_message,
	const QString &				_title,
	ak::ui::core::promptType	_type,
	QWidget *					_parent
) : ui::core::aPaintable(ui::core::oPrompt), ui::core::aDialog(_parent),
	my_button1(nullptr), my_button2(nullptr), my_button3(nullptr), my_buttonsLayout(nullptr),
	my_buttonsWidget(nullptr), my_infoLayout(nullptr), my_infoWidget(nullptr), my_layout(nullptr), my_iconLabel(nullptr)
{
	setupDialog(_message, _title, _type, my_currentIcon);
}

ak::ui::dialog::prompt::prompt(
	const QString &				_message,
	const QString &				_title,
	core::promptType			_type,
	const QIcon &				_icon,
	QWidget *					_parent
) : ui::core::aPaintable(ui::core::oPrompt), ui::core::aDialog(_parent),
	my_button1(nullptr), my_button2(nullptr), my_button3(nullptr), my_buttonsLayout(nullptr),
	my_buttonsWidget(nullptr), my_infoLayout(nullptr), my_infoWidget(nullptr), my_layout(nullptr), my_iconLabel(nullptr), my_currentIcon(_icon)
{
	setupDialog(_message, _title, _type, my_currentIcon);
}

ak::ui::dialog::prompt::~prompt() {
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

void ak::ui::dialog::prompt::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caBackgroundColorDialogWindow | TYPE_COLORAREA::caForegroundColorDialogWindow, "QDialog {", "}"));
	setStyleSheet(sheet);

	if (my_button1 != nullptr) { my_button1->setColorStyle(my_colorStyle); }
	if (my_button2 != nullptr) { my_button2->setColorStyle(my_colorStyle); }
	if (my_button3 != nullptr) { my_button3->setColorStyle(my_colorStyle); }

	if (my_label != nullptr) { my_label->setColorStyle(my_colorStyle); }

}

// ##############################################################################################################

// Protected functions

void ak::ui::dialog::prompt::resizeEvent(QResizeEvent * _event) {
	QDialog::resizeEvent(_event);
}

// ##############################################################################################################

// Slots

void ak::ui::dialog::prompt::slotOk(void) { my_result = ui::core::dialogResult::resultOk; close(); }

void ak::ui::dialog::prompt::slotCancel(void) { my_result = ui::core::dialogResult::resultCancel; close(); }

void ak::ui::dialog::prompt::slotYes(void) { my_result = ui::core::dialogResult::resultYes; close(); }

void ak::ui::dialog::prompt::slotNo(void) { my_result = ui::core::dialogResult::resultNo; close(); }

void ak::ui::dialog::prompt::slotRetry(void) { my_result = ui::core::dialogResult::resultRetry; close(); }

void ak::ui::dialog::prompt::slotIgnore(void) { my_result = ui::core::dialogResult::resultIgnore; close(); }

// ##############################################################################################################

// Private functions

void ak::ui::dialog::prompt::setupDialog(
	const QString &				_message,
	const QString &				_title,
	core::promptType			_type,
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
	my_label = new qt::label(_message);
	my_label->setWordWrap(true);

	switch (my_type)
	{
	case ak::ui::core::promptOk:
		my_button1 = new qt::pushButton("Ok");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotOk);
		my_buttonsLayout->addWidget(my_button1);
		break;
	case ak::ui::core::promptYesNo:
		my_button1 = new qt::pushButton("Yes");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotYes);
		my_button2 = new qt::pushButton("No");
		connect(my_button2, &QPushButton::clicked, this, &prompt::slotNo);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptYesNoCancel:
		my_button1 = new qt::pushButton("Yes");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotYes);
		my_button2 = new qt::pushButton("No");
		connect(my_button2, &QPushButton::clicked, this, &prompt::slotNo);
		my_button3 = new qt::pushButton("Cancel");
		connect(my_button3, &QPushButton::clicked, this, &prompt::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	case ak::ui::core::promptOkCancel:
		my_button1 = new qt::pushButton("Ok");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotOk);
		my_button2 = new qt::pushButton("Cancel");
		connect(my_button2, &QPushButton::clicked, this, &prompt::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptRetryCancel:
		my_button1 = new qt::pushButton("Retry");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotRetry);
		my_button2 = new qt::pushButton("Cancel");
		connect(my_button2, &QPushButton::clicked, this, &prompt::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		break;
	case ak::ui::core::promptIgnoreRetryCancel:
		my_button1 = new qt::pushButton("Ignore");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotIgnore);
		my_button2 = new qt::pushButton("Retry");
		connect(my_button2, &QPushButton::clicked, this, &prompt::slotRetry);
		my_button3 = new qt::pushButton("Cancel");
		connect(my_button3, &QPushButton::clicked, this, &prompt::slotCancel);
		my_buttonsLayout->addWidget(my_button1);
		my_buttonsLayout->addWidget(my_button2);
		my_buttonsLayout->addWidget(my_button3);
		break;
	case core::promptIconLeft:
		my_button1 = new qt::pushButton("Ok");
		connect(my_button1, &QPushButton::clicked, this, &prompt::slotOk);
		my_buttonsLayout->addWidget(my_button1);
		my_iconLabel = new qt::label;
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