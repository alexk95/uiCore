/*
 * ak_uiAPI.cpp
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK main header
#include <ak_uiAPI.h>						// corresponding header
#include <ak_messenger.h>					// messenger
#include <ak_ui_objectManager.h>			// objectManager
#include <ak_notifierStaticEvent.h>			// notifierStaticEvent
#include <ak_notifier.h>					// notifier
#include <ak_exception.h>					// error handling

// AK specialized header
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_iconManager.h>				// iconManager
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_uidMangager.h>					// UID manager
#include <ak_singletonAllowedMessages.h>	// allowed messages

// AK object and widgets
#include <ak_ui_core_aObject.h>
#include <ak_ui_core_aWidget.h>
#include <ak_ui_core_aRestorable.h>
#include <ak_ui_core_ttbContainer.h>
#include <ak_ui_qt_action.h>
#include <ak_ui_qt_dock.h>					// dock
#include <ak_ui_widget_tree.h>				// tree
#include <ak_ui_widget_propertyGrid.h>		// propertyGrid
#include <ak_ui_qt_textEdit.h>
#include <ak_ui_widget_welcomeScreen.h>
#include <ak_ui_widget_tabView.h>
#include <ak_file.h>						// file
#include <ak_ui_dialog_prompt.h>			// prompt dialog
#include <ak_ui_dialog_logIn.h>
#include <ak_ui_qt_timer.h>

// Qt header
#include <qsurfaceformat.h>					// QSurfaceFormat
#include <qfiledialog.h>					// Open/Save file dialog
#include <qfile.h>

static ak::uiAPI::apiManager		my_apiManager;					//! The API manager
static ak::ui::objectManager *		my_objManager = nullptr;					//! The object manager used in this API
static ak::messenger *				my_messenger = nullptr;					//! The messenger used in this API
static ak::uidManager *				my_uidManager = nullptr;					//! The UID manager used in this API
static ak::ui::iconManager *		my_iconManager = nullptr;					//! The icon manager used in this API

ak::uiAPI::apiManager::apiManager()
	: my_iconManagerIsExtern(false),
	my_messengerIsExtern(false),
	my_objManagerIsExtern(false),
	my_uidManagerIsExtern(false),
	my_isInitialized(false),
	my_appIsRunning(false),
	my_defaultSurfaceFormat(nullptr),
	my_fileUidManager(nullptr),
	my_app(nullptr)
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
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager,
	ak::ui::iconManager *								_iconManager,
	ak::ui::objectManager *								_objectManager
) {
	assert(!my_isInitialized); // Is already initialized
	my_app = new ui::application();
	
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
		my_objManager = new ak::ui::objectManager(my_messenger, my_uidManager);
		assert(my_objManager != nullptr); // Failed to create
	}
	else {
		assert(my_messengerIsExtern);	// Internal messenger cannot be used with external objectManager
		assert(my_uidManagerIsExtern);	// Internal uidManager cannot be used with external objectManager
		my_objManager = _objectManager;
	}

	my_isInitialized = true;
}

bool ak::uiAPI::apiManager::isInitialized(void) const { return my_isInitialized; }

int ak::uiAPI::apiManager::exec(void) {
	assert(my_isInitialized);	// API not initialized
	return my_app->exec();
}

QSurfaceFormat * ak::uiAPI::apiManager::getDefaultSurfaceFormat(void) {
	if (my_defaultSurfaceFormat == nullptr) { my_defaultSurfaceFormat = new QSurfaceFormat(); }
	return my_defaultSurfaceFormat;
}

ak::file * ak::uiAPI::apiManager::getFile(
	ak::UID												_fileUid
) {
	if (_fileUid == ak::invalidUID) {
		ak::file * f = new ak::file();
		f->setUid(my_fileUidManager->getId());
		my_mapFiles.insert_or_assign(f->uid(), f);
		return f;
	}
	else {
		my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
		assert(itm != my_mapFiles.end());	// Invalid file UID
		ak::file * f = itm->second;
		return f;
	}
}

ak::file * ak::uiAPI::apiManager::getExistingFile(
	ak::UID												_fileUid
) {
	my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
	assert(itm != my_mapFiles.end());	// Invalid file UID
	ak::file * f = itm->second;
	return f;
}

void ak::uiAPI::apiManager::deleteFile(
	ak::UID												_fileUid
) {
	my_mapFilesIterator itm = my_mapFiles.find(_fileUid);
	assert(itm != my_mapFiles.end());	// Invalid file UID
	ak::file * f = itm->second;
	delete f;
	my_mapFiles.erase(_fileUid);
}

void ak::uiAPI::apiManager::deleteAllFiles() {
	for (my_mapFilesIterator itm = my_mapFiles.begin(); itm != my_mapFiles.end(); itm++) {
		ak::file * f = itm->second; delete f;
	}
	my_mapFiles.clear();
}

// ###############################################################################################################################################

void ak::uiAPI::ini(
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager,
	ak::ui::iconManager *								_iconManager,
	ak::ui::objectManager *								_objectManager
) { my_apiManager.ini(_messenger, _uidManager, _iconManager, _objectManager); }

void ak::uiAPI::destroy(void) {	if (my_objManager != nullptr) { my_objManager->destroyAll(); } }

void ak::uiAPI::enableEventTypes(
	ak::core::eventType									_types
) { ak::singletonAllowedMessages::instance()->setFlag(_types); }

void ak::uiAPI::disableEventTypes(
	ak::core::eventType									_types
) { ak::singletonAllowedMessages::instance()->removeFlag(_types); }

std::vector<ak::core::eventType> ak::uiAPI::enabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->enabledMessages(); }

std::vector<ak::core::eventType> ak::uiAPI::disabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->disabledMessages(); }

std::string ak::uiAPI::saveStateWindow(void) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->saveStateWindow();
}

std::string ak::uiAPI::saveStateColorStyle(void) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->saveStateColorStyle();
}

void ak::uiAPI::restoreStateWindow(
	const std::string &									_json
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return; }
	my_objManager->restoreStateWindow(_json.c_str());
}

void ak::uiAPI::restoreStateColorStyle(
	const std::string &									_json
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return; }
	my_objManager->restoreStateColorStyle(_json.c_str());
}

// ###############################################################################################################################################

ak::UID ak::uiAPI::registerUidNotifier(
	ak::UID												_senderUid,
	ak::notifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerUidReceiver(_senderUid, _notifier);
}

ak::UID ak::uiAPI::registerEventTypeNotifier(
	ak::core::eventType									_event,
	ak::notifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerEventTypeReceiver(_event, _notifier);
}

ak::UID ak::uiAPI::registerAllMessagesNotifier(
	ak::notifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerNotifierForAllMessages(_notifier);
}

void ak::uiAPI::sendMessage(
	ak::UID												_senderUid,
	ak::core::eventType									_event,
	int													_info1,
	int													_info2
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->sendMessage(_senderUid, _event, _info1, _info2);
}

void ak::uiAPI::setSurfaceFormatDefaultSamplesCount(
	int													_count
) {
	QSurfaceFormat * format = my_apiManager.getDefaultSurfaceFormat();
	format->setSamples(_count);
	QSurfaceFormat::setDefaultFormat(*format);
}

// ###############################################################################################################################################

// Object creation

ak::UID ak::uiAPI::createAction(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	return my_objManager->createAction(_creatorUid, _text, *my_iconManager->icon(_iconName, _iconSize));
}

ak::UID ak::uiAPI::createAction(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createAction(_creatorUid, _text, _icon);
}

ak::UID ak::uiAPI::createCheckbox(
	ak::UID												_creatorUid,
	const QString &										_text,
	bool												_checked
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createCheckBox(_creatorUid, _text, _checked);
}

ak::UID ak::uiAPI::createColorEditButton(
	ak::UID												_creatorUid,
	int													_r,
	int													_g,
	int													_b,
	int													_a,
	const QString &										_textOverride
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createColorEditButton(_creatorUid, ui::color(_r, _g, _b, _a), _textOverride);
}

ak::UID ak::uiAPI::createColorEditButton(
	ak::UID												_creatorUid,
	const ak::ui::color &								_color,
	const QString &										_textOverride
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createColorEditButton(_creatorUid, _color, _textOverride);
}

ak::UID ak::uiAPI::createComboBox(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createComboBox(_creatorUid);
}

ak::UID ak::uiAPI::createComboButton(
	ak::UID												_creatorUid,
	const QString &										_text,
	const std::vector<QString> &						_possibleSelection
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createComboButton(_creatorUid, _text, _possibleSelection);
}

ak::UID ak::uiAPI::createWelcomeScreen(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createWelcomeScreen(_creatorUid);
}

ak::UID ak::uiAPI::createDock(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createDock(_creatorUid, _text);
}

ak::UID ak::uiAPI::createLogInDialog(
	ak::UID												_creatorUid,
	bool												_showSavePassword,
	const QString &										_imageName,
	const QString &										_username,
	const QString &										_password
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	return my_objManager->createLogInDialog(_creatorUid, _showSavePassword, *my_iconManager->pixmap(_imageName), _username, _password);
}

ak::UID ak::uiAPI::createPropertyGrid(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPropertyGrid(_creatorUid);
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPushButton(_creatorUid, _text);
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPushButton(_creatorUid, _icon, _text);
}

ak::UID ak::uiAPI::createTable(
	ak::UID												_creatorUid,
	int													_rows,
	int													_columns
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTable(_creatorUid, _rows, _columns);
}

ak::UID ak::uiAPI::createTabToolBarSubContainer(
	ak::UID												_creatorUid,
	ak::UID												_parentUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTabToolBarSubContainer(_creatorUid, _parentUid, _text);
}

ak::UID ak::uiAPI::createTextEdit(
	ak::UID												_creatorUid,
	const QString &										_initialText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTextEdit(_creatorUid, _initialText);
}

ak::UID ak::uiAPI::createTimer(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTimer(_creatorUid);
}

ak::UID ak::uiAPI::createTree(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTree(_creatorUid);
}

ak::UID ak::uiAPI::createTabView(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTabView(_creatorUid);
}

ak::UID ak::uiAPI::createWindow(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createWindow(_creatorUid);
}

// Object creation

// ###############################################################################################################################################

// Action

void ak::uiAPI::action::setEnabled(
	ak::UID												_actionUID,
	bool												_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::action * actualAction = nullptr;
	actualAction = dynamic_cast<ui::qt::action *>(my_objManager->object(_actionUID));
	assert(actualAction != nullptr); // Invalid object type
	actualAction->setEnabled(_enabled);
}

void ak::uiAPI::action::setText(
	ak::UID												_actionUID,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::action * actualAction = nullptr;
	actualAction = dynamic_cast<ui::qt::action *>(my_objManager->object(_actionUID));
	assert(actualAction != nullptr); // Invalid object type
	actualAction->setText(_text);
}

void ak::uiAPI::action::setIcon(
	ak::UID												_actionUID,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::action * actualAction = nullptr;
	actualAction = dynamic_cast<ui::qt::action *>(my_objManager->object(_actionUID));
	assert(actualAction != nullptr); // Invalid object type
	actualAction->setIcon(_icon);
}

void ak::uiAPI::action::setIcon(
	ak::UID												_actionUID,
	const QString &										_iconName,
	const QString &										_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::action * actualAction = nullptr;
	actualAction = dynamic_cast<ui::qt::action *>(my_objManager->object(_actionUID));
	assert(actualAction != nullptr); // Invalid object type
	actualAction->setIcon(*my_iconManager->icon(_iconName, _iconFolder));
}

QString ak::uiAPI::action::getText(
	ak::UID												_actionUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::action * actualAction = nullptr;
	actualAction = dynamic_cast<ui::qt::action *>(my_objManager->object(_actionUID));
	assert(actualAction != nullptr); // Invalid object type
	return actualAction->text();
}

// Action

// ###############################################################################################################################################

// Container

void ak::uiAPI::container::addObject(
	ak::UID												_containerUID,
	ak::UID												_objectUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::ttbContainer * actualContainer = nullptr;
	actualContainer = dynamic_cast<ui::core::ttbContainer *>(my_objManager->object(_containerUID));
	assert(actualContainer != nullptr); // Invalid object type
	actualContainer->addChild(my_objManager->object(_objectUID));
}

// Container

// ###############################################################################################################################################

// Dock

void ak::uiAPI::dock::setCentralWidget(
	ak::UID												_dockUID,
	ak::UID												_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	
	ui::qt::dock * actualDock = nullptr;
	actualDock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_dockUID));
	assert(actualDock != nullptr); // Invalid object type

	ui::core::aWidget * actualWidget = nullptr;
	actualWidget = dynamic_cast<ui::core::aWidget *>(my_objManager->object(_widgetUID));
	assert(actualWidget != nullptr); // Invalid object type
	
	actualDock->setWidget(actualWidget->widget());
}

void ak::uiAPI::dock::setCentralWidget(
	ak::UID												_dockUID,
	QWidget *											_widget
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::dock * actualDock = nullptr;
	actualDock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_dockUID));
	assert(actualDock != nullptr); // Invalid object type

	actualDock->setWidget(_widget);
}

void ak::uiAPI::dock::setVisible(
	ak::UID												_dockUID,
	bool												_visible
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::dock * actualDock = nullptr;
	actualDock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_dockUID));
	assert(actualDock != nullptr); // Invalid object type

	actualDock->setVisible(_visible);
}

// Dock

// ###############################################################################################################################################

// File

ak::UID ak::uiAPI::file::load(
	const QString &										_filePath
) {
	ak::file * f = my_apiManager.getFile(ak::invalidUID);
	f->load(_filePath);
	return f->uid();
}

void ak::uiAPI::file::load(
	ak::UID												_fileUid,
	const QString &										_filePath
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->load(_filePath);
}

void ak::uiAPI::file::save(
	ak::UID												_fileUid,
	bool												_append
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->save(_append);
}

void ak::uiAPI::file::save(
	ak::UID												_fileUid,
	const QString &										_filePath,
	bool												_append
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->save(_filePath, _append);
}

void ak::uiAPI::file::setPath(
	ak::UID												_fileUid,
	const QString &										_path
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->setPath(_path);
}

void ak::uiAPI::file::setLines(
	ak::UID												_fileUid,
	const QStringList &									_lines
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->setLines(_lines);
}

void ak::uiAPI::file::addLine(
	ak::UID												_fileUid,
	const QString &										_line
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	QStringList lst = _line.split("\n");
	for (QString str : lst) { str.append('\n'); f->addLine(str); }
}

void ak::uiAPI::file::addLine(
	ak::UID												_fileUid,
	const QStringList &									_lines
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	f->addLine(_lines);
}

QString ak::uiAPI::file::name(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->name();
}

QString ak::uiAPI::file::path(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->path();
}

QString ak::uiAPI::file::extension(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->extension();
}

QStringList ak::uiAPI::file::lines(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->lines();
}

int ak::uiAPI::file::linesCount(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->linesCount();
}

bool ak::uiAPI::file::hasChanged(
	ak::UID												_fileUid
) {
	ak::file * f = my_apiManager.getExistingFile(_fileUid);
	return f->hasChanged();
}

// file

// ###############################################################################################################################################

// Log in dialog

ak::ui::core::dialogResult ak::uiAPI::logInDialog::showDialog(
	ak::UID												_dialogUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->showDialog();
}

void ak::uiAPI::logInDialog::close(
	ak::UID												_dialogUID,
	ui::core::dialogResult								_result
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->close(_result);
}

QString ak::uiAPI::logInDialog::getUsername(
	ak::UID												_dialogUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->username();
}

QString ak::uiAPI::logInDialog::getPassword(
	ak::UID												_dialogUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->password();
}

bool ak::uiAPI::logInDialog::getSavePassword(
	ak::UID												_dialogUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->savePassword();
}

void ak::uiAPI::logInDialog::showToolTipAtUsername(
	ak::UID												_dialogUID,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->showToolTipAtUsername(_text);
}

void ak::uiAPI::logInDialog::showToolTipAtPassword(
	ak::UID												_dialogUID,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::logIn * actualObject = nullptr;
	actualObject = dynamic_cast<ui::dialog::logIn *>(my_objManager->object(_dialogUID));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->showToolTipAtPassword(_text);
}

// Log in dialog

// ###############################################################################################################################################

// Object

void ak::uiAPI::object::destroy(
	ak::UID												_objectUID,
	bool												_ignoreIfObjectHasChildObjects
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->destroy(_objectUID, _ignoreIfObjectHasChildObjects);
}

void ak::uiAPI::object::setAlias(
	ak::UID												_objectUID,
	const QString &										_alias
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * actualObject = my_objManager->object(_objectUID);
	assert(actualObject != nullptr); // That should not happen
	my_objManager->removeAlias(actualObject->alias());	// Delete last alias
	actualObject->setAlias(_alias);
	if (_alias.length() != 0) { my_objManager->addAlias(_alias, _objectUID); }
}

// Object

// ###############################################################################################################################################

// promptDialog

ak::ui::core::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title,
	ak::ui::core::promptType	_type
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::prompt dialog(_message, _title, _type);
	dialog.setColorStyle(my_objManager->getCurrentColorStyle());
	return dialog.showDialog();
}

ak::ui::core::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title
) {
	ui::dialog::prompt dialog(_message, _title, ak::ui::core::promptType::promptOk);
	return dialog.showDialog();
}

// promptDialog

// ###############################################################################################################################################

// propertyGrid

void ak::uiAPI::propertyGrid::addGroup(
	ak::UID											_propertyGridUID,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->addGroup(_groupName);
}

void ak::uiAPI::propertyGrid::addGroup(
	ak::UID											_propertyGridUID,
	const QColor &									_color,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->addGroup(_color, _groupName);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	bool											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const ui::color &								_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	double											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	int												_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const char *									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, QString(_value));
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	bool											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const ui::color &								_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	double											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	int												_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const char *									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, QString(_value));
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	ak::UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

void ak::uiAPI::propertyGrid::clear(
	ak::UID											_propertyGridUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->clear();
}

bool ak::uiAPI::propertyGrid::getItemIsReadOnly(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->itemIsReadOnly(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemGroup(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemGroup(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemIsMultipleValues(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemIsMultipleValues(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemName(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemName(_itemID);
}

std::vector<QString> ak::uiAPI::propertyGrid::getItemPossibleSelection(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemPossibleSelection(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemValueBool(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueBool(_itemID);
}

ak::ui::color ak::uiAPI::propertyGrid::getItemValueColor(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueColor(_itemID);
}

double ak::uiAPI::propertyGrid::getItemValueDouble(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueDouble(_itemID);
}

int ak::uiAPI::propertyGrid::getItemValueInteger(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueInteger(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueSelection(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueSelection(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueString(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueString(_itemID);
}

ak::core::valueType ak::uiAPI::propertyGrid::getItemValueType(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueType(_itemID);
}

void ak::uiAPI::propertyGrid::setItemIsReadOnly(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID,
	bool											_readOnly
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::widget::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setItemReadOnly(_itemID, _readOnly);
}

// propertyGrid

// ###############################################################################################################################################

// TabView

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title
) {
	assert(my_objManager != nullptr); // API not initialized
	
	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	ui::core::aWidget * actualWidget = nullptr;
	actualWidget = dynamic_cast<ui::core::aWidget *>(my_objManager->object(_widgetUID));
	assert(actualWidget != nullptr); // Invalid object type

	return actualTabView->createTab(actualWidget->widget(), _title);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	ui::core::aWidget * actualWidget = nullptr;
	actualWidget = dynamic_cast<ui::core::aWidget *>(my_objManager->object(_widgetUID));
	assert(actualWidget != nullptr); // Invalid object type

	return actualTabView->createTab(actualWidget->widget(), _title, *my_iconManager->icon(_iconName, _iconFolder));
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title,
	const QIcon &		_icon
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	ui::core::aWidget * actualWidget = nullptr;
	actualWidget = dynamic_cast<ui::core::aWidget *>(my_objManager->object(_widgetUID));
	assert(actualWidget != nullptr); // Invalid object type

	return actualTabView->createTab(actualWidget->widget(), _title, _icon);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->createTab(_widget, _title);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->createTab(_widget, _title, *my_iconManager->icon(_iconName, _iconFolder));
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title,
	const QIcon &		_icon
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->createTab(_widget, _title, _icon);
}

void ak::uiAPI::tabView::closeAllTabs(
	ak::UID				_tabViewUID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->closeAllTabs();
}

void ak::uiAPI::tabView::closeTab(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->closeTab(_tabID);
}

ak::ID ak::uiAPI::tabView::getFocusedTab(
	ak::UID				_tabViewUID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->focusedTab();
}

bool ak::uiAPI::tabView::getTabsCloseable(
	ak::UID				_tabViewUID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->tabsCloseable();
}

QString ak::uiAPI::tabView::getTabText(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	return actualTabView->tabText(_tabID);
}

void ak::uiAPI::tabView::setEnabled(
	ak::UID				_tabViewUID,
	bool				_enabled
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->setEnabled(_enabled);
}

void ak::uiAPI::tabView::setTabbarLocation(
	ak::UID								_tabViewUID,
	ak::ui::core::tabLocation			_location
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->setTabLocation(_location);
}

void ak::uiAPI::tabView::setTabFocused(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->focusTab(_tabID);
}

void ak::uiAPI::tabView::setTabsCloseable(
	ak::UID								_tabViewUID,
	bool								_closeable
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->setTabsCloseable(_closeable);
}

void ak::uiAPI::tabView::setVisible(
	ak::UID				_tabViewUID,
	bool				_visible
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->setVisible(_visible);
}

void ak::uiAPI::tabView::setObjectName(
	ak::UID							_tabViewUID,
	const QString &					_name
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::widget::tabView * actualTabView = nullptr;
	actualTabView = dynamic_cast<ui::widget::tabView *>(my_objManager->object(_tabViewUID));
	assert(actualTabView != nullptr); // Invalid object type

	actualTabView->setObjectName(_name);
}

// TabView

// ###############################################################################################################################################

// TextEdit

void ak::uiAPI::textEdit::appendText(
	ak::UID				_textEditUID,
	const QString &		_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::textEdit * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<ui::qt::textEdit *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->append(_text);
}

void ak::uiAPI::textEdit::clear(
	ak::UID				_textEditUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::textEdit * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<ui::qt::textEdit *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->clear();
}

void ak::uiAPI::textEdit::setAutoScrollToBottomEnabled(
	ak::UID				_textEditUID,
	bool				_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::textEdit * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<ui::qt::textEdit *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->setAutoScrollToBottom(_enabled);
}

void ak::uiAPI::textEdit::setReadOnly(
	ak::UID				_textEditUID,
	bool				_readOnly
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::textEdit * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<ui::qt::textEdit *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	actualTextEdit->setReadOnly(_readOnly);
}

void ak::uiAPI::textEdit::setText(
	ak::UID				_textEditUID,
	const QString &		_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::textEdit * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<ui::qt::textEdit *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->setText(_text);
}

// TextEdit

// ###############################################################################################################################################

// Timer

int ak::uiAPI::timer::getInterval(
	ak::UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	return actualTimer->interval();
}

void ak::uiAPI::timer::setInterval(
	ak::UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
}

void ak::uiAPI::timer::shoot(
	ak::UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setSingleShot(true);
	actualTimer->start();
}

void ak::uiAPI::timer::shoot(
	ak::UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
	actualTimer->setSingleShot(true);
	actualTimer->start();
}

void ak::uiAPI::timer::start(
	ak::UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setSingleShot(false);
	actualTimer->start();
}

void ak::uiAPI::timer::start(
	ak::UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
	actualTimer->setSingleShot(false);
	actualTimer->start();
}

void ak::uiAPI::timer::stop(
	ak::UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::timer * actualTimer = nullptr;
	actualTimer = dynamic_cast<ui::qt::timer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->stop();
}

// Timer

// ###############################################################################################################################################

// Tree

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_text,
	ak::ID							_parentId,
	ak::ui::core::textAlignment		_textAlignment
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->add(_parentId, _text, _textAlignment);
}

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconSize,
	ak::ID							_parentId,
	ak::ui::core::textAlignment		_textAlignment
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->add(_parentId, _text, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_cmd,
	char							_delimiter,
	ak::ui::core::textAlignment		_textAlignment
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->add(_cmd, _delimiter, _textAlignment);
}

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_cmd,
	char							_delimiter,
	const QString &					_iconName,
	const QString &					_iconSize,
	ak::ui::core::textAlignment		_textAlignment
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->add(_cmd, _delimiter, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

void ak::uiAPI::tree::clear(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->clear();
}

void ak::uiAPI::tree::collapseAllItems(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->collapseAllItems();
}

void ak::uiAPI::tree::deleteItem(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->deleteItem(_itemID);
}

void ak::uiAPI::tree::deleteItems(
	ak::UID							_treeUID,
	const std::vector<ak::ID> &		_itemIDs
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->deleteItems(_itemIDs);
}

void ak::uiAPI::tree::deselectAllItems(
	ak::UID							_treeUID,
	bool							_sendSelectionChangedEvent
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->deselectAllItems(_sendSelectionChangedEvent);
}

void ak::uiAPI::tree::expandAllItems(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->expandAllItems();
}

bool ak::uiAPI::tree::getAutoExpandSelectedItemsEnabled(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->autoExpandSelectedItemsEnabled();
}

ak::ID ak::uiAPI::tree::getItemID(
	ak::UID							_treeUID,
	const QString &					_itemPath,
	char							_delimiter
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->getItemID(_itemPath, _delimiter);
}

std::vector<QString> ak::uiAPI::tree::getItemPath(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->getItemPath(_itemID);
}

QString ak::uiAPI::tree::getItemPathString(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	char							_delimiter
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->getItemPathString(_itemID, _delimiter);
}

QString ak::uiAPI::tree::getItemText(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->getItemText(_itemID);
}

bool ak::uiAPI::tree::isEnabled(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->enabled();
}

int ak::uiAPI::tree::itemCount(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->itemCount();
}

std::vector<ak::ID> ak::uiAPI::tree::selectedItems(
	ak::UID							_treeUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	return actualTree->selectedItems();
}

void ak::uiAPI::tree::setAutoExpandSelectedItemsEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setAutoExpandSelectedItemsEnabled(_enabled);
}

void ak::uiAPI::tree::setAutoSelectAndDeselectChildrenEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setAutoSelectAndDeselectChildrenEnabled(_enabled);
}

void ak::uiAPI::tree::setEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setEnabled(_enabled);
}

void ak::uiAPI::tree::setFilterCaseSensitiveEnabled(
	ak::UID							_treeUID,
	bool							_caseSensitiveEnabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setFilterCaseSensitive(_caseSensitiveEnabled);
}

void ak::uiAPI::tree::setFilterRefreshOnChangeEnabled(
	ak::UID							_treeUID,
	bool							_refreshOnChangeEnabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setFilterRefreshOnChange(_refreshOnChangeEnabled);
}

void ak::uiAPI::tree::setFilterVisible(
	ak::UID							_treeUID,
	bool							_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setFilterVisible(_visible);
}

void ak::uiAPI::tree::setItemIcon(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setItemIcon(_itemID, _icon);
}

void ak::uiAPI::tree::setItemIcon(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setItemIcon(_itemID, *my_iconManager->icon(_iconName, _iconFolder));
}

void ak::uiAPI::tree::setItemEnabled(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setItemEnabled(_itemID, _enabled);
}

void ak::uiAPI::tree::setItemSelected(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_selected,
	bool							_singleSelection
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	if (_singleSelection) { actualTree->setSingleItemSelected(_itemID, _selected); }
	else { actualTree->setItemSelected(_itemID, _selected); }
}

void ak::uiAPI::tree::setItemText(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setItemText(_itemID, _text);
}

void ak::uiAPI::tree::setItemVisible(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setItemVisible(_itemID, _visible);
}

void ak::uiAPI::tree::setMultiSelectionEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setMultiSelectionEnabled(_enabled);
}

void ak::uiAPI::tree::setVisible(
	ak::UID							_treeUID,
	bool							_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->setVisible(_visible);
}

void ak::uiAPI::tree::toggleItemSelection(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::tree * actualTree = nullptr;
	actualTree = dynamic_cast<ui::widget::tree *>(my_objManager->object(_treeUID));
	assert(actualTree != nullptr); // Invalid object type
	actualTree->toggleItemSelection(_itemID);
}

// Tree

// ###############################################################################################################################################

// Welcome screen

void ak::uiAPI::welcomeScreen::addItem(
	ak::UID							_welcomeScreenUID,
	ak::ID							_group,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItem(_group, _text);
}

void ak::uiAPI::welcomeScreen::addItem(
	ak::UID							_welcomeScreenUID,
	ak::ID							_group,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItem(_group, *my_iconManager->icon(_iconName, _iconFolder), _text);
}

void ak::uiAPI::welcomeScreen::addItem(
	ak::UID							_welcomeScreenUID,
	ak::ID							_group,
	const QString &					_text,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItem(_group, _icon, _text);
}

void ak::uiAPI::welcomeScreen::addItemAtReecent(
	ak::UID							_welcomeScreenUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtRecents(_text);
}

void ak::uiAPI::welcomeScreen::addItemAtReecent(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtRecents(*my_iconManager->icon(_iconName, _iconFolder), _text);
}

void ak::uiAPI::welcomeScreen::addItemAtReecent(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtRecents(_icon, _text);
}

void ak::uiAPI::welcomeScreen::addItemAtOpen(
	ak::UID							_welcomeScreenUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtOpen(_text);
}

void ak::uiAPI::welcomeScreen::addItemAtOpen(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtOpen(*my_iconManager->icon(_iconName, _iconFolder), _text);
}

void ak::uiAPI::welcomeScreen::addItemAtOpen(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtOpen(_icon, _text);
}

void ak::uiAPI::welcomeScreen::addItemAtNew(
	ak::UID							_welcomeScreenUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtNew(_text);
}

void ak::uiAPI::welcomeScreen::addItemAtNew(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtNew(*my_iconManager->icon(_iconName, _iconFolder), _text);
}

void ak::uiAPI::welcomeScreen::addItemAtNew(
	ak::UID							_welcomeScreenUID,
	const QString &					_text,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->addItemAtNew(_icon, _text);
}

QString ak::uiAPI::welcomeScreen::getItemText(
	ak::UID							_welcomeScreenUID,
	ak::ID							_group,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	return actualWelcomeScreen->itemText(_group, _itemID);
}

QString ak::uiAPI::welcomeScreen::getItemTextAtRecent(
	ak::UID							_welcomeScreenUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	return actualWelcomeScreen->itemTextAtRecent(_itemID);
}

QString ak::uiAPI::welcomeScreen::getItemTextAtOpen(
	ak::UID							_welcomeScreenUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	return actualWelcomeScreen->itemTextAtOpen(_itemID);
}

QString ak::uiAPI::welcomeScreen::getItemTextAtNew(
	ak::UID							_welcomeScreenUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	return actualWelcomeScreen->itemTextAtNew(_itemID);
}

QString ak::uiAPI::welcomeScreen::getGroupName(
	ak::UID							_welcomeScreenUID,
	ak::ID							_group
) { return ui::widget::welcomeScreen::groupName(_group); }

void ak::uiAPI::welcomeScreen::setObjectName(
	ak::UID							_welcomeScreenUID,
	const QString &					_name
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::widget::welcomeScreen * actualWelcomeScreen = nullptr;
	actualWelcomeScreen = dynamic_cast<ui::widget::welcomeScreen *>(my_objManager->object(_welcomeScreenUID));
	assert(actualWelcomeScreen != nullptr); // Invalid object type
	actualWelcomeScreen->setObjectName(_name);
}

// Welcome screen

// ###############################################################################################################################################

// Window

void ak::uiAPI::window::close(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->close();
}

void ak::uiAPI::window::setStatusLabelVisible(
	ak::UID												_windowUID,
	bool												_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusLabelVisible(_visible);
}

void ak::uiAPI::window::setStatusProgressVisible(
	ak::UID												_windowUID,
	bool												_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusBarVisible(_visible);
}

void ak::uiAPI::window::setStatusLabelText(
	ak::UID												_windowUID,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusLabelText(_text);
}

void ak::uiAPI::window::setStatusProgressValue(
	ak::UID												_windowUID,
	int													_value
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusBarProgress(_value);
}

void ak::uiAPI::window::setStatusProgressContinuous(
	ak::UID												_windowUID,
	bool												_continuous
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusBarContinuous(_continuous);
}

void ak::uiAPI::window::setTabToolBarVisible(
	ak::UID												_windowUID,
	bool												_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setTabToolBarVisible(_visible);
}

ak::ID ak::uiAPI::window::getCurrentTabToolBarTab(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->currentTabToolbarTab();
}

int ak::uiAPI::window::getTabToolBarTabCount(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->tabToolbarTabCount();
}

void ak::uiAPI::window::setCurrentTabToolBarTab(
	ak::UID												_windowUID,
	ak::ID												_tabID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setCurrentTabToolBarTab(_tabID);
}

bool ak::uiAPI::window::getStatusLabelVisible(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->getStatusLabelVisible();
}

bool ak::uiAPI::window::getStatusProgressVisible(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->getStatusBarVisible();
}

QString ak::uiAPI::window::getStatusLabelText(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->getStatusLabelText();
}

int ak::uiAPI::window::getStatusProgressValue(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->getStatusBarProgress();
}

bool ak::uiAPI::window::getStatusProgressContinuous(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->getStatusBarContinuous();
}

void ak::uiAPI::window::addDock(
	ak::UID												_windowUID,
	ak::UID												_dockUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	ui::qt::dock * dock = nullptr;

	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	
	dock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_dockUid));
	assert(dock != nullptr); // Invalid object type

	actualWindow->addDock(dock, _dockLocation);
}

void ak::uiAPI::window::tabifyDock(
	ak::UID												_windowUID,
	ak::UID												_parentUid,
	ak::UID												_dockUid
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	ui::qt::dock * parentDock = nullptr;
	ui::qt::dock * dock = nullptr;

	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type

	parentDock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_parentUid));
	assert(parentDock != nullptr); // Invalid object type

	dock = dynamic_cast<ui::qt::dock *>(my_objManager->object(_dockUid));
	assert(dock != nullptr); // Invalid object type

	actualWindow->tabifyDock(parentDock, dock);
}

void ak::uiAPI::window::setDockBottomLeftPriority(
	ak::UID												_windowUID,
	ak::ui::core::dockLocation							_dockLocation
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setDockPriorityBottomLeft(_dockLocation);
}

void ak::uiAPI::window::setDockBottomRightPriority(
	ak::UID												_windowUID,
	ak::ui::core::dockLocation							_dockLocation
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setDockPriorityBottomRight(_dockLocation);
}

void ak::uiAPI::window::setCentralWidget(
	ak::UID												_windowUID,
	ak::UID												_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type

	ui::core::aWidget * actualWidget = nullptr;
	actualWidget = dynamic_cast<ui::core::aWidget *>(my_objManager->object(_widgetUID));
	assert(actualWidget != nullptr); // Invalid object type

	actualWindow->setCentralWidget(actualWidget->widget());
}

void ak::uiAPI::window::setCentralWidget(
	ak::UID												_windowUID,
	QWidget *											_widget
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setCentralWidget(_widget);
}

void ak::uiAPI::window::showMaximized(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->showMaximized();
}

void ak::uiAPI::window::showMinimized(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->showMaximized();
}

// ###############################################################################################################################################

void ak::uiAPI::creatorDestroyed(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->creatorDestroyed(_creatorUid);
}

void ak::uiAPI::addColorStyle(
	ak::ui::colorStyle *								_colorStyle,
	bool												_activate
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->addColorStyle(_colorStyle, _activate);
}

void ak::uiAPI::setColorStyle(
	const QString &				_colorStyleName
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->setColorStyle(_colorStyleName);
}

const ak::ui::colorStyle *  ak::uiAPI::getCurrentColorStyle(void) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->getCurrentColorStyle();
}

QString ak::uiAPI::getCurrentColorStyleName(void) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->getCurrentColorStyleName();
}

void ak::uiAPI::setDefaultDarkColorStyle() {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->setDefaultDarkColorStyle();
}

void ak::uiAPI::setDefaultColorStyle() {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->setDefaultColorStyle();
}

// ###############################################################################################################################################

void ak::uiAPI::addIconSearchPath(
	const QString &										_path
) {
	assert(my_iconManager != nullptr); // API not initialized
	assert(my_objManager != nullptr); // API not initialized
	my_iconManager->addDirectory(_path);
	my_objManager->setIconSearchDirectories(my_iconManager->searchDirectories());
}

void ak::uiAPI::removeIconSearchPath(
	const QString &										_path
) {
	assert(my_iconManager != nullptr); // API not initialized
	assert(my_objManager != nullptr); // API not initialized
	my_iconManager->removeDirectory(_path);
	my_objManager->setIconSearchDirectories(my_iconManager->searchDirectories());
}

ak::UID ak::uiAPI::createUid(void) {
	assert(my_uidManager != nullptr); // API not initialized
	return my_uidManager->getId();
} 

ak::messenger *ak::uiAPI::getMessenger(void) { return my_messenger; }

const QIcon & ak::uiAPI::getIcon(
	const QString &											_name,
	const QString &											_size
) {
	assert(my_iconManager != nullptr); // API not initialized
	return *my_iconManager->icon(_name, _size);
}

// ###############################################################################################################################################

// special

int ak::uiAPI::exec(void) { return my_apiManager.exec(); }

QString ak::uiAPI::special::toString(
	ak::core::eventType									_type
) {
	return ak::core::toQString(_type);
}

QString ak::uiAPI::special::toString(
	ak::core::valueType									_type
) {
	return ak::core::toQString(_type);
}

QString ak::uiAPI::special::toString(
	ak::ui::core::textAlignment							_type
) {
	return ak::ui::core::toQString(_type);
}

QString ak::uiAPI::special::toString(
	ak::ui::core::dockLocation							_dockLocation
) {
	return ak::ui::core::toQString(_dockLocation);
}

QString ak::uiAPI::special::toString(
	ak::ui::core::tabLocation							_tabLocation
) {
	return ak::ui::core::toQString(_tabLocation);
}

QString ak::uiAPI::special::toQString(
	ak::ui::core::dialogResult							_dialogResult
) {
	return ak::ui::core::toQString(_dialogResult);
}

QString ak::uiAPI::special::toString(
	ak::ui::core::keyType								_type
) {
	return ak::ui::core::toQString(_type);
}

QString ak::uiAPI::special::toString(
	ak::ui::core::objectType							_type
) {
	return ak::ui::core::toQString(_type);
}

QString ak::uiAPI::special::toEventText(
	ak::UID												_senderUID,
	ak::core::eventType									_eventType,
	int													_info1,
	int													_info2
) {
	QString out("Event{\"Sender\":");
	out.append(QString::number(_senderUID));
	out.append(", \"EventType\":\"");
	out.append(toString(_eventType));
	out.append("\", \"Info1\":");
	out.append(QString::number(_info1));
	out.append("\", \"Info2\":");
	out.append(QString::number(_info2));
	out.append("}");
	return out;
}
