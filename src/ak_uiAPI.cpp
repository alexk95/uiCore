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
#include <ak_notifierStaticEvent.h>			// notifierStaticEvent
#include <ak_notifier.h>					// notifier
#include <ak_exception.h>					// error handling

// AK specialized header
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_iconManager.h>				// iconManager
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_uidMangager.h>					// UID manager
#include <ak_singletonAllowedMessages.h>	// allowed messages
#include <ak_ui_windowEventHandler.h>
#include <ak_file.h>						// file

// AK object core headers
#include <ak_ui_core_aObject.h>
#include <ak_ui_core_aWidget.h>
#include <ak_ui_core_aRestorable.h>
#include <ak_ui_core_ttbContainer.h>

// AK dialog objects
#include <ak_ui_dialog_prompt.h>			// prompt dialog
#include <ak_ui_dialog_logIn.h>
#include <ak_ui_dialog_options.h>

// AK qt objects
#include <ak_ui_qt_action.h>
#include <ak_ui_qt_colorEditButton.h>
#include <ak_ui_qt_contextMenuItem.h>
#include <ak_ui_qt_checkBox.h>
#include <ak_ui_qt_comboBox.h>
#include <ak_ui_qt_comboButton.h>
#include <ak_ui_qt_dock.h>					// dock
#include <ak_ui_qt_graphicsView.h>
#include <ak_ui_qt_label.h>
#include <ak_ui_qt_lineEdit.h>				// line edit
#include <ak_ui_qt_list.h>
#include <ak_ui_qt_niceLineEdit.h>			// nice line edit
#include <ak_ui_qt_pushButton.h>			// push button
#include <ak_ui_qt_table.h>
#include <ak_ui_qt_tabView.h>
#include <ak_ui_qt_textEdit.h>
#include <ak_ui_qt_specialTabBar.h>
#include <ak_ui_qt_timer.h>
#include <ak_ui_qt_toolButton.h>
#include <ak_ui_qt_window.h>
#include <ak_ui_qt_propertyGrid.h>			// propertyGrid

// AK widget objects
#include <ak_ui_qt_tree.h>					// tree

#include <ak_ui_widget_welcomeScreen.h>

// AK ttb objects
#include <ak_ui_ttb_group.h>
#include <ak_ui_ttb_page.h>
#include <ak_ui_ttb_subgroup.h>

// Qt header
#include <qsurfaceformat.h>					// QSurfaceFormat
#include <qfiledialog.h>					// Open/Save file dialog
#include <qfile.h>
#include <qmovie.h>
#include <qdesktopwidget.h>
#include <qfiledialog.h>

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
	my_desktop = my_app->desktop();
	
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

ak::ui::application * ak::uiAPI::apiManager::app() { return my_app; }

QDesktopWidget * ak::uiAPI::apiManager::desktop() { return my_desktop; }

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

void ak::uiAPI::setMessengerEnabled(
	bool													_enabled
) {
	assert(my_messenger != nullptr);	// Not initialized yet
	if (_enabled) { my_messenger->enable(); }
	else { my_messenger->disable(); }
}

bool ak::uiAPI::messengerIsEnabled(void) {
	assert(my_messenger != nullptr);	// Not initialized yet
	return my_messenger->isEnabled();
}

std::vector<ak::core::eventType> ak::uiAPI::enabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->enabledMessages(); }

std::vector<ak::core::eventType> ak::uiAPI::disabledEventTypes(void) { return ak::singletonAllowedMessages::instance()->disabledMessages(); }

std::string ak::uiAPI::saveStateWindow(
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->saveStateWindow(_applicationVersion);
}

std::string ak::uiAPI::saveStateColorStyle(
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->saveStateColorStyle(_applicationVersion);
}

ak::core::settingsRestoreErrorCode ak::uiAPI::restoreStateWindow(
	const std::string &									_json,
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return ak::core::settingsRestoreErrorCode::srecEmptySettingsString; }
	return my_objManager->restoreStateWindow(_json.c_str(), _applicationVersion);
}

ak::core::settingsRestoreErrorCode ak::uiAPI::restoreStateColorStyle(
	const std::string &									_json,
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return ak::core::settingsRestoreErrorCode::srecEmptySettingsString; }
	return my_objManager->restoreStateColorStyle(_json.c_str(), _applicationVersion);
}

QWidget * ak::uiAPI::getWidget(
	ak::UID												_objectUid
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_objectUid);
	assert(obj != nullptr); // Invalid UID
	assert(obj->isWidgetType()); // Object is not a widget type
	ui::core::aWidget * widget = nullptr;
	widget = dynamic_cast<ui::core::aWidget *>(obj);
	assert(widget != nullptr); // Cast failed
	return widget->widget();
}

ak::UID ak::uiAPI::getObjectCreator(
	ak::UID												_objectUID
) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->objectCreator(_objectUID);
}

ak::ui::objectManager * ak::uiAPI::getObjectManager(void) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager;
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
	return my_objManager->createToolButton(_creatorUid, _text, *my_iconManager->icon(_iconName, _iconSize));
}

ak::UID ak::uiAPI::createAction(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, _icon);
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

ak::UID ak::uiAPI::createLineEdit(
	ak::UID													_creatorUid,
	const QString &											_initialText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createLineEdit(_creatorUid, _initialText);
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

ak::UID ak::uiAPI::createNiceLineEdit(
	ak::UID												_creatorUid,
	const QString &										_initialText,
	const QString &										_infoLabelText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createNiceLineEdit(_creatorUid, _initialText, _infoLabelText);
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

ak::UID ak::uiAPI::createOptionsDialog(
	ak::UID												_creatorUid,
	const QString &										_title
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createOptionsDialog(_creatorUid, _title);
}

ak::UID ak::uiAPI::createPushButton(
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPushButton(_creatorUid, _icon, _text);
}

ak::UID ak::uiAPI::createSpecialTabBar(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createSpecialTabBar(_creatorUid);
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

ak::UID ak::uiAPI::createToolButton(
	ak::UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid);
}

ak::UID ak::uiAPI::createToolButton(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text);
}

ak::UID ak::uiAPI::createToolButton(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, _icon);
}

