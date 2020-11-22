/*
 * ak_ui_ttb_subgroup.cpp
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
#include <ak_ui_ttb_subGroup.h>			// corresponding header
#include <ak_messenger.h>				// messenger
#include <ak_uidMangager.h>				// UID manager
#include <ak_exception.h>				// error handling
#include <ak_ui_core_aWidget.h>			// aWidget
#include <ak_ui_qt_action.h>

 // TTB header (TabToolbar library)
#include <TabToolbar/SubGroup.h>		// tt::Subgroup

ak::ui::ttb::subGroup::subGroup(
	ak::messenger *				_messenger,
	tt::SubGroup *				_group,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, ak::ui::core::objectType::oTabToolbarSubgroup),
my_subGroup(_group)
{
	assert(my_subGroup != nullptr); // Nullptr provided
	assert(_messenger != nullptr); // Nullptr provided
	my_text = _text;
}

ak::ui::ttb::subGroup::~subGroup() {
	TTB_CONTAINER_DESTROYING
		
	delete my_subGroup;
}

void ak::ui::ttb::subGroup::addChild(
	ak::ui::core::aObject *		_child
) {
	assert(_child != nullptr); // Nullptr provided
	if (_child->objectType() == ak::ui::core::objectType::oAction) {
		// Cast action
		qt::action * ac = nullptr;
		ac = dynamic_cast<qt::action *>(_child);
		assert(ac != nullptr); // Cast failed
		//Place action
		my_subGroup->AddAction(ac->popupMode(), ac);
	}
	else {
		// Check child
		assert(_child->isWidgetType()); // Provided object is no action and no widget
		// Cast widget
		ak::ui::core::aWidget * w = nullptr;
		w = dynamic_cast<ak::ui::core::aWidget *>(_child);
		assert(w != nullptr); // Cast failed
		// Place widget
		my_subGroup->AddWidget(w->widget());
	}
	_child->setParentObject(this);
	addChildObject(_child);
	// Store object0
	my_childObjects.insert_or_assign(_child->uid(), _child);
}

ak::ui::core::ttbContainer * ak::ui::ttb::subGroup::createSubContainer(
	const QString &				_text
) {
	assert(0); // Cannot add a sub container to a sub group
	return nullptr;
}

void ak::ui::ttb::subGroup::destroyAllSubContainer(void) {}

void ak::ui::ttb::subGroup::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
		assert(_colorStyle != nullptr); // nullptr provided
		my_colorStyle = _colorStyle;
}

void ak::ui::ttb::subGroup::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	ak::ui::core::aObject::removeChildObject(_child);
	
	if (_child->objectType() == ui::core::objectType::oAction) {
		ui::qt::action * action = nullptr;
		action = dynamic_cast<ui::qt::action *>(_child);
		assert(action != nullptr);	// Cast failed
		my_subGroup->removeAction(action);
	}
}

void ak::ui::ttb::subGroup::setEnabled(
	bool						_enabled
) {
	ui::core::ttbContainer::setEnabled(_enabled);
	my_subGroup->setEnabled(my_isEnabled);
}