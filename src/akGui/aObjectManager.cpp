/*
 *	File:		aObjectManager.cpp
 *	Package:	akGui
 *
 *  Created on: July 14, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// C++ header
#include <string>

// AK Core header
#include <akCore/akCore.h>
#include <akCore/aMessenger.h>
#include <akCore/aNotifierObjectManager.h>
#include <akCore/aObject.h>
#include <akCore/rJSON.h>
#include <akCore/aUidMangager.h>

// AK Dialogs
#include <akDialogs/aLogInDialog.h>
#include <akDialogs/aOptionsDialog.h>
#include <akDialogs/aPromptDialog.h>

// AK GUI header
#include <akGui/aAction.h>
#include <akGui/aColorStyleDefault.h>
#include <akGui/aColorStyleDefaultDark.h>
#include <akGui/aContextMenuItem.h>
#include <akGui/aDialog.h>
#include <akGui/aObjectManager.h>
#include <akGui/aPaintable.h>
#include <akGui/aRestorable.h>
#include <akGui/aSignalLinker.h>
#include <akGui/aSpecialTabBar.h>
#include <akGui/aTimer.h>

// AK Widgets header
#include <akWidgets/aCheckBoxWidget.h>
#include <akWidgets/aColorEditButtonWidget.h>
#include <akWidgets/aColorStyleSwitchWidget.h>
#include <akWidgets/aComboBoxWidget.h>
#include <akWidgets/aComboButtonWidget.h>
#include <akWidgets/aDockWidget.h>
#include <akWidgets/aDockWatcherWidget.h>
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
#include <akWidgets/aWindowManager.h>

#define COLORSTYLE_INDEX_DEFAULT 0
#define COLORSTYLE_INDEX_DARK 1

#define SETTING_VERSION_WINDOWSTATE "Version-WindowState"
#define SETTING_VERSION_APPLICATION "Version-Application"
#define CONFIG_VERSION_WINDOWSTATE "1.0"

ak::aObjectManager::aObjectManager(
	aMessenger *										_messenger,
	aUidManager *									_uidManager
)
	: m_currentColorStyle(nullptr),
	m_messenger(nullptr),
	m_signalLinker(nullptr),
	m_uidManager(nullptr),
	m_notifier(nullptr)
{
	// Check parameter
	assert(_messenger != nullptr); // nullptr provided
	assert(_uidManager != nullptr); // nullptr provided

	// Apply settings
	m_messenger = _messenger;
	m_uidManager = _uidManager;

	// Create signal linker
	m_signalLinker = new aSignalLinker(m_messenger, m_uidManager);

	// Create notifier
	m_notifier = new aNotifierObjectManager(this);

	m_currentColorStyle = new aColorStyleDefault();
	m_colorStyles.push_back(m_currentColorStyle);
	m_colorStyles.push_back(new aColorStyleDefaultDark);
}

ak::aObjectManager::~aObjectManager() {}

// ###############################################################################################################################################

// Object creation

ak::UID ak::aObjectManager::createAction(
	UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon,
	QToolButton::ToolButtonPopupMode					_popupMode
) {
	// Create object
	aAction * obj = new aAction(_icon, _text, _popupMode);
	// Set parameter
	obj->setToolTip("");
	obj->setWhatsThis("");
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createCheckBox(
	UID												_creatorUid,
	const QString &										_text,
	bool												_checked
) {
	// Create object
	aCheckBoxWidget * obj = new aCheckBoxWidget(_text);
	// Set parameter
	obj->setChecked(_checked);
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createColorEditButton(
	UID												_creatorUid,
	const aColor &									_color,
	const QString &										_textOverride
) {
	// Create object
	aColorEditButtonWidget * obj = new aColorEditButtonWidget(_color, _textOverride);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createColorStyleSwitch(
	UID						_creatorUid,
	const QString &			_brightModeTitle,
	const QString &			_darkModeTitle,
	const QIcon &			_brightModeIcon,
	const QIcon &			_darkModeIcon,
	bool					_isBright
) {
	// Create object
	aColorStyleSwitchWidget * obj = new aColorStyleSwitchWidget(_brightModeTitle, _darkModeTitle, _brightModeIcon, _darkModeIcon, _isBright);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createComboBox(
	UID												_creatorUid
) {
	// Create object
	aComboBoxWidget * obj = new aComboBoxWidget();
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createComboButton(
	UID												_creatorUid,
	const QString &										_initialText,
	const std::vector<QString> &						_possibleSelection
) {
	// Create object
	aComboButtonWidget * obj = new aComboButtonWidget(_initialText, m_currentColorStyle);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	obj->setItems(_possibleSelection);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createComboButtonItem(
	UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	// Create object
	aComboButtonWidgetItem * obj = new aComboButtonWidgetItem(_icon, _text);
	// Set parameter
	obj->setUid(m_uidManager->getId());
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createDock(
	UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	aDockWidget * obj = new aDockWidget(_text);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createDockWatcher(
	UID									_creatorUid,
	const QString &						_text
) {
	// Create object
	aDockWatcherWidget * obj = new aDockWatcherWidget(_text);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createDockWatcher(
	UID									_creatorUid,
	const QIcon &						_icon,
	const QString &						_text
) {
	// Create object
	aDockWatcherWidget * obj = new aDockWatcherWidget(_icon, _text);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createLineEdit(
	UID													_creatorUid,
	const QString &											_initialText
) {
	// Create object
	aLineEditWidget * obj = new aLineEditWidget(_initialText);
	// Connect to signal linker
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createLogInDialog(
	UID												_creatorUid,
	bool												_showSavePassword,
	const QPixmap &										_backgroundImage,
	const QString &										_username,
	const QString &										_password
) {
	// Create object
	aLogInDialog * obj = new aLogInDialog(_showSavePassword, _backgroundImage, _username, _password);
	// Connect to signal linker
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }

	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createNiceLineEdit(
	UID												_creatorUid,
	const QString &										_initialText,
	const QString &										_infoLabelText
) {
	// Create object
	aNiceLineEditWidget * obj = new aNiceLineEditWidget(_initialText, _infoLabelText);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	obj->setUid(m_uidManager->getId());
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createPropertyGrid(
	UID												_creatorUid
) {
	// Create object
	aPropertyGridWidget * obj = new aPropertyGridWidget;
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createPushButton(
	UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	aPushButtonWidget * obj = new aPushButtonWidget(_text);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createPushButton(
	UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	// Create object
	aPushButtonWidget * obj = new aPushButtonWidget(_icon, _text);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createOptionsDialog(
	UID												_creatorUid,
	const QString &										_title
) {
	// Create object
	aOptionsDialog * obj = new aOptionsDialog{ _title };
	obj->setUid(m_uidManager->getId());
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	return obj->uid();
}

ak::UID ak::aObjectManager::createSpecialTabBar(
	UID												_creatorUid
) {
	// Create object
	aSpecialTabBar * obj = new aSpecialTabBar;
	obj->setUid(m_uidManager->getId());
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTable(
	UID												_creatorUid,
	int													_rows,
	int													_columns
) {
	// Create object
	aTableWidget * obj = new aTableWidget(_rows, _columns);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTextEdit(
	UID												_creatorUid,
	const QString &										_initialText
) {
	// Create object
	aTextEditWidget * obj = new aTextEditWidget();
	obj->setPlainText(_initialText);
	// Set parameter
	m_signalLinker->addLink(obj);
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTimer(
	UID												_creatorUid
) {
	// Create object
	aTimer * obj = new aTimer();
	// Set parameter
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createToolButton(
	UID												_creatorUid
) {
	// Create object
	aToolButtonWidget * obj = new aToolButtonWidget();
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createToolButton(
	UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	aToolButtonWidget * obj = new aToolButtonWidget(_text);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createToolButton(
	UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	// Create object
	aToolButtonWidget * obj = new aToolButtonWidget(_icon, _text);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTree(
	UID												_creatorUid
) {
	// Create object
	aTreeWidget * obj = new aTreeWidget(m_currentColorStyle);
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTabView(
	UID												_creatorUid
) {
	// Create object
	aTabWidget * obj = new aTabWidget;
	// Set parameter
	if (m_currentColorStyle != nullptr) { obj->setColorStyle(m_currentColorStyle); }
	m_signalLinker->addLink(obj);
	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::aObjectManager::createTabToolBarSubContainer(
	UID												_creatorUid,
	UID												_parentUid,
	const QString &										_text
) {
	// Find parent object
	auto itm = m_mapObjects.find(_parentUid);
	assert(itm != m_mapObjects.end()); // Invalid object UID provided
	// Check object type
	if (itm->second->type() != otTabToolbarPage &&
		itm->second->type() != otTabToolbarGroup &&
		itm->second->type() != otTabToolbarSubgroup
		) {
		// Check if the provided object is located at a main window
		if (itm->second->type() == otMainWindow) {
			return createTabToolBarPage(_creatorUid, _parentUid, _text);
		}
		else {
			assert(0); // Invalid object type
			return invalidUID;
		}
	}
	// Cast container
	aTtbContainer * cont = nullptr;
	cont = dynamic_cast<aTtbContainer *>(itm->second);
	assert(cont != nullptr); // Cast failed

	aTtbContainer * sub = cont->getSubContainer(_text);

	if (sub == nullptr) {
		// Create subcontainer
		sub = cont->createSubContainer(_text);
		assert(sub != nullptr); // Invalid information received
		sub->setUid(m_uidManager->getId());
		// Store data
		if (m_currentColorStyle != nullptr) { sub->setColorStyle(m_currentColorStyle); }
		m_mapObjects.insert_or_assign(sub->uid(), sub);
		addCreatedUid(_creatorUid, sub->uid());

		sub->setParentObject(cont);
		cont->addChildObject(sub);
	}
	return sub->uid();
}

ak::UID ak::aObjectManager::createTabToolBarPage(
	UID												_creatorUid,
	UID												_uiManagerUid,
	const QString &										_text
) {
	// Find object
	auto itm = m_mapObjects.find(_uiManagerUid);
	assert(itm != m_mapObjects.end()); // Invalid object UID provided
	// Check object type
	assert(itm->second->type() == otMainWindow); // Invalid object type provided
	// Cast ui
	aWindowManager * ui = nullptr;
	ui = dynamic_cast<aWindowManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	aTtbContainer * cont = ui->getTabToolBarSubContainer(_text);
	if (cont == nullptr) {
		// Create container
		cont = ui->createTabToolbarSubContainer(_text);
		assert(cont != nullptr); // Invalid information received
		cont->setUid(m_uidManager->getId());
		// Store data
		if (m_currentColorStyle != nullptr) { cont->setColorStyle(m_currentColorStyle); }
		m_mapObjects.insert_or_assign(cont->uid(), cont);
		addCreatedUid(_creatorUid, cont->uid());

		cont->setParentObject(ui);
		ui->addChildObject(cont);
	}
	return cont->uid();
}

/*
ak::UID ak::aObjectManager::createWelcomeScreen(
	UID												_creatorUid
) {
	// Create object
	ak::widget::welcomeScreen * obj = new ak::widget::welcomeScreen(m_messenger, m_uidManager, m_currentColorStyle);

	// Set parameter
	obj->setUid(m_uidManager->getId());

	// Store data
	m_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}
*/

