/*
 *	File:		aNiceLineEditWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: April 02, 2021
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <akGui/aColorStyle.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aNiceLineEditWidget.h>

#include <qevent.h>
#include <qlayout.h>

ak::aNiceLineEditWidget::aNiceLineEditWidget(const QString & _initialText, const QString & _infoLabelText)
	: aWidget{ otNiceLineEdit }, my_label{ nullptr }, my_lineEdit{ nullptr }, my_layout{ nullptr }
{
	// Create layout 
	my_layout = new QHBoxLayout(this);
	my_layout->setMargin(0);
	this->setContentsMargins(0, 0, 0, 0);

	// Create controls
	my_lineEdit = new aLineEditWidget{ _initialText };
	my_label = new aLabelWidget{ _infoLabelText };
	my_label->setBuddy(my_lineEdit);
	my_layout->addWidget(my_label);
	my_layout->addWidget(my_lineEdit);

	// Connect signals
	connect(my_lineEdit, &aLineEditWidget::keyPressed, this, &aNiceLineEditWidget::slotKeyPressed);
	connect(my_lineEdit, &aLineEditWidget::keyReleased, this, &aNiceLineEditWidget::slotKeyReleased);
	connect(my_lineEdit, &aLineEditWidget::finishedChanges, this, &aNiceLineEditWidget::slotEditingFinished);
	connect(my_lineEdit, &QLineEdit::textChanged, this, &aNiceLineEditWidget::slotTextChanged);
	connect(my_lineEdit, &QLineEdit::returnPressed, this, &aNiceLineEditWidget::slotReturnPressed);
}

ak::aNiceLineEditWidget::~aNiceLineEditWidget() {
	A_OBJECT_DESTROYING

	delete my_label;
	delete my_lineEdit;
	delete my_layout;
}

QWidget * ak::aNiceLineEditWidget::widget(void) { return this; }

void ak::aNiceLineEditWidget::setColorStyle(
	aColorStyle *			_colorStyle
) {
	my_colorStyle = _colorStyle;
	my_label->setColorStyle(my_colorStyle);
	my_lineEdit->setColorStyle(my_colorStyle);
}

void ak::aNiceLineEditWidget::setText(const QString & _text) { my_lineEdit->setText(_text); }

void ak::aNiceLineEditWidget::setErrorState(bool _error)
{ my_lineEdit->setErrorState(_error); }

void ak::aNiceLineEditWidget::setInfoLabelText(const QString & _text) { my_label->setText(_text); }

void ak::aNiceLineEditWidget::setErrorStateIsForeground(bool _isForeground) {
	my_lineEdit->setErrorStateIsForeground(_isForeground);
}

QString ak::aNiceLineEditWidget::text(void) const { return my_lineEdit->text(); }

QString ak::aNiceLineEditWidget::infoLabelText(void) const { return my_label->text(); }

// ################################################################################################

// Slots

void ak::aNiceLineEditWidget::slotCursorPositionChanged(int _oldPos, int _newPos) { emit cursorPositionChanged(_oldPos, _newPos); }

void ak::aNiceLineEditWidget::slotSelectionChanged() { emit selectionChanged(); }

void ak::aNiceLineEditWidget::slotKeyPressed(QKeyEvent * _event) { emit keyPressed(_event); }

void ak::aNiceLineEditWidget::slotKeyReleased(QKeyEvent * _event) { emit keyReleased(_event); }

void ak::aNiceLineEditWidget::slotEditingFinished(void) { emit editingFinished(); }

void ak::aNiceLineEditWidget::slotTextChanged(const QString & _text) { emit textChanged(_text); }

void ak::aNiceLineEditWidget::slotReturnPressed() { emit returnPressed(); }