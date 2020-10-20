/*
 * ak_uiAPI.cpp
 *
 *  Created on: February 06, 2020
 *	Last modified on: September 20, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore project.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 *	See license at: https://github.com/alexk95/uiCore
 */

// AK header
#include <ak_uiAPI.h>						// corresponding header
#include <ak_messenger.h>					// messenger
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_notifierStaticEvent.h>			// notifierStaticEvent
#include <ak_notifier.h>					// notifier
#include <ak_exception.h>					// error handling
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_iconManager.h>				// iconManager
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_uidMangager.h>					// UID manager
#include <ak_singletonAllowedMessages.h>	// allowed messages
#include <ak_file.h>						// file
#include <ak_ui_dialog_prompt.h>			// prompt dialog

// Qt header
#include <qapplication.h>					// QApplication
#include <qsurfaceformat.h>					// QSurfaceFormat
#include <qfiledialog.h>					// Open/Save file dialog
#include <qfile.h>

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
	my_isInitialized(false),
	my_app(nullptr),
	my_appIsRunning(false),
	my_defaultSurfaceFormat(nullptr),
	my_fileUidManager(nullptr)
{
	ak::singletonAllowedMessages::instance();
	my_fileUidManager = new ak::uidManager();
}

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
	//delete my_app;
}

