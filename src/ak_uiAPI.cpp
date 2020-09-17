/*
 * ak_uiAPI.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: August 27, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

// AK header
#include <ak_uiAPI.h>							// corresponding header
#include <ak_ui_colorStyleDefaultBright.h>		// colorStyleDefaultBright
#include <ak_ui_colorStyleDefaultDark.h>		// colorStyleDefaultDark
#include <ak_messenger.h>						// messenger
#include <ak_ui_objectManager.h>				// objectManager
#include <ak_notifierStaticEvent.h>				// notifierStaticEvent
#include <ak_notifier.h>						// notifier
#include <ak_exception.h>						// error handling
#include <ak_ui_colorStyle.h>					// colorStyle
#include <ak_ui_iconManager.h>					// iconManager
#include <ak_ui_uiManager.h>					// uiManager
#include <ak_uidMangager.h>						// UID manager
#include <ak_singletonAllowedMessages.h>

// Qt header
#include <qapplication.h>

static ak::uiAPI::apiManager					my_apiManager;					//! The API manager

ak::uiAPI::apiManager::apiManager()
	: my_iconManager(nullptr),
	my_iconManagerIsExtern(false),
	my_messenger(nullptr),
	my_messengerIsExtern(false),
	my_objManager(nullptr),
	my_objManagerIsExtern(false),
	my_uidManager(nullptr),
	my_uidManagerIsExtern(false),
	my_colorStyle(nullptr),
	my_colorStyleIsExtern(false),
	my_isInitialized(false),
	my_app(nullptr),
	my_appIsRunning(false)
{ ak::singletonAllowedMessages::instance(); }

ak::uiAPI::apiManager::~apiManager() {
	// iconManager
	if (!my_iconManagerIsExtern) {
		if (my_iconManager != nullptr) {
			delete my_iconManager; my_iconManager = nullptr;
		}
	}

	// messenger
	if (!my_messengerIsExtern) {
		if (my_messenger != nullptr) {
			delete my_messenger; my_messenger = nullptr;
		}
	}
	// object manager
	if (!my_objManagerIsExtern) {
		if (my_objManager != nullptr) {
			delete my_objManager; my_objManager = nullptr;
		}
	}
	// uid manager
	if (!my_uidManagerIsExtern) {
		if (my_uidManager != nullptr) {
			delete my_uidManager; my_uidManager = nullptr;
		}
	}

	// Qt Application
	delete my_app;
}

void ak::uiAPI::apiManager::ini(
	bool						_createQApplication,
	int							_argc,
	char **						_argv,
	ak::messenger *				_messenger,
	ak::uidManager *			_uidManager,
	ak::ui::iconManager *		_iconManager,
	ak::ui::objectManager *		_objectManager
) {
	try {
		if (my_isInitialized) { throw ak::Exception("API is already initialized!", "Check API status"); }

		// QApplication
		if (_createQApplication) {
			my_app = new QApplication(_argc, _argv);
			if (my_app == nullptr) { throw ak::Exception("Failed to create", "Create Q Application"); }
		}

		// messenger
		if (_messenger == nullptr) {
			my_messenger = new ak::messenger();
			if (my_messenger == nullptr) { throw ak::Exception("Failed to create", "Create messenger"); }
		}
		else { my_messenger = _messenger; my_messengerIsExtern = true; }

		// uid manager
		if (_uidManager == nullptr) {
			my_uidManager = new ak::uidManager();
			if (my_uidManager == nullptr) { throw ak::Exception("Failed to create", "Create uid manager"); }
		}
		else { my_uidManager = _uidManager; my_uidManagerIsExtern = true; }

		// icon manager
		if (_iconManager == nullptr) {
			my_iconManager = new ak::ui::iconManager(QString(""));
			if (my_iconManager == nullptr) { throw ak::Exception("Failed to create", "Create icon manager"); }
		}
		else { my_iconManager = _iconManager; my_iconManagerIsExtern = true; }

		// object manager
		if (_objectManager == nullptr) {
			my_objManager = new ak::ui::objectManager(my_messenger, my_uidManager, my_iconManager, my_colorStyle);
			if (my_objManager == nullptr) { throw ak::Exception("Failed to create", "Create object manager"); }
		}
		else {
			if (!my_messengerIsExtern) { throw ak::Exception("External object manager cannot be used when no external messenger was provided", "Check messenger"); }
			if (!my_uidManager) { throw ak::Exception("External object manager cannot be used when no external uid manager was provided", "Check uidManager"); }
			my_objManager = _objectManager;
		}

		my_isInitialized = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::ini()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::ini()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::ini()"); }
}

bool ak::uiAPI::apiManager::isInitialized(void) const { return my_isInitialized; }

void ak::uiAPI::apiManager::setColorStyle(
	ak::ui::colorStyle *		_colorStyle
) {
	// Check if the same color style was provided
	if (_colorStyle == my_colorStyle) { return; }
	ak::ui::colorStyle * pointerBackup = nullptr;

	try {
		// Backup current color style to delete it after applying the new one
		if (!my_colorStyleIsExtern) { pointerBackup = my_colorStyle; }
		my_colorStyle = _colorStyle;
		my_colorStyle->setDirectories(my_iconManager->searchDirectories());
		my_colorStyleIsExtern = true;
		my_objManager->setColorStyle(my_colorStyle);

		// Free memory of the old color style if was internal created
		if (pointerBackup != nullptr) { delete pointerBackup; pointerBackup = nullptr; }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setColorStyle()"); }
}

void ak::uiAPI::apiManager::setDarkColorStyle(void) {
	ak::ui::colorStyle * pointerBackup = nullptr;
	try {
		// Backup current color style to delete it after applying the new one
		if (!my_colorStyleIsExtern) { pointerBackup = my_colorStyle; }
		my_colorStyle = new ak::ui::colorStyleDefaultDark();
		my_colorStyle->setDirectories(my_iconManager->searchDirectories());
		my_colorStyleIsExtern = false;
		my_objManager->setColorStyle(my_colorStyle);

		// Free memory of the old color style if was internal created
		if (pointerBackup != nullptr) { delete pointerBackup; pointerBackup = nullptr; }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setDarkColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setDarkColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setDarkColorStyle()"); }
}

void ak::uiAPI::apiManager::setBrightColorStyle(void) {
	ak::ui::colorStyle * pointerBackup = nullptr;
	try {
		// Backup current color style to delete it after applying the new one
		if (!my_colorStyleIsExtern) { pointerBackup = my_colorStyle; }
		my_colorStyle = new ak::ui::colorStyleDefaultBright();
		my_colorStyle->setDirectories(my_iconManager->searchDirectories());
		my_colorStyleIsExtern = false;
		my_objManager->setColorStyle(my_colorStyle);

		// Free memory of the old color style if was internal created
		if (pointerBackup != nullptr) { delete pointerBackup; pointerBackup = nullptr; }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setBrightColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setBrightColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setBrightColorStyle()"); }
}

ak::messenger * ak::uiAPI::apiManager::messenger(void) {
	try {
		if (!my_isInitialized) { throw ak::Exception("API is not initialized", "Check API status"); }
		return my_messenger;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::messenger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::messenger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::messenger()"); }
}

ak::uidManager * ak::uiAPI::apiManager::uidManager(void) {
	try {
		if (!my_isInitialized) { throw ak::Exception("API is not initialized", "Check API status"); }
		return my_uidManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::uidManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::uidManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::uidManager()"); }
}

ak::ui::iconManager * ak::uiAPI::apiManager::iconManager(void) {
	try {
		if (!my_isInitialized) { throw ak::Exception("API is not initialized", "Check API status"); }
		return my_iconManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::iconManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::iconManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::iconManager()"); }
}

ak::ui::objectManager * ak::uiAPI::apiManager::objectManager(void) {
	try {
		if (!my_isInitialized) { throw ak::Exception("API is not initialized", "Check API status"); }
		return my_objManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::objectManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::objectManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::objectManager()"); }
}

int ak::uiAPI::apiManager::exec(void) {
	try {
		if (my_app == nullptr) { throw ak::Exception("Application was not created on initialization", "Check application"); }
		if (my_appIsRunning) { throw ak::Exception("Application is already running", "Check application"); }
		return my_app->exec();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::exec()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::exec()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::exec()"); }
}

// ###############################################################################################################################################

void ak::uiAPI::ini(
	bool													_createQApplication,
	int														_argc,
	char **													_argv,
	ak::messenger *											_messenger,
	ak::uidManager *										_uidManager,
	ak::ui::iconManager *									_iconManager,
	ak::ui::objectManager *									_objectManager,
	ak::ui::colorStyle *									_colorStyle
) {
	if (_colorStyle != nullptr) { my_apiManager.setColorStyle(_colorStyle); }
	my_apiManager.ini(_createQApplication, _argc, _argv, _messenger, _uidManager, _iconManager, _objectManager);
}

void ak::uiAPI::destroy(void) {
	assert(0); // Not implemented yet
}

void ak::uiAPI::destroyObject(
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_delete(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::destroyObject()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::destroyObject()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::destroyObject()"); }
}

void ak::uiAPI::enableEventTypes(
	ak::core::eventType										_types
) { ak::singletonAllowedMessages::instance()->setFlag(_types); }

void ak::uiAPI::disableEventTypes(
	ak::core::eventType										_types
) { ak::singletonAllowedMessages::instance()->removeFlag(_types); }

std::vector<ak::core::eventType> ak::uiAPI::enabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->enabledMessages(); }

std::vector<ak::core::eventType> ak::uiAPI::disabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->disabledMessages(); }

// ###############################################################################################################################################

ak::UID ak::uiAPI::registerNotifier(
	ak::UID										_senderUid,
	ak::notifier *											_notifier,
	ak::core::messageType									_messageType
) {
	try
	{
		// Check UID status
		my_apiManager.messenger()->registerReceiver(_senderUid, _messageType, _notifier);
		return _notifier->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::registerNotifier()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::registerNotifier()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::registerNotifier()"); }
}

void ak::uiAPI::sendMessage(
	ak::UID										_senderUid,
	ak::core::messageType									_messageType,
	int														_message,
	int														_info1,
	int														_info2
) {
	try {
		ak::messenger * m = my_apiManager.messenger();
		m->sendMessage(_senderUid, _messageType, _message, _info1, _info2);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::sendMessage()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::sendMessage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::sendMessage()"); }
}

// ###############################################################################################################################################

ak::UID ak::uiAPI::createAction(
	ak::UID										_creatorUid,
	const char *											_text,
	const char *											_iconName,
	const char *											_iconSize
) {
	try {
		if (_iconSize != nullptr && _iconName != nullptr) {
			// Create icon from parameter
			ak::ui::iconManager * iM =  my_apiManager.iconManager();
			return createAction(_creatorUid, QString(_text), *iM->icon(QString(_iconName), QString(_iconSize)));
		}
		else {
			// Create action without an icon
			return createAction(_creatorUid, QString(_text), QIcon());
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createAction(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createAction(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createAction(char *)"); }
}

ak::UID ak::uiAPI::createAction(
	ak::UID										_creatorUid,
	const QString &											_text,
	const QString &											_iconName,
	const QString &											_iconSize
) {
	try {
		if (_iconSize.length() > 0 && _iconName.length() > 0) {
			// Create icon from parameter
			ak::ui::iconManager * iM = my_apiManager.iconManager();
			return createAction(_creatorUid, _text, *iM->icon(_iconName, _iconSize));
		}
		else {
			// Create action without an icon
			return createAction(_creatorUid, _text, QIcon());
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createAction(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createAction(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createAction(QString)"); }
}

ak::UID ak::uiAPI::createAction(
	ak::UID										_creatorUid,
	const QString &											_text,
	const QIcon &											_icon
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createAction(_creatorUid, _text, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createAction(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createAction(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createAction(QIcon)"); }
}

ak::UID ak::uiAPI::createCheckbox(
	ak::UID										_creatorUid,
	const char *											_text,
	bool													_checked
) {
	try {
		return createCheckbox(_creatorUid, QString(_text), _checked);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createCheckbox(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createCheckbox(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createCheckbox(char *)"); }
}

ak::UID ak::uiAPI::createCheckbox(
	ak::UID										_creatorUid,
	const QString &											_text,
	bool													_checked
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		// Create and set checkbox
		ak::UID uid = oM->createCheckBox(_creatorUid, _text);
		oM->obj_setChecked(uid, _checked);
		return uid;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createCheckbox(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createCheckbox(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createCheckbox(QString)"); }
}

ak::UID ak::uiAPI::createColorEditButton(
	ak::UID										_creatorUid,
	int														_r,
	int														_g,
	int														_b,
	int														_a,
	const char *											_textOverride
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createColorEditButton(_creatorUid, ak::ui::color(_r, _g, _b, _a), QString(_textOverride));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createColorEditButton(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createColorEditButton(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createColorEditButton(char *)"); }
}

ak::UID ak::uiAPI::createColorEditButton(
	ak::UID										_creatorUid,
	int														_r,
	int														_g,
	int														_b,
	int														_a,
	const QString &											_textOverride
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createColorEditButton(_creatorUid, ak::ui::color(_r, _g, _b, _a), _textOverride);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createColorEditButton(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createColorEditButton(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createColorEditButton(QString)"); }
}

ak::UID ak::uiAPI::createColorEditButton(
	ak::UID										_creatorUid,
	const ak::ui::color &									_color,
	const QString &											_textOverride
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createColorEditButton(_creatorUid, _color, _textOverride);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createColorEditButton(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createColorEditButton(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createColorEditButton(color)"); }
}

ak::UID ak::uiAPI::createComboBox(
	ak::UID										_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboBox(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboBox()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboBox()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboBox()"); }
}

ak::UID ak::uiAPI::createComboButton(
	ak::UID										_creatorUid,
	const char *											_text,
	const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboButton(_creatorUid, QString(_text), _possibleSelection);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboButton()"); }
}

ak::UID ak::uiAPI::createComboButton(
	ak::UID										_creatorUid,
	const QString &											_text,
	const std::vector<ak::ui::qt::comboButtonItem> &		_possibleSelection
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboButton(_creatorUid, _text, _possibleSelection);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboButton()"); }
}

ak::UID ak::uiAPI::createComboButtonItem(
	ak::UID										_creatorUid,
	const char *											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboButtonItem(_creatorUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboButtonItem(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboButtonItem(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboButtonItem(char *)"); }
}

ak::UID ak::uiAPI::createComboButtonItem(
	ak::UID										_creatorUid,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboButtonItem(_creatorUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboButtonItem(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboButtonItem(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboButtonItem(QString)"); }
}

ak::UID ak::uiAPI::createComboButtonItem(
	ak::UID										_creatorUid,
	const QIcon &											_icon,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createComboButtonItem(_creatorUid, _icon, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createComboButtonItem(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createComboButtonItem(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createComboButtonItem(QIcon)"); }
}

ak::UID ak::uiAPI::createDock(
	ak::UID										_creatorUid,
	const char *											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createDock(_creatorUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createDock(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createDock(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createDock(char *)"); }
}

ak::UID ak::uiAPI::createDock(
	ak::UID										_creatorUid,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createDock(_creatorUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createDock(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createDock(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createDock(QString)"); }
}

ak::UID ak::uiAPI::createPropertyGrid(
	ak::UID										_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createPropertyGrid(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createPropertyGrid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createPropertyGrid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createPropertyGrid()"); }
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID										_creatorUid,
	const char *											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createPushButton(_creatorUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createPushButton(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createPushButton(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createPushButton(char *)"); }
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID										_creatorUid,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createPushButton(_creatorUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createPushButton(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createPushButton(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createPushButton(QString)"); }
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID										_creatorUid,
	const QIcon &											_icon,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createPushButton(_creatorUid, _icon, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createPushButton(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createPushButton(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createPushButton(QIcon)"); }
}

ak::UID ak::uiAPI::createTable(
	ak::UID										_creatorUid,
	int														_rows,
	int														_columns
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTable(_creatorUid, _rows, _columns);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTable()"); }
}

ak::UID ak::uiAPI::createTabToolBarSubContainer(
	ak::UID										_creatorUid,
	ak::UID										_parentUid,
	const char *											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTabToolBarSubContainer(_creatorUid, _parentUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTabToolBarSubContainer(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTabToolBarSubContainer(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTabToolBarSubContainer(char *)"); }
}

ak::UID ak::uiAPI::createTabToolBarSubContainer(
	ak::UID										_creatorUid,
	ak::UID										_parentUid,
	const QString &											_text
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTabToolBarSubContainer(_creatorUid, _parentUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTabToolBarSubContainer(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTabToolBarSubContainer(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTabToolBarSubContainer(QString)"); }
}

ak::UID ak::uiAPI::createTextEdit(
	ak::UID										_creatorUid,
	const char *											_initialText
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTextEdit(_creatorUid, QString(_initialText));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTextEdit(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTextEdit(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTextEdit(char *)"); }
}

ak::UID ak::uiAPI::createTextEdit(
	ak::UID										_creatorUid,
	const QString &											_initialText
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTextEdit(_creatorUid, _initialText);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTextEdit(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTextEdit(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTextEdit(QString)"); }
}

ak::UID ak::uiAPI::createTree(
	ak::UID										_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTree(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTree()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTree()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTree()"); }
}

ak::UID ak::uiAPI::createUiManager(
	ak::UID										_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createUiManager(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createUiManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createUiManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createUiManager()"); }
}

// ###############################################################################################################################################

void ak::uiAPI::addObjectToContainer(
	ak::UID										_parentUid,
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addObjectToContainer(_parentUid, _objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addObjectToContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addObjectToContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addObjectToContainer()"); }
}

void ak::uiAPI::setCentralWidget(
	ak::UID										_parentUid,
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setCentralWidget(_parentUid, _objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setCentralWidget(UID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setCentralWidget(UID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setCentralWidget(UID)"); }
}

void ak::uiAPI::setCentralWidget(
	ak::UID										_parentUid,
	QWidget *									_widget
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setCentralWidget(_parentUid, _widget);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setCentralWidget(QWidget)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setCentralWidget(QWidget)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setCentralWidget(QWidget)"); }
}

void ak::uiAPI::setObjectText(
	ak::UID										_objectUid,
	const char *											_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setText(_objectUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectText(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectText(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectText(char *)"); }
}

void ak::uiAPI::setObjectText(
	ak::UID										_objectUid,
	const QString &											_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setText(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectText(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectText(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectText(QString)"); }
}

void ak::uiAPI::setObjectChecked(
	ak::UID										_objectUid,
	bool													_checked
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setChecked(_objectUid, _checked);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectChecked()"); }
}

void ak::uiAPI::setObjectTristate(
	ak::UID										_objectUid,
	bool													_isTristate
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTristate(_objectUid, _isTristate);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectTristate()"); }
}

void ak::uiAPI::setFilterVisible(
	ak::UID										_objectUid,
	bool													_vis
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterVisible(_objectUid, _vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setFilterVisible()"); }
}

void ak::uiAPI::setFilterCaseSensitive(
	ak::UID												_objectUid,
	bool												_caseSensitive,
	bool												_refresh
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterCaseSensitive(_objectUid, _caseSensitive, _refresh);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setFilterCaseSensitive()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setFilterCaseSensitive()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setFilterCaseSensitive()"); }
}

void ak::uiAPI::setFilterRefreshOnChange(
	ak::UID												_objectUid,
	bool												_refreshOnChange
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterRefreshOnChange(_objectUid, _refreshOnChange);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setFilterRefreshOnChange()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setFilterRefreshOnChange()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setFilterRefreshOnChange()"); }
}

void ak::uiAPI::setObjectItems(
	ak::UID										_objectUid,
	const std::vector<ak::ui::qt::comboButtonItem> &		_items
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setItems(_objectUid, _items);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectItems()"); }
}

void ak::uiAPI::setObjectColor(
	ak::UID										_objectUid,
	int														_r,
	int														_g,
	int														_b,
	int														_a
) {
	try {
		setObjectColor(_objectUid, ak::ui::color(_r, _g, _b, _a));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectColor(RGBA)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectColor(RGBA)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectColor(RGBA)"); }
}

void ak::uiAPI::setObjectColor(
	ak::UID										_objectUid,
	const ak::ui::color &									_color
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setColor(_objectUid, _color);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setObjectColor(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setObjectColor(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setObjectColor(color)"); }
}

void ak::uiAPI::setObjectIcon(
	ak::UID									_objectUid,
	const char *										_iconName,
	const char *										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, *iM->icon(QString(_iconName), QString(_iconSize)));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setIcon(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setIcon(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setIcon(char *)"); }
}

void ak::uiAPI::setObjectIcon(
	ak::UID									_objectUid,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, *iM->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setIcon(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setIcon(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setIcon(QString)"); }
}

void ak::uiAPI::setObjectIcon(
	ak::UID									_objectUid,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setIcon(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setIcon(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setIcon(QIcon)"); }
}

void ak::uiAPI::setTabToolBarVisible(
	ak::UID										_uiManagerUid,
	bool													_vis
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTabToolBarVisible(_uiManagerUid, _vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setTabToolBarVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setTabToolBarVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setTabToolBarVisible()"); }
}

void ak::uiAPI::appendText(
	ak::UID									_objectUid,
	const char *										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_appendText(_objectUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::appendText(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::appendText(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::appendText(char *)"); }
}

void ak::uiAPI::appendText(
	ak::UID									_objectUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_appendText(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::appendText(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::appendText(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::appendText(QString)"); }
}

void ak::uiAPI::setAutoScrollToBottomEnabled(
	ak::UID									_objectUid,
	bool									_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setAutoScrollToBottomEnabled(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setAutoScrollToBottomEnabled()"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	bool											_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(bool)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(bool)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(bool)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	bool											_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(bool)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(bool)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(bool)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	int												_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(int)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(int)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(int)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	int												_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(int)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(int)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(int)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	double											_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(double)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(double)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(double)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	double											_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(double)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(double)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(double)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	const char *									_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), QString(_value), _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(QString)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const QString &									_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(QString)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	const ak::ui::color &								_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(color)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const ak::ui::color &								_value,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(color)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const char *									_itemName,
	const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
	const QString &									_selectedValue,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _selection, _selectedValue, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(comboButtonItems)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(comboButtonItems)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(comboButtonItems)"); }
}

void ak::uiAPI::addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
	const QString &									_selectedValue,
	bool											_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _selection, _selectedValue, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addProperty(comboButtonItems)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addProperty(comboButtonItems)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(comboButtonItems)"); }
}

ak::UID ak::uiAPI::addObjectToTree(
	ak::UID								_objectUid,
	ak::UID								_parentUid,
	const QString &									_text,
	ak::ui::core::textAlignment						_textAlignment,
	QIcon											_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_createItem(_objectUid, _parentUid, _text, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addObjectToTree(parent)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addObjectToTree(parent)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addObjectToTree(parent)"); }
}

ak::UID ak::uiAPI::addObjectToTree(
	ak::UID								_objectUid,
	const QString &									_cmd,
	char											_delimiter,
	ak::ui::core::textAlignment						_textAlignment,
	const QIcon  &									_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_createItem(_objectUid, _cmd, _delimiter, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addObjectToTree(command)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addObjectToTree(command)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addObjectToTree(command)"); }
}

void ak::uiAPI::setItemSelected(
	ak::UID											_objectUid,
	ak::ID											_itemId,
	bool											_selected
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setSelected(_objectUid, _itemId, _selected);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setItemSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setItemSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setItemSelected()"); }
}

void ak::uiAPI::setSingleItemSelected(
	ak::UID											_objectUid,
	ak::ID											_itemId,
	bool											_selected
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setSingleSelected(_objectUid, _itemId, _selected);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setSingleItemSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setSingleItemSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setSingleItemSelected()"); }
}

void ak::uiAPI::toggleItemSelection(
	ak::UID											_objectUid,
	ak::ID											_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_toggleSelection(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toggleItemSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toggleItemSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toggleItemSelection()"); }
}

void ak::uiAPI::deselectAllItems(
	ak::UID											_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_deselectAllItems(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::deselectAllItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::deselectAllItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::deselectAllItems()"); }
}

void ak::uiAPI::setMultiSelectionEnabled(
	ak::UID											_objectUid,
	bool											_multiSelection
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setMultiSelectionEnabled(_objectUid, _multiSelection);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setMultiSelectionEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setMultiSelectionEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setMultiSelectionEnabled()"); }
}

void ak::uiAPI::setSelectAndDeselectChildrenEnabled(
	ak::UID											_objectUid,
	bool											_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setAutoSelectAndDeselectChildrenEnabled(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setSelectAndDeselectChildrenEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setSelectAndDeselectChildrenEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setSelectAndDeselectChildrenEnabled()"); }
}

// ###############################################################################################################################################

QString ak::uiAPI::getObjectText(
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getText(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getObjectText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getObjectText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getObjectText()"); }
}

bool ak::uiAPI::getObjectTristate(
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getTristate(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getObjectTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getObjectTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getObjectTristate()"); }
}

bool ak::uiAPI::getObjectChecked(
	ak::UID										_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getChecked(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getObjectChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getObjectChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getObjectChecked()"); }
}

std::vector<ak::ID> ak::uiAPI::getSelectedItems(
	ak::UID									_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getSelectedItems(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getSelectedItems()"); }
}

bool ak::uiAPI::getAutoScrollToBottomEnabled(
	ak::UID									_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getAutoScrollToBottomEnabled(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getAutoScrollToBottomEnabled()"); }
}

std::vector<QString> ak::uiAPI::getTreeItemPath(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getPath(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getTreeItemPath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getTreeItemPath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getTreeItemPath()"); }
}

QString ak::uiAPI::getTreeItemPathString(
	ak::UID									_objectUid,
	ak::ID									_itemId,
	char									_delimiter
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getPathString(_objectUid, _itemId, _delimiter);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getTreeItemPathString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getTreeItemPathString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getTreeItemPathString()"); }
}

// ###############################################################################################################################################

std::string ak::uiAPI::toString(
	ak::core::eventType									_eventType
) {
	try {
		QString v = ak::core::toQString(_eventType);
		return v.toStdString();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(eventType)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(eventType)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(eventType)"); }
}

// ###############################################################################################################################################

void ak::uiAPI::creatorDestroyed(
	ak::UID										_creatorUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->creatorDestroyed(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::creatorDestroyed()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::creatorDestroyed()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::creatorDestroyed()"); }
}

void ak::uiAPI::setColorStyle(
	ak::ui::colorStyle *									_colorStyle
) {
	try {
		my_apiManager.setColorStyle(_colorStyle);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setColorStyle()"); }
}

void ak::uiAPI::setDefaultDarkColorStyle() {
	try {
		my_apiManager.setDarkColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDefaultDarkColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDefaultDarkColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDefaultDarkColorStyle()"); }
}

void ak::uiAPI::setDefaultBrightColorStyle() {
	try {
		my_apiManager.setBrightColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDefaultBrightColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDefaultBrightColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDefaultBrightColorStyle()"); }
}

void ak::uiAPI::showMessageBox(
	ak::UID									_uiManagerUid,
	const char *										_message,
	const char *										_title
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		// Get UI manager
		ak::ui::core::aObject * obj = oM->obj_get(_uiManagerUid);
		if (obj->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected main window", "Check object type"); }
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		ui->showMessageBox(QString(_message), QString(_title));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::showMessageBox(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::showMessageBox(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::showMessageBox(char *)"); }
}

void ak::uiAPI::showMessageBox(
	ak::UID									_uiManagerUid,
	const QString &										_message,
	const QString &										_title
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		// Get UI manager
		ak::ui::core::aObject * obj = oM->obj_get(_uiManagerUid);
		if (obj->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected main window", "Check object type"); }
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		ui->showMessageBox(_message, _title);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::showMessageBox(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::showMessageBox(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::showMessageBox(QString)"); }
}

// ###############################################################################################################################################

// dock

void ak::uiAPI::addDock(
	ak::UID									_uiManagerUid,
	ak::UID									_dockUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->addDock(_uiManagerUid, _dockUid, _dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addDock()"); }
}

void ak::uiAPI::tabifyDock(
	ak::UID									_uiManagerUid,
	ak::UID									_parentUid,
	ak::UID									_dockUid
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->tabifyDock(_uiManagerUid, _parentUid, _dockUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::tabifyDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::tabifyDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::tabifyDock()"); }
}

// ###############################################################################################################################################

void ak::uiAPI::addIconSearchPath(
	const char *											_path
) {
	try {
		ak::ui::iconManager * manager = my_apiManager.iconManager();
		manager->addDirectory(QString(_path));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addIconSearchPath(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addIconSearchPath(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addIconSearchPath(char *)"); }
}

void ak::uiAPI::addIconSearchPath(
	const QString &											_path
) {
	try {
		ak::ui::iconManager * manager = my_apiManager.iconManager();
		manager->addDirectory(_path);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addIconSearchPath(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addIconSearchPath(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addIconSearchPath(QString)"); }
}

void ak::uiAPI::removeIconSearchPath(
	const char *											_path
) {
	try {
		ak::ui::iconManager * manager = my_apiManager.iconManager();
		manager->removeDirectory(QString(_path));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::removeIconSearchPath(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::removeIconSearchPath(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::removeIconSearchPath(char *)"); }
}

void ak::uiAPI::removeIconSearchPath(
	const QString &											_path
) {
	try {
		ak::ui::iconManager * manager = my_apiManager.iconManager();
		manager->removeDirectory(_path);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::removeIconSearchPath(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::removeIconSearchPath(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::removeIconSearchPath(QString)"); }
}

ak::UID ak::uiAPI::createUid(void) {
	try {
		return my_apiManager.uidManager()->getId();
	} catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createUid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createUid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createUid()"); }
} 

ak::messenger *ak::uiAPI::getMessenger(void) {
	try {
		// Return messenger
		return my_apiManager.messenger();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getMessenger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getMessenger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getMessenger()"); }
}

ak::ui::iconManager * ak::uiAPI::getIconManager(void) {
	try {
		// Return icon manager
		return my_apiManager.iconManager();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getIconManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getIconManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getIconManager()"); }
}

int ak::uiAPI::exec(void) {
	try {
		// Return icon manager
		return my_apiManager.exec();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getIconManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getIconManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getIconManager()"); }
}