ak::UID ak::aObjectManager::createWindow(
	UID												_creatorUid
) {
	// Create new ui manager
	aWindowManager * ui = nullptr;
	ui = new aWindowManager(m_messenger, m_uidManager, m_currentColorStyle);
	// Store data
	m_mapObjects.insert_or_assign(ui->uid(), ui);
	addCreatedUid(_creatorUid, ui->uid());
	return ui->uid();
}

// ###############################################################################################################################################

ak::aObject * ak::aObjectManager::object(
	UID												_objectUid
) {
	auto itm = m_mapObjects.find(_objectUid);
	assert(itm != m_mapObjects.end());	// Invalid object UID provided
	return itm->second;
}

ak::aObject * ak::aObjectManager::object(
	const QString &										_objectUniqueName
) {
	auto itm = m_mapUniqueNames.find(_objectUniqueName);
	if (itm == m_mapUniqueNames.end()) return nullptr; // Invalid object UID provided
	return itm->second;
}

// ###############################################################################################################################################

void ak::aObjectManager::creatorDestroyed(
	UID												_creatorUid
) {
	try {
		m_notifier->disable();
		// Get all UIDs created by this creator
		auto itm = m_mapCreators.find(_creatorUid);
		if (itm != m_mapCreators.end()) {

			bool erased{ true };
			while (erased) {
				erased = false;

				// Get and destroy every single object created by this creator
				for (int i = 0; i < itm->second->size() && !erased; i++) {
					destroy(itm->second->at(i), true);
					if (!objectExists(itm->second->at(i))) {
						itm->second->erase(itm->second->begin() + i);
						erased = true;
						break;
					}
				}

			}

			// Delete vector
			std::vector<UID> *	r = itm->second;
			delete r;
			itm->second = nullptr;
			// Remove creator UID
			m_mapCreators.erase(_creatorUid);
		}
		m_notifier->enable();
	}
	catch (const std::exception & e) {
		assert(0);	// Something went wrong
	}
	catch (...) {
		assert(0);	// Something went horribly wrong
	}
}

