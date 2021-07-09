/*
 *	File:		aDialog.cpp
 *	Package:	akGui
 *
 *  Created on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

#include <akGui/aDialog.h>

#include <qwidget.h>

ak::aDialog::aDialog(
	QWidget *			_parent
)	: QDialog(_parent), m_result(resultNone) {}

ak::aDialog::~aDialog() {}

void ak::aDialog::setResult(
	dialogResult		_result
) { m_result = _result; }

ak::dialogResult ak::aDialog::result(void) const { return m_result; }

ak::dialogResult ak::aDialog::showDialog(void) { exec(); return m_result; }

void ak::aDialog::hideInfoButton(void) {
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

void ak::aDialog::Close(
	dialogResult				_result
) {
	m_result = _result; 
	QDialog::close();
}