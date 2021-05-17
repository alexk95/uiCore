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
)	: QDialog(_parent), my_result(resultNone) {}

ak::aDialog::~aDialog() {}

void ak::aDialog::setResult(
	dialogResult		_result
) { my_result = _result; }

ak::dialogResult ak::aDialog::result(void) const { return my_result; }

ak::dialogResult ak::aDialog::showDialog(void) { exec(); return my_result; }