void ak::aObjectManager::destroy(
	UID												_objectUID,
	bool												_ignoreIfObjectHasChildObjects
) {
	m_notifier->disable();

	auto object = m_mapObjects.find(_objectUID);
	assert(object != m_mapObjects.end());	// Invalid object UID

	if (object != m_mapObjects.end()) // Avoid a UI crash if something goes wrong here
	{
		aObject * actualObject = object->second;

		if (_ignoreIfObjectHasChildObjects) {
			if (actualObject->childObjectCount() != 0) { return; }
			switch (actualObject->type())
			{
			case otTabToolbarPage:
			case otTabToolbarGroup:
			case otTabToolbarSubgroup:
			{
				aTtbContainer * container = nullptr;
				container = dynamic_cast<aTtbContainer *>(actualObject);
				assert(container != nullptr); // Cast failed
				if (container->subContainerCount() != 0) { return; }
			}
			break;
			}
		}

		m_mapAliases.erase(actualObject->alias());
		m_mapOwners.erase(_objectUID);
		m_mapObjects.erase(_objectUID);

		// Remove the unique name access to this object
		if (!actualObject->uniqueName().isEmpty()) {
			m_mapUniqueNames.erase(actualObject->uniqueName());
		}

		// Destroy object
		delete actualObject;
	}

	m_notifier->enable();
}

