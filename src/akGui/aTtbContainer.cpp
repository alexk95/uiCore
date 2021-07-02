/*
 *	File:		aTtbContainer.cpp
 *	Package:	akGui
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akCore/aMessenger.h>
#include <akGui/aColorStyle.h>
#include <akGui/aTtbContainer.h>

ak::aTtbContainer::aTtbContainer(
	aMessenger *			_messenger,
	objectType				_type,
	aColorStyle *		_colorStyle,
	UID						_uid
) : aPaintable(_type, _colorStyle, _uid),
	my_messenger(nullptr)
{
	assert(_messenger != nullptr); // Is nullptr
	my_messenger = _messenger;
}

ak::aTtbContainer::~aTtbContainer() {}

ak::aTtbContainer * ak::aTtbContainer::getSubContainer(
	const QString &				_text
) {
	for (auto itm : my_subContainer) { if (itm->text() == _text) { return itm; } }
	return nullptr;
}

int ak::aTtbContainer::subContainerCount(void) const { return my_subContainer.size(); }