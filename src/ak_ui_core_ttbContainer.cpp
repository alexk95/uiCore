/*
 * ak_ui_core_ttbContainer.cpp
 *
 *  Created on: August 05, 2020
 *	Last modified on: August 26, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_ui_core_ttbContainer.h>	// corresponding header
#include <ak_exception.h>				// error handling
#include <ak_messenger.h>				// messenger
#include <ak_ui_colorStyle.h>			// colorStyle
#include <ak_uidMangager.h>				// uidManager

ak::ui::core::ttbContainer::ttbContainer(
	ak::messenger *				_messenger,
	ak::uidManager *			_uidManager,
	ak::ui::core::objectType	_type,
	ak::ui::colorStyle *		_colorStyle,
	ak::UID						_uid,
	int							_references
) : ak::ui::core::aPaintable(_type, _colorStyle, _uid, _references),
	my_messenger(nullptr),
	my_uidManager(nullptr)
{
	try {
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }
		my_messenger = _messenger;
		my_uidManager = _uidManager;
		my_uid = my_uidManager->getId();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::core::ttbContainer::ttbContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::core::ttbContainer::ttbContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::core::ttbContainer::ttbContainer()"); }
}

ak::ui::core::ttbContainer::~ttbContainer() {}

int ak::ui::core::ttbContainer::childCount(void) const { return my_childs.size(); }