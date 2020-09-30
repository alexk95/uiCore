/*
 * ak_ui_core_aWidgetManager.cpp
 *
 *  Created on: July 26, 2020
 *	Last modified on: September 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_ui_core_aWidgetManager.h>		// corresponding header
#include <ak_exception.h>					// error handling
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_messenger.h>					// messenger
#include <ak_ui_iconManager.h>				// iconManager
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_uidMangager.h>					// uidManager

ak::ui::core::aWidgetManager::aWidgetManager(
	ak::ui::core::objectType		_type,
	ak::ui::iconManager *			_iconManager,
	ak::messenger *					_messenger,
	ak::uidManager *				_uidManager,
	ak::ui::objectManager *			_objectManager,
	ak::ui::colorStyle *			_colorStyle,
	ak::UID							 _UID,
	int								_references
)
	: ak::ui::core::aWidget(_type, _colorStyle, _UID, _references),
	my_messenger(_messenger), my_messengerCreated(false), my_uidManager(_uidManager), my_uidManagerCreated(false),
	my_objectManager(_objectManager), my_objectManagerCreated(false), my_iconManager(_iconManager), my_signalLinker(nullptr)
{
	try {
		// Check if the icon manager was provided
		if (my_iconManager == nullptr) { throw ak::Exception("Is nullptr", "Check icon manager", ak::Exception::exceptionType::Nullptr); }

		// Create objects if required
		if (my_messenger == nullptr) { my_messenger = new ak::messenger(); my_messengerCreated = true; }
		if (my_uidManager == nullptr) { my_uidManager = new ak::uidManager(); my_uidManagerCreated = true; }
		if (my_objectManager == nullptr) { my_objectManager = new ak::ui::objectManager(my_messenger, my_uidManager, my_iconManager, my_colorStyle); my_objectManagerCreated = true; }
		my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);
	}
	catch (const ak::Exception & e) { memFree(); throw ak::Exception(e, "ak::ui::core::aWidgetManager::aWidgetManager()"); }
	catch (const std::exception & e) { memFree(); throw ak::Exception(e.what(), "ak::ui::core::aWidgetManager::aWidgetManager()"); }
	catch (...) { memFree(); throw ak::Exception("Unknown error", "ak::ui::core::aWidgetManager::aWidgetManager()",ak::Exception::exceptionType::Undefined); }
}

ak::ui::core::aWidgetManager::~aWidgetManager() {
	// Call the cleanup function
	memFree();
}

void ak::ui::core::aWidgetManager::memFree(void) {
	try { if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; } } catch (...) { assert(0); /*ERROR*/ }
	try { if (my_messengerCreated && my_messenger != nullptr) { delete my_messenger; my_messenger = nullptr; my_messengerCreated = false; } } catch (...) { assert(0); /*ERROR*/ }
	try { if (my_uidManagerCreated && my_uidManager != nullptr) { delete my_uidManager; my_uidManager = nullptr; my_uidManagerCreated = false; } } catch (...) { assert(0); /*ERROR*/ }
	try { if (my_objectManagerCreated && my_objectManager != nullptr) { delete my_objectManager; my_objectManager = nullptr; my_objectManagerCreated = false; } } catch (...) { assert(0); /*ERROR*/ }
}