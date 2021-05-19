/*
 *	File:		uiAPI.cpp
 *	Package:	akAPI
 *
 *  Created on: February 06, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK main header
#include <akAPI/uiAPI.h>					// corresponding header

// AK Core
#include <akCore/aException.h>				// Error handling
#include <akCore/aFile.h>
#include <akCore/aMessenger.h>				// Messaging
#include <akCore/aNotifier.h>				// Notifier
#include <akCore/aObject.h>
#include <akCore/aSingletonAllowedMessages.h>
#include <akGui/aTimer.h>
#include <akCore/aUidMangager.h>

// AK dialogs
#include <akDialogs/aLogInDialog.h>
#include <akDialogs/aOptionsDialog.h>
#include <akDialogs/aPromptDialog.h>

// AK GUI
#include <akGui/aAction.h>
#include <akGui/aApplication.h>
#include <akGui/aColorStyle.h>
#include <akGui/aContextMenuItem.h>
#include <akGui/aIconManager.h>
#include <akGui/aRestorable.h>
#include <akGui/aSpecialTabBar.h>
#include <akGui/aTtbContainer.h>
#include <akGui/aWindowEventHandler.h>

// AK widgets
#include <akWidgets/aCheckBoxWidget.h>
#include <akWidgets/aColorEditButtonWidget.h>
#include <akWidgets/aComboBoxWidget.h>
#include <akWidgets/aComboButtonWidget.h>
#include <akWidgets/aDockWidget.h>
#include <akWidgets/aGraphicsWidget.h>
#include <akWidgets/aLabelWidget.h>
#include <akWidgets/aLineEditWidget.h>
#include <akWidgets/aListWidget.h>
#include <akWidgets/aNiceLineEditWidget.h>
#include <akWidgets/aPropertyGridWidget.h>
#include <akWidgets/aPushButtonWidget.h>
#include <akWidgets/aTableWidget.h>
#include <akWidgets/aTabWidget.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aToolButtonWidget.h>
#include <akWidgets/aTreeWidget.h>
#include <akWidgets/aTtbGroup.h>
#include <akWidgets/aTtbPage.h>
#include <akWidgets/aTtbSubgroup.h>
#include <akWidgets/aWidget.h>
#include <akWidgets/aWindow.h>
#include <akWidgets/aWindowManager.h>

// Qt header
#include <qsurfaceformat.h>					// QSurfaceFormat
#include <qfiledialog.h>					// Open/Save file dialog
#include <qfile.h>
#include <qmovie.h>
#include <qdesktopwidget.h>
#include <qfiledialog.h>

static ak::uiAPI::apiManager		my_apiManager;					//! The API manager
static ak::aObjectManager *			my_objManager = nullptr;					//! The object manager used in this API
static ak::aMessenger *				my_messenger = nullptr;					//! The messenger used in this API
static ak::aUidManager *			my_uidManager = nullptr;					//! The UID manager used in this API
static ak::aIconManager *			my_iconManager = nullptr;					//! The icon manager used in this API

template <class T> T * akCastObject(ak::aObject * _obj) {
	T * ret = nullptr;
	ret = dynamic_cast<T *>(_obj);
	assert(ret != nullptr);	// Cast failed
	return ret;
}

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
	aSingletonAllowedMessages::instance();
	my_fileUidManager = new aUidManager();
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
	aMessenger *										_messenger,
	aUidManager *										_uidManager,
	aIconManager *										_iconManager,
	aObjectManager *									_objectManager
) {
	assert(!my_isInitialized); // Is already initialized
	my_app = new aApplication();
	my_desktop = my_app->desktop();
	
	// messenger
	if (_messenger == nullptr) {
		my_messenger = new aMessenger();
		assert(my_messenger != nullptr); // Failed to create
	}
	else { my_messenger = _messenger; my_messengerIsExtern = true; }

	// uid manager
	if (_uidManager == nullptr) {
		my_uidManager = new aUidManager();
		assert(my_uidManager != nullptr); // Failed to create
	}
	else { my_uidManager = _uidManager; my_uidManagerIsExtern = true; }

	// icon manager
	if (_iconManager == nullptr) {
		my_iconManager = new aIconManager(QString(""));
		assert(my_iconManager != nullptr); // Failed to create
	}
	else { my_iconManager = _iconManager; my_iconManagerIsExtern = true; }

	// object manager
	if (_objectManager == nullptr) {
		my_objManager = new aObjectManager(my_messenger, my_uidManager);
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

ak::aFile * ak::uiAPI::apiManager::getFile(
	UID												_fileUid
) {
	if (_fileUid == ak::invalidUID) {
		aFile * f = new aFile();
		f->setUid(my_fileUidManager->getId());
		my_mapFiles.insert_or_assign(f->uid(), f);
		return f;
	}
	else {
		auto itm = my_mapFiles.find(_fileUid);
		assert(itm != my_mapFiles.end());	// Invalid file UID
		aFile * f = itm->second;
		return f;
	}
}

ak::aFile * ak::uiAPI::apiManager::getExistingFile(
	UID												_fileUid
) {
	auto itm = my_mapFiles.find(_fileUid);
	assert(itm != my_mapFiles.end());	// Invalid file UID
	aFile * f = itm->second;
	return f;
}

void ak::uiAPI::apiManager::deleteFile(
	UID												_fileUid
) {
	auto itm = my_mapFiles.find(_fileUid);
	assert(itm != my_mapFiles.end());	// Invalid file UID
	aFile * f = itm->second;
	delete f;
	my_mapFiles.erase(_fileUid);
}

void ak::uiAPI::apiManager::deleteAllFiles() {
	for (auto itm = my_mapFiles.begin(); itm != my_mapFiles.end(); itm++) {
		aFile * f = itm->second; delete f;
	}
	my_mapFiles.clear();
}

ak::aApplication * ak::uiAPI::apiManager::app() { return my_app; }

QDesktopWidget * ak::uiAPI::apiManager::desktop() { return my_desktop; }

// ###############################################################################################################################################

void ak::uiAPI::ini(
	aMessenger *										_messenger,
	aUidManager *										_uidManager,
	aIconManager *										_iconManager,
	aObjectManager *									_objectManager
) { my_apiManager.ini(_messenger, _uidManager, _iconManager, _objectManager); }

void ak::uiAPI::destroy(void) {	if (my_objManager != nullptr) { my_objManager->destroyAll(); } }

void ak::uiAPI::enableEventTypes(
	eventType											_types
) { aSingletonAllowedMessages::instance()->setFlag(_types); }

void ak::uiAPI::disableEventTypes(
	eventType									_types
) { aSingletonAllowedMessages::instance()->removeFlag(_types); }

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

std::vector<ak::eventType> ak::uiAPI::enabledEventTypes(void) { return aSingletonAllowedMessages::instance()->enabledMessages(); }

std::vector<ak::eventType> ak::uiAPI::disabledEventTypes(void) { return aSingletonAllowedMessages::instance()->disabledMessages(); }

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

ak::settingsRestoreErrorCode ak::uiAPI::restoreStateWindow(
	const std::string &									_json,
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return ak::settingsRestoreErrorCode::srecEmptySettingsString; }
	return my_objManager->restoreStateWindow(_json.c_str(), _applicationVersion);
}

ak::settingsRestoreErrorCode ak::uiAPI::restoreStateColorStyle(
	const std::string &									_json,
	const std::string &									_applicationVersion
) {
	assert(my_objManager != nullptr); // Not initialized
	if (_json.length() == 0) { return ak::settingsRestoreErrorCode::srecEmptySettingsString; }
	return my_objManager->restoreStateColorStyle(_json.c_str(), _applicationVersion);
}

QWidget * ak::uiAPI::getWidget(
	UID												_objectUid
) { return object::get<aWidget>(_objectUid)->widget(); }

ak::UID ak::uiAPI::getObjectCreator(
	UID												_objectUID
) {
	assert(my_objManager != nullptr);	// API not initialized
	return my_objManager->objectCreator(_objectUID);
}

ak::aObjectManager * ak::uiAPI::getObjectManager(void) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager;
}

// ###############################################################################################################################################

ak::UID ak::uiAPI::registerUidNotifier(
	UID												_senderUid,
	aNotifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerUidReceiver(_senderUid, _notifier);
}

ak::UID ak::uiAPI::registerEventTypeNotifier(
	eventType										_event,
	aNotifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerEventTypeReceiver(_event, _notifier);
}

ak::UID ak::uiAPI::registerAllMessagesNotifier(
	aNotifier *										_notifier
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->registerNotifierForAllMessages(_notifier);
}

void ak::uiAPI::sendMessage(
	UID												_senderUid,
	eventType										_event,
	int												_info1,
	int												_info2
) {
	assert(my_messenger != nullptr); // API not initialized
	return my_messenger->sendMessage(_senderUid, _event, _info1, _info2);
}

void ak::uiAPI::setSurfaceFormatDefaultSamplesCount(
	int												_count
) {
	QSurfaceFormat * format = my_apiManager.getDefaultSurfaceFormat();
	format->setSamples(_count);
	QSurfaceFormat::setDefaultFormat(*format);
}

// ###############################################################################################################################################

// Object creation

ak::UID ak::uiAPI::createAction(
	UID												_creatorUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, *my_iconManager->icon(_iconName, _iconSize));
}

ak::UID ak::uiAPI::createAction(
	UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, _icon);
}

ak::UID ak::uiAPI::createCheckbox(
	UID												_creatorUid,
	const QString &										_text,
	bool												_checked
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createCheckBox(_creatorUid, _text, _checked);
}

ak::UID ak::uiAPI::createColorEditButton(
	UID												_creatorUid,
	int													_r,
	int													_g,
	int													_b,
	int													_a,
	const QString &										_textOverride
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createColorEditButton(_creatorUid, aColor(_r, _g, _b, _a), _textOverride);
}

ak::UID ak::uiAPI::createColorEditButton(
	UID												_creatorUid,
	const aColor &									_color,
	const QString &									_textOverride
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createColorEditButton(_creatorUid, _color, _textOverride);
}

ak::UID ak::uiAPI::createComboBox(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createComboBox(_creatorUid);
}

ak::UID ak::uiAPI::createComboButton(
	UID												_creatorUid,
	const QString &										_text,
	const std::vector<QString> &						_possibleSelection
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createComboButton(_creatorUid, _text, _possibleSelection);
}

ak::UID ak::uiAPI::createDock(
	UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createDock(_creatorUid, _text);
}

ak::UID ak::uiAPI::createLineEdit(
	UID													_creatorUid,
	const QString &											_initialText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createLineEdit(_creatorUid, _initialText);
}

ak::UID ak::uiAPI::createLogInDialog(
	UID												_creatorUid,
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
	UID												_creatorUid,
	const QString &										_initialText,
	const QString &										_infoLabelText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createNiceLineEdit(_creatorUid, _initialText, _infoLabelText);
}

ak::UID ak::uiAPI::createPropertyGrid(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPropertyGrid(_creatorUid);
}

ak::UID ak::uiAPI::createPushButton(
	UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPushButton(_creatorUid, _text);
}

ak::UID ak::uiAPI::createOptionsDialog(
	UID												_creatorUid,
	const QString &										_title
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createOptionsDialog(_creatorUid, _title);
}

ak::UID ak::uiAPI::createPushButton(
	UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createPushButton(_creatorUid, _icon, _text);
}

ak::UID ak::uiAPI::createSpecialTabBar(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createSpecialTabBar(_creatorUid);
}

ak::UID ak::uiAPI::createTable(
	UID												_creatorUid,
	int													_rows,
	int													_columns
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTable(_creatorUid, _rows, _columns);
}

ak::UID ak::uiAPI::createTabToolBarSubContainer(
	UID												_creatorUid,
	UID												_parentUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTabToolBarSubContainer(_creatorUid, _parentUid, _text);
}

ak::UID ak::uiAPI::createTextEdit(
	UID												_creatorUid,
	const QString &										_initialText
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTextEdit(_creatorUid, _initialText);
}

ak::UID ak::uiAPI::createTimer(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTimer(_creatorUid);
}

ak::UID ak::uiAPI::createToolButton(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid);
}

ak::UID ak::uiAPI::createToolButton(
	UID												_creatorUid,
	const QString &										_text
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text);
}

ak::UID ak::uiAPI::createToolButton(
	UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, _icon);
}

ak::UID ak::uiAPI::createToolButton(
	UID												_creatorUid,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	return my_objManager->createToolButton(_creatorUid, _text, *my_iconManager->icon(_iconName, _iconFolder));
}

ak::UID ak::uiAPI::createTree(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTree(_creatorUid);
}

ak::UID ak::uiAPI::createTabView(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createTabView(_creatorUid);
}

ak::UID ak::uiAPI::createWindow(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	return my_objManager->createWindow(_creatorUid);
}

// Object creation

// ###############################################################################################################################################

// Action

void ak::uiAPI::action::setEnabled(
	UID												_actionUID,
	bool												_enabled
) { object::get<aAction>(_actionUID)->setEnabled(_enabled); }

void ak::uiAPI::action::setText(
	UID												_actionUID,
	const QString &										_text
) { object::get<aAction>(_actionUID)->setText(_text); }

void ak::uiAPI::action::setIcon(
	UID												_actionUID,
	const QIcon &										_icon
) { object::get<aAction>(_actionUID)->setIcon(_icon); }

void ak::uiAPI::action::setIcon(
	UID												_actionUID,
	const QString &										_iconName,
	const QString &										_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	object::get<aAction>(_actionUID)->setIcon(*my_iconManager->icon(_iconName, _iconFolder));
}

QString ak::uiAPI::action::getText(
	UID												_actionUID
) { return object::get<aAction>(_actionUID)->text(); }

// Action

// ###############################################################################################################################################

// CheckBox

void ak::uiAPI::checkBox::setChecked(
	UID												_checkBoxUID,
	bool												_checked
) { object::get<aCheckBoxWidget>(_checkBoxUID)->setChecked(_checked); }

void ak::uiAPI::checkBox::setText(
	UID												_checkBoxUID,
	const QString &										_text
) { object::get<aCheckBoxWidget>(_checkBoxUID)->setText(_text); }

void ak::uiAPI::checkBox::setEnabled(
	UID												_checkBoxUID,
	bool												_enabled
) { object::get<aCheckBoxWidget>(_checkBoxUID)->setEnabled(_enabled); }

QString ak::uiAPI::checkBox::text(
	UID												_checkBoxUID
) { return object::get<aCheckBoxWidget>(_checkBoxUID)->text(); }

bool ak::uiAPI::checkBox::isChecked(
	UID												_checkBoxUID
) { return object::get<aCheckBoxWidget>(_checkBoxUID)->isChecked(); }

bool ak::uiAPI::checkBox::isEnabled(
	UID												_checkBoxUID
) { return object::get<aCheckBoxWidget>(_checkBoxUID)->isEnabled(); }

// ###############################################################################################################################################

// Container

void ak::uiAPI::container::addObject(
	UID												_containerUID,
	UID												_objectUID
) { object::get<aTtbContainer>(_containerUID)->addChild(my_objManager->object(_objectUID)); }

void ak::uiAPI::container::setEnabled(
	UID												_containerUID,
	bool												_isEnabled
) { object::get<aTtbContainer>(_containerUID)->setEnabled(_isEnabled); }

bool ak::uiAPI::container::isEnabled(
	UID												_containerUID
) { return object::get<aTtbContainer>(_containerUID)->enabled(); }

// Container

// ###############################################################################################################################################

// ContextMenu

ak::ID ak::uiAPI::contextMenu::addItem(
	UID							_widgetUID,
	const QString &					_text,
	contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	aObject * obj = my_objManager->object(_widgetUID);
	aContextMenuItem * newItem = new aContextMenuItem(_text, _role);

	switch (obj->type())
	{
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->addContextMenuItem(newItem);
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::uiAPI::contextMenu::addItem(
	UID							_widgetUID,
	const QIcon &					_icon,
	const QString &					_text,
	contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	aContextMenuItem * newItem = new aContextMenuItem(_icon, _text, _role);

	switch (obj->type())
	{
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->addContextMenuItem(newItem);
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::uiAPI::contextMenu::addItem(
	UID							_widgetUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconSize,
	contextMenuRole		_role
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	aContextMenuItem * newItem = new aContextMenuItem(*my_iconManager->icon(_iconName, _iconSize), _text, _role);

	switch (obj->type())
	{
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->addContextMenuItem(newItem);
	default:
		delete newItem;
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

void ak::uiAPI::contextMenu::addSeparator(
	UID							_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID
	switch (obj->type())
	{
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->addContextMenuSeparator();
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::uiAPI::contextMenu::clear(
	UID							_widgetUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aObject * obj = my_objManager->object(_widgetUID);
	assert(obj != nullptr); // Invalid UID

	switch (obj->type())
	{
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->clearContextMenu();
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
	UID												_dockUID,
	UID												_widgetUID
) {
	auto actualDock{ object::get<aDockWidget>(_dockUID) };
	auto actualWidget{ object::get<aWidget>(_widgetUID) };
	
	QWidget * currentCentralWidget = actualDock->widget();
	if (currentCentralWidget != nullptr) {
		aObject * actualCentralObject = nullptr;
		actualCentralObject = dynamic_cast<aObject *>(currentCentralWidget);
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
	UID												_dockUID,
	QWidget *											_widget
) { object::get<aDockWidget>(_dockUID)->setWidget(_widget); }

void ak::uiAPI::dock::setVisible(
	UID												_dockUID,
	bool												_visible
) { object::get<aDockWidget>(_dockUID)->setVisible(_visible); }

bool ak::uiAPI::dock::isVisible(
	UID												_dockUID
) { return object::get<aDockWidget>(_dockUID)->isVisible(); }

// Dock

// ###############################################################################################################################################

// File

ak::UID ak::uiAPI::file::load(
	const QString &										_filePath
) {
	aFile * f = my_apiManager.getFile(ak::invalidUID);
	f->load(_filePath);
	return f->uid();
}

void ak::uiAPI::file::load(
	UID												_fileUid,
	const QString &										_filePath
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->load(_filePath);
}

void ak::uiAPI::file::save(
	UID												_fileUid,
	bool												_append
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->save(_append);
}

void ak::uiAPI::file::save(
	UID												_fileUid,
	const QString &										_filePath,
	bool												_append
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->save(_filePath, _append);
}

void ak::uiAPI::file::setPath(
	UID												_fileUid,
	const QString &										_path
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->setPath(_path);
}

void ak::uiAPI::file::setLines(
	UID												_fileUid,
	const QStringList &									_lines
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->setLines(_lines);
}

void ak::uiAPI::file::addLine(
	UID												_fileUid,
	const QString &										_line
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	QStringList lst = _line.split("\n");
	for (QString str : lst) { str.append('\n'); f->addLine(str); }
}

void ak::uiAPI::file::addLine(
	UID												_fileUid,
	const QStringList &									_lines
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	f->addLine(_lines);
}

QString ak::uiAPI::file::name(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->name();
}

QString ak::uiAPI::file::path(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->path();
}

QString ak::uiAPI::file::extension(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->extension();
}

QStringList ak::uiAPI::file::lines(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->lines();
}

int ak::uiAPI::file::linesCount(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->linesCount();
}

bool ak::uiAPI::file::hasChanged(
	UID												_fileUid
) {
	aFile * f = my_apiManager.getExistingFile(_fileUid);
	return f->hasChanged();
}

// file

// ###############################################################################################################################################

// Line edit

void ak::uiAPI::lineEdit::setText(
	UID												_lineEditUid,
	const QString &										_text
) { object::get<aLineEditWidget>(_lineEditUid)->setText(_text); }

void ak::uiAPI::lineEdit::setErrorState(
	UID												_lineEditUid,
	bool												_error
) { object::get<aLineEditWidget>(_lineEditUid)->setErrorState(_error); }

void ak::uiAPI::lineEdit::setErrorStateIsForeground(
	UID													_lineEditUid,
	bool												_isForeground
) { object::get<aLineEditWidget>(_lineEditUid)->setErrorStateIsForeground(_isForeground); }

void ak::uiAPI::lineEdit::setEnabled(
	UID												_lineEditUid,
	bool												_enabled
) { object::get<aLineEditWidget>(_lineEditUid)->setEnabled(_enabled); }

QString ak::uiAPI::lineEdit::text(
	UID												_lineEditUid
) { return object::get<aLineEditWidget>(_lineEditUid)->text(); }

bool ak::uiAPI::lineEdit::isEnabled(
	UID												_lineEditUid
) { return object::get<aLineEditWidget>(_lineEditUid)->isEnabled(); }

// ###############################################################################################################################################

// Log in dialog

ak::dialogResult ak::uiAPI::logInDialog::showDialog(
	UID												_dialogUID
) { return object::get<aLogInDialog>(_dialogUID)->showDialog(); }

void ak::uiAPI::logInDialog::close(
	UID												_dialogUID,
	dialogResult									_result
) { object::get<aLogInDialog>(_dialogUID)->close(_result); }

QString ak::uiAPI::logInDialog::getUsername(
	UID												_dialogUID
) { return object::get<aLogInDialog>(_dialogUID)->username(); }

QString ak::uiAPI::logInDialog::getPassword(
	UID												_dialogUID
) { return object::get<aLogInDialog>(_dialogUID)->password(); }

bool ak::uiAPI::logInDialog::getSavePassword(
	UID												_dialogUID
) { return object::get<aLogInDialog>(_dialogUID)->savePassword(); }

void ak::uiAPI::logInDialog::showToolTipAtUsername(
	UID												_dialogUID,
	const QString &										_text
) { object::get<aLogInDialog>(_dialogUID)->showToolTipAtUsername(_text); }

void ak::uiAPI::logInDialog::showToolTipAtPassword(
	UID												_dialogUID,
	const QString &										_text
) { object::get<aLogInDialog>(_dialogUID)->showToolTipAtPassword(_text); }

void ak::uiAPI::logInDialog::showToolTipAtCustomField(
	UID												_dialogUID,
	ID												_inputID,
	const QString &										_text
) { object::get<aLogInDialog>(_dialogUID)->showToolTipAtCustomInput(_inputID, _text); }

ak::ID ak::uiAPI::logInDialog::addCustomInputField(
	UID												_dialogUID,
	const QString &										_labelText,
	const QString &										_inputFieldInitialText
) { return object::get<aLogInDialog>(_dialogUID)->addCustomInput(_labelText, _inputFieldInitialText); }

QString ak::uiAPI::logInDialog::getCustomFieldText(
	UID												_dialogUID,
	ID												_fieldID
) { return object::get<aLogInDialog>(_dialogUID)->customInputText(_fieldID); }

// Log in dialog

// ###############################################################################################################################################

// Nice line edit

void ak::uiAPI::niceLineEdit::setText(
	UID												_lineEditUid,
	const QString &										_text
) { object::get<aNiceLineEditWidget>(_lineEditUid)->setText(_text); }

void ak::uiAPI::niceLineEdit::setErrorState(
	UID												_lineEditUid,
	bool												_error
) { object::get<aNiceLineEditWidget>(_lineEditUid)->setErrorState(_error); }

void ak::uiAPI::niceLineEdit::setErrorStateIsForeground(
	UID													_lineEditUid,
	bool												_isForeground
) { object::get<aNiceLineEditWidget>(_lineEditUid)->setErrorStateIsForeground(_isForeground); }

void ak::uiAPI::niceLineEdit::setInfoLabelText(
	UID												_lineEditUid,
	const QString &										_text
) { object::get<aNiceLineEditWidget>(_lineEditUid)->setInfoLabelText(_text); }

void ak::uiAPI::niceLineEdit::setEnabled(
	UID												_lineEditUid,
	bool												_enabled
) { object::get<aNiceLineEditWidget>(_lineEditUid)->setEnabled(_enabled); }

QString ak::uiAPI::niceLineEdit::text(
	UID												_lineEditUid
) { return object::get<aNiceLineEditWidget>(_lineEditUid)->text(); }

QString ak::uiAPI::niceLineEdit::infoLabelText(
	UID												_lineEditUid
) { return object::get<aNiceLineEditWidget>(_lineEditUid)->infoLabelText(); }

bool ak::uiAPI::niceLineEdit::isEnabled(
	UID												_lineEditUid
) { return object::get<aNiceLineEditWidget>(_lineEditUid)->isEnabled(); }

// ###############################################################################################################################################

// Object

void ak::uiAPI::object::destroy(
	UID												_objectUID,
	bool												_ignoreIfObjectHasChildObjects
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->destroy(_objectUID, _ignoreIfObjectHasChildObjects);
}

void ak::uiAPI::object::setAlias(
	UID												_objectUID,
	const QString &										_alias
) {
	assert(my_objManager != nullptr); // API not initialized
	aObject * actualObject = my_objManager->object(_objectUID);
	assert(actualObject != nullptr); // That should not happen
	my_objManager->removeAlias(actualObject->alias());	// Delete last alias
	actualObject->setAlias(_alias);
	if (_alias.length() != 0) { my_objManager->addAlias(_alias, _objectUID); }
}

void ak::uiAPI::object::setObjectUniqueName(
	UID												_objectUID,
	const QString &										_name
) { my_objManager->setObjectUniqueName(_objectUID, _name); }

QString ak::uiAPI::object::getObjectUniqueName(
	UID												_objectUID
) { return my_objManager->object(_objectUID)->uniqueName(); }

ak::UID ak::uiAPI::object::getUidFromObjectUniqueName(
	const QString &										_name
) { return my_objManager->object(_name)->uid(); }

void ak::uiAPI::object::setToolTip(
	UID													_objectUID,
	const QString &										_toolTip
) {
	assert(my_objManager != nullptr); // API not initialized
	auto obj{ my_objManager->object(_objectUID) };
	switch (obj->type())
	{
	case otToolButton: akCastObject<aToolButtonWidget>(obj)->SetToolTip(_toolTip); return;
	default:
		assert(0);
		throw std::exception("Object is not suitable");
	}
}

void ak::uiAPI::object::setEnabled(
	UID													_objectUID,
	bool												_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	auto obj{ my_objManager->object(_objectUID) };
	switch (obj->type())
	{
	case otAction: akCastObject<aAction>(obj)->setEnabled(_enabled); return;
	case otColorEditButton: akCastObject<aColorEditButtonWidget>(obj)->SetEnabled(_enabled); return;
	case otComboBox: akCastObject<aComboBoxWidget>(obj)->setEnabled(_enabled); return;
	case otComboButton: akCastObject<aComboButtonWidget>(obj)->setEnabled(_enabled); return;
	case otCheckBox: akCastObject<aCheckBoxWidget>(obj)->setEnabled(_enabled); return;
	case otDock: akCastObject<aDockWidget>(obj)->setEnabled(_enabled); return;
	case otGraphicsView: akCastObject<aGraphicsWidget>(obj)->setEnabled(_enabled); return;
	case otLabel: akCastObject<aLabelWidget>(obj)->setEnabled(_enabled); return;
	case otLineEdit: akCastObject<aLineEditWidget>(obj)->setEnabled(_enabled); return;
	case otList: akCastObject<aListWidget>(obj)->setEnabled(_enabled); return;
	case otLogInDialog: akCastObject<aLogInDialog>(obj)->setEnabled(_enabled); return;
	case otNiceLineEdit: akCastObject<aNiceLineEditWidget>(obj)->setEnabled(_enabled); return;
	case otOptionsDialog: akCastObject<aOptionsDialog>(obj)->setEnabled(_enabled); return;
	case otPrompt: akCastObject<aPromptDialog>(obj)->setEnabled(_enabled); return;
	case otPropertyGrid: akCastObject<aPropertyGridWidget>(obj)->setEnabled(_enabled); return;
	case otPushButton: akCastObject<aPushButtonWidget>(obj)->setEnabled(_enabled); return;
	case otSpecialTabBar: akCastObject<aSpecialTabBar>(obj)->setEnabled(_enabled); return;
	case otTable: akCastObject<aTableWidget>(obj)->setEnabled(_enabled); return;
	case otTabToolbarGroup: akCastObject<aTtbGroup>(obj)->setEnabled(_enabled); return;
	case otTabToolbarPage: akCastObject<aTtbPage>(obj)->setEnabled(_enabled); return;
	case otTabToolbarSubgroup: akCastObject<aTtbSubGroup>(obj)->setEnabled(_enabled); return;
	case otTabView: akCastObject<aTabWidget>(obj)->setEnabled(_enabled); return;
	case otTextEdit: akCastObject<aTextEditWidget>(obj)->setEnabled(_enabled); return;
	case otToolButton: akCastObject<aToolButtonWidget>(obj)->setEnabled(_enabled); return;
	case otTree: akCastObject<aTreeWidget>(obj)->setEnabled(_enabled); return;
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
	switch (obj->type())
	{
	case otAction: return akCastObject<aAction>(obj)->isEnabled();
	case otColorEditButton: return akCastObject<aColorEditButtonWidget>(obj)->Enabled();
	case otComboBox: return akCastObject<aComboBoxWidget>(obj)->isEnabled();
	case otComboButton: return akCastObject<aComboButtonWidget>(obj)->isEnabled();
	case otCheckBox: return akCastObject<aCheckBoxWidget>(obj)->isEnabled();
	case otDock: return akCastObject<aDockWidget>(obj)->isEnabled();
	case otGraphicsView: return akCastObject<aGraphicsWidget>(obj)->isEnabled();
	case otLabel: return akCastObject<aLabelWidget>(obj)->isEnabled();
	case otLineEdit: return akCastObject<aLineEditWidget>(obj)->isEnabled();
	case otList: return akCastObject<aListWidget>(obj)->isEnabled();
	case otLogInDialog: return akCastObject<aLogInDialog>(obj)->isEnabled();
	case otNiceLineEdit: return akCastObject<aNiceLineEditWidget>(obj)->isEnabled();
	case otOptionsDialog: return akCastObject<aOptionsDialog>(obj)->isEnabled();
	case otPrompt: return akCastObject<aPromptDialog>(obj)->isEnabled();
	case otPropertyGrid: return akCastObject<aPropertyGridWidget>(obj)->isEnabled();
	case otPushButton: return akCastObject<aPushButtonWidget>(obj)->isEnabled();
	case otSpecialTabBar: return akCastObject<aSpecialTabBar>(obj)->isEnabled();
	case otTable: return akCastObject<aTableWidget>(obj)->isEnabled();
	case otTabToolbarGroup: return akCastObject<aTtbGroup>(obj)->enabled();
	case otTabToolbarPage: return akCastObject<aTtbPage>(obj)->enabled();
	case otTabToolbarSubgroup: return akCastObject<aTtbSubGroup>(obj)->enabled();
	case otTabView: return akCastObject<aTabWidget>(obj)->isEnabled();
	case otTextEdit: return akCastObject<aTextEditWidget>(obj)->isEnabled();
	case otToolButton: return akCastObject<aPushButtonWidget>(obj)->isEnabled();
	case otTree: return akCastObject<aTreeWidget>(obj)->enabled();
	default:
		assert(0);	// Invalid object type
		return false;
	}
}

bool ak::uiAPI::object::exists(
	UID													_objectUID
) {
	assert(my_objManager != nullptr); // API is not initialized
	return my_objManager->objectExists(_objectUID);
}

ak::objectType ak::uiAPI::object::type(
	UID													_objectUID
) {
	assert(my_objManager != nullptr); // API is not initialized
	return my_objManager->object(_objectUID)->type();
}

// Object

// ###############################################################################################################################################

// Options dialog

ak::dialogResult ak::uiAPI::optionsDialog::show(
	UID				_dialogUID
) { return object::get<aOptionsDialog>(_dialogUID)->showDialog(); }

ak::ID ak::uiAPI::optionsDialog::createCategory(
	UID												_dialogUID,
	ID												_parentID,
	const QString &									_text
) { return object::get<aOptionsDialog>(_dialogUID)->createCategory(_parentID, _text); }

ak::ID ak::uiAPI::optionsDialog::createCategory(
	UID												_dialogUID,
	ID												_parentID,
	const QString &									_text,
	const QIcon &									_icon
) { return object::get<aOptionsDialog>(_dialogUID)->createCategory(_parentID, _text, _icon); }

void ak::uiAPI::optionsDialog::createGroup(
	UID												_dialogUID,
	ID												_categoryID,
	const QString &									_name,
	const aColor &								_color
) { return object::get<aOptionsDialog>(_dialogUID)->createGroup(_categoryID, _name, _color.toQColor()); }

void ak::uiAPI::optionsDialog::setGroupStateIcons(
	UID												_dialogUID,
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) { object::get<aOptionsDialog>(_dialogUID)->setGroupStateIcons(_groupExpanded, _groupCollapsed); }

void ak::uiAPI::optionsDialog::setGroupStateIcons(
	UID												_dialogUID,
	const QString &									_groupExpandedIconName,
	const QString &									_groupExpandedIconPath,
	const QString &									_groupCollapsedIconName,
	const QString &									_groupCollapsedIconPath
) {
	assert(my_iconManager != nullptr); // API not initialized
	object::get<aOptionsDialog>(_dialogUID)->setGroupStateIcons(
		*my_iconManager->icon(_groupExpandedIconName, _groupExpandedIconPath),
		*my_iconManager->icon(_groupCollapsedIconName, _groupExpandedIconPath));
}

// Add item to default group

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aColor &								_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _possibleSelection, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _settingName, _value); }

// Add item to specified group

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aColor &								_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _possibleSelection, _value); }

ak::ID ak::uiAPI::optionsDialog::addItem(
	UID												_dialogUID,
	ID												_categoryID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) { return object::get<aOptionsDialog>(_dialogUID)->addItem(_categoryID, _isMultipleValues, _groupName, _settingName, _value); }

void ak::uiAPI::optionsDialog::clear(
	UID												_dialogUID
) { object::get<aOptionsDialog>(_dialogUID)->clear(); }

bool ak::uiAPI::optionsDialog::settingsChanged(
	UID												_dialogUID
) { return object::get<aOptionsDialog>(_dialogUID)->settingsChanged(); }

void ak::uiAPI::optionsDialog::addWindowEventHandler(
	UID												_dialogUID,
	aWindowEventHandler *							_handler
) { object::get<aOptionsDialog>(_dialogUID)->addWindowEventHandler(_handler); }

// Options dialog

// ###############################################################################################################################################

// promptDialog

ak::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QString &				_iconName,
	const QString &				_iconPath
) {
	assert(my_iconManager != nullptr); // API not initialized
	return show(_message, _title, _type, *my_iconManager->icon(_iconName, _iconPath));
}

ak::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title,
	promptType					_type,
	const QIcon &				_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	aPromptDialog dialog(_message, _title, _type, _icon);
	auto cs{ my_objManager->getCurrentColorStyle() };
	if (cs != nullptr) { dialog.setColorStyle(cs); }
	return dialog.showDialog();
}

ak::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title,
	promptType					_type
) {
	assert(my_objManager != nullptr); // API not initialized
	aPromptDialog dialog(_message, _title, _type);
	auto cs{ my_objManager->getCurrentColorStyle() };
	if (cs != nullptr) { dialog.setColorStyle(cs); }
	return dialog.showDialog();
}

ak::dialogResult ak::uiAPI::promptDialog::show(
	const QString &				_message,
	const QString &				_title
) { return show(_message, _title, promptOk); }

// promptDialog

// ###############################################################################################################################################

// propertyGrid

void ak::uiAPI::propertyGrid::addGroup(
	UID											_propertyGridUID,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->addGroup(_groupName);
}

void ak::uiAPI::propertyGrid::addGroup(
	UID											_propertyGridUID,
	const QColor &									_color,
	const QString &									_groupName
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->addGroup(_color, _groupName);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	bool											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const aColor &								_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	double											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	int												_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const char *									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, QString(_value));
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_itemName,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	bool											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const aColor &								_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	double											_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	int												_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _possibleSelection, _value);
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const char *									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, QString(_value));
}

ak::ID ak::uiAPI::propertyGrid::addItem(
	UID											_propertyGridUID,
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_itemName,
	const QString &									_value
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->addItem(_isMultipleValues, _groupName, _itemName, _value);
}

void ak::uiAPI::propertyGrid::clear(
	UID											_propertyGridUID,
	bool											_keepGroups
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->clear(_keepGroups);
}

bool ak::uiAPI::propertyGrid::getItemIsReadOnly(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->itemIsReadOnly(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemGroup(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemGroup(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemIsMultipleValues(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemIsMultipleValues(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemName(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemName(_itemID);
}

std::vector<QString> ak::uiAPI::propertyGrid::getItemPossibleSelection(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemPossibleSelection(_itemID);
}

bool ak::uiAPI::propertyGrid::getItemValueBool(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueBool(_itemID);
}

ak::aColor ak::uiAPI::propertyGrid::getItemValueColor(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueColor(_itemID);
}

double ak::uiAPI::propertyGrid::getItemValueDouble(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueDouble(_itemID);
}

int ak::uiAPI::propertyGrid::getItemValueInteger(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueInteger(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueSelection(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueSelection(_itemID);
}

QString ak::uiAPI::propertyGrid::getItemValueString(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueString(_itemID);
}

ak::valueType ak::uiAPI::propertyGrid::getItemValueType(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->getItemValueType(_itemID);
}

void ak::uiAPI::propertyGrid::setItemIsReadOnly(
	UID											_propertyGridUID,
	ID											_itemID,
	bool											_readOnly
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setItemReadOnly(_itemID, _readOnly);
}

void ak::uiAPI::propertyGrid::setGroupStateIcons(
	UID											_propertyGridUID,
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) {
	assert(my_objManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setGroupStateIcons(_groupExpanded, _groupCollapsed);
}

void ak::uiAPI::propertyGrid::setGroupStateIcons(
	UID											_propertyGridUID,
	const QString &									_groupExpandedIconName,
	const QString &									_groupExpandedIconSize,
	const QString &									_groupCollapsedIconName,
	const QString &									_groupCollapsedIconSize
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setGroupStateIcons(*my_iconManager->icon(_groupExpandedIconName, _groupExpandedIconSize),
		*my_iconManager->icon(_groupCollapsedIconName, _groupCollapsedIconSize));
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	UID											_propertyGridUID,
	ID											_itemID,
	const QString &									_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	UID											_propertyGridUID,
	ID											_itemID,
	int												_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::resetItemAsError(
	UID											_propertyGridUID,
	ID											_itemID,
	double											_valueToReset
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->resetItemAsError(_itemID, _valueToReset);
}

void ak::uiAPI::propertyGrid::showItemAsError(
	UID											_propertyGridUID,
	ID											_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->showItemAsError(_itemID);
}

void ak::uiAPI::propertyGrid::setEnabled(
	UID											_propertyGridUID,
	bool											_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	actualPropertyGrid->setEnabled(_enabled);
}

bool ak::uiAPI::propertyGrid::isEnabled(
	UID											_propertyGridUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aPropertyGridWidget * actualPropertyGrid = nullptr;
	actualPropertyGrid = dynamic_cast<aPropertyGridWidget *>(my_objManager->object(_propertyGridUID));
	assert(actualPropertyGrid != nullptr); // Invalid object type
	return actualPropertyGrid->isEnabled();
}

// propertyGrid

// ###############################################################################################################################################

// Special tab bar

void ak::uiAPI::specialTabBar::clearColors(
	UID			_specialTabBarUID,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	aSpecialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<aSpecialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->clearColors(_repaint);
}

void ak::uiAPI::specialTabBar::clearColor(
	UID			_specialTabBarUID,
	int				_index,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	aSpecialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<aSpecialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->clearColor(_index, _repaint);
}

void ak::uiAPI::specialTabBar::addColor(
	UID			_specialTabBarUID,
	int				_index,
	aColor		_color,
	bool			_repaint
) {
	assert(my_objManager != nullptr); // API not initialized

	aSpecialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<aSpecialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->addColor(_index, _color, _repaint);
}

void ak::uiAPI::specialTabBar::setRepaintBlocked(
	UID			_specialTabBarUID,
	bool			_blocked
) {
	assert(my_objManager != nullptr); // API not initialized

	aSpecialTabBar * actualTabBar = nullptr;
	actualTabBar = dynamic_cast<aSpecialTabBar *>(my_objManager->object(_specialTabBarUID));
	assert(actualTabBar != nullptr); // Invalid object type

	actualTabBar->setRepaintBlocked(_blocked);
}

// Special tab bar
	
// ###############################################################################################################################################

// TabView

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	UID				_widgetUID,
	const QString &		_title
) { return object::get<aTabWidget>(_tabWidgetUID)->addTab(object::get<aWidget>(_widgetUID)->widget(), _title); }

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	UID				_widgetUID,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	return object::get<aTabWidget>(_tabWidgetUID)->addTab(object::get<aWidget>(_widgetUID)->widget(), *my_iconManager->icon(_iconName, _iconFolder), _title);
}

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	UID				_widgetUID,
	const QString &		_title,
	const QIcon &		_icon
) { return object::get<aTabWidget>(_tabWidgetUID)->addTab(object::get<aWidget>(_widgetUID)->widget(), _icon, _title); }

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	QWidget *			_widget,
	const QString &		_title
) {
	return object::get<aTabWidget>(_tabWidgetUID)->addTab(_widget, _title);
}

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	QWidget *			_widget,
	const QString &		_title,
	const QString &		_iconName,
	const QString &		_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	return object::get<aTabWidget>(_tabWidgetUID)->addTab(_widget, *my_iconManager->icon(_iconName, _iconFolder), _title);
}

ak::ID ak::uiAPI::tabWidget::addTab(
	UID				_tabWidgetUID,
	QWidget *			_widget,
	const QString &		_title,
	const QIcon &		_icon
) { return object::get<aTabWidget>(_tabWidgetUID)->addTab(_widget, _icon, _title); }

void ak::uiAPI::tabWidget::closeAllTabs(
	UID				_tabWidgetUID
) { object::get<aTabWidget>(_tabWidgetUID)->clear(); }

void ak::uiAPI::tabWidget::closeTab(
	UID				_tabWidgetUID,
	ID				_tabID
) { return object::get<aTabWidget>(_tabWidgetUID)->removeTab(_tabID); }

ak::ID ak::uiAPI::tabWidget::getFocusedTab(
	UID				_tabWidgetUID
) { return object::get<aTabWidget>(_tabWidgetUID)->currentIndex(); }

bool ak::uiAPI::tabWidget::getTabsClosable(
	UID				_tabWidgetUID
) { return object::get<aTabWidget>(_tabWidgetUID)->tabsClosable(); }

QString ak::uiAPI::tabWidget::getTabText(
	UID				_tabWidgetUID,
	ID				_tabID
) { return object::get<aTabWidget>(_tabWidgetUID)->tabText(_tabID); }

void ak::uiAPI::tabWidget::setEnabled(
	UID				_tabWidgetUID,
	bool				_enabled
) { object::get<aTabWidget>(_tabWidgetUID)->setEnabled(_enabled); }

void ak::uiAPI::tabWidget::setTabbarLocation(
	UID								_tabWidgetUID,
	tabLocation						_location
) { object::get<aTabWidget>(_tabWidgetUID)->setTabLocation(_location); }

void ak::uiAPI::tabWidget::setTabFocused(
	UID				_tabWidgetUID,
	ID				_tabID
) { object::get<aTabWidget>(_tabWidgetUID)->setCurrentIndex(_tabID); }

void ak::uiAPI::tabWidget::setTabsClosable(
	UID								_tabWidgetUID,
	bool								_closeable
) { object::get<aTabWidget>(_tabWidgetUID)->setTabsClosable(_closeable); }

void ak::uiAPI::tabWidget::setTabText(
	UID								_tabWidgetUID,
	ID								_tab,
	const QString &						_text
) { object::get<aTabWidget>(_tabWidgetUID)->setTabText(_tab, _text); }

void ak::uiAPI::tabWidget::setSpecialTabBar(
	UID								_tabWidgetUID,
	UID								_specialTabBarUID
) { setSpecialTabBar(_tabWidgetUID, object::get<aSpecialTabBar>(_specialTabBarUID)); }

void ak::uiAPI::tabWidget::setSpecialTabBar(
	UID								_tabWidgetUID,
	QTabBar *							_specialTabBar
) { object::get<aTabWidget>(_tabWidgetUID)->setCustomTabBar(_specialTabBar); }

void ak::uiAPI::tabWidget::setVisible(
	UID				_tabWidgetUID,
	bool				_visible
) { object::get<aTabWidget>(_tabWidgetUID)->setVisible(_visible); }

void ak::uiAPI::tabWidget::setObjectName(
	UID							_tabWidgetUID,
	const QString &					_name
) {	object::get<aTabWidget>(_tabWidgetUID)->setObjectName(_name); }

bool ak::uiAPI::tabWidget::hasTab(
	UID							_tabWidgetUID,
	const QString &				_tabText
) { return object::get<aTabWidget>(_tabWidgetUID)->hasTab(_tabText); }

// TabView

// ###############################################################################################################################################

// TextEdit

void ak::uiAPI::textEdit::appendText(
	UID				_textEditUID,
	const QString &		_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aTextEditWidget * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<aTextEditWidget *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	actualTextEdit->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
	return actualTextEdit->insertPlainText(_text);
}

void ak::uiAPI::textEdit::clear(
	UID				_textEditUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aTextEditWidget * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<aTextEditWidget *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->clear();
}

void ak::uiAPI::textEdit::setAutoScrollToBottomEnabled(
	UID				_textEditUID,
	bool				_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	aTextEditWidget * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<aTextEditWidget *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->setAutoScrollToBottom(_enabled);
}

void ak::uiAPI::textEdit::setReadOnly(
	UID				_textEditUID,
	bool				_readOnly
) {
	assert(my_objManager != nullptr); // API not initialized
	aTextEditWidget * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<aTextEditWidget *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	actualTextEdit->setReadOnly(_readOnly);
}

void ak::uiAPI::textEdit::setText(
	UID				_textEditUID,
	const QString &		_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aTextEditWidget * actualTextEdit = nullptr;
	actualTextEdit = dynamic_cast<aTextEditWidget *>(my_objManager->object(_textEditUID));
	assert(actualTextEdit != nullptr); // Invalid object type
	return actualTextEdit->setPlainText(_text);
}

// TextEdit

// ###############################################################################################################################################

// Timer

int ak::uiAPI::timer::getInterval(
	UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	return actualTimer->interval();
}

void ak::uiAPI::timer::setInterval(
	UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
}

void ak::uiAPI::timer::shoot(
	UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setSingleShot(true);
	actualTimer->start();
}

void ak::uiAPI::timer::shoot(
	UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
	actualTimer->setSingleShot(true);
	actualTimer->start();
}

void ak::uiAPI::timer::start(
	UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setSingleShot(false);
	actualTimer->start();
}

void ak::uiAPI::timer::start(
	UID							_timerUID,
	int								_interval
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->setInterval(_interval);
	actualTimer->setSingleShot(false);
	actualTimer->start();
}

void ak::uiAPI::timer::stop(
	UID							_timerUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aTimer * actualTimer = nullptr;
	actualTimer = dynamic_cast<aTimer *>(my_objManager->object(_timerUID));
	assert(actualTimer != nullptr); // Invalid object type
	actualTimer->stop();
}

// Timer

// ###############################################################################################################################################

// ToolButton

bool ak::uiAPI::toolButton::getIsEnabled(
	UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getAction()->isEnabled();
}

QString ak::uiAPI::toolButton::getText(
	UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getAction()->text();
}

QString ak::uiAPI::toolButton::getToolTip(
	UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->ToolTip();
}

void ak::uiAPI::toolButton::setEnabled(
	UID							_toolButtonUID,
	bool							_enabled
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setEnabled(_enabled);
}

void ak::uiAPI::toolButton::setText(
	UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setText(_text);
}

void ak::uiAPI::toolButton::setToolTip(
	UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->SetToolTip(_text);
}

void ak::uiAPI::toolButton::setIcon(
	UID							_toolButtonUID,
	const QIcon &					_icon
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setIcon(_icon);
}

void ak::uiAPI::toolButton::setIcon(
	UID							_toolButtonUID,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->getAction()->setIcon(*my_iconManager->icon(_iconName, _iconFolder));
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	UID							_toolButtonUID,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	aContextMenuItem * itm = new aContextMenuItem(_text, cmrNone);
	return actualToolButton->addMenuItem(itm);
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	UID							_toolButtonUID,
	const QIcon &					_icon,
	const QString &					_text
) {
	assert(my_objManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	aContextMenuItem * itm = new aContextMenuItem(_icon ,_text, cmrNone);
	return actualToolButton->addMenuItem(itm);
}

ak::ID ak::uiAPI::toolButton::addMenuItem(
	UID							_toolButtonUID,
	const QString &					_text,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	aContextMenuItem * itm = new aContextMenuItem(*my_iconManager->icon(_iconName, _iconFolder), _text, cmrNone);
	return actualToolButton->addMenuItem(itm);
}

void ak::uiAPI::toolButton::addMenuSeperator(
	UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->addMenuSeperator();
}

void ak::uiAPI::toolButton::clearMenu(
	UID							_toolButtonUID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->clearMenu();
}

void ak::uiAPI::toolButton::setMenuItemChecked(
	UID							_toolButtonUID,
	ID							_itemID,
	bool							_checked
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->setMenuItemChecked(_itemID, _checked);
}

void ak::uiAPI::toolButton::setMenuItemNotCheckable(
	UID							_toolButtonUID,
	ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	actualToolButton->setMenuItemNotCheckable(_itemID);
}

QString ak::uiAPI::toolButton::getMenuItemText(
	UID							_toolButtonUID,
	ID							_itemID
) {
	assert(my_objManager != nullptr); // API not initialized
	assert(my_iconManager != nullptr); // API not initialized
	aToolButtonWidget * actualToolButton = nullptr;
	actualToolButton = dynamic_cast<aToolButtonWidget *>(my_objManager->object(_toolButtonUID));
	assert(actualToolButton != nullptr); // Invalid object type
	return actualToolButton->getMenuItemText(_itemID);
}

// ###############################################################################################################################################

// Tree

ak::ID ak::uiAPI::tree::addItem(
	UID							_treeUID,
	const QString &				_text,
	ID							_parentId,
	textAlignment				_textAlignment
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->add(_parentId, _text, _textAlignment);
}

ak::ID ak::uiAPI::tree::addItem(
	UID							_treeUID,
	const QString &				_text,
	const QString &				_iconName,
	const QString &				_iconSize,
	ID							_parentId,
	textAlignment				_textAlignment
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->add(_parentId, _text, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

ak::ID ak::uiAPI::tree::addItem(
	UID							_treeUID,
	const QString &				_cmd,
	char						_delimiter,
	textAlignment				_textAlignment
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->add(_cmd, _delimiter, _textAlignment);
}

ak::ID ak::uiAPI::tree::addItem(
	UID							_treeUID,
	const QString &				_cmd,
	char						_delimiter,
	const QString &				_iconName,
	const QString &				_iconSize,
	textAlignment				_textAlignment
) {
	assert(my_iconManager != nullptr); // API not initialized
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->add(_cmd, _delimiter, _textAlignment, *my_iconManager->icon(_iconName, _iconSize));
}

void ak::uiAPI::tree::clear(
	UID							_treeUID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->clear();
}

void ak::uiAPI::tree::collapseAllItems(
	UID							_treeUID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->collapseAllItems();
}

void ak::uiAPI::tree::deleteItem(
	UID							_treeUID,
	ID							_itemID
) { object::get<aTreeWidget>(_treeUID)->deleteItem(_itemID); }

void ak::uiAPI::tree::deleteItems(
	UID							_treeUID,
	const std::vector<ID> &		_itemIDs
) { object::get<aTreeWidget>(_treeUID)->deleteItems(_itemIDs); }

void ak::uiAPI::tree::setItemsAreEditable(
	UID							_treeUID,
	bool							_editable,
	bool							_applyToAll
) { object::get<aTreeWidget>(_treeUID)->setItemsAreEditable(_editable, _applyToAll); }

void ak::uiAPI::tree::setItemIsEditable(
	UID							_treeUID,
	ID							_itemID,
	bool							_editable
) { object::get<aTreeWidget>(_treeUID)->setItemIsEditable(_itemID, _editable); }

void ak::uiAPI::tree::setIsReadOnly(
	UID							_treeUID,
	bool							_readOnly
) { object::get<aTreeWidget>(_treeUID)->setIsReadOnly(_readOnly); }

void ak::uiAPI::tree::setItemIsEditable(
	UID							_treeUID,
	const std::vector<ID> &		_itemIDs,
	bool							_editable
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemIsEditable(_itemIDs, _editable);
}

void ak::uiAPI::tree::deselectAllItems(
	UID							_treeUID,
	bool							_sendSelectionChangedEvent
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->deselectAllItems(_sendSelectionChangedEvent);
}

void ak::uiAPI::tree::expandAllItems(
	UID							_treeUID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->expandAllItems();
}

ak::ID ak::uiAPI::tree::getItemID(
	UID							_treeUID,
	const QString &					_itemPath,
	char							_delimiter
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->getItemID(_itemPath, _delimiter);
}

std::vector<QString> ak::uiAPI::tree::getItemPath(
	UID							_treeUID,
	ID							_itemID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->getItemPath(_itemID);
}

QString ak::uiAPI::tree::getItemPathString(
	UID							_treeUID,
	ID							_itemID,
	char							_delimiter
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->getItemPathString(_itemID, _delimiter);
}

QString ak::uiAPI::tree::getItemText(
	UID							_treeUID,
	ID							_itemID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->getItemText(_itemID);
}

bool ak::uiAPI::tree::isEnabled(
	UID							_treeUID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->enabled();
}

int ak::uiAPI::tree::itemCount(
	UID							_treeUID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	return actualTree->itemCount();
}

std::vector<ak::ID> ak::uiAPI::tree::selectedItems(
	UID							_treeUID
) { return object::get<aTreeWidget>(_treeUID)->selectedItems(); }

void ak::uiAPI::tree::setAutoSelectAndDeselectChildrenEnabled(
	UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setAutoSelectAndDeselectChildrenEnabled(_enabled);
}

void ak::uiAPI::tree::setEnabled(
	UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setEnabled(_enabled);
}

void ak::uiAPI::tree::setFilterCaseSensitiveEnabled(
	UID							_treeUID,
	bool							_caseSensitiveEnabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setFilterCaseSensitive(_caseSensitiveEnabled);
}

void ak::uiAPI::tree::setFilterRefreshOnChangeEnabled(
	UID							_treeUID,
	bool							_refreshOnChangeEnabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setFilterRefreshOnChange(_refreshOnChangeEnabled);
}

void ak::uiAPI::tree::setFilterVisible(
	UID							_treeUID,
	bool							_visible
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setFilterVisible(_visible);
}

void ak::uiAPI::tree::setItemIcon(
	UID							_treeUID,
	ID							_itemID,
	const QIcon &					_icon
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemIcon(_itemID, _icon);
}

void ak::uiAPI::tree::setItemIcon(
	UID							_treeUID,
	ID							_itemID,
	const QString &					_iconName,
	const QString &					_iconFolder
) {
	assert(my_iconManager != nullptr); // API not initialized
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemIcon(_itemID, *my_iconManager->icon(_iconName, _iconFolder));
}

void ak::uiAPI::tree::setItemEnabled(
	UID							_treeUID,
	ID							_itemID,
	bool							_enabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemEnabled(_itemID, _enabled);
}

void ak::uiAPI::tree::setItemSelected(
	UID							_treeUID,
	ID							_itemID,
	bool							_selected,
	bool							_singleSelection
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	if (_singleSelection) {
		actualTree->setSingleItemSelected(_itemID, _selected);
	}
	else {
		actualTree->setItemSelected(_itemID, _selected);
	}
	
}

void ak::uiAPI::tree::setItemVisible(
	UID							_treeUID,
	ID							_itemID,
	bool							_visible
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemVisible(_itemID, _visible);
}

void ak::uiAPI::tree::setItemText(
	UID							_treeUID,
	ID							_itemId,
	const QString &					_text
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setItemText(_itemId, _text);
}

void ak::uiAPI::tree::setMultiSelectionEnabled(
	UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setMultiSelectionEnabled(_enabled);
}

void ak::uiAPI::tree::setVisible(
	UID							_treeUID,
	bool							_visible
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setVisible(_visible);
}

void ak::uiAPI::tree::toggleItemSelection(
	UID							_treeUID,
	ID							_itemID
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->toggleItemSelection(_itemID);
}

void ak::uiAPI::tree::setSortingEnabled(
	UID							_treeUID,
	bool							_enabled
) {
	auto actualTree = object::get<aTreeWidget>(_treeUID);
	actualTree->setSortingEnabled(_enabled);
}

bool ak::uiAPI::tree::isSortingEnabled(
	UID							_treeUID
) { return object::get<aTreeWidget>(_treeUID)->isSortingEnabled(); }

bool ak::uiAPI::tree::isReadOnly(
	UID							_treeUID
) { return object::get<aTreeWidget>(_treeUID)->isReadOnly(); }

// Tree

// ###############################################################################################################################################

// Window

void ak::uiAPI::window::addEventHandler(
	UID												_windowUID,
	aWindowEventHandler *							_eventHandler
) { object::get<aWindowManager>(_windowUID)->addEventHandler(_eventHandler); }

void ak::uiAPI::window::close(
	UID												_windowUID
) { object::get<aWindowManager>(_windowUID)->close(); }

void ak::uiAPI::window::removeEventHandler(
	UID												_windowUID,
	aWindowEventHandler *							_eventHandler
) { object::get<aWindowManager>(_windowUID)->removeEventHandler(_eventHandler); }

void ak::uiAPI::window::setStatusLabelVisible(
	UID												_windowUID,
	bool												_visible,
	bool												_delayed
) { object::get<aWindowManager>(_windowUID)->setStatusLabelVisible(_visible, _delayed); }

void ak::uiAPI::window::setStatusProgressVisible(
	UID												_windowUID,
	bool												_visible,
	bool												_delayed
) { object::get<aWindowManager>(_windowUID)->setStatusBarVisible(_visible, _delayed); }

void ak::uiAPI::window::setStatusLabelText(
	UID												_windowUID,
	const QString &										_text
) { object::get<aWindowManager>(_windowUID)->setStatusLabelText(_text); }

void ak::uiAPI::window::setStatusProgressValue(
	UID												_windowUID,
	int													_value
) { object::get<aWindowManager>(_windowUID)->setStatusBarProgress(_value); }

void ak::uiAPI::window::setStatusProgressContinuous(
	UID												_windowUID,
	bool												_continuous
) { object::get<aWindowManager>(_windowUID)->setStatusBarContinuous(_continuous); }

void ak::uiAPI::window::setTabToolBarVisible(
	UID												_windowUID,
	bool												_visible
) { object::get<aWindowManager>(_windowUID)->setTabToolBarVisible(_visible); }

ak::ID ak::uiAPI::window::getCurrentTabToolBarTab(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->currentTabToolbarTab(); }

int ak::uiAPI::window::getTabToolBarTabCount(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->tabToolbarTabCount(); }

void ak::uiAPI::window::setCurrentTabToolBarTab(
	UID												_windowUID,
	ID												_tabID
) { object::get<aWindowManager>(_windowUID)->setCurrentTabToolBarTab(_tabID); }

void ak::uiAPI::window::setCurrentTabToolBarTab(
	UID												_windowUID,
	const std::string								   &_tabName
) { object::get<aWindowManager>(_windowUID)->setCurrentTabToolBarTab(_tabName); }

void ak::uiAPI::window::setTitle(
	UID												_windowUID,
	const QString &										_title
) { object::get<aWindowManager>(_windowUID)->setWindowTitle(_title); }

bool ak::uiAPI::window::getStatusLabelVisible(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->getStatusLabelVisible(); }

bool ak::uiAPI::window::getStatusProgressVisible(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->getStatusBarVisible(); }

QString ak::uiAPI::window::getStatusLabelText(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->getStatusLabelText(); }

int ak::uiAPI::window::getStatusProgressValue(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->getStatusBarProgress(); }

bool ak::uiAPI::window::getStatusProgressContinuous(
	UID												_windowUID
) { return object::get<aWindowManager>(_windowUID)->getStatusBarContinuous(); }

void ak::uiAPI::window::addDock(
	UID												_windowUID,
	UID												_dockUid,
	dockLocation									_dockLocation
) { object::get<aWindowManager>(_windowUID)->addDock(object::get<aDockWidget>(_dockUid), _dockLocation); }

void ak::uiAPI::window::tabifyDock(
	UID												_windowUID,
	UID												_parentUid,
	UID												_dockUid
) { object::get<aWindowManager>(_windowUID)->tabifyDock(object::get<aDockWidget>(_parentUid), object::get<aDockWidget>(_dockUid)); }

void ak::uiAPI::window::setDockBottomLeftPriority(
	UID												_windowUID,
	dockLocation									_dockLocation
) { object::get<aWindowManager>(_windowUID)->setDockPriorityBottomLeft(_dockLocation); }

void ak::uiAPI::window::setDockBottomRightPriority(
	UID												_windowUID,
	dockLocation									_dockLocation
) { object::get<aWindowManager>(_windowUID)->setDockPriorityBottomRight(_dockLocation); }

void ak::uiAPI::window::setCentralWidget(
	UID												_windowUID,
	UID												_widgetUID
) { object::get<aWindowManager>(_windowUID)->setCentralWidget(getWidget(_widgetUID)); }

void ak::uiAPI::window::setCentralWidget(
	UID												_windowUID,
	QWidget *											_widget
) { object::get<aWindowManager>(_windowUID)->setCentralWidget(_widget); }

void ak::uiAPI::window::showMaximized(
	UID												_windowUID
) { object::get<aWindowManager>(_windowUID)->showMaximized(); }

void ak::uiAPI::window::showMinimized(
	UID												_windowUID
) { object::get<aWindowManager>(_windowUID)->showMaximized(); }

QSize ak::uiAPI::window::size(
	UID												_windowUID
) {	return object::get<aWindowManager>(_windowUID)->window()->size(); }

void ak::uiAPI::window::setWaitingAnimationVisible(
	UID												_windowUID,
	bool												_visible
) { object::get<aWindowManager>(_windowUID)->setWaitingAnimationVisible(_visible); }

void ak::uiAPI::window::setWaitingAnimation(
	UID												_windowUID,
	QMovie *											_movie
) { object::get<aWindowManager>(_windowUID)->setWaitingAnimation(_movie); }

void ak::uiAPI::window::setWaitingAnimation(
	UID												_windowUID,
	const QString &										_animationName
) {
	assert(my_iconManager != nullptr); // API not initialized
	object::get<aWindowManager>(_windowUID)->setWaitingAnimation(my_iconManager->movie(_animationName));
 }

void ak::uiAPI::window::setCentralWidgetMinimumSize(
	UID												_windowUID,
	int													_width,
	int													_height
) { object::get<aWindowManager>(_windowUID)->setCentralWidgetMinimumSize(QSize(_width, _height)); }

int ak::uiAPI::window::devicePixelRatio(void) { return my_apiManager.desktop()->devicePixelRatio(); }

// ###############################################################################################################################################

// Crypto

QString ak::uiAPI::crypto::hash(
	const QString &			_orig,
	HashAlgorithm			_algorithm
) { return hashString(_orig, _algorithm); }

// ###############################################################################################################################################

void ak::uiAPI::creatorDestroyed(
	UID												_creatorUid
) {
	assert(my_objManager != nullptr); // API not initialized
	my_objManager->creatorDestroyed(_creatorUid);
}

void ak::uiAPI::addColorStyle(
	aColorStyle *								_colorStyle,
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

const ak::aColorStyle *  ak::uiAPI::getCurrentColorStyle(void) {
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

ak::aMessenger *ak::uiAPI::getMessenger(void) { return my_messenger; }

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
	eventType									_type
) {
	return ak::toQString(_type);
}

QString ak::uiAPI::special::toString(
	valueType									_type
) {
	return ak::toQString(_type);
}

QString ak::uiAPI::special::toString(
	textAlignment							_type
) {
	return ak::toQString(_type);
}

QString ak::uiAPI::special::toString(
	dockLocation							_dockLocation
) {
	return ak::toQString(_dockLocation);
}

QString ak::uiAPI::special::toString(
	tabLocation							_tabLocation
) {
	return ak::toQString(_tabLocation);
}

QString ak::uiAPI::special::toQString(
	dialogResult							_dialogResult
) {
	return ak::toQString(_dialogResult);
}

QString ak::uiAPI::special::toString(
	keyType								_type
) {
	return ak::toQString(_type);
}

QString ak::uiAPI::special::toString(
	objectType							_type
) {
	return ak::toQString(_type);
}

QString ak::uiAPI::special::toString(
	settingsRestoreErrorCode					_settingsRestoreErrorCode
) {
	return ak::toQString(_settingsRestoreErrorCode);
}

QString ak::uiAPI::special::toEventText(
	UID												_senderUID,
	eventType									_eventType,
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