void ak::uiAPI::apiManager::ini(
	bool												_createQApplication,
	int													_argc,
	char **												_argv,
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager,
	ak::ui::iconManager *								_iconManager,
	ak::ui::objectManager *								_objectManager
) {
	try {
		assert(!my_isInitialized); // Is already initialized

		// QApplication
		if (_createQApplication) {
			my_app = new QApplication(_argc, _argv);
			assert(my_app != nullptr); // Failed to create
		}

		// messenger
		if (_messenger == nullptr) {
			my_messenger = new ak::messenger();
			assert(my_messenger != nullptr); // Failed to create
		}
		else { my_messenger = _messenger; my_messengerIsExtern = true; }

		// uid manager
		if (_uidManager == nullptr) {
			my_uidManager = new ak::uidManager();
			assert(my_uidManager != nullptr); // Failed to create
		}
		else { my_uidManager = _uidManager; my_uidManagerIsExtern = true; }

		// icon manager
		if (_iconManager == nullptr) {
			my_iconManager = new ak::ui::iconManager(QString(""));
			assert(my_iconManager != nullptr); // Failed to create
		}
		else { my_iconManager = _iconManager; my_iconManagerIsExtern = true; }

		// object manager
		if (_objectManager == nullptr) {
			my_objManager = new ak::ui::objectManager(my_messenger, my_uidManager, my_iconManager);
			assert(my_objManager != nullptr); // Failed to create
		}
		else {
			assert(my_messengerIsExtern);	// Internal messenger cannot be used with external objectManager
			assert(my_uidManagerIsExtern);	// Internal uidManager cannot be used with external objectManager
			my_objManager = _objectManager;
		}

		my_isInitialized = true;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::ini()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::ini()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::ini()"); }
}

bool ak::uiAPI::apiManager::isInitialized(void) const { return my_isInitialized; }

void ak::uiAPI::apiManager::addColorStyle(
	ak::ui::colorStyle *								_colorStyle,
	bool												_activate
) {
	try {
		assert(my_isInitialized); // Not initialized
		my_objManager->addColorStyle(_colorStyle, _activate);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::addColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::addColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::addColorStyle()"); }
}

void ak::uiAPI::apiManager::setColorStyle(
	const QString &				_colorStyleName
) {
	try {
		assert(my_isInitialized); // Not initialized
		my_objManager->setColorStyle(_colorStyleName);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setColorStyle()"); }
}

void ak::uiAPI::apiManager::setDarkColorStyle(void) {
	try {
		assert(my_isInitialized); // Not initialized
		my_objManager->setDefaultDarkColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setDarkColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setDarkColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setDarkColorStyle()"); }
}

void ak::uiAPI::apiManager::setDefaultColorStyle(void) {
	try {
		assert(my_isInitialized); // Not initialized
		my_objManager->setDefaultColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::setDefaultColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::setDefaultColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::setDefaultColorStyle()"); }
}

ak::messenger * ak::uiAPI::apiManager::messenger(void) const {
	try {
		assert(my_isInitialized); // API is not initialized
		return my_messenger;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::messenger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::messenger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::messenger()"); }
}

ak::uidManager * ak::uiAPI::apiManager::uidManager(void) const {
	try {
		assert(my_isInitialized); // API is not initialized
		return my_uidManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::uidManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::uidManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::uidManager()"); }
}

ak::ui::iconManager * ak::uiAPI::apiManager::iconManager(void) const {
	try {
		assert(my_isInitialized); // API is not initialized
		return my_iconManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::iconManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::iconManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::iconManager()"); }
}

ak::ui::objectManager * ak::uiAPI::apiManager::objectManager(void) const {
	try {
		assert(my_isInitialized); // API is not initialized
		return my_objManager;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::objectManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::objectManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::objectManager()"); }
}

ak::ui::colorStyle * ak::uiAPI::apiManager::currentColorStyle(void) const {
	try {
		assert(my_isInitialized); // API is not initialized
		return my_objManager->getCurrentColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::colorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::colorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::colorStyle()"); }
}

QString ak::uiAPI::apiManager::currentColorStyleName(void) const {
	assert(my_isInitialized); // API is not initialized
	return my_objManager->getCurrentColorStyleName();
}

int ak::uiAPI::apiManager::exec(void) {
	try {
		assert(my_app != nullptr); // App not created on initialization
		assert(!my_appIsRunning); // App is already running
		return my_app->exec();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::exec()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::exec()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::exec()"); }
}

QSurfaceFormat * ak::uiAPI::apiManager::getDefaultSurfaceFormat(void) {
	if (my_defaultSurfaceFormat == nullptr) { my_defaultSurfaceFormat = new QSurfaceFormat(); }
	return my_defaultSurfaceFormat;
}

ak::file * ak::uiAPI::apiManager::getFile(
	ak::UID												_fileUid
) {
	try {
		if (_fileUid == ak::invalidUID) {
			ak::file * f = new ak::file();
			f->setUid(my_fileUidManager->getId());
			my_mapFiles.insert_or_assign(f->uid(), f);
			return f;
		}
		else {
			my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
			if (itm == my_mapFiles.end()) { throw ak::Exception("Invalid file UID", "Check file UID"); }
			ak::file * f = itm->second;
			return f;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::getFile()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::getFile()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::getFile()"); }
}

ak::file * ak::uiAPI::apiManager::getExistingFile(
	ak::UID												_fileUid
) {
	try {
		my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
		if (itm == my_mapFiles.end()) { throw ak::Exception("Invalid file UID", "Check file UID"); }
		ak::file * f = itm->second;
		return f;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::getFile()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::getFile()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::getFile()"); }
}

void ak::uiAPI::apiManager::deleteFile(
	ak::UID												_fileUid
) {
	try {
		my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
		if (itm == my_mapFiles.end()) { throw ak::Exception("Invalid file UID", "Check file UID"); }
		ak::file * f = itm->second;
		delete f;
		my_mapFiles.erase(_fileUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::apiManager::deleteFile()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::apiManager::deleteFile()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::apiManager::deleteFile()"); }
}

void ak::uiAPI::apiManager::deleteAllFiles() {
	for (my_mapFilesIterator itm = my_mapFiles.begin(); itm != my_mapFiles.end(); itm++) {
		ak::file * f = itm->second; delete f;
	}
	my_mapFiles.clear();
}

// ###############################################################################################################################################

void ak::uiAPI::ini(
	bool												_createQApplication,
	int													_argc,
	char **												_argv,
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager,
	ak::ui::iconManager *								_iconManager,
	ak::ui::objectManager *								_objectManager
) { my_apiManager.ini(_createQApplication, _argc, _argv, _messenger, _uidManager, _iconManager, _objectManager); }

void ak::uiAPI::destroy(void) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->destroyAll();
	}
	catch (const ak::Exception &e) { throw ak::Exception(e, "ak::uiAPI::destroy()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::destroy()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::destroy()"); }
}

void ak::uiAPI::enableEventTypes(
	ak::core::eventType									_types
) { ak::singletonAllowedMessages::instance()->setFlag(_types); }

void ak::uiAPI::disableEventTypes(
	ak::core::eventType									_types
) { ak::singletonAllowedMessages::instance()->removeFlag(_types); }

std::vector<ak::core::eventType> ak::uiAPI::enabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->enabledMessages(); }

std::vector<ak::core::eventType> ak::uiAPI::disabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->disabledMessages(); }

std::string ak::uiAPI::getSettingsJSON(void) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getSettingsJSON();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getSettingsJSON()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getSettingsJSON()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getSettingsJSON()"); }
}

void ak::uiAPI::setupSettings(
	const char *										_json
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setupSettings(_json);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setupSettings()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setupSettings()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setupSettings()"); }
}

// ###############################################################################################################################################

ak::UID ak::uiAPI::registerUidNotifier(
	ak::UID												_senderUid,
	ak::notifier *										_notifier
) {
	try
	{
		// Check UID status
		return my_apiManager.messenger()->registerUidReceiver(_senderUid, _notifier);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::registerUidNotifier()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::registerUidNotifier()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::registerUidNotifier()"); }
}

ak::UID ak::uiAPI::registerEventTypeNotifier(
	ak::core::eventType									_event,
	ak::notifier *										_notifier
) {
	try
	{
		// Check UID status
		return my_apiManager.messenger()->registerEventTypeReceiver(_event, _notifier);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::registerEventTypeNotifier()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::registerEventTypeNotifier()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::registerEventTypeNotifier()"); }
}

ak::UID ak::uiAPI::registerAllMessagesNotifier(
	ak::notifier *										_notifier
) {
	try
	{
		// Check UID status
		return my_apiManager.messenger()->registerNotifierForAllMessages(_notifier);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::registerAllMessagesNotifier()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::registerAllMessagesNotifier()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::registerAllMessagesNotifier()"); }
}

void ak::uiAPI::sendMessage(
	ak::UID												_senderUid,
	ak::core::eventType									_event,
	int													_info1,
	int													_info2
) {
	try {
		ak::messenger * m = my_apiManager.messenger();
		m->sendMessage(_senderUid, _event, _info1, _info2);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::sendMessage()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::sendMessage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::sendMessage()"); }
}

void ak::uiAPI::setSurfaceFormatDefaultSamplesCount(
	int													_count
) {
	QSurfaceFormat * format = my_apiManager.getDefaultSurfaceFormat();
	format->setSamples(_count);
	QSurfaceFormat::setDefaultFormat(*format);
}

// ###############################################################################################################################################

ak::UID ak::uiAPI::createAction(
	ak::UID												_creatorUid,
	const char *										_text,
	const char *										_iconName,
	const char *										_iconSize
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
	ak::UID												_creatorUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
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
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
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
	ak::UID												_creatorUid,
	const char *										_text,
	bool												_checked
) {
	try {
		return createCheckbox(_creatorUid, QString(_text), _checked);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createCheckbox(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createCheckbox(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createCheckbox(char *)"); }
}

ak::UID ak::uiAPI::createCheckbox(
	ak::UID												_creatorUid,
	const QString &										_text,
	bool												_checked
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
	ak::UID												_creatorUid,
	int													_r,
	int													_g,
	int													_b,
	int													_a,
	const char *										_textOverride
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
	ak::UID												_creatorUid,
	int													_r,
	int													_g,
	int													_b,
	int													_a,
	const QString &										_textOverride
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
	ak::UID												_creatorUid,
	const ak::ui::color &								_color,
	const QString &										_textOverride
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
	ak::UID												_creatorUid
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
	ak::UID												_creatorUid,
	const char *										_text,
	const std::vector<ak::ui::qt::comboButtonItem> &	_possibleSelection
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
	ak::UID												_creatorUid,
	const QString &										_text,
	const std::vector<ak::ui::qt::comboButtonItem> &	_possibleSelection
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
	ak::UID												_creatorUid,
	const char *										_text
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
	ak::UID												_creatorUid,
	const QString &										_text
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
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
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

ak::UID ak::uiAPI::createDefaultWelcomeScreen(
	ak::UID												_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createDefaultWelcomeScreen(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createDefaultWelcomeScreen()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createDefaultWelcomeScreen()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createDefaultWelcomeScreen()"); }
}

ak::UID ak::uiAPI::createDock(
	ak::UID												_creatorUid,
	const char *										_text
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
	ak::UID												_creatorUid,
	const QString &										_text
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

ak::UID ak::uiAPI::createLogInDialog(
	ak::UID												_creatorUid,
	bool												_showSavePassword,
	const QString &										_imageName,
	const QString &										_username,
	const QString &										_password
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createLogInDialog(_creatorUid, _showSavePassword, _imageName, _username, _password);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createLogInDialog(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createLogInDialog(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createLogInDialog(QString)"); }
}

ak::UID ak::uiAPI::createPropertyGrid(
	ak::UID												_creatorUid
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
	ak::UID												_creatorUid,
	const char *										_text
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
	ak::UID												_creatorUid,
	const QString &										_text
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
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
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
	ak::UID												_creatorUid,
	int													_rows,
	int													_columns
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
	ak::UID												_creatorUid,
	ak::UID												_parentUid,
	const char *										_text
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
	ak::UID												_creatorUid,
	ak::UID												_parentUid,
	const QString &										_text
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
	ak::UID												_creatorUid,
	const char *										_initialText
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
	ak::UID												_creatorUid,
	const QString &										_initialText
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

ak::UID ak::uiAPI::createTimer(
	ak::UID												_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTimer(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTimer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTimer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTimer()"); }
}

ak::UID ak::uiAPI::createTree(
	ak::UID												_creatorUid
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
	ak::UID												_creatorUid
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

ak::UID ak::uiAPI::createTabView(
	ak::UID												_creatorUid
) {
	try {
		// Get manager
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->createTabView(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::createTabView()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::createTabView()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::createTabView()"); }
}

// ###############################################################################################################################################

// object setter

void ak::uiAPI::obj::setAlias(
	ak::UID												_objectUid,
	const QString &										_alias
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setAlias(_objectUid, _alias);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setAlias()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setAlias()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setAlias()"); }
}

void ak::uiAPI::obj::addObjectToContainer(
	ak::UID												_parentUid,
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addObjectToContainer(_parentUid, _objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addObjectToContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addObjectToContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addObjectToContainer()"); }
}

ak::ID ak::uiAPI::obj::addRecentsItem(
	ak::UID												_objectUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addRecentsItem(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addRecentsItem(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addRecentsItem(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addRecentsItem(QString)"); }
}

ak::ID ak::uiAPI::obj::addRecentsItem(
	ak::UID												_objectUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addRecentsItem(_objectUid, _text, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addRecentsItem(QString, QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addRecentsItem(QString, QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addRecentsItem(QString, QIcon)"); }
}

ak::ID ak::uiAPI::obj::addRecentsItem(
	ak::UID												_objectUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addRecentsItem(_objectUid, _text, _iconName, _iconSize);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addRecentsItem(QString, QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addRecentsItem(QString, QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addRecentsItem(QString, QString)"); }
}

ak::ID ak::uiAPI::obj::addOpenItem(
	ak::UID												_objectUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addOpenItem(_objectUid, _text, _iconName, _iconSize);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addOpenItem(QString, QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addOpenItem(QString, QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addOpenItem(QString, QString)"); }
}

void ak::uiAPI::obj::setCentralWidget(
	ak::UID												_parentUid,
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setCentralWidget(_parentUid, _objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setCentralWidget(UID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setCentralWidget(UID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setCentralWidget(UID)"); }
}	

void ak::uiAPI::obj::setCentralWidget(
	ak::UID												_parentUid,
	QWidget *											_widget
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setCentralWidget(_parentUid, _widget);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setCentralWidget(QWidget)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setCentralWidget(QWidget)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setCentralWidget(QWidget)"); }
}

void ak::uiAPI::obj::setText(
	ak::UID												_objectUid,
	const char *										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setText(_objectUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setText(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setText(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setText(char *)"); }
}

void ak::uiAPI::obj::setText(
	ak::UID												_objectUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setText(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setText(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setText(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setText(QString)"); }
}

void ak::uiAPI::obj::setChecked(
	ak::UID												_objectUid,
	bool												_checked
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setChecked(_objectUid, _checked);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setChecke()"); }
}

void ak::uiAPI::obj::setTristate(
	ak::UID												_objectUid,
	bool												_isTristate
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTristate(_objectUid, _isTristate);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setTristate()"); }
}

void ak::uiAPI::obj::setFilterVisible(
	ak::UID												_objectUid,
	bool												_vis
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterVisible(_objectUid, _vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setFilterVisible()"); }
}

void ak::uiAPI::obj::setFilterCaseSensitive(
	ak::UID												_objectUid,
	bool												_caseSensitive,
	bool												_refresh
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterCaseSensitive(_objectUid, _caseSensitive, _refresh);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setFilterCaseSensitive()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setFilterCaseSensitive()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setFilterCaseSensitive()"); }
}

void ak::uiAPI::obj::setFilterRefreshOnChange(
	ak::UID												_objectUid,
	bool												_refreshOnChange
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setFilterRefreshOnChange(_objectUid, _refreshOnChange);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setFilterRefreshOnChange()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setFilterRefreshOnChange()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setFilterRefreshOnChange()"); }
}

void ak::uiAPI::obj::setItems(
	ak::UID												_objectUid,
	const std::vector<ak::ui::qt::comboButtonItem> &	_items
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setItems(_objectUid, _items);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setItems()"); }
}

void ak::uiAPI::obj::setColor(
	ak::UID												_objectUid,
	int													_r,
	int													_g,
	int													_b,
	int													_a
) {
	try {
		setColor(_objectUid, ak::ui::color(_r, _g, _b, _a));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setColor(RGBA)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setColor(RGBA)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setColor(RGBA)"); }
}

void ak::uiAPI::obj::setColor(
	ak::UID												_objectUid,
	const ak::ui::color &								_color
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setColor(_objectUid, _color);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setColor(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setColor(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setColor(color)"); }
}

void ak::uiAPI::obj::setIcon(
	ak::UID												_objectUid,
	const char *										_iconName,
	const char *										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, *iM->icon(QString(_iconName), QString(_iconSize)));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setIcon(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setIcon(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setIcon(char *)"); }
}

void ak::uiAPI::obj::setIcon(
	ak::UID												_objectUid,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, *iM->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setIcon(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setIcon(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setIcon(QString)"); }
}

void ak::uiAPI::obj::setIcon(
	ak::UID												_objectUid,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		ak::ui::iconManager * iM = my_apiManager.iconManager();
		oM->obj_setIcon(_objectUid, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setIcon(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setIcon(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setIcon(QIcon)"); }
}

void ak::uiAPI::obj::setReadOnly(
	ak::UID												_objectUid,
	bool												_readOnly
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setReadOnly(_objectUid, _readOnly);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setReadOnly()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setReadOnly()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setReadOnly()"); }
}

void ak::uiAPI::obj::setTabToolBarVisible(
	ak::UID												_uiManagerUid,
	bool												_vis
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTabToolBarVisible(_uiManagerUid, _vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setTabToolBarVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setTabToolBarVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setTabToolBarVisible()"); }
}

void ak::uiAPI::obj::setTabLocation(
	ak::UID												_objectUid,
	ak::ui::core::tabLocation							_location
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTabLocation(_objectUid, _location);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setTabLocation()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setTabLocation()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setTabLocation()"); }
}

void ak::uiAPI::obj::setTabFocused(
	ak::UID												_objectUid,
	ak::ID												_tab
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setTabFocused(_objectUid, _tab);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setTabFocused()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setTabFocused()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setTabFocused()"); }
}

void ak::uiAPI::obj::appendText(
	ak::UID												_objectUid,
	const char *										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_appendText(_objectUid, QString(_text));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::appendText(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::appendText(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::appendText(char *)"); }
}

void ak::uiAPI::obj::appendText(
	ak::UID												_objectUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_appendText(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::appendText(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::appendText(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::appendText(QString)"); }
}

void ak::uiAPI::obj::setAutoScrollToBottomEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_setAutoScrollToBottomEnabled(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setAutoScrollToBottomEnabled()"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	bool												_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(bool)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(bool)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(bool)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	bool												_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(bool)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(bool)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(bool)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	int													_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(int)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(int)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(int)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	int													_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(int)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(int)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(int)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	double												_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(double)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(double)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(double)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	double												_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(double)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(double)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addProperty(double)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	const char *										_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), QString(_value), _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(QString)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const QString &										_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(QString)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	const ak::ui::color &								_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(color)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const ak::ui::color &								_value,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(color)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const char *										_itemName,
	const std::vector<QString> &						_selection,
	const QString &										_selectedValue,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, QString(_itemName), toComboButtonItem(_selection), _selectedValue, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
}

void ak::uiAPI::obj::addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const std::vector<QString> &						_selection,
	const QString &										_selectedValue,
	bool												_isMultipleValues
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_addProperty(_objectUid, _itemName, toComboButtonItem(_selection), _selectedValue, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addProperty(comboButtonItems)"); }
}

ak::ID ak::uiAPI::obj::createItem(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	const QString &										_text,
	ak::ui::core::textAlignment							_textAlignment,
	QIcon												_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_createItem(_objectUid, _parentId, _text, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::createItem(parent)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::createItem(parent)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::createItem(parent)"); }
}

ak::ID ak::uiAPI::obj::createItem(
	ak::UID												_objectUid,
	ak::ID												_parentItemId,
	const QString &										_iconName,
	const QString &										_iconSize,
	const QString &										_text,
	ak::ui::core::textAlignment							_textAlignment
) {
	try {
		return createItem(_objectUid, _parentItemId, _text, _textAlignment, *my_apiManager.iconManager()->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::createItem(parent, icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::createItem(parent, icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::createItem(parent, icon)"); }
}

ak::ID ak::uiAPI::obj::createItem(
	ak::UID												_objectUid,
	const QString &										_cmd,
	char												_delimiter,
	ak::ui::core::textAlignment							_textAlignment,
	const QIcon  &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_createItem(_objectUid, _cmd, _delimiter, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::createItem(command)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::createItem(command)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::createItem(command)"); }
}

ak::ID ak::uiAPI::obj::createItem(
	ak::UID												_objectUid,
	const QString &										_cmd,
	const QString &										_iconName,
	const QString &										_iconSize,
	char												_delimiter,
	ak::ui::core::textAlignment							_textAlignment
) {
	try {
		return createItem(_objectUid, _cmd, _delimiter, _textAlignment, *my_apiManager.iconManager()->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::createItem(command, icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::createItem(command, icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::createItem(command, icon)"); }
}

void ak::uiAPI::obj::deselectAllItems(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_deselectAllItems(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::deselectAllItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::deselectAllItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::deselectAllItems()"); }
}

ak::ID ak::uiAPI::obj::addTab(
	ak::UID												_objectUid,
	ak::UID												_widgetUid,
	const QString &										_title,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addTab(_objectUid, _widgetUid, _title, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addTab(UID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addTab(UID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addTab(UID)"); }
}

ak::ID ak::uiAPI::obj::addTab(
	ak::UID												_objectUid,
	QWidget *											_widget,
	const QString &										_title,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addTab(_objectUid, _widget, _title, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addTab()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addTab()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addTab()"); }
}

ak::ID ak::uiAPI::obj::addTab(
	ak::UID												_objectUid,
	ak::UID												_widgetUid,
	const QString &										_title,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addTab(_objectUid, _widgetUid, _title, _iconName, _iconSize);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addTab(UID, icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addTab(UID, icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addTab(UID, icon)"); }
}

ak::ID ak::uiAPI::obj::addTab(
	ak::UID												_objectUid,
	QWidget *											_widget,
	const QString &										_title,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_addTab(_objectUid, _widget, _title, _iconName, _iconSize);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::addTab(widget, icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::addTab(widget, icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::addTab(widget, icon)"); }
}

void ak::uiAPI::obj::setMultiSelectionEnabled(
	ak::UID												_objectUid,
	bool												_multiSelection
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setMultiSelectionEnabled(_objectUid, _multiSelection);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setMultiSelectionEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setMultiSelectionEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setMultiSelectionEnabled()"); }
}

void ak::uiAPI::obj::setAutoSelectAndDeselectChildrenEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setAutoSelectAndDeselectChildrenEnabled(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
}

void ak::uiAPI::obj::setAutoExpandSelectedItems(
	ak::UID												_objectUid,
	bool												_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setAutoExpandSelectedItems(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setSelectAndDeselectChildrenEnabled()"); }
}

void ak::uiAPI::obj::setEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setEnabled(_objectUid, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setEnabled()"); }
}

void ak::uiAPI::obj::setVisible(
	ak::UID												_objectUid,
	bool												_visible
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_setVisible(_objectUid, _visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::setVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::setVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::setVisible()"); }
}

void ak::uiAPI::obj::clear(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_clear(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::clear()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::clear()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::clear()"); }
}

void ak::uiAPI::obj::deleteItems(
	ak::UID												_objectUid,
	const std::vector<ak::ID> &							_items
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		std::vector<ak::ID> v(_items);
		std::sort(v.begin(), v.end());
		v.erase(std::unique(v.begin(), v.end()), v.end());
		return oM->obj_deleteItems(_objectUid, v);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::deleteItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::deleteItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::deleteItems()"); }
}

void ak::uiAPI::obj::start(
	ak::UID												_objectUid,
	int													_value
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_start(_objectUid, _value);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::start()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::start()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::start()"); }
}

void ak::uiAPI::obj::stop(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_stop(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::stop()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::stop()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::stop()"); }
}

void ak::uiAPI::obj::shoot(
	ak::UID												_objectUid,
	int													_value
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_shoot(_objectUid, _value);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::shoot()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::shoot()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::shoot()"); }
}

void ak::uiAPI::obj::destroy(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_delete(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::destroy()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::destroy()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::destroy()"); }
}

void ak::uiAPI::obj::showMaximized(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->obj_showMaximized(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::showMaximized()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::showMaximized()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::showMaximized()"); }
}

// ###############################################################################################################################################

// Object getter

QString ak::uiAPI::obj::getAlias(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getAlias(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getAlias()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getAlias()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getAlias()"); }
}

QString ak::uiAPI::obj::getText(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getText(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getText()"); }
}

bool ak::uiAPI::obj::getTristate(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getTristate(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getTristate()"); }
}

bool ak::uiAPI::obj::getChecked(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getChecked(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getChecked()"); }
}

std::vector<ak::ID> ak::uiAPI::obj::getSelectedItems(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getSelectedItems(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getSelectedItems()"); }
}

bool ak::uiAPI::obj::getAutoScrollToBottomEnabled(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getAutoScrollToBottomEnabled(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getAutoScrollToBottomEnabled()"); }
}

ak::ID ak::uiAPI::obj::getItem(
	ak::UID												_objectUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getItem(_objectUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getItem()"); }
}

int ak::uiAPI::obj::getItemCount(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getItemCount(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getItemCount()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getItemCount()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getItemCount()"); }
}

int ak::uiAPI::obj::getFocusedTab(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getFocusedTab(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getFocusedTab()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getFocusedTab()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getFocusedTab()"); }
}

bool ak::uiAPI::obj::getAutoExpandSelectedItems(
	ak::UID												_objectUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->obj_getAutoExpandSelectedItems(_objectUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::obj::getAutoExpandSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::obj::getAutoExpandSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::obj::getAutoExpandSelectedItems()"); }
}

// ###############################################################################################################################################

// Item setter

void ak::uiAPI::itm::setSelected(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_selected
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setSelected(_objectUid, _itemId, _selected);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setItemSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setItemSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setItemSelected()"); }
}

void ak::uiAPI::itm::setSingleSelected(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_selected
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setSingleSelected(_objectUid, _itemId, _selected);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setSingleSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setSingleSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setSingleSelected()"); }
}

void ak::uiAPI::itm::toggleSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_toggleSelection(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::toggleSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::toggleSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::toggleSelection()"); }
}

void ak::uiAPI::itm::setText(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setText(_objectUid, _itemId, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setText()"); }
}

void ak::uiAPI::itm::setIcon(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QIcon &										_icon
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setIcon(_objectUid, _itemId, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setIcon(QIcon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setIcon(QIcon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setIcon(QIcon)"); }
}

void ak::uiAPI::itm::setIcon(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setIcon(_objectUid, _itemId, _iconName, _iconSize);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setIcon()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setIcon()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setIcon()"); }
}

void ak::uiAPI::itm::setEnabled(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_enabled
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setEnabled(_objectUid, _itemId, _enabled);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setEnabled()"); }
}

void ak::uiAPI::itm::setVisible(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_visible
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_setVisible(_objectUid, _itemId, _visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::setVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::setVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::setVisible()"); }
}

// ###############################################################################################################################################

// Item getter

std::vector<QString> ak::uiAPI::itm::getPath(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getPath(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getPath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getPath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getPath()"); }
}

QString ak::uiAPI::itm::getPathString(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	char												_delimiter
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getPathString(_objectUid, _itemId, _delimiter);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getPathString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getPathString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getPathString()"); }
}

QString ak::uiAPI::itm::getText(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getText(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getText()"); }
}

ak::core::valueType ak::uiAPI::itm::getValueType(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueType(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueType()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueType()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueType()"); }
}

ak::ID ak::uiAPI::itm::getID(
	ak::UID												_objectUid,
	const QString &										_itemPath,
	char												_delimiter
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getID(_objectUid, _itemPath, _delimiter);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getID()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getID()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getID()"); }
}

bool ak::uiAPI::itm::getValueBoolean(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueBoolean(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueBoolean()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueBoolean()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueBoolean()"); }
}

ak::ui::color ak::uiAPI::itm::getValueColor(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueColor(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueColor()"); }
}

double ak::uiAPI::itm::getValueDouble(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueDouble(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueDouble()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueDouble()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueDouble()"); }
}

int ak::uiAPI::itm::getValueInteger(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueInteger(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueInteger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueInteger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueInteger()"); }
}

std::vector<QString> ak::uiAPI::itm::getValuePossibleSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValuePossibleSelection(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValuePossibleSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValuePossibleSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValuePossibleSelection()"); }
}

QString ak::uiAPI::itm::getValueSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueSelection(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueSelection()"); }
}

QString ak::uiAPI::itm::getValueString(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueString(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueString()"); }
}

bool ak::uiAPI::itm::getValueIsMultivalued(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->itm_getValueIsMultivalued(_objectUid, _itemId);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::itm::getValueIsMultivalued()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::itm::getValueIsMultivalued()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::itm::getValueIsMultivalued()"); }
}

// ###############################################################################################################################################

void ak::uiAPI::special::showMessageBox(
	ak::UID												_uiManagerUid,
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

void ak::uiAPI::special::showMessageBox(
	ak::UID												_uiManagerUid,
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

QString ak::uiAPI::special::openFileDialog(
	ak::UID												_uiManagerUid,
	const QString &										_caption,
	const QString &										_initialDir,
	const QString &										_filter,
	QString *											_selectedFilter
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		// Get UI manager
		ak::ui::core::aObject * obj = oM->obj_get(_uiManagerUid);
		if (obj->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected main window", "Check object type"); }
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		return QFileDialog::getOpenFileName(nullptr, _caption, _initialDir, _filter, _selectedFilter);
		
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::special::openFileDialog()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::special::openFileDialog()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::special::openFileDialog()"); }
}

QString ak::uiAPI::special::saveFileDialog(
	ak::UID												_uiManagerUid,
	const QString &										_caption,
	const QString &										_initialDir,
	const QString &										_filter,
	QString *											_selectedFilter
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		// Get UI manager
		ak::ui::core::aObject * obj = oM->obj_get(_uiManagerUid);
		if (obj->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected main window", "Check object type"); }
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		return QFileDialog::getSaveFileName(nullptr, _caption, _initialDir, _filter, _selectedFilter);

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::special::saveFileDialog()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::special::saveFileDialog()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::special::saveFileDialog()"); }
}

QString ak::uiAPI::special::createEventText(
	ak::UID												_sender,
	ak::core::eventType									_event,
	int													_info1,
	int													_info2
) {
	QString str("Event{Sender=\"");
	str.append(QString::number(_sender));
	str.append("\";EventType=\"");
	str.append(ak::uiAPI::toString(_event));
	str.append("\";Info1=\"");
	str.append(QString::number(_info1));
	if (_event == ak::core::eKeyPressed || _event == ak::core::eKeyReleased) {
		str.append("\";Info2=Key{KeyName=\"");
		str.append(ak::uiAPI::toString((ak::ui::core::keyType)_info2));
		str.append("\"}}");
	} else {
		str.append("\";Info2=\"");
		str.append(QString::number(_info2));
		str.append("\"}");
	}
	return str;
}

// ###############################################################################################################################################

// File setter

ak::UID ak::uiAPI::file::load(
	const QString &										_filePath
) {
	try {
		ak::file * f = my_apiManager.getFile(ak::invalidUID);
		f->load(_filePath);
		return f->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::load()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::load()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::load()"); }
}

void ak::uiAPI::file::load(
	ak::UID												_fileUid,
	const QString &										_filePath
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->load(_filePath);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::load(file)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::load(file)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::load(file)"); }
}

void ak::uiAPI::file::save(
	ak::UID												_fileUid,
	bool												_append
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->save(_append);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::save()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::save()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::save()"); }
}

void ak::uiAPI::file::save(
	ak::UID												_fileUid,
	const QString &										_filePath,
	bool												_append
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->save(_filePath, _append);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::save(filePath)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::save(filePath)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::save(filePath)"); }
}

void ak::uiAPI::file::setPath(
	ak::UID												_fileUid,
	const QString &										_path
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->setPath(_path);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::setPath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::setPath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::setPath()"); }
}

void ak::uiAPI::file::setLines(
	ak::UID												_fileUid,
	const QStringList &									_lines
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->setLines(_lines);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::setLines(QStringList)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::setLines(QStringList)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::setLines(QStringList)"); }
}

void ak::uiAPI::file::addLine(
	ak::UID												_fileUid,
	const QString &										_line
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		QStringList lst = _line.split("\n");
		for (QString str : lst) { str.append('\n'); f->addLine(str); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::addLine(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::addLine(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::addLine(QString)"); }
}

void ak::uiAPI::file::addLine(
	ak::UID												_fileUid,
	const QStringList &									_lines
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		f->addLine(_lines);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::addLine(QStringList)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::addLine(QStringList)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::addLine(QStringList)"); }
}

// ###############################################################################################################################################

// File Getter

ak::UID ak::uiAPI::file::uid(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::uid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::uid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::uid()"); }
}

QString ak::uiAPI::file::name(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->name();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::name()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::name()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::name()"); }
}

QString ak::uiAPI::file::path(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->path();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::path()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::path()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::path()"); }
}

QString ak::uiAPI::file::extension(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->extension();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::extension()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::extension()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::extension()"); }
}

QStringList ak::uiAPI::file::lines(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->lines();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::lines()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::lines()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::lines()"); }
}

int ak::uiAPI::file::linesCount(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->linesCount();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::linesCount()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::linesCount()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::linesCount()"); }
}

bool ak::uiAPI::file::hasChanged(
	ak::UID												_fileUid
) {
	try {
		ak::file * f = my_apiManager.getExistingFile(_fileUid);
		return f->hasChanged();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::file::hasChanged()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::file::hasChanged()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::file::hasChanged()"); }
}

// ###############################################################################################################################################

// Dialogs

ak::ui::core::dialogResult ak::uiAPI::dialog::show(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->dialog_show(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::show()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::show()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::show()"); }
}

ak::ui::core::dialogResult ak::uiAPI::dialog::result(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->dialog_result(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::result()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::result()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::result()"); }
}

QString ak::uiAPI::dialog::username(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->dialog_username(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::username()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::username()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::username()"); }
}

QString ak::uiAPI::dialog::password(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->dialog_password(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::password()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::password()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::password()"); }
}

bool ak::uiAPI::dialog::savePassword(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->dialog_savePassword(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::savePassword()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::savePassword()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::savePassword()"); }
}

void ak::uiAPI::dialog::showInvalidLogIn(
	ak::UID												_dialogUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->dialog_showInvalidLogIn(_dialogUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::showInvalidLogIn()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::showInvalidLogIn()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::showInvalidLogIn()"); }
}

void ak::uiAPI::dialog::close(
	ak::UID												_dialogUid,
	ak::ui::core::dialogResult							_result
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->dialog_close(_dialogUid, _result);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::close()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::close()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::close()"); }
}

ak::ui::core::dialogResult ak::uiAPI::dialog::showPrompt(
	const QString &										_message,
	const QString &										_title,
	ak::ui::core::promptType							_type
) {
	try {
		ui::dialog::prompt dia(_message, _title, _type);
		const ak::ui::colorStyle * cS = my_apiManager.currentColorStyle();
		if (cS != nullptr) { dia.setColorStyle(cS); }
		return dia.showDialog();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::dialog::showPrompt()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::dialog::showPrompt()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::dialog::showPrompt()"); }
}

// ###############################################################################################################################################

QString ak::uiAPI::toString(
	ak::core::eventType									_type
) {
	try {
		return ak::core::toQString(_type);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(eventType)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(eventType)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(eventType)"); }
}

QString ak::uiAPI::toString(
	ak::core::valueType									_type
) {
	try {
		return ak::core::toQString(_type);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(valueType)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(valueType)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(valueType)"); }
}

QString ak::uiAPI::toString(
	ak::ui::core::textAlignment							_type
) {
	try {
		return ak::ui::core::toQString(_type);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(textAlignment)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(textAlignment)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(textAlignment)"); }
}

QString ak::uiAPI::toString(
	ak::ui::core::dockLocation							_dockLocation
) {
	try {
		return ak::ui::core::toQString(_dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(dockLocation)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(dockLocation)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(dockLocation)"); }
}

QString ak::uiAPI::toString(
	ak::ui::core::tabLocation							_tabLocation
) {
	try {
		return ak::ui::core::toQString(_tabLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(tabLocation)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(tabLocation)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(tabLocation)"); }
}

QString ak::uiAPI::toQString(
	ak::ui::core::dialogResult							_dialogResult
) {
	try { return ak::ui::core::toQString(_dialogResult); }
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(dialogResult)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(dialogResult)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(dialogResult)"); }
}

QString ak::uiAPI::toString(
	ak::ui::core::keyType								_type
) {
	try {
		return ak::ui::core::toQString(_type);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(keyType)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(keyType)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(keyType)"); }
}

QString ak::uiAPI::toString(
	ak::ui::core::objectType							_type
) {
	try {
		return ak::ui::core::toQString(_type);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::toString(objectType)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::toString(objectType)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::toString(objectType)"); }
}

// ###############################################################################################################################################

void ak::uiAPI::creatorDestroyed(
	ak::UID												_creatorUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->creatorDestroyed(_creatorUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::creatorDestroyed()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::creatorDestroyed()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::creatorDestroyed()"); }
}

void ak::uiAPI::addColorStyle(
	ak::ui::colorStyle *								_colorStyle,
	bool												_activate
) {
	try {
		my_apiManager.addColorStyle(_colorStyle, _activate);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setColorStyle()"); }
}

void ak::uiAPI::setColorStyle(
	const QString &				_colorStyleName
) {
	try {
		my_apiManager.setColorStyle(_colorStyleName);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setColorStyle()"); }
}

const ak::ui::colorStyle *  ak::uiAPI::getCurrentColorStyle(void) {
	try { return my_apiManager.currentColorStyle(); }
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getColorStyle()"); }
}

QString ak::uiAPI::getCurrentColorStyleName(void) { return my_apiManager.currentColorStyleName(); }

void ak::uiAPI::setDefaultDarkColorStyle() {
	try {
		my_apiManager.setDarkColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDefaultDarkColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDefaultDarkColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDefaultDarkColorStyle()"); }
}

void ak::uiAPI::setDefaultColorStyle() {
	try {
		my_apiManager.setDefaultColorStyle();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDefaultBrightColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDefaultBrightColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDefaultBrightColorStyle()"); }
}

void ak::uiAPI::setStatusLabelVisible(
	ak::UID												_uiManagerUid,
	bool												_visible
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setStatusLabelVisible(_uiManagerUid, _visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setStatusLabelVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setStatusLabelVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setStatusLabelVisible()"); }
}

void ak::uiAPI::setStatusProgressVisible(
	ak::UID												_uiManagerUid,
	bool												_visible
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setStatusProgressVisible(_uiManagerUid, _visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setStatusProgressVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setStatusProgressVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setStatusProgressVisible()"); }
}

void ak::uiAPI::setStatusLabelText(
	ak::UID												_uiManagerUid,
	const QString &										_text
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setStatusLabelText(_uiManagerUid, _text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setStatusLabelText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setStatusLabelText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setStatusLabelText()"); }
}

void ak::uiAPI::setStatusProgressValue(
	ak::UID												_uiManagerUid,
	int													_value
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setStatusProgressValue(_uiManagerUid, _value);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setStatusProgressValue()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setStatusProgressValue()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setStatusProgressValue()"); }
}

void ak::uiAPI::setStatusProgressContinuous(
	ak::UID												_uiManagerUid,
	bool												_continuous
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		oM->setStatusProgressContinuous(_uiManagerUid, _continuous);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setStatusProgressContinuous()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setStatusProgressContinuous()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setStatusProgressContinuous()"); }
}

bool ak::uiAPI::getStatusLabelVisible(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getStatusLabelVisible(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getStatusLabelVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getStatusLabelVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getStatusLabelVisible()"); }
}

bool ak::uiAPI::getStatusProgressVisible(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getStatusProgressVisible(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getStatusProgressVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getStatusProgressVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getStatusProgressVisible()"); }
}

QString ak::uiAPI::getStatusLabelText(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getStatusLabelText(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getStatusLabelText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getStatusLabelText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getStatusLabelText()"); }
}

int ak::uiAPI::getStatusProgressValue(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getStatusProgressValue(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getStatusProgressValue()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getStatusProgressValue()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getStatusProgressValue()"); }
}

bool ak::uiAPI::getStatusProgressContinuous(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * oM = my_apiManager.objectManager();
		return oM->getStatusProgressContinuous(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getStatusProgressContinuous()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getStatusProgressContinuous()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getStatusProgressContinuous()"); }
}

// ###############################################################################################################################################

// dock

void ak::uiAPI::addDock(
	ak::UID												_uiManagerUid,
	ak::UID												_dockUid,
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
	ak::UID												_uiManagerUid,
	ak::UID												_parentUid,
	ak::UID												_dockUid
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->tabifyDock(_uiManagerUid, _parentUid, _dockUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::tabifyDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::tabifyDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::tabifyDock()"); }
}

void ak::uiAPI::setDockBottomLeftPriority(
	ak::UID												_uiManagerUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->setDockBottomLeftPriority(_uiManagerUid, _dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDockBottomLeftPriority()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDockBottomLeftPriority()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDockBottomLeftPriority()"); }
}

void ak::uiAPI::setDockBottomRightPriority(
	ak::UID												_uiManagerUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->setDockBottomRightPriority(_uiManagerUid, _dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::setDockBottomRightPriority()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::setDockBottomRightPriority()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::setDockBottomRightPriority()"); }
}

void ak::uiAPI::closeWindow(
	ak::UID												_uiManagerUid
) {
	try {
		ak::ui::objectManager * obj = my_apiManager.objectManager();
		obj->close(_uiManagerUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::closeWindow()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::closeWindow()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::closeWindow()"); }
}

// ###############################################################################################################################################

void ak::uiAPI::addIconSearchPath(
	const QString &										_path
) {
	try {
		ak::ui::objectManager * manager = my_apiManager.objectManager();
		manager->addIconSearchPath(_path);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::addIconSearchPath(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::addIconSearchPath(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::addIconSearchPath(QString)"); }
}

void ak::uiAPI::removeIconSearchPath(
	const QString &										_path
) {
	try {
		ak::ui::objectManager * manager = my_apiManager.objectManager();
		manager->removeIconSearchPath(_path);
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

const QIcon & ak::uiAPI::getIcon(
	const QString &											_name,
	const QString &											_size
) {
	try {
		// Return icon manager
		return *my_apiManager.iconManager()->icon(_name, _size);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getIcon()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getIcon()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getIcon()"); }
}

// ###############################################################################################################################################

// special

int ak::uiAPI::exec(void) {
	try {
		// Return icon manager
		return my_apiManager.exec();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::uiAPI::getIconManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::uiAPI::getIconManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::uiAPI::getIconManager()"); }
}

std::vector<ak::ui::qt::comboButtonItem> ak::uiAPI::toComboButtonItem(
	const std::vector<QString> &						_items
) {
	std::vector<ak::ui::qt::comboButtonItem> ret;
	if (_items.size() > 0) {
		ret.reserve(_items.size());
		assert(ret.size() == 0);

		for (QString str : _items) {
			ak::ui::qt::comboButtonItem itm(str);
			ret.push_back(itm);
		}
	}
	assert(ret.size() == _items.size());
	return ret;
}

void ak::uiAPI::testCall(void) {
	assert(0); // No functionallity
}