void ak::aObjectManager::setIconSearchDirectories(
	const std::vector<QString>							_paths
) {
	m_iconSearchPaths = _paths;
	for (auto itm : m_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		itm->setDirectories(m_iconSearchPaths);
	}
}

void ak::aObjectManager::addColorStyle(
	aColorStyle *								_colorStyle,
	bool												_activate
) {
	assert(_colorStyle != nullptr); // Nullptr provided
	for (auto itm : m_colorStyles) {
		if (itm->getColorStyleName() == _colorStyle->getColorStyleName()) {
			assert(0); // ColorStyle with the specified name already exists
			return;
		}
	}
	m_colorStyles.push_back(_colorStyle);
	if (_activate) { setColorStyle(_colorStyle); }
}

void ak::aObjectManager::setColorStyle(
	const QString &										_colorStyleName
) {
	for (auto itm : m_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		if (itm->getColorStyleName() == _colorStyleName) {
			setColorStyle(itm);
			return;
		}
	}
}

void ak::aObjectManager::setDefaultColorStyle(void) {
	setColorStyle(m_colorStyles[COLORSTYLE_INDEX_DEFAULT]);
}

void ak::aObjectManager::setDefaultDarkColorStyle(void) {
	setColorStyle(m_colorStyles[COLORSTYLE_INDEX_DARK]);
}

ak::aColorStyle * ak::aObjectManager::getCurrentColorStyle(void) const { return m_currentColorStyle; }

QString ak::aObjectManager::getCurrentColorStyleName(void) const {
	if (m_currentColorStyle == nullptr) { return QString(); }
	return m_currentColorStyle->getColorStyleName();
}

