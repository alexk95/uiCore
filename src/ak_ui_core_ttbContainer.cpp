/*
 * ak_ui_core_ttbContainer.cpp
 *
 *  Created on: August 05, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_core_ttbContainer.h>	// corresponding header
#include <ak_exception.h>				// error handling
#include <ak_messenger.h>				// messenger
#include <ak_ui_colorStyle.h>			// colorStyle
#include <ak_uidMangager.h>				// uidManager

ak::ui::core::ttbContainer::ttbContainer(
	ak::messenger *				_messenger,
	ak::ui::core::objectType	_type,
	const ak::ui::colorStyle *	_colorStyle,
	ak::UID						_uid,
	int							_references
) : ak::ui::core::aPaintable(_type, _colorStyle, _uid, _references),
my_messenger(nullptr)
{
	assert(_messenger != nullptr); // Is nullptr
	my_messenger = _messenger;
}

ak::ui::core::ttbContainer::~ttbContainer() {}

int ak::ui::core::ttbContainer::subContainerCount(void) const { return my_subContainer.size(); }