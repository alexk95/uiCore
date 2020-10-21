/*
 * ak_ui_core_aDialog.cpp
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

#include <ak_ui_core_aDialog.h>		// corresponding header

#include <qwidget.h>

ak::ui::core::aDialog::aDialog(
	QWidget *					_parent
)	: QDialog(_parent), my_result(ui::core::resultNone)
{}

ak::ui::core::aDialog::~aDialog() {}

void ak::ui::core::aDialog::setResult(
	ui::core::dialogResult		_result
) { my_result = _result; }

ak::ui::core::dialogResult ak::ui::core::aDialog::result(void) const { return my_result; }

ak::ui::core::dialogResult ak::ui::core::aDialog::showDialog(void) { exec(); return my_result; }
