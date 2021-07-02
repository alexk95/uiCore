/*
 *	File:		aTtbPage.cpp
 *	Package:	akWidgets
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
#include <akGui/aColorStyle.h>
#include <akWidgets/aTtbGroup.h>
#include <akWidgets/aTtbPage.h>

// TTB header (TabToolbar library)
#include <TabToolbar/Page.h>			// tt::Page

#define TABTOOLBAR_GROUPSEPERATOR_OBJECTNAME "#TabToolBarGroupSeperatorLine"

ak::aTtbPage::aTtbPage(
	aMessenger *				_messenger,
	tt::Page *					_page,
	const QString &				_text
) : ak::aTtbContainer(_messenger, otTabToolbarPage),
my_page(_page)
{
	assert(my_page != nullptr); // Nullptr provided
	assert(_messenger != nullptr); // Nullptr { throw ak::Exception("Is nullptr", "Check messenger"); }
	my_text = _text;
}

ak::aTtbPage::~aTtbPage() {
	TTB_CONTAINER_DESTROYING

	destroyAllSubContainer();
	delete my_page;

}

void ak::aTtbPage::addChild(
	ak::aObject *		_child
) {
	assert(0); //Childs can't be added to a page, only to groups
}

ak::aTtbContainer * ak::aTtbPage::createSubContainer(
	const QString &				_text
) {
	aTtbGroup * obj = new aTtbGroup(my_messenger, my_page->AddGroup(_text), _text);
	if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
	my_subContainer.push_back(obj);
	return obj;
}

void ak::aTtbPage::destroyAllSubContainer(void) {
	for (int i = 0; i < my_subContainer.size(); i++) {
		aTtbContainer * obj = my_subContainer.at(i);
		delete obj;
	}
}

void ak::aTtbPage::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorWindow |
		cafBackgroundColorWindow));;
	my_page->setStyleSheet(sheet);
}

void ak::aTtbPage::removeChildObject(
	aObject *								_child
) {
	assert(_child != nullptr); // Nullptr provided
	ak::aObject::removeChildObject(_child);
	if (_child->type() == otTabToolbarGroup) {
		aTtbGroup * Group = nullptr;
		Group = dynamic_cast<aTtbGroup *>(_child);
		assert(Group != nullptr);
		for (int i = 0; i < my_subContainer.size(); i++) {
			if (my_subContainer.at(i) == Group) {
				my_subContainer.erase(my_subContainer.begin() + i);
				return;
			}
		}
		assert(0); // Group not found
	}
	else {
		// Its a widget/action

	}
}

int ak::aTtbPage::index(void) const { return my_page->getIndex(); }

void ak::aTtbPage::setEnabled(
	bool						_enabled
) {
	aTtbContainer::setEnabled(_enabled);
	my_page->setEnabled(my_isEnabled);
}