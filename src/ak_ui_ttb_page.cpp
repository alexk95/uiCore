/*
 * ak_ui_ttb_page.cpp
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *
 *	This class is using a modified version of the "Qt TabToolbar" library.
 *	The changes to the library are listed in the documentation which was
 *	provided with this source code package.
 */

// AK header
#include <ak_ui_ttb_page.h>				// corresponding header
#include <ak_ui_ttb_group.h>			// group
#include <ak_exception.h>				// error handling
#include <ak_messenger.h>				// messenger
#include <ak_uidMangager.h>				// UID manager
#include <ak_ui_colorStyle.h>			// colorStyle

// TTB header (TabToolbar library)
#include <TabToolbar/Page.h>			// tt::Page

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::ttb::page::page(
	ak::messenger *				_messenger,
	tt::Page *					_page,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, ak::ui::core::objectType::oTabToolbarPage),
my_page(_page)
{
	assert(my_page != nullptr); // Nullptr provided
	assert(_messenger != nullptr); // Nullptr { throw ak::Exception("Is nullptr", "Check messenger"); }
	my_text = _text;
}

ak::ui::ttb::page::~page() {
	destroyAllSubContainer();
	delete my_page;

}

void ak::ui::ttb::page::addChild(
	ak::ui::core::aObject *		_child
) {
	assert(0); //Childs can't be added to a page, only to groups
}

ak::ui::core::ttbContainer * ak::ui::ttb::page::createSubContainer(
	const QString &				_text
) {
	ak::ui::ttb::group * obj = new ak::ui::ttb::group(my_messenger, my_page->AddGroup(_text), _text);
	if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
	my_subContainer.push_back(obj);
	return obj;
}

void ak::ui::ttb::page::destroyAllSubContainer(void) {
	for (int i = 0; i < my_subContainer.size(); i++) {
		ui::core::ttbContainer * obj = my_subContainer.at(i);
		delete obj;
	}
}

void ak::ui::ttb::page::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		my_page->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
			TYPE_COLORAREA::caBorderColorWindow, "#" + my_alias + "{", "}"));
	}
	else {
		my_page->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
			TYPE_COLORAREA::caBorderColorWindow));
	}
}
