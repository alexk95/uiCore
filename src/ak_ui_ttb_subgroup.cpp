/*
 * ak_ui_ttb_subgroup.cpp
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
#include <ak_ui_ttb_subGroup.h>			// corresponding header
#include <ak_messenger.h>				// messenger
#include <ak_uidMangager.h>				// UID manager
#include <ak_exception.h>				// error handling
#include <ak_ui_core_aWidget.h>			// aWidget

 // TTB header (TabToolbar library)
#include <TabToolbar/SubGroup.h>		// tt::Subgroup

// Qt header
#include <qaction.h>					// QAction

ak::ui::ttb::subGroup::subGroup(
	ak::messenger *				_messenger,
	ak::uidManager *			_uidManager,
	tt::SubGroup *				_group,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, _uidManager, ak::ui::core::objectType::oTabToolbarPage),
	my_subGroup(nullptr)
{
	try {
		if (_group == nullptr) { throw ak::Exception("Is nullptr", "Check group"); }
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }

		my_subGroup = _group;
		my_text = _text;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::subGroup::subGroup()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::subGroup::subGroup()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::subGroup::subGroup()"); }
}

ak::ui::ttb::subGroup::~subGroup() { delete my_subGroup; }

void ak::ui::ttb::subGroup::addChild(
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
			my_subGroup->AddAction(QToolButton::ToolButtonPopupMode::InstantPopup, ac);
			
		}
		else {
			// Check child
			if (!_child->isWidgetType()) { throw ak::Exception("Object is not widget type", "Check object type"); }
			// Cast widget
			ak::ui::core::aWidget * w = nullptr;
			w = dynamic_cast<ak::ui::core::aWidget *>(_child);
			if (w == nullptr) { throw ak::Exception("Cast failed", "Check cast"); }
			// Place widget
			my_subGroup->AddWidget(w->widget());
		}
		// Store object0
		my_childs.push_back(_child);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::subGroup::addChild()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::subGroup::addChild()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::subGroup::addChild()"); }
}

ak::ui::core::ttbContainer * ak::ui::ttb::subGroup::createSubContainer(
	const QString &				_text
) {
	try { throw ak::Exception("Cannot add a sub container to a sub group", "Invalid operation"); }
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::subGroup::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::subGroup::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::subGroup::createSubContainer()"); }
}

void ak::ui::ttb::subGroup::destroyAllSubContainer(void) {
	try {
		// Nothing to do	
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::subGroup::createSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::subGroup::createSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::subGroup::createSubContainer()"); }
}

void ak::ui::ttb::subGroup::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::ttb::subGroup::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::ttb::subGroup::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::ttb::subGroup::setColorStyle()"); }
}