void ak::aObjectManager::destroyAll(void) {
	if (m_signalLinker != nullptr) { delete m_signalLinker; m_signalLinker = nullptr; }
	m_signalLinker = new aSignalLinker(m_messenger, m_uidManager);

	auto itm = m_mapObjects.begin();

	// Destroy all objects that are not a main window
	while (itm != m_mapObjects.end()) {
		ak::aObject * obj = itm->second;
		if (obj->type() != ak::otMainWindow) {
			// Erase item
			m_mapObjects.erase(obj->uid());
			// Destroy all objects that were created by this item
			creatorDestroyed(obj->uid());
			delete obj;
			itm = m_mapObjects.begin();
		}
		else {
			creatorDestroyed(obj->uid()); itm++;
		}
	}
	for (itm = m_mapObjects.begin(); itm != m_mapObjects.end(); itm++) {
		ak::aObject * obj = itm->second;
		if (obj != nullptr) { delete obj; }
	}
	m_mapCreators.clear();
	m_mapObjects.clear();
	m_mapUniqueNames.clear();
}

std::string ak::aObjectManager::saveStateWindow(
	const std::string &									_applicationVersion
) {
	// Prepare document
	AK_rJSON_createDOC(doc);
	ak::rJSON::add(doc, SETTING_VERSION_APPLICATION, _applicationVersion);
	ak::rJSON::add(doc, SETTING_VERSION_WINDOWSTATE, CONFIG_VERSION_WINDOWSTATE);

	AK_rJSON_createValueArray(items);
	for (auto itm = m_mapAliases.begin(); itm != m_mapAliases.end(); itm++) {
		auto obj = m_mapObjects.find(itm->second);
		ak::aRestorable * restorable = nullptr;
		restorable = dynamic_cast<ak::aRestorable *>(obj->second);
		assert(restorable != nullptr); // Upps
		restorable->addObjectSettingsToValue(items, doc.GetAllocator());
	}

	ak::rJSON::add(doc, RESTORABLE_UI_SETTINGS, items);
	return ak::rJSON::toJSON(doc);
}

std::string ak::aObjectManager::saveStateColorStyle(
	const std::string &									_applicationVersion
) {
	// Prepare document
	AK_rJSON_createDOC(doc);
	ak::rJSON::add(doc, SETTING_VERSION_APPLICATION, _applicationVersion);
	ak::rJSON::add(doc, SETTING_VERSION_WINDOWSTATE, CONFIG_VERSION_WINDOWSTATE);

	AK_rJSON_createValueArray(items);
	if (m_currentColorStyle != nullptr) {
		AK_rJSON_createValueObject(colorSettings);
		ak::rJSON::add(doc, colorSettings, RESTORABLE_UI_COLORSTYLE, getCurrentColorStyleName().toStdString());
		items.PushBack(colorSettings, doc.GetAllocator());
	}

	ak::rJSON::add(doc, RESTORABLE_UI_SETTINGS, items);
	return ak::rJSON::toJSON(doc);
}

