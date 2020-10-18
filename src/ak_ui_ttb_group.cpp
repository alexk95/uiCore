/*
 * ak_ui_ttb_group.cpp
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
 *	This class is using a modified version of the "Qt TabToolbar" library.
 *	The changes to the library are listed in the documentation.
 */

// AK header
#include <ak_ui_ttb_group.h>			// corresponding class
#include <ak_ui_ttb_subgroup.h>			// subgroup
#include <ak_exception.h>				// Error handling
#include <ak_messenger.h>				// messenger
#include <ak_uidMangager.h>				// UID manager
#include <ak_ui_colorStyle.h>			// colorStyle
#include <ak_ui_core_aWidget.h>			// aWidget

 // TTB header (TabToolbar library)
#include <TabToolbar/Group.h>			// tt::Group

// Qt header
#include <qaction.h>					// QAction

ak::ui::ttb::group::group(
	ak::messenger *				_messenger,
	ak::uidManager *			_uidManager,
	tt::Group *					_group,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, _uidManager, ak::ui::core::objectType::oTabToolbarPage),
	my_group(nullptr)
{
	try {
		if (_group == nullptr) { throw ak::Exception("Is nullptr", "Check group"); }
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }
		my_group = _group;
		my_text = _text;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::group::group()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::group::group()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::group::group()"); }
}

ak::ui::ttb::group::~group() {
	destroyAllSubContainer();
	delete my_group;

}

void ak::ui::ttb::group::addChild(
	ak::ui::core::aObject *		_child
) {
	try {
		if (_child == nullptr) { throw ak::Exception("Is nullptr", "Check child"); }
		if (_child->objectType() == ak::ui::core::objectType::oAction) {
			// Cast action
			QAction * ac = nullptr;
			ac = dynamic_cast<QAction *>(_child);
			if (ac == nullptr) { throw ak::Exception("Cast failed", "Check cast"); }
			//Place action
			my_group->AddAction(QToolButton::ToolButtonPopupMode::InstantPopup, ac);
		}
		else {
			// Check child
			if (!_child->isWidgetType()) { throw ak::Exception("Object is not widget type", "Check object type"); }
			// Cast widget
			ak::ui::core::aWidget * w = nullptr;
			w = dynamic_cast<ak::ui::core::aWidget *>(_child);
			if (w == nullptr) { throw ak::Exception("Cast failed", "Check cast"); }
			// Place widget
			my_group->AddWidget(w->widget());
		}
		// Store object0
		my_childs.push_back(_child);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::group::addChild()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::group::addChild()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::group::addChild()"); }
}

ak::ui::core::ttbContainer * ak::ui::ttb::group::createSubContainer(
	const QString &				_text
) {
	try {
		subGroup * obj = new ak::ui::ttb::subGroup(my_messenger, my_uidManager, my_group->AddSubGroup(tt::SubGroup::Align::Yes), _text);
		my_subgroups.push_back(obj);
		return obj;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::group::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::group::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::group::createSubContainer()"); }
}

void ak::ui::ttb::group::destroyAllSubContainer(void) {
	try {
		for (int i = 0; i < my_subgroups.size(); i++) {
			subGroup * obj = my_subgroups.at(i);
			delete obj;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::group::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::group::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::group::createSubContainer()"); }
}

void ak::ui::ttb::group::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
		my_group->setStyleSheet(my_colorStyle->getStylesheet(colorStyle::styleableObject::sWidget));
		for (int i = 0; i < my_subgroups.size(); i++) {
			ak::ui::ttb::subGroup * obj = my_subgroups.at(i);
			obj->setColorStyle(my_colorStyle);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::group::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::group::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::group::setColorStyle()"); }
}