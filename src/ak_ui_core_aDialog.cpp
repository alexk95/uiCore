/*
 * ak_ui_core_aDialog.cpp
 *
 *  Created on: October 06, 2020
 *	Last modified on: October 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

#include <ak_ui_core_aDialog.h>		// corresponding header

ak::ui::core::aDialog::aDialog(
	QWidget *					_parent
)	: QDialog(_parent), my_result(ui::core::resultNone)
{

}

ak::ui::core::aDialog::~aDialog() {

}

void ak::ui::core::aDialog::setResult(
	ui::core::dialogResult		_result
) { my_result = _result; }

ak::ui::core::dialogResult ak::ui::core::aDialog::result(void) const { return my_result; }

ak::ui::core::dialogResult ak::ui::core::aDialog::showDialog(void) { exec(); return my_result; }
