/*
 * ak_ui_widget_niceLineEdit.cpp
 *
 *  Created on: April 02, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <ak_ui_qt_niceLineEdit.h>

#include <ak_ui_qt_label.h>
#include <ak_ui_qt_lineEdit.h>

#include <qevent.h>
#include <qlayout.h>

ak::ui::qt::niceLineEdit::niceLineEdit(const QString & _initialText, const QString & _infoLabelText)
	: ak::ui::core::aWidget{ ak::ui::core::oNiceLineEdit }, my_label{ nullptr }, my_lineEdit{ nullptr }, my_layout{ nullptr }
{
	// Create layout 
	my_layout = new QHBoxLayout(this);
	my_layout->setMargin(0);
	this->setContentsMargins(0, 0, 0, 0);

	// Create controls
	my_lineEdit = new qt::lineEdit{ _initialText };
	my_label = new qt::label{ _infoLabelText };
	my_label->setBuddy(my_lineEdit);
	my_layout->addWidget(my_label);
	my_layout->addWidget(my_lineEdit);

	// Connect signals
	connect(my_lineEdit, SIGNAL(keyPressed(QKeyEvent *)), this, SLOT(slotKeyPressed(QKeyEvent *)));
	connect(my_lineEdit, SIGNAL(keyReleased(QKeyEvent *)), this, SLOT(slotKeyReleased(QKeyEvent *)));
	connect(my_lineEdit, SIGNAL(editingFinished()), this, SLOT(slotEditingFinished()));
	connect(my_lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(slotTextChanged(const QString &)));
	connect(my_lineEdit, SIGNAL(returnPressed()), this, SLOT(slotReturnPressed()));
}

ak::ui::qt::niceLineEdit::~niceLineEdit() {
	A_OBJECT_DESTROYING

	delete my_label;
	delete my_lineEdit;
	delete my_layout;
}

QWidget * ak::ui::qt::niceLineEdit::widget(void) { return this; }

void ak::ui::qt::niceLineEdit::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	my_label->setColorStyle(_colorStyle);
	my_lineEdit->setColorStyle(_colorStyle);

	my_styleSheet = my_lineEdit->styleSheet();
}

void ak::ui::qt::niceLineEdit::setText(const QString & _text) { my_lineEdit->setText(_text); }

void ak::ui::qt::niceLineEdit::setErrorState(bool _error)
{
	if (_error)
	{
		my_lineEdit->setStyleSheet("QLineEdit {background-color: red;}");
	}
	else
	{
		my_lineEdit->setStyleSheet(my_styleSheet);
	}
}

void ak::ui::qt::niceLineEdit::setInfoLabelText(const QString & _text) { my_label->setText(_text); }

QString ak::ui::qt::niceLineEdit::text(void) const { return my_lineEdit->text(); }

QString ak::ui::qt::niceLineEdit::infoLabelText(void) const { return my_label->text(); }

// ################################################################################################

// Slots

void ak::ui::qt::niceLineEdit::slotCursorPositionChanged(int _oldPos, int _newPos) { emit cursorPositionChanged(_oldPos, _newPos); }

void ak::ui::qt::niceLineEdit::slotSelectionChanged() { emit selectionChanged(); }

void ak::ui::qt::niceLineEdit::slotKeyPressed(QKeyEvent * _event) { emit keyPressed(_event); }

void ak::ui::qt::niceLineEdit::slotKeyReleased(QKeyEvent * _event) { emit keyReleased(_event); }

void ak::ui::qt::niceLineEdit::slotEditingFinished(void) { emit editingFinished(); }

void ak::ui::qt::niceLineEdit::slotTextChanged(const QString & _text) { emit textChanged(_text); }

void ak::ui::qt::niceLineEdit::slotReturnPressed() { emit returnPressed(); }