ak::UID ak::uiAPI::createToolButton(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, *my_iconManager->icon(_iconName, _iconFolder));
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
) { object::get<ui::qt::action>(_actionUID)->setEnabled(_enabled); }

void ak::uiAPI::action::setText(
	ak::UID												_actionUID,
	const QString &										_text
) { object::get<ui::qt::action>(_actionUID)->setText(_text); }

void ak::uiAPI::action::setIcon(
	ak::UID												_actionUID,
	const QIcon &										_icon
) { object::get<ui::qt::action>(_actionUID)->setIcon(_icon); }

void ak::uiAPI::action::setIcon(
	ak::UID												_actionUID,
	const QString &										_iconName,
	const QString &										_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	object::get<ui::qt::action>(_actionUID)->setIcon(*my_iconManager->icon(_iconName, _iconFolder));
}

QString ak::uiAPI::action::getText(
	ak::UID												_actionUID
) { return object::get<ui::qt::action>(_actionUID)->text(); }

// Action

// ###############################################################################################################################################

// CheckBox

void ak::uiAPI::checkBox::setChecked(
	ak::UID												_checkBoxUID,
	bool												_checked
) { object::get<ui::qt::checkBox>(_checkBoxUID)->setChecked(_checked); }

void ak::uiAPI::checkBox::setText(
	ak::UID												_checkBoxUID,
	const QString &										_text
) { object::get<ui::qt::checkBox>(_checkBoxUID)->setText(_text); }

void ak::uiAPI::checkBox::setEnabled(
	ak::UID												_checkBoxUID,
	bool												_enabled
) { object::get<ui::qt::checkBox>(_checkBoxUID)->setEnabled(_enabled); }

QString ak::uiAPI::checkBox::text(
	ak::UID												_checkBoxUID
) { return object::get<ui::qt::checkBox>(_checkBoxUID)->text(); }

bool ak::uiAPI::checkBox::isChecked(
	ak::UID												_checkBoxUID
) { return object::get<ui::qt::checkBox>(_checkBoxUID)->isChecked(); }

bool ak::uiAPI::checkBox::isEnabled(
	ak::UID												_checkBoxUID
) { return object::get<ui::qt::checkBox>(_checkBoxUID)->isEnabled(); }

// ###############################################################################################################################################

// Container

void ak::uiAPI::container::addObject(
	ak::UID												_containerUID,
	ak::UID												_objectUID
) { object::get<ui::core::ttbContainer>(_containerUID)->addChild(my_objManager->object(_objectUID)); }

void ak::uiAPI::container::setEnabled(
	ak::UID												_containerUID,
	bool												_isEnabled
) { object::get<ui::core::ttbContainer>(_containerUID)->setEnabled(_isEnabled); }

bool ak::uiAPI::container::isEnabled(
	ak::UID												_containerUID
) { return object::get<ui::core::ttbContainer>(_containerUID)->enabled(); }

// Container

// ###############################################################################################################################################

// ContextMenu

