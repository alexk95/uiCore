/*
 * ak_ui_ttb_group.cpp
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
#include <ak_ui_ttb_group.h>			// corresponding class
#include <ak_ui_ttb_subgroup.h>			// subgroup
#include <ak_exception.h>				// Error handling
#include <ak_messenger.h>				// messenger
#include <ak_uidMangager.h>				// UID manager
#include <ak_ui_colorStyle.h>			// colorStyle
#include <ak_ui_core_aWidget.h>			// aWidget
#include <ak_ui_qt_action.h>
#include <ak_ui_qt_toolButton.h>

 // TTB header (TabToolbar library)
#include <TabToolbar/Group.h>			// tt::Group

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::ttb::group::group(
	ak::messenger *				_messenger,
	tt::Group *					_group,
	const QString &				_text
) : ak::ui::core::ttbContainer(_messenger, ak::ui::core::objectType::oTabToolbarGroup),
	my_group(_group)
{
	assert(my_group != nullptr); // Nullptr provided
	assert(_messenger != nullptr); // Nullptr provided
	my_text = _text;
}

ak::ui::ttb::group::~group() {
	TTB_CONTAINER_DESTROYING

	destroyAllSubContainer();
	delete my_group;

}

void ak::ui::ttb::group::addChild(
	ak::ui::core::aObject *		_child
) {
	assert(_child != nullptr); // Nullptr provided
	if (_child->objectType() == ak::ui::core::objectType::oAction) {
		// Cast action
		qt::action * ac = nullptr;
		ac = dynamic_cast<qt::action *>(_child);
		assert(ac != nullptr); // Wrong object type
		//Place action
		my_group->AddAction(ac->popupMode(), ac);
	}
	else if (_child->objectType() == ak::ui::core::objectType::oToolButton) {
		// Cast widget
		ak::ui::qt::toolButton * w = nullptr;
		w = dynamic_cast<ak::ui::qt::toolButton *>(_child);
		assert(w != nullptr); // Cast failed
		// Setup widget
		const int iconSize = QApplication::style()->pixelMetric(QStyle::PM_LargeIconSize);
		w->setAutoRaise(true);
		w->setIconSize(QSize(iconSize, iconSize));
		w->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
		w->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		// Place widget
		my_group->AddWidget(w->widget());
	} else {
		// Check child
		assert(_child->isWidgetType()); // Provided object es no action and no widget
		// Cast widget
		ak::ui::core::aWidget * w = nullptr;
		w = dynamic_cast<ak::ui::core::aWidget *>(_child);
		assert(w != nullptr); // Cast failed
		// Place widget
		my_group->AddWidget(w->widget());
	}
	_child->setParentObject(this);
	addChildObject(_child);
	// Store object
	my_childObjects.insert_or_assign(_child->uid(), _child);
}

ak::ui::core::ttbContainer * ak::ui::ttb::group::createSubContainer(
	const QString &				_text
) {
	subGroup * obj = new ak::ui::ttb::subGroup(my_messenger, my_group->AddSubGroup(tt::SubGroup::Align::Yes), _text);
	my_subContainer.push_back(obj);
	return obj;
}

void ak::ui::ttb::group::destroyAllSubContainer(void) {
	for (int i = 0; i < my_subContainer.size(); i++) {
		ui::core::ttbContainer * obj = my_subContainer.at(i);
		delete obj;
	}
}

void ak::ui::ttb::group::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_group->setStyleSheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorWindow |
		TYPE_COLORAREA::caBackgroundColorWindow));
	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus | TYPE_COLORAREA::caBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected | TYPE_COLORAREA::caBackgroundColorSelected, "QToolButton:pressed{", "}"));
	//sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls | TYPE_COLORAREA::caBackgroundColorControls, "QToolButton QToolTip{", "}"));
	my_group->SetToolButtonStylesheet(sheet);
	sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caTabToolBarGroupSeperatorLine);
	my_group->SetSeparatorStyleSheet(sheet);
}

void ak::ui::ttb::group::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	ak::ui::core::aObject::removeChildObject(_child);
	if (_child->objectType() == ui::core::objectType::oTabToolbarSubgroup) {
		ttb::subGroup * Group = nullptr;
		Group = dynamic_cast<ttb::subGroup *>(_child);
		assert(Group != nullptr);
		for (int i = 0; i < my_subContainer.size(); i++) {
			if (my_subContainer.at(i) == Group) {
				my_subContainer.erase(my_subContainer.begin() + i);
				return;
			}
		}
		assert(0); // Group not found
	}
	else if (_child->objectType() == ui::core::objectType::oAction) {
		// Its a action
		qt::action * action = nullptr;
		action = dynamic_cast<qt::action *>(_child);
		assert(action != nullptr);	// Cast failed
		my_group->RemoveAction(action);
	}
}

void ak::ui::ttb::group::setEnabled(
	bool						_enabled
) {
	ui::core::ttbContainer::setEnabled(_enabled);
	my_group->setEnabled(my_isEnabled);
}