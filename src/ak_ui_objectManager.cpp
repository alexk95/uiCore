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

#include <qpixmap.h>

 // AK header
#include <ak_ui_objectManager.h>			// corresponding header
#include <ak_messenger.h>					// messenger
#include <ak_notifierObjectManager.h>		// notifierObjectManager
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_iconManager.h>				// icon manager
#include <ak_ui_core_aWidget.h>
#include <ak_ui_core_aObject.h>
#include <ak_ui_core_aRestorable.h>
#include <ak_ui_core_aDialog.h>
#include <ak_ui_colorStyleDefault.h>
#include <ak_ui_colorStyleDefaultDark.h>

// AK dialogs
#include <ak_ui_dialog_logIn.h>

// AK Qt objects
#include <ak_ui_qt_action.h>				// action
#include <ak_ui_qt_checkBox.h>				// checkBox
#include <ak_ui_qt_colorEditButton.h>		// colorEditButton
#include <ak_ui_qt_comboBox.h>				// comboBox
#include <ak_ui_qt_comboButton.h>			// comboButton
#include <ak_ui_qt_dock.h>					// dock
#include <ak_ui_qt_pushButton.h>			// pushButton
#include <ak_ui_qt_textEdit.h>				// textEdit
#include <ak_ui_qt_timer.h>

// AK widget objects
#include <ak_ui_widget_propertyGrid.h>		// propertyGrid
#include <ak_ui_widget_defaultWelcomeScreen.h>
#include <ak_ui_widget_propertyGridItem.h>	// propertyGridItem
#include <ak_ui_widget_table.h>				// table
#include <ak_ui_widget_tabView.h>			// tabView
#include <ak_ui_widget_tree.h>				// tree

// AK ttb objects
#include <ak_ui_ttb_group.h>				// ttb Group
#include <ak_ui_ttb_page.h>					// ttb Page
#include <ak_ui_ttb_subgroup.h>				// ttb Subroup

// Rapid JSON header
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

