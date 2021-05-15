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
#include <akCore/aException.h>
#include <akCore/aMessenger.h>
#include <akCore/aUidMangager.h>

#include <akGui/aAction.h>
#include <akGui/aColorStyle.h>

#include <akWidgets/aToolButtonWidget.h>
#include <akWidgets/aTtbGroup.h>
#include <akWidgets/aTtbSubgroup.h>
#include <akWidgets/aWidget.h>

 // TTB header (TabToolbar library)
#include <TabToolbar/Group.h>			// tt::Group

ak::aTtbGroup::aTtbGroup(
	aMessenger *				_messenger,
	tt::Group *					_group,
	const QString &				_text
) : aTtbContainer(_messenger, otTabToolbarGroup),
	my_group(_group)
{
	assert(my_group != nullptr); // Nullptr provided
	assert(_messenger != nullptr); // Nullptr provided
	my_text = _text;
}

ak::aTtbGroup::~aTtbGroup() {
	TTB_CONTAINER_DESTROYING
	destroyAllSubContainer();
	delete my_group;
}

void ak::aTtbGroup::addChild(
	ak::aObject *		_child
) {
	assert(_child != nullptr); // Nullptr provided
	if (_child->type() == ak::objectType::otAction) {
		// Cast action
		aAction * ac = nullptr;
		ac = dynamic_cast<aAction *>(_child);
		assert(ac != nullptr); // Wrong object type
		//Place action
		my_group->AddAction(ac->popupMode(), ac);
	}
	else if (_child->type() == ak::objectType::otToolButton) {
		// Cast widget
		aToolButtonWidget * w = nullptr;
		w = dynamic_cast<aToolButtonWidget *>(_child);
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
		ak::aWidget * w = nullptr;
		w = dynamic_cast<ak::aWidget *>(_child);
		assert(w != nullptr); // Cast failed
		// Place widget
		my_group->AddWidget(w->widget());
	}
	_child->setParentObject(this);
	addChildObject(_child);
	// Store object
	my_childObjects.insert_or_assign(_child->uid(), _child);
}

ak::aTtbContainer * ak::aTtbGroup::createSubContainer(
	const QString &				_text
) {
	aTtbSubGroup * obj = new aTtbSubGroup(my_messenger, my_group->AddSubGroup(tt::SubGroup::Align::Yes), _text);
	my_subContainer.push_back(obj);
	return obj;
}

void ak::aTtbGroup::destroyAllSubContainer(void) {
	for (int i = 0; i < my_subContainer.size(); i++) {
		aTtbContainer * obj = my_subContainer.at(i);
		delete obj;
	}
}

void ak::aTtbGroup::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_group->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorFocus | cafBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected | cafBackgroundColorSelected, "QToolButton:pressed{", "}"));
	//sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorControls | cafBackgroundColorControls, "QToolButton QToolTip{", "}"));
	my_group->SetToolButtonStylesheet(sheet);
	sheet = my_colorStyle->toStyleSheet(cafTabToolBarGroupSeperatorLine);
	my_group->SetSeparatorStyleSheet(sheet);
}

void ak::aTtbGroup::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	ak::aObject::removeChildObject(_child);
	if (_child->type() == objectType::otTabToolbarSubgroup) {
		aTtbSubGroup * Group = nullptr;
		Group = dynamic_cast<aTtbSubGroup *>(_child);
		assert(Group != nullptr);
		for (int i = 0; i < my_subContainer.size(); i++) {
			if (my_subContainer.at(i) == Group) {
				my_subContainer.erase(my_subContainer.begin() + i);
				return;
			}
		}
		assert(0); // Group not found
	}
	else if (_child->type() == objectType::otAction) {
		// Its a action
		aAction * action = nullptr;
		action = dynamic_cast<aAction *>(_child);
		assert(action != nullptr);	// Cast failed
		my_group->RemoveAction(action);
	}
}

void ak::aTtbGroup::setEnabled(
	bool						_enabled
) {
	aTtbContainer::setEnabled(_enabled);
	my_group->setEnabled(my_isEnabled);
}