ak::ID ak::uiAPI::contextMenu::addItem(
	ak::UID							_widgetUID,
	const QString &					_text,
	ui::core::contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_widgetUID);
	ui::qt::contextMenuItem * newItem = new ui::qt::contextMenuItem(_text, _role);

	switch (obj->objectType())
	{
	case ui::core::objectType::oTextEdit:
	{
		ui::qt::textEdit * actualTextEdit = nullptr;
		actualTextEdit = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualTextEdit != nullptr); // Invalid object type
		return actualTextEdit->addContextMenuItem(newItem);
	}
		break;
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::uiAPI::contextMenu::addItem(
	ak::UID							_widgetUID,
	const QIcon &					_icon,
	const QString &					_text,
	ui::core::contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	ui::qt::contextMenuItem * newItem = new ui::qt::contextMenuItem(_icon, _text, _role);

	switch (obj->objectType())
	{
	case ui::core::objectType::oTextEdit:
	{
		ui::qt::textEdit * actualTextEdit = nullptr;
		actualTextEdit = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualTextEdit != nullptr); // Invalid object type
		return actualTextEdit->addContextMenuItem(newItem);
	}
	break;
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::uiAPI::contextMenu::addItem(
	ak::UID							_widgetUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconSize,
	ui::core::contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	ui::qt::contextMenuItem * newItem = new ui::qt::contextMenuItem(*my_iconManager->icon(_iconName, _iconSize), _text, _role);

	switch (obj->objectType())
	{
	case ui::core::objectType::oTextEdit:
	{
		ui::qt::textEdit * actualTextEdit = nullptr;
		actualTextEdit = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualTextEdit != nullptr); // Invalid object type
		return actualTextEdit->addContextMenuItem(newItem);
	}
	break;
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

void ak::uiAPI::contextMenu::addSeparator(
	ak::UID							_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	switch (obj->objectType())
	{
	case ui::core::objectType::oTextEdit:
	{
		ui::qt::textEdit * actualTextEdit = nullptr;
		actualTextEdit = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualTextEdit != nullptr); // Invalid object type
		actualTextEdit->addContextMenuSeparator();
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::uiAPI::contextMenu::clear(
	ak::UID							_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::core::aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID

	switch (obj->objectType())
	{
	case ui::core::objectType::oTextEdit:
	{
		ui::qt::textEdit * actualTextEdit = nullptr;
		actualTextEdit = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualTextEdit != nullptr); // Invalid object type
		actualTextEdit->clearContextMenu();
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

// ContextMenu

// ###############################################################################################################################################

// Dialog

QString ak::uiAPI::dialog::openDirectory(const QString & _title, const QString & _initialDir) {
	return QFileDialog::getExistingDirectory(nullptr, _title, _initialDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

// ###############################################################################################################################################

// Dock

void ak::uiAPI::dock::setCentralWidget(
	ak::UID												_dockUID,
	ak::UID												_widgetUID
) {
	auto actualDock{ object::get<ui::qt::dock>(_dockUID) };
	auto actualWidget{ object::get<ui::core::aWidget>(_widgetUID) };
	
	QWidget * currentCentralWidget = actualDock->widget();
	if (currentCentralWidget != nullptr) {
		ui::core::aObject * actualCentralObject = nullptr;
		actualCentralObject = dynamic_cast<ui::core::aObject *>(currentCentralWidget);
		if (actualCentralObject != nullptr) {
			actualCentralObject->setParentObject(nullptr);
		}
	}

	actualDock->setWidget(actualWidget->widget());

	// Get the objects to know each other
	actualWidget->setParentObject(actualDock);
	actualDock->addChildObject(actualWidget);
}

void ak::uiAPI::dock::setCentralWidget(
	ak::UID												_dockUID,
	QWidget *											_widget
) { object::get<ui::qt::dock>(_dockUID)->setWidget(_widget); }

void ak::uiAPI::dock::setVisible(
	ak::UID												_dockUID,
	bool												_visible
) { object::get<ui::qt::dock>(_dockUID)->setVisible(_visible); }

bool ak::uiAPI::dock::isVisible(
	ak::UID												_dockUID
) { return object::get<ui::qt::dock>(_dockUID)->isVisible(); }

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

// Line edit

void ak::uiAPI::lineEdit::setText(
	ak::UID												_lineEditUid,
	const QString &										_text
) { object::get<ui::qt::lineEdit>(_lineEditUid)->setText(_text); }

void ak::uiAPI::lineEdit::setEnabled(
	ak::UID												_lineEditUid,
	bool												_enabled
) { object::get<ui::qt::lineEdit>(_lineEditUid)->setEnabled(_enabled); }

QString ak::uiAPI::lineEdit::text(
	ak::UID												_lineEditUid
) { return object::get<ui::qt::lineEdit>(_lineEditUid)->text(); }

bool ak::uiAPI::lineEdit::isEnabled(
	ak::UID												_lineEditUid
) { return object::get<ui::qt::lineEdit>(_lineEditUid)->isEnabled(); }

// ###############################################################################################################################################

// Log in dialog

ak::ui::core::dialogResult ak::uiAPI::logInDialog::showDialog(
	ak::UID												_dialogUID
) { return object::get<ui::dialog::logIn>(_dialogUID)->showDialog(); }

void ak::uiAPI::logInDialog::close(
	ak::UID												_dialogUID,
	ui::core::dialogResult								_result
) { object::get<ui::dialog::logIn>(_dialogUID)->close(_result); }

QString ak::uiAPI::logInDialog::getUsername(
	ak::UID												_dialogUID
) { return object::get<ui::dialog::logIn>(_dialogUID)->username(); }

QString ak::uiAPI::logInDialog::getPassword(
	ak::UID												_dialogUID
) { return object::get<ui::dialog::logIn>(_dialogUID)->password(); }

bool ak::uiAPI::logInDialog::getSavePassword(
	ak::UID												_dialogUID
) { return object::get<ui::dialog::logIn>(_dialogUID)->savePassword(); }

void ak::uiAPI::logInDialog::showToolTipAtUsername(
	ak::UID												_dialogUID,
	const QString &										_text
) { object::get<ui::dialog::logIn>(_dialogUID)->showToolTipAtUsername(_text); }

void ak::uiAPI::logInDialog::showToolTipAtPassword(
	ak::UID												_dialogUID,
	const QString &										_text
) { object::get<ui::dialog::logIn>(_dialogUID)->showToolTipAtPassword(_text); }

void ak::uiAPI::logInDialog::showToolTipAtCustomField(
	ak::UID												_dialogUID,
	ak::ID												_inputID,
	const QString &										_text
) { object::get<ui::dialog::logIn>(_dialogUID)->showToolTipAtCustomInput(_inputID, _text); }

ak::ID ak::uiAPI::logInDialog::addCustomInputField(
	ak::UID												_dialogUID,
	const QString &										_labelText,
	const QString &										_inputFieldInitialText
) { return object::get<ui::dialog::logIn>(_dialogUID)->addCustomInput(_labelText, _inputFieldInitialText); }

QString ak::uiAPI::logInDialog::getCustomFieldText(
	ak::UID												_dialogUID,
	ak::ID												_fieldID
) { return object::get<ui::dialog::logIn>(_dialogUID)->customInputText(_fieldID); }

// Log in dialog

// ###############################################################################################################################################

// Nice line edit

void ak::uiAPI::niceLineEdit::setText(
	ak::UID												_lineEditUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->setText(_text);
}

void ak::uiAPI::niceLineEdit::setErrorState(
	ak::UID												_lineEditUid,
	bool												_error
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->setErrorState(_error);
}

void ak::uiAPI::niceLineEdit::setInfoLabelText(
	ak::UID												_lineEditUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->setInfoLabelText(_text);
}

void ak::uiAPI::niceLineEdit::setEnabled(
	ak::UID												_lineEditUid,
	bool												_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	actualObject->setEnabled(_enabled);
}

QString ak::uiAPI::niceLineEdit::text(
	ak::UID												_lineEditUid
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->text();
}

QString ak::uiAPI::niceLineEdit::infoLabelText(
	ak::UID												_lineEditUid
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->infoLabelText();
}

bool ak::uiAPI::niceLineEdit::isEnabled(
	ak::UID												_lineEditUid
) {
	assert(my_objManager != nullptr); // API not initialized
	ak::ui::qt::niceLineEdit * actualObject = nullptr;
	actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(my_objManager->object(_lineEditUid));
	assert(actualObject != nullptr); // Invalid object type
	return actualObject->isEnabled();
}

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

void ak::uiAPI::object::setObjectUniqueName(
	ak::UID												_objectUID,
	const QString &										_name
) { my_objManager->setObjectUniqueName(_objectUID, _name); }

QString ak::uiAPI::object::getObjectUniqueName(
	ak::UID												_objectUID
) { return my_objManager->object(_objectUID)->uniqueName(); }

ak::UID ak::uiAPI::object::getUidFromObjectUniqueName(
	const QString &										_name
) { return my_objManager->object(_name)->uid(); }

void ak::uiAPI::object::setEnabled(
	UID													_objectUID,
	bool												_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	auto obj{ my_objManager->object(_objectUID) };
	switch (obj->objectType())
	{
	case ak::ui::core::oAction:
	{
		ui::qt::action * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::action *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oColorEditButton:
	{
		ui::qt::colorEditButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::colorEditButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->SetEnabled(_enabled);
	}
		break;
	case ak::ui::core::oComboBox:
	{
		ui::qt::comboBox * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::comboBox *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oComboButton:
	{
		ui::qt::comboButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::comboButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oCheckBox:
	{
		ui::qt::checkBox * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::checkBox *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oDock:
	{
		ui::qt::dock * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::dock *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oGraphicsView:
	{
		ui::qt::graphicsView * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::graphicsView *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oLabel:
	{
		ui::qt::label * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::label *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oLineEdit:
	{
		ui::qt::lineEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::lineEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oList:
	{
		ui::qt::list * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::list *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oLogInDialog:
	{
		ui::dialog::logIn * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::logIn *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oMainWindow:
	{
		ui::qt::window * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::window *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oNiceLineEdit:
	{
		ak::ui::qt::niceLineEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oOptionsDialog:
	{
		ui::dialog::options * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::options *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oPrompt:
	{
		ui::dialog::prompt * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::prompt *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oPropertyGrid:
	{
		ui::qt::propertyGrid * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::propertyGrid *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oPushButton:
	{
		ui::qt::pushButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::pushButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oSpecialTabBar:
	{
		ui::qt::specialTabBar * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::specialTabBar *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTable:
	{
		ui::qt::table * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::table *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTabToolbarGroup:
	{
		ui::ttb::group * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::group *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTabToolbarPage:
	{
		ui::ttb::page * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::page *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTabToolbarSubgroup:
	{
		ui::ttb::subGroup * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::subGroup *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTabView:
	{
		ui::qt::tabView * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::tabView *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTextEdit:
	{
		ui::qt::textEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oToolButton:
	{
		ui::qt::toolButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::toolButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oTree:
	{
		ui::qt::tree * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::tree *>(obj);
		assert(actualObject != nullptr); // Cast failed
		actualObject->setEnabled(_enabled);
	}
		break;
	case ak::ui::core::oNone:
	case ak::ui::core::oComboButtonItem:
	case ak::ui::core::oDefaultWelcomeScreen:
	case ak::ui::core::oMenu:
	case ak::ui::core::oPropertyGridItem:
	case ak::ui::core::oTabToolbar:
	case ak::ui::core::oTimer:
	case ak::ui::core::oTreeItem:
		assert(0); // This object does not support setEnabled()
		break;
	default:
		assert(0);	// Invalid object type
		break;
	}
}

bool ak::uiAPI::object::getIsEnabled(
	UID													_objectUID
) {
	assert(my_objManager != nullptr); // API not initialized
	auto obj{ my_objManager->object(_objectUID) };
	switch (obj->objectType())
	{
	case ak::ui::core::oAction:
	{
		ui::qt::action * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::action *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oColorEditButton:
	{
		ui::qt::colorEditButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::colorEditButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->Enabled();
	}
		break;
	case ak::ui::core::oComboBox:
	{
		ui::qt::comboBox * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::comboBox *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oComboButton:
	{
		ui::qt::comboButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::comboButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oCheckBox:
	{
		ui::qt::checkBox * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::checkBox *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oDock:
	{
		ui::qt::dock * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::dock *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oGraphicsView:
	{
		ui::qt::graphicsView * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::graphicsView *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oLabel:
	{
		ui::qt::label * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::label *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oLineEdit:
	{
		ui::qt::lineEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::lineEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oList:
	{
		ui::qt::list * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::list *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oLogInDialog:
	{
		ui::dialog::logIn * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::logIn *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oMainWindow:
	{
		ui::qt::window * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::window *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oNiceLineEdit:
	{
		ak::ui::qt::niceLineEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ak::ui::qt::niceLineEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oOptionsDialog:
	{
		ui::dialog::options * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::options *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oPrompt:
	{
		ui::dialog::prompt * actualObject = nullptr;
		actualObject = dynamic_cast<ui::dialog::prompt *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oPropertyGrid:
	{
		ui::qt::propertyGrid * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::propertyGrid *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oPushButton:
	{
		ui::qt::pushButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::pushButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oSpecialTabBar:
	{
		ui::qt::specialTabBar * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::specialTabBar *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oTable:
	{
		ui::qt::table * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::table *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oTabToolbarGroup:
	{
		ui::ttb::group * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::group *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->enabled();
	}
		break;
	case ak::ui::core::oTabToolbarPage:
	{
		ui::ttb::page * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::page *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->enabled();
	}
		break;
	case ak::ui::core::oTabToolbarSubgroup:
	{
		ui::ttb::subGroup * actualObject = nullptr;
		actualObject = dynamic_cast<ui::ttb::subGroup *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->enabled();
	}
		break;
	case ak::ui::core::oTabView:
	{
		ui::qt::tabView * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::tabView *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oTextEdit:
	{
		ui::qt::textEdit * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::textEdit *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oToolButton:
	{
		ui::qt::toolButton * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::toolButton *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->isEnabled();
	}
		break;
	case ak::ui::core::oTree:
	{
		ui::qt::tree * actualObject = nullptr;
		actualObject = dynamic_cast<ui::qt::tree *>(obj);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->enabled();
	}
		break;
	case ak::ui::core::oNone:
	case ak::ui::core::oComboButtonItem:
	case ak::ui::core::oDefaultWelcomeScreen:
	case ak::ui::core::oMenu:
	case ak::ui::core::oPropertyGridItem:
	case ak::ui::core::oTabToolbar:
	case ak::ui::core::oTimer:
	case ak::ui::core::oTreeItem:
		assert(0); // This object does not support setEnabled()
		return false;
	default:
		assert(0);	// Invalid object type
		return false;
	}
}

bool ak::uiAPI::object::exists(
	UID													_objectUID
) { return my_objManager->objectExists(_objectUID); }

// Object

// ###############################################################################################################################################

// Options dialog

void ak::uiAPI::optionsDialog::show(
	UID				_dialogUID
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	actualDialog->showDialog();
}

ak::ID ak::uiAPI::optionsDialog::createCategory(
	UID												_dialogUID,
	ID												_parentID,
	const QString &									_text
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->createCategory(_parentID, _text);
}

ak::ID ak::uiAPI::optionsDialog::createCategory(
	UID												_dialogUID,
	ID												_parentID,
	const QString &									_text,
	const QIcon &									_icon
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->createCategory(_parentID, _text, _icon);
}

void ak::uiAPI::optionsDialog::createGroup(
	UID												_dialogUID,
	ID												_categoryID,
	const QString &									_name,
	const ui::color &								_color
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	actualDialog->createGroup(_categoryID, _name, _color.toQColor());
}

void ak::uiAPI::optionsDialog::setGroupStateIcons(
	UID												_dialogUID,
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	actualDialog->setGroupStateIcons(_groupExpanded, _groupCollapsed);
}

void ak::uiAPI::optionsDialog::setGroupStateIcons(
	UID												_dialogUID,
	const QString &									_groupExpandedIconName,
	const QString &									_groupExpandedIconPath,
	const QString &									_groupCollapsedIconName,
	const QString &									_groupCollapsedIconPath
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	assert(my_iconManager != nullptr); // API not initialized
	actualDialog->setGroupStateIcons(*my_iconManager->icon(_groupExpandedIconName, _groupExpandedIconPath),
		*my_iconManager->icon(_groupCollapsedIconName, _groupExpandedIconPath));
}

// Add item to default group

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const ui::color &								_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _settingName, _value);
}

// Add item to specified group

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const ui::color &								_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value);
}

void ak::uiAPI::optionsDialog::clear(
	UID												_dialogUID
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	actualDialog->clear();
}

bool ak::uiAPI::optionsDialog::settingsChanged(
	UID												_dialogUID
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	return actualDialog->settingsChanged();
}

void ak::uiAPI::optionsDialog::addWindowEventHandler(
	UID												_dialogUID,
	ui::windowEventHandler *						_handler
) {
	auto actualDialog = object::get<ui::dialog::options>(_dialogUID);
	actualDialog->addWindowEventHandler(_handler);
}

// Options dialog

// ###############################################################################################################################################

// promptDialog

ak::ui::core::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title,
	ak::ui::core::promptType	_type
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::dialog::prompt dialog(_message, _title, _type);
	auto cs{ my_objManager->getCurrentColorStyle() };
	if (cs != nullptr) { dialog.setColorStyle(cs); }
	return dialog.showDialog();
}

ak::ui::core::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title
) { return show(_message, _title, ak::ui::core::promptType::promptOk); }

// promptDialog

// ###############################################################################################################################################

// propertyGrid

void ak::uiAPI::propertyGrid::addGroup(
	ak::UID											_propertyGridUID,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->addGroup(_groupName);
}

void ak::uiAPI::propertyGrid::addGroup(
	ak::UID											_propertyGridUID,
	const QColor &									_color,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
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
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

void ak::uiAPI::propertyGrid::clear(
	ak::UID											_propertyGridUID,
	bool											_keepGroups
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->clear(_keepGroups);
}

bool ak::uiAPI::propertyGrid::getItemIsReadOnly(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->itemIsReadOnly(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemGroup(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemGroup(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemIsMultipleValues(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemIsMultipleValues(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemName(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemName(_itemID);
}

std::vector<QString> ak::uiAPI::propertyGrid::getItemPossibleSelection(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemPossibleSelection(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemValueBool(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueBool(_itemID);
}

ak::ui::color ak::uiAPI::propertyGrid::getItemValueColor(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueColor(_itemID);
}

double ak::uiAPI::propertyGrid::getItemValueDouble(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueDouble(_itemID);
}

int ak::uiAPI::propertyGrid::getItemValueInteger(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueInteger(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueSelection(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueSelection(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueString(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueString(_itemID);
}

ak::core::valueType ak::uiAPI::propertyGrid::getItemValueType(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueType(_itemID);
}

void ak::uiAPI::propertyGrid::setItemIsReadOnly(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID,
	bool											_readOnly
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setItemReadOnly(_itemID, _readOnly);
}

void ak::uiAPI::propertyGrid::setGroupStateIcons(
	ak::UID											_propertyGridUID,
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setGroupStateIcons(_groupExpanded, _groupCollapsed);
}

void ak::uiAPI::propertyGrid::setGroupStateIcons(
	ak::UID											_propertyGridUID,
	const QString &									_groupExpandedIconName,
	const QString &									_groupExpandedIconSize,
	const QString &									_groupCollapsedIconName,
	const QString &									_groupCollapsedIconSize
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setGroupStateIcons(*my_iconManager->icon(_groupExpandedIconName, _groupExpandedIconSize),
		*my_iconManager->icon(_groupCollapsedIconName, _groupCollapsedIconSize));
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID,
	const QString &									_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID,
	int												_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID,
	double											_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::showItemAsError(
	ak::UID											_propertyGridUID,
	ak::ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->showItemAsError(_itemID);
}

void ak::uiAPI::propertyGrid::setEnabled(
	ak::UID											_propertyGridUID,
	bool											_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setEnabled(_enabled);
}

bool ak::uiAPI::propertyGrid::isEnabled(
	ak::UID											_propertyGridUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::propertyGrid * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<ui::qt::propertyGrid *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->isEnabled();
}

// propertyGrid

// ###############################################################################################################################################

// Special tab bar

void ak::uiAPI::specialTabBar::clearColors(
	ak::UID			_specialTabBarUID,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::specialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<ui::qt::specialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->clearColors(_repaint);
}

void ak::uiAPI::specialTabBar::clearColor(
	ak::UID			_specialTabBarUID,
	int				_index,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::specialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<ui::qt::specialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->clearColor(_index, _repaint);
}

void ak::uiAPI::specialTabBar::addColor(
	ak::UID			_specialTabBarUID,
	int				_index,
	ui::color		_color,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::specialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<ui::qt::specialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->addColor(_index, _color, _repaint);
}

void ak::uiAPI::specialTabBar::setRepaintBlocked(
	ak::UID			_specialTabBarUID,
	bool			_blocked
) {
	assert(my_objManager != nullptr); // API not initialized

	ui::qt::specialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<ui::qt::specialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->setRepaintBlocked(_blocked);
}

// Special tab bar
	
// ###############################################################################################################################################

// TabView

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title
) { return object::get<ui::qt::tabView>(_tabViewUID)->addTab(object::get<ui::core::aWidget>(_widgetUID)->widget(), _title); }

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	return object::get<ui::qt::tabView>(_tabViewUID)->addTab(object::get<ui::core::aWidget>(_widgetUID)->widget(), *my_iconManager->icon(_iconName, _iconFolder), _title);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	ak::UID				_widgetUID,
	const QString &		_title,
	const QIcon &		_icon
) { return object::get<ui::qt::tabView>(_tabViewUID)->addTab(object::get<ui::core::aWidget>(_widgetUID)->widget(), _icon, _title); }

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title
) {
	return object::get<ui::qt::tabView>(_tabViewUID)->addTab(_widget, _title);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	return object::get<ui::qt::tabView>(_tabViewUID)->addTab(_widget, *my_iconManager->icon(_iconName, _iconFolder), _title);
}

ak::ID ak::uiAPI::tabView::addTab(
	ak::UID				_tabViewUID,
	QWidget *			_widget,
	const QString &		_title,
	const QIcon &		_icon
) { return object::get<ui::qt::tabView>(_tabViewUID)->addTab(_widget, _icon, _title); }

void ak::uiAPI::tabView::closeAllTabs(
	ak::UID				_tabViewUID
) { object::get<ui::qt::tabView>(_tabViewUID)->clear(); }

void ak::uiAPI::tabView::closeTab(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) { return object::get<ui::qt::tabView>(_tabViewUID)->removeTab(_tabID); }

ak::ID ak::uiAPI::tabView::getFocusedTab(
	ak::UID				_tabViewUID
) { return object::get<ui::qt::tabView>(_tabViewUID)->currentIndex(); }

bool ak::uiAPI::tabView::getTabsClosable(
	ak::UID				_tabViewUID
) { return object::get<ui::qt::tabView>(_tabViewUID)->tabsClosable(); }

QString ak::uiAPI::tabView::getTabText(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) { return object::get<ui::qt::tabView>(_tabViewUID)->tabText(_tabID); }

void ak::uiAPI::tabView::setEnabled(
	ak::UID				_tabViewUID,
	bool				_enabled
) { object::get<ui::qt::tabView>(_tabViewUID)->setEnabled(_enabled); }

void ak::uiAPI::tabView::setTabbarLocation(
	ak::UID								_tabViewUID,
	ak::ui::core::tabLocation			_location
) { object::get<ui::qt::tabView>(_tabViewUID)->setTabLocation(_location); }

void ak::uiAPI::tabView::setTabFocused(
	ak::UID				_tabViewUID,
	ak::ID				_tabID
) { object::get<ui::qt::tabView>(_tabViewUID)->setCurrentIndex(_tabID); }

void ak::uiAPI::tabView::setTabsClosable(
	ak::UID								_tabViewUID,
	bool								_closeable
) { object::get<ui::qt::tabView>(_tabViewUID)->setTabsClosable(_closeable); }

void ak::uiAPI::tabView::setTabText(
	ak::UID								_tabViewUID,
	ak::ID								_tab,
	const QString &						_text
) { object::get<ui::qt::tabView>(_tabViewUID)->setTabText(_tab, _text); }

void ak::uiAPI::tabView::setSpecialTabBar(
	ak::UID								_tabViewUID,
	ak::UID								_specialTabBarUID
) { setSpecialTabBar(_tabViewUID, object::get<ui::qt::specialTabBar>(_specialTabBarUID)); }

void ak::uiAPI::tabView::setSpecialTabBar(
	ak::UID								_tabViewUID,
	QTabBar *							_specialTabBar
) { object::get<ui::qt::tabView>(_tabViewUID)->setCustomTabBar(_specialTabBar); }

void ak::uiAPI::tabView::setVisible(
	ak::UID				_tabViewUID,
	bool				_visible
) { object::get<ui::qt::tabView>(_tabViewUID)->setVisible(_visible); }

void ak::uiAPI::tabView::setObjectName(
	ak::UID							_tabViewUID,
	const QString &					_name
) {	object::get<ui::qt::tabView>(_tabViewUID)->setObjectName(_name); }

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
	actualTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	return actualTextEdit->insertPlainText(_text);
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
	return actualTextEdit->setPlainText(_text);
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

// ToolButton

bool ak::uiAPI::toolButton::getIsEnabled(
	ak::UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getAction()->isEnabled();
}

QString ak::uiAPI::toolButton::getText(
	ak::UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getAction()->text();
}

QString ak::uiAPI::toolButton::getToolTip(
	ak::UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->ToolTip();
}

void ak::uiAPI::toolButton::setEnabled(
	ak::UID							_toolButtonUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setEnabled(_enabled);
}

void ak::uiAPI::toolButton::setText(
	ak::UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setText(_text);
}

void ak::uiAPI::toolButton::setToolTip(
	ak::UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->SetToolTip(_text);
}

void ak::uiAPI::toolButton::setIcon(
	ak::UID							_toolButtonUID,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setIcon(_icon);
}

void ak::uiAPI::toolButton::setIcon(
	ak::UID							_toolButtonUID,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setIcon(*my_iconManager->icon(_iconName, _iconFolder));
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	ak::UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	ui::qt::contextMenuItem * itm = new ui::qt::contextMenuItem(_text, ui::core::contextMenuRole::crNone);
	return actualToolButton->addMenuItem(itm);
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	ak::UID							_toolButtonUID,
	const QIcon &					_icon,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	ui::qt::contextMenuItem * itm = new ui::qt::contextMenuItem(_icon ,_text, ui::core::contextMenuRole::crNone);
	return actualToolButton->addMenuItem(itm);
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	ak::UID							_toolButtonUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	ui::qt::contextMenuItem * itm = new ui::qt::contextMenuItem(*my_iconManager->icon(_iconName, _iconFolder), _text, ui::core::contextMenuRole::crNone);
	return actualToolButton->addMenuItem(itm);
}

void ak::uiAPI::toolButton::addMenuSeperator(
	ak::UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->addMenuSeperator();
}

void ak::uiAPI::toolButton::clearMenu(
	ak::UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->clearMenu();
}

void ak::uiAPI::toolButton::setMenuItemChecked(
	ak::UID							_toolButtonUID,
	ak::ID							_itemID,
	bool							_checked
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->setMenuItemChecked(_itemID, _checked);
}

void ak::uiAPI::toolButton::setMenuItemNotCheckable(
	ak::UID							_toolButtonUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->setMenuItemNotCheckable(_itemID);
}

QString ak::uiAPI::toolButton::getMenuItemText(
	ak::UID							_toolButtonUID,
	ak::ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::qt::toolButton * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<ui::qt::toolButton *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getMenuItemText(_itemID);
}

// ###############################################################################################################################################

// Tree

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_text,
	ak::ID							_parentId,
	ak::ui::core::textAlignment		_textAlignment
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
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
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->add(_parentId, _text, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

ak::ID ak::uiAPI::tree::addItem(
	ak::UID							_treeUID,
	const QString &					_cmd,
	char							_delimiter,
	ak::ui::core::textAlignment		_textAlignment
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
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
	assert(my_iconManager != nullptr); // API not initialized
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->add(_cmd, _delimiter, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

void ak::uiAPI::tree::clear(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->clear();
}

void ak::uiAPI::tree::collapseAllItems(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->collapseAllItems();
}

void ak::uiAPI::tree::deleteItem(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->deleteItem(_itemID);
}

void ak::uiAPI::tree::deleteItems(
	ak::UID							_treeUID,
	const std::vector<ak::ID> &		_itemIDs
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->deleteItems(_itemIDs);
}

void ak::uiAPI::tree::setItemsAreEditable(
	ak::UID							_treeUID,
	bool							_editable,
	bool							_applyToAll
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemsAreEditable(_editable, _applyToAll);
}

void ak::uiAPI::tree::setItemIsEditable(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_editable
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemIsEditable(_itemID, _editable);
}

void ak::uiAPI::tree::setItemIsEditable(
	ak::UID							_treeUID,
	const std::vector<ak::ID> &		_itemIDs,
	bool							_editable
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemIsEditable(_itemIDs, _editable);
}

void ak::uiAPI::tree::deselectAllItems(
	ak::UID							_treeUID,
	bool							_sendSelectionChangedEvent
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->deselectAllItems(_sendSelectionChangedEvent);
}

void ak::uiAPI::tree::expandAllItems(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->expandAllItems();
}

ak::ID ak::uiAPI::tree::getItemID(
	ak::UID							_treeUID,
	const QString &					_itemPath,
	char							_delimiter
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->getItemID(_itemPath, _delimiter);
}

std::vector<QString> ak::uiAPI::tree::getItemPath(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->getItemPath(_itemID);
}

QString ak::uiAPI::tree::getItemPathString(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	char							_delimiter
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->getItemPathString(_itemID, _delimiter);
}

QString ak::uiAPI::tree::getItemText(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->getItemText(_itemID);
}

bool ak::uiAPI::tree::isEnabled(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->enabled();
}

int ak::uiAPI::tree::itemCount(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->itemCount();
}

std::vector<ak::ID> ak::uiAPI::tree::selectedItems(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->selectedItems();
}

void ak::uiAPI::tree::setAutoSelectAndDeselectChildrenEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setAutoSelectAndDeselectChildrenEnabled(_enabled);
}

void ak::uiAPI::tree::setEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setEnabled(_enabled);
}

void ak::uiAPI::tree::setFilterCaseSensitiveEnabled(
	ak::UID							_treeUID,
	bool							_caseSensitiveEnabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setFilterCaseSensitive(_caseSensitiveEnabled);
}

void ak::uiAPI::tree::setFilterRefreshOnChangeEnabled(
	ak::UID							_treeUID,
	bool							_refreshOnChangeEnabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setFilterRefreshOnChange(_refreshOnChangeEnabled);
}

void ak::uiAPI::tree::setFilterVisible(
	ak::UID							_treeUID,
	bool							_visible
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setFilterVisible(_visible);
}

void ak::uiAPI::tree::setItemIcon(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	const QIcon &					_icon
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemIcon(_itemID, _icon);
}

void ak::uiAPI::tree::setItemIcon(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemIcon(_itemID, *my_iconManager->icon(_iconName, _iconFolder));
}

void ak::uiAPI::tree::setItemEnabled(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_enabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemEnabled(_itemID, _enabled);
}

void ak::uiAPI::tree::setItemSelected(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_selected,
	bool							_singleSelection
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	if (_singleSelection) {
		actualTree->setSingleItemSelected(_itemID, _selected);
	}
	else {
		actualTree->setItemSelected(_itemID, _selected);
	}
	
}

void ak::uiAPI::tree::setItemVisible(
	ak::UID							_treeUID,
	ak::ID							_itemID,
	bool							_visible
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemVisible(_itemID, _visible);
}

void ak::uiAPI::tree::setItemText(
	ak::UID							_treeUID,
	ak::ID							_itemId,
	const QString &					_text
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setItemText(_itemId, _text);
}

void ak::uiAPI::tree::setMultiSelectionEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setMultiSelectionEnabled(_enabled);
}

void ak::uiAPI::tree::setVisible(
	ak::UID							_treeUID,
	bool							_visible
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setVisible(_visible);
}

void ak::uiAPI::tree::toggleItemSelection(
	ak::UID							_treeUID,
	ak::ID							_itemID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->toggleItemSelection(_itemID);
}

void ak::uiAPI::tree::setSortingEnabled(
	ak::UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	actualTree->setSortingEnabled(_enabled);
}

bool ak::uiAPI::tree::isSortingEnabled(
	ak::UID							_treeUID
) {
	auto actualTree = object::get<ui::qt::tree>(_treeUID);
	return actualTree->isSortingEnabled();
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

void ak::uiAPI::window::addEventHandler(
	ak::UID												_windowUID,
	ui::windowEventHandler *							_eventHandler
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->addEventHandler(_eventHandler);
}

void ak::uiAPI::window::close(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->close();
}

void ak::uiAPI::window::removeEventHandler(
	ak::UID												_windowUID,
	ui::windowEventHandler *							_eventHandler
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->removeEventHandler(_eventHandler);
}

void ak::uiAPI::window::setStatusLabelVisible(
	ak::UID												_windowUID,
	bool												_visible,
	bool												_delayed
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusLabelVisible(_visible, _delayed);
}

void ak::uiAPI::window::setStatusProgressVisible(
	ak::UID												_windowUID,
	bool												_visible,
	bool												_delayed
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setStatusBarVisible(_visible, _delayed);
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

void ak::uiAPI::window::setCurrentTabToolBarTab(
	ak::UID												_windowUID,
	const std::string								   &_tabName
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setCurrentTabToolBarTab(_tabName);
}

void ak::uiAPI::window::setTitle(
	ak::UID												_windowUID,
	const QString &										_title
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setWindowTitle(_title);
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

QSize ak::uiAPI::window::size(
	ak::UID												_windowUID
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	return actualWindow->window()->size();
}

void ak::uiAPI::window::setWaitingAnimationVisible(
	ak::UID												_windowUID,
	bool												_visible
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setWaitingAnimationVisible(_visible);
}

void ak::uiAPI::window::setWaitingAnimation(
	ak::UID												_windowUID,
	QMovie *											_movie
) {
	assert(my_objManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setWaitingAnimation(_movie);
}

void ak::uiAPI::window::setWaitingAnimation(
	ak::UID												_windowUID,
	const QString &										_animationName
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setWaitingAnimation(my_iconManager->movie(_animationName));
 }

void ak::uiAPI::window::setCentralWidgetMinimumSize(
	ak::UID												_windowUID,
	int													_width,
	int													_height
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	ui::uiManager * actualWindow = nullptr;
	actualWindow = dynamic_cast<ui::uiManager *>(my_objManager->object(_windowUID));
	assert(actualWindow != nullptr); // Invalid object type
	actualWindow->setCentralWidgetMinimumSize(QSize(_width, _height));
}

int ak::uiAPI::window::devicePixelRatio(void) { return my_apiManager.desktop()->devicePixelRatio(); }

// ###############################################################################################################################################

// Crypto

QString ak::uiAPI::crypto::hash(
	const QString &				_orig,
	core::crypto::algorithm		_algorithm
) { return ak::core::crypto::hash(_orig, _algorithm); }


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

QString ak::uiAPI::special::toString(
	ak::core::settingsRestoreErrorCode					_settingsRestoreErrorCode
) {
	return ak::core::toQString(_settingsRestoreErrorCode);
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