ak::ui::objectManager::objectManager(
	ak::messenger *										_messenger,
	ak::uidManager *									_uidManager,
	ak::ui::iconManager *								_iconManager
)
	: my_currentColorStyle(nullptr),
	my_messenger(nullptr),
	my_signalLinker(nullptr),
	my_uidManager(nullptr),
	my_notifier(nullptr),
	my_iconManager(nullptr)
{
	try {
		// Check parameter
		assert(_messenger != nullptr); // nullptr provided
		assert(_uidManager != nullptr); // nullptr provided
		assert(_iconManager != nullptr); // nullptr provided

		// Apply settings
		my_messenger = _messenger;
		my_uidManager = _uidManager;
		my_iconManager = _iconManager;

		// Create signal linker
		my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);

		// Create notifier
		my_notifier = new ak::notifierObjectManager(this);

		my_currentColorStyle = new ui::colorStyleDefault();
		my_colorStyles.push_back(my_currentColorStyle);
		my_currentColorStyle->setDirectories(my_iconManager->searchDirectories());
		ui::colorStyle * cS = new ui::colorStyleDefaultDark();
		my_colorStyles.push_back(cS);
		cS->setDirectories(my_iconManager->searchDirectories());

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::objectManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::objectManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::objectManager()"); }
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
	my_signalLinker->addLink(obj);
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createCheckBox(
	ak::UID												_creatorUid,
	const QString &										_text
) {
	// Create object
	ak::ui::qt::checkBox * obj = new ak::ui::qt::checkBox(_text);
	// Set parameter
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
	const std::vector<ak::ui::qt::comboButtonItem> &	_possibleSelection
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
	const QString &										_text
) {
	// Create object
	ak::ui::qt::comboButtonItem * obj = new ak::ui::qt::comboButtonItem(_text);
	// Set parameter
	obj->setUid(my_uidManager->getId());
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

ak::UID ak::ui::objectManager::createDefaultWelcomeScreen(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::widget::defaultWelcomeScreen * obj = new ak::ui::widget::defaultWelcomeScreen(my_messenger, my_uidManager, my_currentColorStyle);

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

ak::UID ak::ui::objectManager::createLogInDialog(
	ak::UID												_creatorUid,
	bool												_showSavePassword,
	const QString &										_imageName,
	const QString &										_username,
	const QString &										_password
) {
	// Find image
	const QPixmap * pix = my_iconManager->pixmap(_imageName);

	// Create object
	ak::ui::dialog::logIn * obj = new ak::ui::dialog::logIn(my_messenger, _showSavePassword, *pix, _username, _password);
	assert(obj != nullptr);

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
	ak::ui::widget::propertyGrid * obj = new ak::ui::widget::propertyGrid(my_messenger, my_uidManager, my_currentColorStyle);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
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

ak::UID ak::ui::objectManager::createTable(
	ak::UID												_creatorUid,
	int													_rows,
	int													_columns
) {
	// Create object
	ak::ui::widget::table * obj = new ak::ui::widget::table(my_messenger, my_uidManager, my_currentColorStyle, _rows, _columns);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
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
	ak::ui::qt::textEdit * obj = new ak::ui::qt::textEdit(_initialText);
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

ak::UID ak::ui::objectManager::createTree(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::widget::tree * obj = new ak::ui::widget::tree(my_messenger, my_uidManager, my_currentColorStyle);
	// Set parameter
	if (my_currentColorStyle != nullptr) { obj->setColorStyle(my_currentColorStyle); }
	// Store data
	my_mapObjects.insert_or_assign(obj->uid(), obj);
	addCreatedUid(_creatorUid, obj->uid());
	return obj->uid();
}

ak::UID ak::ui::objectManager::createUiManager(
	ak::UID												_creatorUid
) {
	// Create new ui manager
	ak::ui::uiManager * ui = nullptr;
	ui = new ak::ui::uiManager(my_messenger, my_uidManager, my_iconManager, this, my_currentColorStyle);
	// Store data
	my_mapObjects.insert_or_assign(ui->uid(), ui);
	addCreatedUid(_creatorUid, ui->uid());
	return ui->uid();
}

ak::UID ak::ui::objectManager::createTabView(
	ak::UID												_creatorUid
) {
	// Create object
	ak::ui::widget::tabView * obj = new ak::ui::widget::tabView(my_iconManager, my_messenger, my_uidManager, this, my_currentColorStyle);
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
	my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
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
	// Create subcontainer
	ak::ui::core::ttbContainer * sub = cont->createSubContainer(_text);
	assert(sub != nullptr); // Invalid information received
	// Store data
	if (my_currentColorStyle != nullptr) { sub->setColorStyle(my_currentColorStyle); }
	my_mapObjects.insert_or_assign(sub->uid(), sub);
	addCreatedUid(_creatorUid, sub->uid());
	return sub->uid();
}

ak::UID ak::ui::objectManager::createTabToolBarPage(
	ak::UID												_creatorUid,
	ak::UID												_uiManagerUid,
	const QString &										_text
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	// Check object type
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type provided
	// Cast ui
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed
	// Create container
	ak::ui::core::ttbContainer * cont = ui->createTabToolbarSubContainer(_text);
	assert(cont != nullptr); // Invalid information received
	// Store data
	if (my_currentColorStyle != nullptr) { cont->setColorStyle(my_currentColorStyle); }
	my_mapObjects.insert_or_assign(cont->uid(), cont);
	addCreatedUid(_creatorUid, cont->uid());
	return cont->uid();
}

// ###############################################################################################################################################

// Object setter

void ak::ui::objectManager::obj_setAlias(
	ak::UID												_objectUid,
	const QString &										_alias
) {
	// Find parent object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	ak::ui::core::aRestorable * restorable = nullptr;
	restorable = dynamic_cast<ak::ui::core::aRestorable *>(obj->second);
	assert(restorable != nullptr); // Object is not a restorable type

	// Check for duplicate alias
	my_mapAliasesIterator alias = my_mapAliases.find(_alias);
	assert(alias == my_mapAliases.end()); // Alias already set to an object
		
	// Check if alias was registered before
	if (restorable->alias().length() > 0) {
		my_mapAliases.erase(restorable->alias());
	}

	// Add new alias if provided
	if (_alias.length() > 0) {
		restorable->setAlias(_alias);
		my_mapAliases.insert_or_assign(_alias, _objectUid);
	}
}

void ak::ui::objectManager::obj_addObjectToContainer(
	ak::UID												_parentUid,
	ak::UID												_objectUid
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
	assert(itm != my_mapObjects.end()); // Invalid parent object UID
	// Find object
	my_mapObjectsIterator itmO = my_mapObjects.find(_objectUid);
	assert(itmO != my_mapObjects.end()); // Invalid object UID

	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabToolbarPage:
	case ak::ui::core::objectType::oTabToolbarGroup:
	case ak::ui::core::objectType::oTabToolbarSubgroup:
	{
		// Cast container
		ak::ui::core::ttbContainer * cont = nullptr;
		cont = dynamic_cast<ak::ui::core::ttbContainer *>(itm->second);
		assert(cont != nullptr); // Cast failed
		// Add object
		cont->addChild(itmO->second);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setCentralWidget(
	ak::UID												_parentUid,
	ak::UID												_objectUid
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
	assert(itm != my_mapObjects.end()); // Invalid parent object UID
	// Find object
	my_mapObjectsIterator itmO = my_mapObjects.find(_objectUid);
	assert(itmO != my_mapObjects.end()); // Invalid object UID
	assert(itmO->second->isWidgetType()); // Object is not a widget
	ak::ui::core::aWidget * widget = dynamic_cast<ak::ui::core::aWidget *>(itmO->second);

	switch (itm->second->objectType())
	{
	case ak::ui::core::objectType::oDock:
	{
		// Cast dock
		ak::ui::qt::dock * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWidget(widget->widget());
		widget->setParentDock(obj);
	}
	break;
	case ak::ui::core::objectType::oMainWindow:
	{
		// Cast dock
		ak::ui::uiManager * obj = nullptr;
		obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setCentralWidget(widget->widget());
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setCentralWidget(
	ak::UID												_parentUid,
	QWidget *											_widget
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
	assert(itm != my_mapObjects.end()); // Invalid parent object UID

	switch (itm->second->objectType())
	{
	case ak::ui::core::objectType::oDock:
	{
		// Cast dock
		ak::ui::qt::dock * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWidget(_widget);
	}
	break;
	case ak::ui::core::objectType::oMainWindow:
	{
		// Cast dock
		ak::ui::uiManager * obj = nullptr;
		obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setCentralWidget(_widget);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

ak::ID ak::ui::objectManager::obj_addItem(
	ak::UID												_objectUid,
	ak::ID												_groupId,
	const QString &										_text
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	switch (itm->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		ak::ui::widget::defaultWelcomeScreen * actualObject = nullptr;
		actualObject = dynamic_cast<ui::widget::defaultWelcomeScreen *>(itm->second);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->addItem(_groupId, _text);
	}
	break;
	default:
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::ui::objectManager::obj_addItem(
	ak::UID												_objectUid,
	ak::ID												_groupId,
	const QString &										_text,
	const QIcon &										_icon
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	switch (itm->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		ak::ui::widget::defaultWelcomeScreen * actualObject = nullptr;
		actualObject = dynamic_cast<ui::widget::defaultWelcomeScreen *>(itm->second);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->addItem(_groupId, _icon, _text);
	}
	break;
	default:
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::ui::objectManager::obj_addItem(
	ak::UID												_objectUid,
	ak::ID												_groupId,
	const QString &										_text,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	// Find parent object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		ak::ui::widget::defaultWelcomeScreen * actualObject = nullptr;
		actualObject = dynamic_cast<ui::widget::defaultWelcomeScreen *>(itm->second);
		assert(actualObject != nullptr); // Cast failed
		return actualObject->addItem(_groupId, *my_iconManager->icon(_iconName, _iconSize), _text);
	}
	break;
	default:
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

void ak::ui::objectManager::obj_setText(
	ak::UID												_objectUid,
	const QString &										_text
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		ak::ui::qt::colorEditButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::colorEditButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->overrideText(_text);
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWindowIconText(_text);
	}
	break;
	case ak::ui::core::objectType::oMainWindow:
	{
		// Cast object
		ak::ui::uiManager * obj = nullptr;
		obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWindowTitle(_text);
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(_text);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setChecked(
	ak::UID												_objectUid,
	bool												_checked
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setChecked(_checked);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setTristate(
	ak::UID												_objectUid,
	bool												_isTristate
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setTristate(_isTristate);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setFilterVisible(
	ak::UID												_objectUid,
	bool												_vis
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setFilterVisible(_vis);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setFilterCaseSensitive(
	ak::UID												_objectUid,
	bool												_caseSensitive,
	bool												_refresh
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setFilterCaseSensitive(_caseSensitive, _refresh);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setFilterRefreshOnChange(
	ak::UID												_objectUid,
	bool												_refreshOnChange
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setFilterRefreshOnChange(_refreshOnChange);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setItems(
	ak::UID												_objectUid,
	const std::vector<ak::ui::qt::comboButtonItem> &	_items
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setItems(_items);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

//ouiahfiudsjfhpis seai fhsid fhsdpfhspiuf hpsidf hsüdop fjsof hdsrpofudfspoif poisdv poijksdfhpuios dpoiewfj poise pioufeshj psefheal püoiesfj üose jfüewso 

void ak::ui::objectManager::obj_setColor(
	ak::UID												_objectUid,
	const ak::ui::color &								_color
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * obj = nullptr;
		obj = dynamic_cast<qt::colorEditButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setColor(_color);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setIcon(
	ak::UID												_objectUid,
	const QIcon &										_icon
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setIcon(_icon);
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setIcon(_icon);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setReadOnly(
	ak::UID												_objectUid,
	bool												_readOnly
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setReadOnly(_readOnly);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_appendText(
	ak::UID												_objectUid,
	const QString &										_text
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(obj->text() + _text);
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(obj->text() + _text);
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(obj->text() + _text);
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(obj->text() + _text);
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWindowIconText(obj->windowIconText() + _text);
	}
	break;
	case ak::ui::core::objectType::oMainWindow:
	{
		// Cast object
		ak::ui::uiManager * obj = nullptr;
		obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setWindowTitle(obj->windowTitle() + _text);
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setText(obj->text() + _text);
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->append(_text);

	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setAutoScrollToBottomEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setAutoScrollToBottom(_enabled);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setTabToolBarVisible(
	ak::UID												_uiManagerUid,
	bool												_vis
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oMainWindow); // Invalid object type

	// Cast ui manager
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setTabToolBarVisible(_vis);
}

void ak::ui::objectManager::obj_setTabLocation(
	ak::UID												_objectUid,
	ak::ui::core::tabLocation							_location
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->setTabLocation(_location);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setTabFocused(
	ak::UID												_objectUid,
	ak::ID												_tab
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->focusTab(_tab);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	bool												_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ak::ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	int													_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	double												_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const char *										_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const QString &										_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const ak::ui::color &								_value,
	bool												_isMultipleValues
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _value, _isMultipleValues);
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
	const QString &										_selectedValue,
	bool												_isMultipleValues
) {
	assert(_selection.size() > 0); // No items provided
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oPropertyGrid); // Invalid object type

	// Cast property grid
	ak::ui::widget::propertyGrid * p = nullptr;
	p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
	assert(p != nullptr); // Cast failed
	// Add property
	p->createItem(_itemName, _selection, _selectedValue, _isMultipleValues);
}

ak::ID ak::ui::objectManager::obj_createItem(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	const QString &										_text,
	ak::ui::core::textAlignment							_textAlignment,
	QIcon												_icon
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oTree); // Invalid object type

	// Cast tree
	ak::ui::widget::tree * t = nullptr;
	t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
	assert(t != nullptr); // Cast failed
	// Add object
	return t->add(_parentId, _text, _textAlignment, _icon);
}

ak::ID ak::ui::objectManager::obj_createItem(
	ak::UID												_objectUid,
	const QString &										_cmd,
	char												_delimiter,
	ak::ui::core::textAlignment							_textAlignment,
	const QIcon  &										_icon
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ui::core::objectType::oTree); // Invalid object type

	// Cast tree
	ak::ui::widget::tree * t = nullptr;
	t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
	assert(t != nullptr); // Cast failed
	// Add object
	return t->add(_cmd, _delimiter, _textAlignment, _icon);
}

void ak::ui::objectManager::obj_setMultiSelectionEnabled(
	ak::UID												_objectUid,
	bool												_multiSelection
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->setMultiSelectionEnabled(_multiSelection);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->setSelectAndDeselectChildrenEnabled(_enabled);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setAutoExpandSelectedItems(
	ak::UID												_objectUid,
	bool												_enabled
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->setAutoExpandSelectedItemsEnabled(_enabled);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_expandAllItems(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->expandAllItems();
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_collapseAllItems(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->collapseAllItems();
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_deselectAllItems(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->deselectAllItems(true);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID												_objectUid,
	ak::UID												_widgetUid,
	const QString &										_title,
	const QIcon &										_icon
) {
	QWidget * Widget = castToWidget(_widgetUid);
	return obj_addTab(_objectUid, Widget, _title, _icon);
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID												_objectUid,
	QWidget *											_widget,
	const QString &										_title,
	const QIcon &										_icon
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTabView:
	{
		// Cast tree
		ak::ui::widget::tabView * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		return t->createTab(_widget, _title, _icon);
	}
	break;
	default:
		assert(0); // Invalid object type
		return ak::invalidID;
	}
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID												_objectUid,
	ak::UID												_widgetUid,
	const QString &										_title,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	return obj_addTab(_objectUid, _widgetUid, _title, *my_iconManager->icon(_iconName, _iconSize));
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID												_objectUid,
	QWidget *											_widget,
	const QString &										_title,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	return obj_addTab(_objectUid, _widget, _title, *my_iconManager->icon(_iconName, _iconSize));
}

void ak::ui::objectManager::obj_setEnabled(
	ak::UID												_objectUid,
	bool												_enabled
) {
	// Find object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * o = nullptr;
		o = dynamic_cast<ak::ui::qt::action *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * o = nullptr;
		o = dynamic_cast<ak::ui::qt::checkBox *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * o = nullptr;
		o = dynamic_cast<qt::colorEditButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->SetEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oComboBox:
	{
		// Cast object
		ak::ui::qt::comboBox * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboBox *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButtonItem *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * o = nullptr;
		o = dynamic_cast<ak::ui::qt::dock *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::pushButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oTable:
	{
		// Cast object
		ak::ui::widget::table * o = nullptr;
		o = dynamic_cast<ak::ui::widget::table *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * o = nullptr;
		o = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * o = nullptr;
		o = dynamic_cast<ak::ui::qt::textEdit *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * o = nullptr;
		o = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setEnabled(_enabled);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_setVisible(
	ak::UID												_objectUid,
	bool												_visible
) {
	// Find object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * o = nullptr;
		o = dynamic_cast<ak::ui::qt::action *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * o = nullptr;
		o = dynamic_cast<ak::ui::qt::checkBox *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * o = nullptr;
		o = dynamic_cast<qt::colorEditButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->SetVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oComboBox:
	{
		// Cast object
		ak::ui::qt::comboBox * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboBox *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButtonItem *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * o = nullptr;
		o = dynamic_cast<ak::ui::qt::dock *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::pushButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oTable:
	{
		// Cast object
		ak::ui::widget::table * o = nullptr;
		o = dynamic_cast<ak::ui::widget::table *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * o = nullptr;
		o = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * o = nullptr;
		o = dynamic_cast<ak::ui::qt::textEdit *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * o = nullptr;
		o = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(o != nullptr); // Cast failed
		o->setVisible(_visible);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::obj_clear(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid UID provided

	switch (obj->second->objectType())
	{
	case ui::core::oDefaultWelcomeScreen:
	{
		// Cast tree
		ak::ui::widget::defaultWelcomeScreen * t = nullptr;
		t = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->clear();
	}
	break;
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->clear();
	}
	break;
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast tree
		ak::ui::widget::propertyGrid * t = nullptr;
		t = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->clear();
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_clear(
	ak::UID												_objectUid,
	ak::ID												_parentId
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid UID provided

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		// Cast tree
		ak::ui::widget::defaultWelcomeScreen * t = nullptr;
		t = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->clear(_parentId);
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_deleteItems(
	ak::UID												_objectUid,
	const std::vector<ak::ID> &							_items
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->deleteItems(_items);
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_start(
	ak::UID												_objectUid,
	int													_value
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTimer:
	{
		// Cast tree
		ak::ui::qt::timer * t = nullptr;
		t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->setInterval(_value);
		t->setSingleShot(false);
		t->start();
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_stop(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTimer:
	{
		// Cast tree
		ak::ui::qt::timer * t = nullptr;
		t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->stop();
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_shoot(
	ak::UID												_objectUid,
	int													_value
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTimer:
	{
		// Cast tree
		ak::ui::qt::timer * t = nullptr;
		t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
		assert(t != nullptr); // Cast failed
		// Add object
		t->setInterval(_value);
		t->setSingleShot(true);
		t->start();
	}
	break;
	default:
		assert(0); // Invalid object type
		break;
	}
}

void ak::ui::objectManager::obj_delete(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	// Check if is UI manager
	if (obj->second->objectType() == ak::ui::core::objectType::oMainWindow) {
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj->second);
		assert(ui != nullptr); // Cast failed
		ui->close();
	}
	// Destroy object
	ak::ui::core::aObject * o = obj->second;
	my_mapObjects.erase(_objectUid);
	delete o;
}

void ak::ui::objectManager::obj_showMaximized(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oMainWindow:
	{
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(obj->second);
		assert(ui != nullptr); // Cast failed
		ui->maximizeWindow();
	}
	break;
	default:
		assert(0);
		break;
	}
}

// ###############################################################################################################################################

// Object getter

QString ak::ui::objectManager::obj_getAlias(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	ak::ui::core::aRestorable * restorable = nullptr;
	restorable = dynamic_cast<ak::ui::core::aRestorable *>(obj->second);
	assert(restorable != nullptr); // Cast failed (object not restorable ?)
	return restorable->alias();
}

ak::ui::core::aObject * ak::ui::objectManager::obj_get(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	return obj->second;
}

QWidget * ak::ui::objectManager::obj_getWidget(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID

	// Check type
	assert(obj->second->isWidgetType()); // Object is not a widget

	// Cast widget
	ak::ui::core::aWidget * w = nullptr;
	w = dynamic_cast<ak::ui::core::aWidget *>(obj->second);

	// Check cast
	assert(w != nullptr); // Cast failed

	return w->widget();
}

QAction * ak::ui::objectManager::obj_getAction(
	ak::UID												_actionUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_actionUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->objectType() == ak::ui::core::objectType::oAction); // Invalid object type

	// Cast action
	QAction * ret = nullptr;
	ret = dynamic_cast<QAction *>(obj->second);
	assert(ret != nullptr); // Cast failed
	return ret;
}

ak::ui::core::objectType ak::ui::objectManager::obj_getType(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	return obj->second->objectType();
}

QString ak::ui::objectManager::obj_getText(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->text();
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->text();
	}
	break;
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * obj = nullptr;
		obj = dynamic_cast<qt::colorEditButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->color().toRGBString(".");
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->text();
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->text();
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->windowIconText();
	}
	break;
	case ak::ui::core::objectType::oMainWindow:
	{
		// Cast object
		ak::ui::uiManager * obj = nullptr;
		obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->windowTitle();
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->text();
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->toPlainText();
	}
	break;
	default:
		assert(0); // Invalid object type
		return "";
	}
}

bool ak::ui::objectManager::obj_getTristate(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->isTristate();
	}
	break;
	default:
		assert(0); // Invalid object type
		return false;
	}
}

bool ak::ui::objectManager::obj_getChecked(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->isChecked();
	}
	break;
	default: assert(0); // Invalid object type
		return false;
	}
}

ak::ui::color ak::ui::objectManager::obj_getColor(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * obj = nullptr;
		obj = dynamic_cast<qt::colorEditButton *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->color();
	}
	break;
	default: assert(0); // Invalid object type
		return ak::ui::color();
	}
}

std::vector<ak::ID> ak::ui::objectManager::obj_getSelectedItems(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->selectedItems();
	}
	break;
	default: assert(0); // Invalid object type
		return std::vector<ak::ID>();
	}
}

bool ak::ui::objectManager::obj_getAutoScrollToBottomEnabled(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * obj = nullptr;
		obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->autoScrollToBottom();
	}
	break;
	default: assert(0); // Invalid object type
		return false;
	}
}

bool ak::ui::objectManager::obj_getEnabled(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType()) {
	case ak::ui::core::objectType::oAction:
	{
		// Cast object
		ak::ui::qt::action * o = nullptr;
		o = dynamic_cast<ak::ui::qt::action *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oCheckBox:
	{
		// Cast object
		ak::ui::qt::checkBox * o = nullptr;
		o = dynamic_cast<ak::ui::qt::checkBox *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oColorEditButton:
	{
		// Cast object
		qt::colorEditButton * o = nullptr;
		o = dynamic_cast<qt::colorEditButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->Enabled();
	}
	break;
	case ak::ui::core::objectType::oComboButton:
	{
		// Cast object
		ak::ui::qt::comboButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oComboButtonItem:
	{
		// Cast object
		ak::ui::qt::comboButtonItem * o = nullptr;
		o = dynamic_cast<ak::ui::qt::comboButtonItem *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oDock:
	{
		// Cast object
		ak::ui::qt::dock * o = nullptr;
		o = dynamic_cast<ak::ui::qt::dock *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oPushButton:
	{
		// Cast object
		ak::ui::qt::pushButton * o = nullptr;
		o = dynamic_cast<ak::ui::qt::pushButton *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	case ak::ui::core::objectType::oTextEdit:
	{
		// Cast object
		ak::ui::qt::textEdit * o = nullptr;
		o = dynamic_cast<ak::ui::qt::textEdit *>(obj->second);
		assert(o != nullptr); // Cast failed
		return o->isEnabled();
	}
	break;
	default:
		assert(0); // Invalid object type
		return false;
	}
}

ak::ID ak::ui::objectManager::obj_getItem(
	ak::UID												_objectUid,
	const QString &										_text
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		std::vector<QString> titles = obj->tabTitles();
		for (int i = 0; i < titles.size(); i++) {
			if (titles.at(i) == _text) { return i; }
		}
		return ak::invalidID;
	}
	break;
	default: assert(0); // Invalid object type
		return ak::invalidID;
	}
}

int ak::ui::objectManager::obj_getItemCount(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->tabCount();
	}
	break;
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->itemCount();
	}
	break;
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->itemCount();
	}
	break;
	default: assert(0); // Invalid object type
		return 0;
	}
}

int ak::ui::objectManager::obj_getFocusedTab(
	ak::UID												_objectUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->focusedTab();
	}
	break;
	default: assert(0); // Invalid object type
		return 0;
	}
}

bool ak::ui::objectManager::obj_getAutoExpandSelectedItems(
	ak::UID												_objectUid
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		return t->autoExpandSelectedItemsEnabled();
	}
	break;
	default:
		assert(0); // Invalid object type
		return false;
	}
}

// ###############################################################################################################################################

// Item setter

void ak::ui::objectManager::itm_setSelected(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_selected
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->setItemSelected(_itemId, _selected);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setSingleSelected(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_selected
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->setSingleItemSelected(_itemId, _selected);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_toggleSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed

		t->toggleItemSelection(_itemId);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setText(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QString &										_text
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemText(_itemId, _text);
	}
	break;
	case ak::ui::core::objectType::oTabView:
	{
		// Cast tree
		ak::ui::widget::tabView * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setTabText(_itemId, _text);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setText(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	ak::ID												_itemId,
	const QString &										_text
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		// Cast tree
		ak::ui::widget::defaultWelcomeScreen * t = nullptr;
		t = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemText(_itemId, _parentId, _text);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setIcon(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QIcon &										_icon
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemIcon(_itemId, _icon);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setIcon(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemIcon(_itemId, *my_iconManager->icon(_iconName, _iconSize));
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setIcon(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	ak::ID												_itemId,
	const QIcon &										_icon
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		// Cast tree
		ak::ui::widget::defaultWelcomeScreen * t = nullptr;
		t = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemIcon(_parentId, _itemId, _icon);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setIcon(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	ak::ID												_itemId,
	const QString &										_iconName,
	const QString &										_iconSize
) {
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object ID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		// Cast tree
		ak::ui::widget::defaultWelcomeScreen * t = nullptr;
		t = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemIcon(_parentId, _itemId, *my_iconManager->icon(_iconName, _iconSize));
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setEnabled(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_enabled
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemEnabled(_itemId, _enabled);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::itm_setVisible(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	bool												_visible
) {
	// Get object
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	switch (obj->second->objectType())
	{
	case ak::ui::core::objectType::oTree:
	{
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		assert(t != nullptr); // Cast failed
		t->setItemVisible(_itemId, _visible);
	}
	break;
	default:
		assert(0); // Invalid object type
	}
}

// ###############################################################################################################################################

// Item getter

std::vector<QString> ak::ui::objectManager::itm_getPath(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getItemPath(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return std::vector<QString>();
	}
}

QString ak::ui::objectManager::itm_getPathString(
	ak::UID												_objectUid,
	ak::ID												_itemId,
	char												_delimiter
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getItemPathString(_itemId, _delimiter);
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

QString ak::ui::objectManager::itm_getText(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getItemText(_itemId);
	}
	break;
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getPropertyName(_itemId);
	}
	break;
	case ak::ui::core::objectType::oTabView:
	{
		// Cast object
		ak::ui::widget::tabView * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->tabText(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

QString ak::ui::objectManager::itm_getText(
	ak::UID												_objectUid,
	ak::ID												_parentId,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID provided
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oDefaultWelcomeScreen:
	{
		// Cast object
		ak::ui::widget::defaultWelcomeScreen * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::defaultWelcomeScreen *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->itemText(_parentId, _itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

ak::core::valueType ak::ui::objectManager::itm_getValueType(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getValueType(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return ak::core::valueType::vNull;
	}
}

ak::ID ak::ui::objectManager::itm_getID(
	ak::UID												_objectUid,
	const QString &										_itemPath,
	char												_delimiter
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oTree:
	{
		// Cast object
		ak::ui::widget::tree * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getItemID(_itemPath, _delimiter);
	}
	break;
	default: assert(0); // Invalid object type
		return ak::invalidID;
	}
}

bool ak::ui::objectManager::itm_getValueBoolean(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getBool(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return false;
	}
}

ak::ui::color ak::ui::objectManager::itm_getValueColor(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getColor(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return ui::color();
	}
}

double ak::ui::objectManager::itm_getValueDouble(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getDouble(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return 0.0;
	}
}

int ak::ui::objectManager::itm_getValueInteger(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getInt(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return 0;
	}
}

std::vector<QString> ak::ui::objectManager::itm_getValuePossibleSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getSelectionPossibleValues(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return std::vector<QString>();
	}
}

QString ak::ui::objectManager::itm_getValueSelection(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getSelection(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

QString ak::ui::objectManager::itm_getValueString(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getString(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

bool ak::ui::objectManager::itm_getValueIsMultivalued(
	ak::UID												_objectUid,
	ak::ID												_itemId
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oPropertyGrid:
	{
		// Cast object
		ak::ui::widget::propertyGrid * obj = nullptr;
		obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->getIsMultivalued(_itemId);
	}
	break;
	default: assert(0); // Invalid object type
		return false;
	}
}

// ###############################################################################################################################################

// Dialog

ak::ui::core::dialogResult ak::ui::objectManager::dialog_show(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->showDialog();
	}
	break;
	default: assert(0); // Invalid object type
		return ui::core::dialogResult::resultNone;
	}
}

ak::ui::core::dialogResult ak::ui::objectManager::dialog_result(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->result();
	}
	break;
	default: assert(0); // Invalid object type
		return ui::core::resultNone;
	}
}

QString ak::ui::objectManager::dialog_username(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->username();
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

QString ak::ui::objectManager::dialog_password(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->password();
	}
	break;
	default: assert(0); // Invalid object type
		return "";
	}
}

bool ak::ui::objectManager::dialog_savePassword(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		return obj->savePassword();
	}
	break;
	default: assert(0); // Invalid object type
		return false;
	}
}

void ak::ui::objectManager::dialog_showInvalidLogIn(
	ak::UID												_dialogUid
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid UID
	switch (itm->second->objectType()) {
	case ak::ui::core::objectType::oLogInDialog:
	{
		// Cast object
		ak::ui::dialog::logIn * obj = nullptr;
		obj = dynamic_cast<ak::ui::dialog::logIn *>(itm->second);
		assert(obj != nullptr); // Cast failed
		obj->showInvalidLogIn();
	}
	break;
	default: assert(0); // Invalid object type
	}
}

void ak::ui::objectManager::dialog_close(
	ak::UID												_dialogUid,
	ak::ui::core::dialogResult							_result
) {
	// Find object
	my_mapObjectsIterator itm = my_mapObjects.find(_dialogUid);
	assert(itm != my_mapObjects.end()); // Invalid UID
	ak::ui::core::aDialog * dia = nullptr;
	dia = dynamic_cast<ak::ui::core::aDialog *>(itm->second);
	assert(dia != nullptr); // Wrong object type
	dia->setResult(_result);
	dia->close();
}

// ###############################################################################################################################################

void ak::ui::objectManager::addDock(
	ak::UID												_uiManagerUid,
	ak::UID												_dockUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed
	// Find dock
	itm = my_mapObjects.find(_dockUid);
	assert(itm != my_mapObjects.end()); // Invalid dock UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oDock); // Invalid object type
	// Cast dock
	ak::ui::qt::dock * actualDock = nullptr;
	actualDock = dynamic_cast<ak::ui::qt::dock *>(itm->second);
	assert(actualDock != nullptr); // Cast failed

	// Add dock
	ui->addDock(actualDock, _dockLocation);
}

void ak::ui::objectManager::tabifyDock(
	ak::UID												_uiManagerUid,
	ak::UID												_parentUid,
	ak::UID												_dockUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->tabifyDock(_parentUid, _dockUid);
}

void ak::ui::objectManager::setDockBottomLeftPriority(
	ak::UID												_uiManagerUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setDockPriorityBottomLeft(_dockLocation);
}

void ak::ui::objectManager::setDockBottomRightPriority(
	ak::UID												_uiManagerUid,
	ak::ui::core::dockLocation							_dockLocation
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setDockPriorityBottomRight(_dockLocation);
}

void ak::ui::objectManager::setStatusLabelVisible(
	ak::UID												_uiManagerUid,
	bool												_visible
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setStatusLabelVisible(_visible);
}

void ak::ui::objectManager::setStatusProgressVisible(
	ak::UID												_uiManagerUid,
	bool												_visible
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setStatusBarVisible(_visible);
}

void ak::ui::objectManager::setStatusLabelText(
	ak::UID												_uiManagerUid,
	const QString &										_text
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setStatusLabelText(_text);
}

void ak::ui::objectManager::setStatusProgressValue(
	ak::UID												_uiManagerUid,
	int													_value
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setStatusBarProgress(_value);
}

void ak::ui::objectManager::setStatusProgressContinuous(
	ak::UID												_uiManagerUid,
	bool												_continuous
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->setStatusBarContinuous(_continuous);
}

bool ak::ui::objectManager::getStatusLabelVisible(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	return ui->getStatusLabelVisible();
}

bool ak::ui::objectManager::getStatusProgressVisible(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	return ui->getStatusBarVisible();
}

QString ak::ui::objectManager::getStatusLabelText(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ak::ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	return ui->getStatusLabelText();
}

int ak::ui::objectManager::getStatusProgressValue(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	return ui->getStatusBarProgress();
}

bool ak::ui::objectManager::getStatusProgressContinuous(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	return ui->getStatusBarContinuous();
}

void ak::ui::objectManager::close(
	ak::UID												_uiManagerUid
) {
	// Find ui manager
	my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
	assert(itm != my_mapObjects.end()); // Invalid object UID
	assert(itm->second->objectType() == ui::core::objectType::oMainWindow); // Invalid object type
	// Cast UI
	ak::ui::uiManager * ui = nullptr;
	ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
	assert(ui != nullptr); // Cast failed

	ui->close();
}

// ###############################################################################################################################################

void ak::ui::objectManager::creatorDestroyed(
	ak::UID												_creatorUid
) {
	try {
		my_notifier->disable();
		// Get all UIDs created by this creator
		my_mapCreatorsIterator itm = my_mapCreators.find(_creatorUid);
		if (itm != my_mapCreators.end()) {
			// Get and destroy every single object created by this creator
			for (int i = 0; i < itm->second->size(); i++) {
				my_mapObjectsIterator obj = my_mapObjects.find(itm->second->at(i));
				if (obj != my_mapObjects.end()) {
					// Destroy object
					ak::ui::core::aObject * d = obj->second;
					delete d;
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
		assert(0);
	}
	catch (...) {
		assert(0);
	}
}

void ak::ui::objectManager::addIconSearchPath(
	const QString &										_path
) {
	my_iconManager->addDirectory(_path);
	for (auto itm : my_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		itm->setDirectories(my_iconManager->searchDirectories());
	}
}

void ak::ui::objectManager::removeIconSearchPath(
	const QString &										_path
) {
	my_iconManager->removeDirectory(_path);
	for (auto itm : my_colorStyles) {
		assert(itm != nullptr); // nullptr stored
		itm->setDirectories(my_iconManager->searchDirectories());
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
	setColorStyle(my_colorStyles[0]);
}

void ak::ui::objectManager::setDefaultDarkColorStyle(void) {
	setColorStyle(my_colorStyles[1]);
}

ak::ui::colorStyle * ak::ui::objectManager::getCurrentColorStyle(void) const { return my_currentColorStyle; }

QString ak::ui::objectManager::getCurrentColorStyleName(void) const {
	if (my_currentColorStyle == nullptr) { return QString(); }
	return my_currentColorStyle->getColorStyleName();
}

void ak::ui::objectManager::destroyAll(void) {
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
	my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);

	my_mapObjectsIterator itm = my_mapObjects.begin();

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
}

std::string ak::ui::objectManager::getSettingsJSON(void) {
	// Prepare document
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

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

	// Collect data for objects that have to be restored
	for (my_mapAliasesIterator itm = my_mapAliases.begin(); itm != my_mapAliases.end(); itm++) {
		my_mapObjectsIterator obj = my_mapObjects.find(itm->second);
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

void ak::ui::objectManager::setupSettings(
	const char *										_json
) {
	rapidjson::Document doc;
	doc.Parse(_json);

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
		else {
			assert(obj.HasMember(RESTORABLE_NAME_ALIAS));			// Does not contain name
			assert(obj.HasMember(RESTORABLE_NAME_TYPE));			// Does not contain type
			assert(obj.HasMember(RESTORABLE_NAME_SETTINGS));		// Does not contain settings
			assert(obj[RESTORABLE_NAME_ALIAS].IsString());			// Alias is not a string
			assert(obj[RESTORABLE_NAME_TYPE].IsString());			// Type is not a string
			assert(obj[RESTORABLE_NAME_SETTINGS].IsObject());		// Settins is not an object

			QString objAlias = QString(obj[RESTORABLE_NAME_ALIAS].GetString());
			QString objType = QString(obj[RESTORABLE_NAME_TYPE].GetString());

			// Check if an object with the provided alias exist
			my_mapAliasesIterator oAlias = my_mapAliases.find(objAlias);
			assert(oAlias != my_mapAliases.end()); // No object with specified alias exists

			// Get the object
			my_mapObjectsIterator object = my_mapObjects.find(oAlias->second);
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
	}
}

// ###############################################################################################################################################

void ak::ui::objectManager::setColorStyle(
	ui::colorStyle *									_colorStyle
) {
	assert(_colorStyle != nullptr); // Nullptr provided
	my_currentColorStyle = _colorStyle;

	for (my_mapObjectsIterator obj = my_mapObjects.begin(); obj != my_mapObjects.end(); obj++) {
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
	my_mapCreatorsIterator itm = my_mapCreators.find(_creatorUid);
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
}

QWidget * ak::ui::objectManager::castToWidget(
	ak::UID												_objectUid
) {
	// Get widget
	my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
	assert(obj != my_mapObjects.end()); // Invalid object UID
	assert(obj->second->isWidgetType()); // Object is not a widget
	ak::ui::core::aWidget * widget = nullptr;
	widget = dynamic_cast<ak::ui::core::aWidget *>(obj->second);
	assert(widget != nullptr); // Cast failed
	return widget->widget();
}
