/*
 * ak_ui_objectManager.cpp
 *
 *  Created on: November 03, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_dialog_options.h>		// Corresponding header
#include <ak_ui_qt_tree.h>
#include <ak_ui_widget_propertyGrid.h>

ak::ui::dialog::options::options(
	messenger *							_messenger
)	: ui::core::aPaintable(ui::core::objectType::oOptionsDialog), my_externalMessenger(_messenger)
{
	assert(my_externalMessenger != nullptr);

}

ak::ui::dialog::options::~options() {

}

void ak::ui::dialog::options::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {

}

ak::ID ak::ui::dialog::options::createMainCategory(
	const QString &						_text
) {
	return -1;
}

ak::ID ak::ui::dialog::options::createSubCategory(
	ak::ID								_parentID,
	const QString &						_text
) {
	assert(0);
	return -1;
}

ak::ID ak::ui::dialog::options::createGroup(
	ak::ID								_categoryID,
	const QString &						_name
) {
	assert(0);
	return -1;
}

void ak::ui::dialog::options::slotCategoryChange() {

}

void ak::ui::dialog::options::slotApplyClicked() {

}

void ak::ui::dialog::options::slotCancelClicked() {
	my_result = ui::core::dialogResult::resultCancel;
}

void ak::ui::dialog::options::slotOkClicked() {
	my_result = ui::core::dialogResult::resultOk;
}

void ak::ui::dialog::options::slotCategoryFilterChanged() {

}

// ###################################################################################################################################################

// ###################################################################################################################################################

// ###################################################################################################################################################



// ###################################################################################################################################################

// ###################################################################################################################################################

// ###################################################################################################################################################



// ###################################################################################################################################################

// ###################################################################################################################################################

// ###################################################################################################################################################


