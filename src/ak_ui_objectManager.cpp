/*
 * ak_ui_objectManager.cpp
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

 // AK header
#include <ak_ui_objectManager.h>			// corresponding header
#include <ak_ui_core.h>						// object Type
#include <ak_messenger.h>					// messenger
#include <ak_notifierObjectManager.h>		// notifierObjectManager
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_core_aWidget.h>
#include <ak_ui_core_aObject.h>
#include <ak_ui_core_aRestorable.h>
#include <ak_ui_core_aDialog.h>
#include <ak_ui_colorStyleDefault.h>
#include <ak_ui_colorStyleDefaultDark.h>

// AK dialogs
#include <ak_ui_dialog_logIn.h>
#include <ak_ui_dialog_options.h>

// AK Qt objects
#include <ak_ui_qt_action.h>				// action
#include <ak_ui_qt_checkBox.h>				// checkBox
#include <ak_ui_qt_colorEditButton.h>		// colorEditButton
#include <ak_ui_qt_comboBox.h>				// comboBox
#include <ak_ui_qt_comboButton.h>			// comboButton
#include <ak_ui_qt_dock.h>					// dock
#include <ak_ui_qt_propertyGrid.h>			// propertyGrid
#include <ak_ui_qt_pushButton.h>			// pushButton
#include <ak_ui_qt_specialTabBar.h>			// special TabBar
#include <ak_ui_qt_table.h>					// table
#include <ak_ui_qt_tabView.h>				// tabView
#include <ak_ui_qt_textEdit.h>				// textEdit
#include <ak_ui_qt_timer.h>					// timer
#include <ak_ui_qt_toolButton.h>			// toolButton
#include <ak_ui_qt_lineEdit.h>				// lineEdit
#include <ak_ui_qt_niceLineEdit.h>			// niceLineEdit

// AK widget objects
#include <ak_ui_widget_welcomeScreen.h>
#include <ak_ui_qt_tree.h>				// tree

// AK ttb objects
#include <ak_ui_ttb_group.h>				// ttb Group
#include <ak_ui_ttb_page.h>					// ttb Page
#include <ak_ui_ttb_subgroup.h>				// ttb Subroup

// Rapid JSON header
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#define COLORSTYLE_INDEX_DEFAULT 0
#define COLORSTYLE_INDEX_DARK 1

#define SETTING_VERSION_WINDOWSTATE "Version-WindowState"
#define SETTING_VERSION_APPLICATION "Version-Application"
#define CONFIG_VERSION_WINDOWSTATE "1.0"

ak::ui::objectManager::objectManager(
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager
)
	: my_currentColorStyle(nullptr),
	my_messenger(nullptr),
	my_signalLinker(nullptr),
	my_uidManager(nullptr),
	my_notifier(nullptr)
{
	// Check parameter
	assert(_messenger != nullptr); // nullptr provided
	assert(_uidManager != nullptr); // nullptr provided

	// Apply settings
	my_messenger = _messenger;
	my_uidManager = _uidManager;

	// Create signal linker
	my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);

	// Create notifier
	my_notifier = new ak::notifierObjectManager(this);

	my_currentColorStyle = new ui::colorStyleDefault();
	my_colorStyles.push_back(my_currentColorStyle);
	ui::colorStyle * cS = new ui::colorStyleDefaultDark();
	my_colorStyles.push_back(cS);
}

ak::ui::objectManager::~objectManager() {}

// ###############################################################################################################################################

// Object creation

ak::UID ak::ui::objectManager::createAction(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon,
	QToolButton::ToolButtonPopupMode					_popupMode
) {
	// Create object
	ak::ui::qt::action * obj = new ak::ui::qt::action(_icon, _text, _popupMode);
	// Set parameter
	obj->setToolTip("");
	obj->setWhatsThis("");
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createCheckBox(
	ak::UID												_creatorUid,
	const QString &										_text,
	bool												_checked
) {
	// Create object
	ak::ui::qt::checkBox * obj = new ak::ui::qt::checkBox(_text);
	// Set parameter
	obj->setChecked(_checked);
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createColorEditButton(
	ak::UID												_creatorUid,
	const ak::ui::color &								_color,
	const QString &										_textOverride
) {
	// Create object
	qt::colorEditButton * obj = new qt::colorEditButton(_color, _textOverride);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createComboBox(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::qt::comboBox * obj = new ak::ui::qt::comboBox();
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createComboButton(
	ak::UID												_creatorUid,
	const QString &										_initialText,
	const std::vector<QString> &						_possibleSelection
) {
	// Create object
	ak::ui::qt::comboButton * obj = new ak::ui::qt::comboButton(_initialText, my_currentColorStyle);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	obj->setItems(_possibleSelection);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createComboButtonItem(
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::comboButtonItem * obj = new ak::ui::qt::comboButtonItem(_icon, _text);
	// Set parameter
	obj->setUid(my_uidManager->getId());
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createDock(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::dock * obj = new ak::ui::qt::dock(_text);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createLineEdit(
	ak::UID													_creatorUid,
	const QString &											_initialText
) {
	// Create object
	ak::ui::qt::lineEdit * obj = new ak::ui::qt::lineEdit(_initialText);
	// Connect to signal linker
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createLogInDialog(
	ak::UID												_creatorUid,
	bool												_showSavePassword,
	const QPixmap &										_backgroundImage,
	const QString &										_username,
	const QString &										_password
) {
	// Create object
	ak::ui::dialog::logIn * obj = new ak::ui::dialog::logIn(my_messenger, _showSavePassword, _backgroundImage, _username, _password);
	assert(obj != nullptr);

	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	obj->setUid(my_uidManager->getId());

	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
	QWidget *d;
}

ak::UID ak::ui::objectManager::createNiceLineEdit(
	ak::UID												_creatorUid,
	const QString &										_initialText,
	const QString &										_infoLabelText
) {
	// Create object
	qt::niceLineEdit * obj = new qt::niceLineEdit(_initialText, _infoLabelText);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	obj->setUid(my_uidManager->getId());
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createPropertyGrid(
	ak::UID												_creatorUid
) {
	// Create object
	qt::propertyGrid * obj = new qt::propertyGrid;
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createPushButton(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::pushButton * obj = new ak::ui::qt::pushButton(_text);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createPushButton(
	ak::UID												_creatorUid,
	const QIcon &										_icon,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::pushButton * obj = new ak::ui::qt::pushButton(_icon, _text);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createOptionsDialog(
	ak::UID												_creatorUid,
	const QString &										_title
) {
	// Create object
	dialog::options * obj = new dialog::options{ _title };
	obj->setUid(my_uidManager->getId());
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	return obj->uid();
}

ak::UID ak::ui::objectManager::createSpecialTabBar(
	ak::UID												_creatorUid
) {
	// Create object
	qt::specialTabBar * obj = new qt::specialTabBar;
	obj->setUid(my_uidManager->getId());
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTable(
	ak::UID												_creatorUid,
	int													_rows,
	int													_columns
) {
	// Create object
	ak::ui::qt::table * obj = new ak::ui::qt::table(_rows, _columns);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTextEdit(
	ak::UID												_creatorUid,
	const QString &										_initialText
) {
	// Create object
	ak::ui::qt::textEdit * obj = new ak::ui::qt::textEdit();
	obj->setPlainText(_initialText);
	// Set parameter
	my_signalLinker->addLink(obj);
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTimer(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::qt::timer * obj = new ak::ui::qt::timer();
	// Set parameter
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createToolButton(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::qt::toolButton * obj = new ak::ui::qt::toolButton();
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createToolButton(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::toolButton * obj = new ak::ui::qt::toolButton(_text);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createToolButton(
	ak::UID												_creatorUid,
	const QString &										_text,
	const QIcon &										_icon
) {
	// Create object
	ak::ui::qt::toolButton * obj = new ak::ui::qt::toolButton(_icon, _text);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTree(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::qt::tree * obj = new ak::ui::qt::tree(my_currentColorStyle);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTabView(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::qt::tabView * obj = new ak::ui::qt::tabView;
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createTabToolBarSubContainer(
	ak::UID												_creatorUid,
	ak::UID												_parentUid,
	const QString &										_text
) {
	// Find parent object
	auto itm = my_mapObjects.find(_parentUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	// Check object type
	if (itm->second->objectType() != ak::ui::core::objectType::oTabToolbarPage &&
		itm->second->objectType() != ak::ui::core::objectType::oTabToolbarGroup &&
		itm->second->objectType() != ak::ui::core::objectType::oTabToolbarSubgroup
		) {
		// Check if the provided object is located at a main window
		if (itm->second->objectType() == ak::ui::core::objectType::oMainWindow) {
			return createTabToolBarPage(_creatorUid, _parentUid, _text);
		}
		else {
			assert(0); // Invalid object type
			return ak::invalidUID;
		}
	}
	// Cast container
	ak::ui::core::ttbContainer * cont = nullptr;
	cont = dynamic_cast<ak::ui::core::ttbContainer *>(itm->second);
	assert(cont != nullptr); // Cast failed

	ak::ui::core::ttbContainer * sub = cont->getSubContainer(_text);

	if (sub == nullptr) {
		// Create subcontainer
		sub = cont->createSubContainer(_text);
		assert(sub != nullptr); // Invalid information received
		sub->setUid(my_uidManager->getId());
		// Store data
		if (my_currentColorStyle != nullptr) { sub->setColorStyle(my_currentColorStyle); }
		my_mapObjects.insert_or_assign(sub->uid(), sub);
		addCreatedUid(_creatorUid, sub->uid());

		sub->setParentObject(cont);
		cont->addChildObject(sub);
	}
	return sub->uid();
}

ak::UID ak::ui::objectManager::createTabToolBarPage(
	ak::UID												_creatorUid,
	ak::UID												_uiManagerUid,
	const QString &										_text
) {
	// Find object
	auto itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	// Check object type
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type provided
	// Cast ui
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ak::ui::core::ttbContainer * cont = ui->getTabToolBarSubContainer(_text);
	if (cont == nullptr) {
		// Create container
		cont = ui->createTabToolbarSubContainer(_text);
		assert(cont != nullptr); // Invalid information received
		cont->setUid(my_uidManager->getId());
		// Store data
		if (my_currentColorStyle != nullptr) { cont->setColorStyle(my_currentColorStyle); }
		my_mapObjects.insert_or_assign(cont->uid(), cont);
		addCreatedUid(_creatorUid, cont->uid());

		cont->setParentObject(ui);
		ui->addChildObject(cont);
	}
	return cont->uid();
}

ak::UID ak::ui::objectManager::createWelcomeScreen(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::widget::welcomeScreen * obj = new ak::ui::widget::welcomeScreen(my_messenger, my_uidManager, my_currentColorStyle);

	// Set parameter
	obj->setUid(my_uidManager->getId());

	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createWindow(
	ak::UID												_creatorUid
) {
	// Create new ui manager
	ak::ui::uiManager * ui = nullptr;
	ui = new ak::ui::uiManager(my_messenger, my_uidManager, my_currentColorStyle);
	// Store data
	my_mapObjects.insert_or_assign(ui->uid(), ui);
	addCreatedUid(_creatorUid, ui->uid());
	return ui->uid();
}

// ###############################################################################################################################################

ak::ui::core::aObject * ak::ui::objectManager::object(
	ak::UID												_objectUid
) {
	auto itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end());	// Invalid object UID provided
	return itm->second;
}

ak::ui::core::aObject * ak::ui::objectManager::object(
	const QString &										_objectUniqueName
) {
	auto itm = my_mapUniqueNames.find(_objectUniqueName);
	assert(itm != my_mapUniqueNames.end());	// Invalid object UID provided
	if (itm == my_mapUniqueNames.end()) return ak::invalidUID;
	return itm->second;
}

// ###############################################################################################################################################

void ak::ui::objectManager::creatorDestroyed(
	ak::UID												_creatorUid
) {
	try {
		my_notifier->disable();
		// Get all UIDs created by this creator
		auto itm = my_mapCreators.find(_creatorUid);
		if (itm != my_mapCreators.end()) {
			// Get and destroy every single object created by this creator
			for (int i = 0; i < itm->second->size(); i++) {
				auto obj = my_mapObjects.find(itm->second->at(i));
				if (obj != my_mapObjects.end()) {
					ak::ui::core::aObject * actualObject = obj->second;
					// Check if object is a restorable type and remove it from the map
					my_mapAliases.erase(actualObject->alias());

					// Remove the unique name access to this object
					if (!actualObject->uniqueName().isEmpty()) {
						my_mapUniqueNames.erase(actualObject->uniqueName());
					}

					// Delete object
					delete actualObject;
					obj->second = nullptr;
					my_mapObjects.erase(itm->second->at(i));
				}
			}
			// Delete vector
			std::vector<ak::UID> *	r = itm->second;
			delete r;
			itm->second = nullptr;
			// Remove creator UID
			my_mapCreators.erase(_creatorUid);
		}
		my_notifier->enable();
	}
	catch (const std::exception & e) {
		assert(0);	// Something went wrong
	}
	catch (...) {
		assert(0);	// Something went horribly wrong
	}
}

void ak::ui::objectManager::destroy(
	ak::UID												_objectUID,
	bool												_ignoreIfObjectHasChildObjects
) {
	my_notifier->disable();

	auto object = my_mapObjects.find(_objectUID);
	assert(object != my_mapObjects.end());	// Invalid object UID

	if (object != my_mapObjects.end()) // Avoid a UI crash if something goes wrong here
	{
		ui::core::aObject * actualObject = object->second;

		if (_ignoreIfObjectHasChildObjects) {
			if (actualObject->childObjectCount() != 0) { return; }
			switch (actualObject->objectType())
			{
			case ui::core::objectType::oTabToolbarPage:
			case ui::core::objectType::oTabToolbarGroup:
			case ui::core::objectType::oTabToolbarSubgroup:
			{
				ui::core::ttbContainer * container = nullptr;
				container = dynamic_cast<ui::core::ttbContainer *>(actualObject);
				assert(container != nullptr); // Cast failed
				if (container->subContainerCount() != 0) { return; }
			}
			break;
			}
		}

		my_mapAliases.erase(actualObject->alias());
		my_mapOwners.erase(_objectUID);
		my_mapObjects.erase(_objectUID);

		// Remove the unique name access to this object
		if (!actualObject->uniqueName().isEmpty()) {
			my_mapUniqueNames.erase(actualObject->uniqueName());
		}

		// Destroy object
		delete actualObject;
	}

	my_notifier->enable();
}

void ak::ui::objectManager::setIconSearchDirectories(
	const std::vector<QString>							_paths
) {
	my_iconSearchPaths = _paths;
	for (auto itm : my_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		itm->setDirectories(my_iconSearchPaths);
	}
}

void ak::ui::objectManager::addColorStyle(
	ak::ui::colorStyle *								_colorStyle,
	bool												_activate
) {
	assert(_colorStyle != nullptr); // Nullptr provided
	for (auto itm : my_colorStyles) {
		if (itm->getColorStyleName() == _colorStyle->getColorStyleName()) {
			assert(0); // ColorStyle with the specified name already exists
			return;
		}
	}
	my_colorStyles.push_back(_colorStyle);
	if (_activate) { setColorStyle(_colorStyle); }
}

void ak::ui::objectManager::setColorStyle(
	const QString &										_colorStyleName
) {
	for (auto itm : my_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		if (itm->getColorStyleName() == _colorStyleName) {
			setColorStyle(itm);
			return;
		}
	}
}

void ak::ui::objectManager::setDefaultColorStyle(void) {
	setColorStyle(my_colorStyles[COLORSTYLE_INDEX_DEFAULT]);
}

void ak::ui::objectManager::setDefaultDarkColorStyle(void) {
	setColorStyle(my_colorStyles[COLORSTYLE_INDEX_DARK]);
}

ak::ui::colorStyle * ak::ui::objectManager::getCurrentColorStyle(void) const { return my_currentColorStyle; }

QString ak::ui::objectManager::getCurrentColorStyleName(void) const {
	if (my_currentColorStyle == nullptr) { return QString(); }
	return my_currentColorStyle->getColorStyleName();
}

void ak::ui::objectManager::destroyAll(void) {
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
	my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);

	auto itm = my_mapObjects.begin();

	// Destroy all objects that are not a main window
	while (itm != my_mapObjects.end()) {
		ak::ui::core::aObject * obj = itm->second;
		if (obj->objectType() != ak::ui::core::objectType::oMainWindow) {
			// Erase item
			my_mapObjects.erase(obj->uid());
			// Destroy all objects that were created by this item
			creatorDestroyed(obj->uid());
			delete obj;
			itm = my_mapObjects.begin();
		}
		else {
			creatorDestroyed(obj->uid()); itm++;
		}
	}
	for (itm = my_mapObjects.begin(); itm != my_mapObjects.end(); itm++) {
		ak::ui::core::aObject * obj = itm->second;
		if (obj != nullptr) { delete obj; }
	}
	my_mapCreators.clear();
	my_mapObjects.clear();
	my_mapUniqueNames.clear();
}

std::string ak::ui::objectManager::saveStateWindow(
	const std::string &									_applicationVersion
) {
	// Prepare document
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	// Add app version string
	rapidjson::Value appV(_applicationVersion.c_str(), allocator);
	doc.AddMember(SETTING_VERSION_APPLICATION, appV, allocator);

	// Add uiCore settings version string
	rapidjson::Value cfgV(CONFIG_VERSION_WINDOWSTATE, allocator);
	doc.AddMember(SETTING_VERSION_WINDOWSTATE, cfgV, allocator);

	// Create array value
	rapidjson::Value items(rapidjson::kArrayType);

	// Collect data for objects that have to be restored
	for (auto itm = my_mapAliases.begin(); itm != my_mapAliases.end(); itm++) {
		auto obj = my_mapObjects.find(itm->second);
		ak::ui::core::aRestorable * restorable = nullptr;
		restorable = dynamic_cast<ak::ui::core::aRestorable *>(obj->second);
		assert(restorable != nullptr); // Upps
		restorable->addObjectSettingsToValue(items, allocator);
	}

	// Create JSON string
	doc.AddMember(RESTORABLE_UI_SETTINGS, items, allocator);

	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	char * json = strdup(buffer.GetString());
	std::string info(json);
	return info;
}

std::string ak::ui::objectManager::saveStateColorStyle(
	const std::string &									_applicationVersion
) {
	// Prepare document
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

	// Add app version string
	rapidjson::Value appV(_applicationVersion.c_str(), allocator);
	doc.AddMember(SETTING_VERSION_APPLICATION, appV, allocator);

	// Add uiCore settings version string
	rapidjson::Value cfgV(CONFIG_VERSION_WINDOWSTATE, allocator);
	doc.AddMember(SETTING_VERSION_WINDOWSTATE, cfgV, allocator);

	// Create array value
	rapidjson::Value items(rapidjson::kArrayType);

	// Create current color style settings entry
	if (my_currentColorStyle != nullptr) {
		rapidjson::Value colorSettings;
		colorSettings.SetObject();
		std::string currentColorStyleString = getCurrentColorStyleName().toStdString();
		rapidjson::Value currentColorStyle(currentColorStyleString.c_str(), allocator);
		colorSettings.AddMember(RESTORABLE_UI_COLORSTYLE, currentColorStyle, allocator);
		items.PushBack(colorSettings, allocator);
	}

	// Create JSON string
	doc.AddMember(RESTORABLE_UI_SETTINGS, items, allocator);

	rapidjson::StringBuffer buffer;
	buffer.Clear();

	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);

	char * json = strdup(buffer.GetString());
	std::string info(json);
	return info;
}

ak::core::settingsRestoreErrorCode ak::ui::objectManager::restoreStateWindow(
	const char *										_json,
	const std::string &									_applicationVersion
) {
	rapidjson::Document doc;
	doc.Parse(_json);

	// Check version
	if (!doc.HasMember(SETTING_VERSION_APPLICATION) ||
		!doc.HasMember(SETTING_VERSION_WINDOWSTATE)) {
		return ak::core::settingsRestoreErrorCode::srecSettingsVersionMismatch;
	}
	assert(doc[SETTING_VERSION_APPLICATION].IsString());
	assert(doc[SETTING_VERSION_WINDOWSTATE].IsString());
	std::string settingVersionApp(doc[SETTING_VERSION_APPLICATION].GetString());
	std::string settingVersionWindowState(doc[SETTING_VERSION_WINDOWSTATE].GetString());
	if (_applicationVersion != settingVersionApp) { return ak::core::settingsRestoreErrorCode::srecAppVersionMismatch; }
	if (CONFIG_VERSION_WINDOWSTATE != settingVersionWindowState) { return ak::core::settingsRestoreErrorCode::srecSettingsVersionMismatch; }

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
		auto oAlias = my_mapAliases.find(objAlias);
		assert(oAlias != my_mapAliases.end()); // No object with specified alias exists

		// Get the object
		auto object = my_mapObjects.find(oAlias->second);
		assert(object != my_mapObjects.end()); // Registered alias is not stored

		// Check the object type
		assert(ak::ui::core::toQString(object->second->objectType()) == objType); // Current object type differs from saved object type

		// Check object
		ak::ui::core::aRestorable * restorable = nullptr;
		restorable = dynamic_cast<ak::ui::core::aRestorable *>(object->second);
		assert(restorable != nullptr); // Cast failed

		// Apply the settings
		const rapidjson::Value & settings = obj[RESTORABLE_NAME_SETTINGS];
		assert(settings.IsObject()); // Not a setting
		restorable->restoreSettings(settings);

	}
	return ak::core::settingsRestoreErrorCode::srecNone;
}

ak::core::settingsRestoreErrorCode ak::ui::objectManager::restoreStateColorStyle(
	const char *										_json,
	const std::string &									_applicationVersion
) {
	rapidjson::Document doc;
	doc.Parse(_json);

	// Check version
	if (!doc.HasMember(SETTING_VERSION_APPLICATION) ||
		!doc.HasMember(SETTING_VERSION_WINDOWSTATE)) {
		return ak::core::settingsRestoreErrorCode::srecSettingsVersionMismatch;
	}
	assert(doc[SETTING_VERSION_APPLICATION].IsString());
	assert(doc[SETTING_VERSION_WINDOWSTATE].IsString());
	std::string settingVersionApp(doc[SETTING_VERSION_APPLICATION].GetString());
	std::string settingVersionWindowState(doc[SETTING_VERSION_WINDOWSTATE].GetString());
	if (_applicationVersion != settingVersionApp) { return ak::core::settingsRestoreErrorCode::srecAppVersionMismatch; }
	if (CONFIG_VERSION_WINDOWSTATE != settingVersionWindowState) { return ak::core::settingsRestoreErrorCode::srecSettingsVersionMismatch; }

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
	return ak::core::settingsRestoreErrorCode::srecNone;
}

void ak::ui::objectManager::addAlias(
	const QString &										_alias,
	ak::UID												_UID
) {
	auto obj = my_mapAliases.find(_alias);
	assert(obj == my_mapAliases.end()); // Object with the specified alias already exists
	my_mapAliases.insert_or_assign(_alias, _UID);
}

void ak::ui::objectManager::setObjectUniqueName(
	ak::UID												_objectUid,
	const QString &										_uniqueName
) {
	auto actualObject{ object(_objectUid) };
	assert(actualObject != nullptr);	// The object was not created

	auto duplicateEntry{ my_mapUniqueNames.find(_uniqueName) };
	if (duplicateEntry != my_mapUniqueNames.end()) {
		if (duplicateEntry->second == actualObject) { return; }
		assert(0); // Another object with the same unique name does already exist
	}

	// Remove the old unique name
	if (!actualObject->uniqueName().isEmpty()) { my_mapUniqueNames.erase(actualObject->uniqueName()); }
	
	// Store information
	actualObject->setUniqueName(_uniqueName);
	if (!_uniqueName.isEmpty()) {
		my_mapUniqueNames.insert_or_assign(_uniqueName, actualObject);
	}
}

void ak::ui::objectManager::removeAlias(
	const QString &										_alias
) { my_mapAliases.erase(_alias); }

ak::UID ak::ui::objectManager::objectCreator(
	ak::UID												_objectUID
) {
	auto owner = my_mapOwners.find(_objectUID);
	assert(owner != my_mapOwners.end());	// Invalid UID
	return owner->second;
}

bool ak::ui::objectManager::objectExists(
	ak::UID												_objectUID
) {
	auto itm{ my_mapObjects.find(_objectUID) };
	return !(itm == my_mapObjects.end());
}

// ###############################################################################################################################################

void ak::ui::objectManager::setColorStyle(
	ui::colorStyle *									_colorStyle
) {
	assert(_colorStyle != nullptr); // Nullptr provided
	my_currentColorStyle = _colorStyle;

	for (auto obj = my_mapObjects.begin(); obj != my_mapObjects.end(); obj++) {
		assert(obj->second != nullptr); // nullptr stored
		if (obj->second->isPaintableType()) {
			// Cast paintable
			ak::ui::core::aPaintable * itm = nullptr;
			itm = dynamic_cast<ak::ui::core::aPaintable *>(obj->second);
			assert(itm != nullptr); // Cast failed
			itm->setColorStyle(my_currentColorStyle);
		}
	}
}

void ak::ui::objectManager::addCreatedUid(
	ak::UID												_creatorUid,
	ak::UID												_createdUid
) {
	auto itm = my_mapCreators.find(_creatorUid);
	if (itm == my_mapCreators.end()) {
		// First object created by this creator
		std::vector<ak::UID> *	v = nullptr;
		v = new std::vector<ak::UID>;

		// Store data
		v->push_back(_createdUid);
		my_mapCreators.insert_or_assign(_creatorUid, v);
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
	assert(my_mapOwners.find(_createdUid) == my_mapOwners.end());	// Created UID already stored
	my_mapOwners.insert_or_assign(_createdUid, _creatorUid);

}

QWidget * ak::ui::objectManager::castToWidget(
	ak::UID												_objectUid
) {
	// Get widget
	auto obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->isWidgetType()); // Object is not a widget
	ak::ui::core::aWidget * widget = nullptr;
	widget = dynamic_cast<ak::ui::core::aWidget *>(obj->second);
	assert(widget != nullptr); // Cast failed
	return widget->widget();
}