ak::settingsRestoreErrorCode ak::aObjectManager::restoreStateWindow(
	const char *										_json,
	const std::string &									_applicationVersion
) {
	rapidjson::Document doc;
	doc.Parse(_json);

	// Check version
	if (!doc.HasMember(SETTING_VERSION_APPLICATION) ||
		!doc.HasMember(SETTING_VERSION_WINDOWSTATE)) {
		return ak::settingsRestoreErrorCode::srecSettingsVersionMismatch;
	}
	assert(doc[SETTING_VERSION_APPLICATION].IsString());
	assert(doc[SETTING_VERSION_WINDOWSTATE].IsString());
	std::string settingVersionApp(doc[SETTING_VERSION_APPLICATION].GetString());
	std::string settingVersionWindowState(doc[SETTING_VERSION_WINDOWSTATE].GetString());
	if (_applicationVersion != settingVersionApp) { return ak::settingsRestoreErrorCode::srecAppVersionMismatch; }
	if (CONFIG_VERSION_WINDOWSTATE != settingVersionWindowState) { return ak::settingsRestoreErrorCode::srecSettingsVersionMismatch; }

	// Check document
	assert(doc.HasMember(RESTORABLE_UI_SETTINGS));	// Member missing
	assert(doc[RESTORABLE_UI_SETTINGS].IsArray());	// Invalid type
	rapidjson::Value UIsettings = doc[RESTORABLE_UI_SETTINGS].GetArray();

	// Go trough all items in the array
	for (rapidjson::Value::ConstValueIterator itm = UIsettings.Begin(); itm != UIsettings.End(); itm++) {
		const rapidjson::Value & obj = *itm;
		assert(obj.IsObject());									// Stored item is not an object
		assert(obj.HasMember(RESTORABLE_NAME_ALIAS));			// Does not contain name
		assert(obj.HasMember(RESTORABLE_NAME_TYPE));			// Does not contain type
		assert(obj.HasMember(RESTORABLE_NAME_SETTINGS));		// Does not contain settings
		assert(obj[RESTORABLE_NAME_ALIAS].IsString());			// Alias is not a string
		assert(obj[RESTORABLE_NAME_TYPE].IsString());			// Type is not a string
		assert(obj[RESTORABLE_NAME_SETTINGS].IsObject());		// Settins is not an object

		QString objAlias = QString(obj[RESTORABLE_NAME_ALIAS].GetString());
		QString objType = QString(obj[RESTORABLE_NAME_TYPE].GetString());

		// Check if an object with the provided alias exist
		auto oAlias = m_mapAliases.find(objAlias);
		assert(oAlias != m_mapAliases.end()); // No object with specified alias exists

		// Get the object
		auto object = m_mapObjects.find(oAlias->second);
		assert(object != m_mapObjects.end()); // Registered alias is not stored

		// Check the object type
		assert(ak::toQString(object->second->type()) == objType); // Current object type differs from saved object type

		// Check object
		ak::aRestorable * restorable = nullptr;
		restorable = dynamic_cast<ak::aRestorable *>(object->second);
		assert(restorable != nullptr); // Cast failed

		// Apply the settings
		const rapidjson::Value & settings = obj[RESTORABLE_NAME_SETTINGS];
		assert(settings.IsObject()); // Not a setting
		restorable->restoreSettings(settings);

	}
	return ak::settingsRestoreErrorCode::srecNone;
}

ak::settingsRestoreErrorCode ak::aObjectManager::restoreStateColorStyle(
	const char *										_json,
	const std::string &									_applicationVersion
) {
	rapidjson::Document doc;
	doc.Parse(_json);

	// Check version
	if (!doc.HasMember(SETTING_VERSION_APPLICATION) ||
		!doc.HasMember(SETTING_VERSION_WINDOWSTATE)) {
		return ak::settingsRestoreErrorCode::srecSettingsVersionMismatch;
	}
	assert(doc[SETTING_VERSION_APPLICATION].IsString());
	assert(doc[SETTING_VERSION_WINDOWSTATE].IsString());
	std::string settingVersionApp(doc[SETTING_VERSION_APPLICATION].GetString());
	std::string settingVersionWindowState(doc[SETTING_VERSION_WINDOWSTATE].GetString());
	if (_applicationVersion != settingVersionApp) { return ak::settingsRestoreErrorCode::srecAppVersionMismatch; }
	if (CONFIG_VERSION_WINDOWSTATE != settingVersionWindowState) { return ak::settingsRestoreErrorCode::srecSettingsVersionMismatch; }

	// Check document
	assert(doc.HasMember(RESTORABLE_UI_SETTINGS));	// Member missing
	assert(doc[RESTORABLE_UI_SETTINGS].IsArray());	// Invalid type
	rapidjson::Value UIsettings = doc[RESTORABLE_UI_SETTINGS].GetArray();

	// Go trough all items in the array
	for (rapidjson::Value::ConstValueIterator itm = UIsettings.Begin(); itm != UIsettings.End(); itm++) {
		const rapidjson::Value & obj = *itm;
		assert(obj.IsObject());									// Stored item is not an object
		if (obj.HasMember(RESTORABLE_UI_COLORSTYLE)) {
			assert(obj[RESTORABLE_UI_COLORSTYLE].IsString());
			QString colorStyleName = obj[RESTORABLE_UI_COLORSTYLE].GetString();
			setColorStyle(colorStyleName);
		}
	}
	return ak::settingsRestoreErrorCode::srecNone;
}

