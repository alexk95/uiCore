/*
 * ak_ui_ttb_page.cpp
 *
 *  Created on: August 05, 2020
 *	Last modified on: August 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 *
 *
 *	This class is using a modified version of the "Qt TabToolbar" library.
 *	The changes to the library are listed in the documentation.
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

ak::ui::ttb::page::page(
	ak::messenger *				_messenger,
	ak::uidManager *			_uidManager,
	tt::Page *					_page,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, _uidManager, ak::ui::core::objectType::oTabToolbarPage),
	my_page(nullptr)
{
	try {
		if (_page == nullptr) { throw ak::Exception("Is nullptr", "Check page"); }
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }

		my_page = _page;
		my_text = _text;
		
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::page::page()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::page::page()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::page::page()"); }
}

ak::ui::ttb::page::~page() {
	destroyAllSubContainer();
	delete my_page;

}

void ak::ui::ttb::page::addChild(
	ak::ui::core::aObject *		_child
) { throw ak::Exception("Childs can't be added to a page only groups", "ak::ui::ttb::page::addChild()"); }

ak::ui::core::ttbContainer * ak::ui::ttb::page::createSubContainer(
	const QString &				_text
) {
	try {
		ak::ui::ttb::group * obj = new ak::ui::ttb::group(my_messenger, my_uidManager, my_page->AddGroup(_text), _text);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		my_groups.push_back(obj);
		return obj;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::page::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::page::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::page::createSubContainer()"); }
}

void ak::ui::ttb::page::destroyAllSubContainer(void) {
	try {
		for (int i = 0; i < my_groups.size(); i++) {
			ak::ui::ttb::group * obj = my_groups.at(i);
			delete obj;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::page::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::page::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::page::createSubContainer()"); }
}

void ak::ui::ttb::page::setColorStyle(
	ak::ui::colorStyle *			_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_colorStyle = _colorStyle;
		my_page->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::sTabWidgetPage));
		for (int i = 0; i < my_groups.size(); i++) {
			ak::ui::ttb::group * obj = my_groups.at(i);
			obj->setColorStyle(my_colorStyle);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::page::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::page::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::page::setColorStyle()"); }
}