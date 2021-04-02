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

#include <ak_ui_widget_niceLineEdit.h>

#include <ak_ui_qt_label.h>
#include <ak_ui_qt_lineEdit.h>

#include <qevent.h>
#include <qlayout.h>

ak::ui::widget::niceLineEdit::niceLineEdit(const QString & _initialText, const QString & _infoLabelText)
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
}

ak::ui::widget::niceLineEdit::~niceLineEdit() {
	delete my_label;
	delete my_lineEdit;
	delete my_layout;
}

QWidget * ak::ui::widget::niceLineEdit::widget(void) { return this; }

void ak::ui::widget::niceLineEdit::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	my_label->setColorStyle(_colorStyle);
	my_lineEdit->setColorStyle(_colorStyle);
}

void ak::ui::widget::niceLineEdit::setText(const QString & _text) { my_lineEdit->setText(_text); }

void ak::ui::widget::niceLineEdit::setInfoLabelText(const QString & _text) { my_label->setText(_text); }

QString ak::ui::widget::niceLineEdit::text(void) const { return my_lineEdit->text(); }

QString ak::ui::widget::niceLineEdit::infoLabelText(void) const { return my_label->text(); }

void ak::ui::widget::niceLineEdit::slotKeyPressed(QKeyEvent * _event) { emit keyPressed(_event); }

void ak::ui::widget::niceLineEdit::slotKeyReleased(QKeyEvent * _event) { emit keyReleased(_event); }