void ak::aObjectManager::addAlias(
	const QString &										_alias,
	UID												_UID
) {
	auto obj = m_mapAliases.find(_alias);
	assert(obj == m_mapAliases.end()); // Object with the specified alias already exists
	m_mapAliases.insert_or_assign(_alias, _UID);
}

void ak::aObjectManager::setObjectUniqueName(
	UID												_objectUid,
	const QString &										_uniqueName
) {
	auto actualObject{ object(_objectUid) };
	assert(actualObject != nullptr);	// The object was not created

	auto duplicateEntry{ m_mapUniqueNames.find(_uniqueName) };
	if (duplicateEntry != m_mapUniqueNames.end()) {
		if (duplicateEntry->second == actualObject) { return; }
		assert(0); // Another object with the same unique name does already exist
	}

	// Remove the old unique name
	if (!actualObject->uniqueName().isEmpty()) { m_mapUniqueNames.erase(actualObject->uniqueName()); }
	
	// Store information
	actualObject->setUniqueName(_uniqueName);
	if (!_uniqueName.isEmpty()) {
		m_mapUniqueNames.insert_or_assign(_uniqueName, actualObject);
	}
}

void ak::aObjectManager::removeAlias(
	const QString &										_alias
) { m_mapAliases.erase(_alias); }

ak::UID ak::aObjectManager::objectCreator(
	UID												_objectUID
) {
	auto owner = m_mapOwners.find(_objectUID);
	assert(owner != m_mapOwners.end());	// Invalid UID
	return owner->second;
}

bool ak::aObjectManager::objectExists(
	UID												_objectUID
) {
	auto itm{ m_mapObjects.find(_objectUID) };
	return !(itm == m_mapObjects.end());
}

void ak::aObjectManager::addPaintable(aPaintable * _object) {
	m_externalPaintableObjects.insert_or_assign(_object, false);
}

void ak::aObjectManager::removePaintable(aPaintable * _object) {
	m_externalPaintableObjects.erase(_object);
}

// ###############################################################################################################################################

void ak::aObjectManager::setColorStyle(
	aColorStyle *									_colorStyle
) {
	assert(_colorStyle != nullptr); // Nullptr provided
	m_currentColorStyle = _colorStyle;

	for (auto obj : m_mapObjects) {
		assert(obj.second != nullptr); // nullptr stored
		if (obj.second->isPaintableType()) {
			// Cast paintable
			ak::aPaintable * itm = nullptr;
			itm = dynamic_cast<ak::aPaintable *>(obj.second);
			assert(itm != nullptr); // Cast failed
			itm->setColorStyle(m_currentColorStyle);
		}
	}

	for (auto obj : m_externalPaintableObjects) {
		obj.first->setColorStyle(_colorStyle);
	}
}

void ak::aObjectManager::addCreatedUid(
	UID												_creatorUid,
	UID												_createdUid
) {
	auto itm = m_mapCreators.find(_creatorUid);
	if (itm == m_mapCreators.end()) {
		// First object created by this creator
		std::vector<UID> *	v = nullptr;
		v = new std::vector<UID>;

		// Store data
		v->push_back(_createdUid);
		m_mapCreators.insert_or_assign(_creatorUid, v);
	}
	else {
		// Object(s) already created by this creator before

		// Check for duplicate
		for (int i = 0; i < itm->second->size(); i++) {
			assert(itm->second->at(i) != _createdUid); // UID already stored
		}
		// Store data
		itm->second->push_back(_createdUid);
	}

	// Store owner information
	assert(m_mapOwners.find(_createdUid) == m_mapOwners.end());	// Created UID already stored
	m_mapOwners.insert_or_assign(_createdUid, _creatorUid);

}

QWidget * ak::aObjectManager::castToWidget(
	UID												_objectUid
) {
	// Get widget
	auto obj = m_mapObjects.find(_objectUid);
	assert(obj != m_mapObjects.end()); // Invalid object UID
	assert(obj->second->isWidgetType()); // Object is not a widget
	ak::aWidget * widget = nullptr;
	widget = dynamic_cast<ak::aWidget *>(obj->second);
	assert(widget != nullptr); // Cast failed
	return widget->widget();
}
