/*
 * ak_ui_objectManager.cpp
 *
 *  Created on: July 14, 2020
 *	Last modified on: September 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 */

 // AK header
#include <ak_ui_objectManager.h>			// corresponding header
#include <ak_messenger.h>					// messenger
#include <ak_notifierObjectManager.h>		// notifierObjectManager
#include <ak_exception.h>					// error handling
#include <ak_ui_uiManager.h>				// uiManager
#include <ak_ui_signalLinker.h>				// signalLinker
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_iconManager.h>				// icon manager
#include <ak_ui_core_aWidget.h>
#include <ak_ui_core_aObject.h>

// AK Qt objects
#include <ak_ui_qt_action.h>				// action
#include <ak_ui_qt_checkBox.h>				// checkBox
#include <ak_ui_qt_comboBox.h>				// comboBox
#include <ak_ui_qt_comboButton.h>			// comboButton
#include <ak_ui_qt_dock.h>					// dock
#include <ak_ui_qt_pushButton.h>			// pushButton
#include <ak_ui_qt_textEdit.h>				// textEdit
#include <ak_ui_qt_timer.h>

// AK widget objects
#include <ak_ui_widget_propertyGrid.h>		// propertyGrid
#include <ak_ui_widget_colorEditButton.h>	// colorEditButton
#include <ak_ui_widget_propertyGridItem.h>	// propertyGridItem
#include <ak_ui_widget_table.h>				// table
#include <ak_ui_widget_tabView.h>			// tabView
#include <ak_ui_widget_tree.h>				// tree

// AK ttb objects
#include <ak_ui_ttb_group.h>				// ttb Group
#include <ak_ui_ttb_page.h>					// ttb Page
#include <ak_ui_ttb_subgroup.h>				// ttb Subroup

ak::ui::objectManager::objectManager(
	ak::messenger *								_messenger,
	ak::uidManager *							_uidManager,
	ak::ui::iconManager *						_iconManager,
	ak::ui::colorStyle *						_colorStyle
)
	: my_colorStyle(nullptr),
	my_messenger(nullptr),
	my_signalLinker(nullptr),
	my_uidManager(nullptr),
	my_notifier(nullptr),
	my_iconManager(nullptr)
{
	try {
		// Check parameter
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger", ak::Exception::exceptionType::Nullptr); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager", ak::Exception::exceptionType::Nullptr); }
		if (_iconManager == nullptr) { throw ak::Exception("Is nullptr", "Check icon manager", ak::Exception::exceptionType::Nullptr); }

		// Apply settings
		my_messenger = _messenger;
		my_uidManager = _uidManager;
		my_colorStyle = _colorStyle;
		my_iconManager = _iconManager;

		try {
			// Create signal linker
			my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);
			if (my_signalLinker == nullptr) { throw ak::Exception("Failed to create", "Create signal linker"); }

			// Create notifier
			my_notifier = new ak::notifierObjectManager(this);
			if (my_notifier == nullptr) { throw ak::Exception("Failed to create", "Create notifier"); }
		}
		catch (const ak::Exception & e) {
			if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
			if (my_notifier != nullptr) { delete my_notifier; my_notifier = nullptr; }
			throw e; }
		catch (const std::exception & e) {
			if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
			if (my_notifier != nullptr) { delete my_notifier; my_notifier = nullptr; }
			throw e; }
		catch (...) {
			if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
			if (my_notifier != nullptr) { delete my_notifier; my_notifier = nullptr; }
			throw ak::Exception("Unknown error", ""); }
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
	try {
		// Create object
		ak::ui::qt::action * obj = new ak::ui::qt::action(_icon, _text, _popupMode);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		obj->setToolTip("Test information");
		// Set parameter
		my_signalLinker->addLink(obj);
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createAction()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createAction()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createAction()"); }
}

ak::UID ak::ui::objectManager::createCheckBox(
	ak::UID								_creatorUid,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::checkBox * obj = new ak::ui::qt::checkBox(_text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createCheckBox()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createCheckBox()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createCheckBox()"); }
}

ak::UID ak::ui::objectManager::createColorEditButton(
	ak::UID									_creatorUid,
	const ak::ui::color &								_color,
	const QString &										_textOverride
) {
	try {
		// Create object
		ak::ui::widget::colorEditButton * obj = new ak::ui::widget::colorEditButton(my_messenger, my_uidManager, _color, my_iconManager, _textOverride);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createColorEditButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createColorEditButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createColorEditButton()"); }
}

ak::UID ak::ui::objectManager::createComboBox(
	ak::UID									_creatorUid
) {
	try {
		// Create object
		ak::ui::qt::comboBox * obj = new ak::ui::qt::comboBox();
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createComboBox()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createComboBox()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createComboBox()"); }
}

ak::UID ak::ui::objectManager::createComboButton(
	ak::UID									_creatorUid,
	const QString &										_initialText,
	const std::vector<ak::ui::qt::comboButtonItem> &	_possibleSelection
) {
	try {
		// Create object
		ak::ui::qt::comboButton * obj = new ak::ui::qt::comboButton(_initialText, my_colorStyle);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		obj->setItems(_possibleSelection);
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createComboButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createComboButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createComboButton()"); }
}

ak::UID ak::ui::objectManager::createComboButtonItem(
	ak::UID								_creatorUid,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::comboButtonItem * obj = new ak::ui::qt::comboButtonItem(_text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		obj->setUid(my_uidManager->getId());
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createComboButtonItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createComboButtonItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createComboButtonItem()"); }
}

ak::UID ak::ui::objectManager::createComboButtonItem(
	ak::UID								_creatorUid,
	const QIcon &									_icon,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::comboButtonItem * obj = new ak::ui::qt::comboButtonItem(_icon, _text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create combo button item"); }
		// Set parameter
		obj->setUid(my_uidManager->getId());
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createComboButtonItem(icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createComboButtonItem(icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createComboButtonItem(icon)"); }
}

ak::UID ak::ui::objectManager::createDock(
	ak::UID								_creatorUid,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::dock * obj = new ak::ui::qt::dock(_text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create dock"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createDock()"); }
}

ak::UID ak::ui::objectManager::createPropertyGrid(
	ak::UID									_creatorUid
) {
	try {
		// Create object
		ak::ui::widget::propertyGrid * obj = new ak::ui::widget::propertyGrid(my_messenger, my_uidManager, my_iconManager, my_colorStyle);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create property grid"); }
		// Set parameter
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createPropertyGrid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createPropertyGrid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createPropertyGrid()"); }
}

ak::UID ak::ui::objectManager::createPushButton(
	ak::UID								_creatorUid,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::pushButton * obj = new ak::ui::qt::pushButton(_text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create push button"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createPushButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createPushButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createPushButton()"); }
}

ak::UID ak::ui::objectManager::createPushButton(
	ak::UID								_creatorUid,
	const QIcon &									_icon,
	const QString &									_text
) {
	try {
		// Create object
		ak::ui::qt::pushButton * obj = new ak::ui::qt::pushButton(_icon, _text);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create push button"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createPushButton(icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createPushButton(icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createPushButton(icon)"); }
}

ak::UID ak::ui::objectManager::createTable(
	ak::UID								_creatorUid,
	int												_rows,
	int												_columns
) {
	try {
		// Create object
		ak::ui::widget::table * obj = new ak::ui::widget::table(my_messenger, my_uidManager, this, my_iconManager, my_colorStyle, _rows, _columns);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create table"); }
		// Set parameter
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTable()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTable()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTable()"); }
}

ak::UID ak::ui::objectManager::createTextEdit(
	ak::UID								_creatorUid,
	const QString &									_initialText
) {
	try {
		// Create object
		ak::ui::qt::textEdit * obj = new ak::ui::qt::textEdit(_initialText);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create text edit"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTextEdit()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTextEdit()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTextEdit()"); }
}

ak::UID ak::ui::objectManager::createTimer(
	ak::UID												_creatorUid
) {
	try {
		// Create object
		ak::ui::qt::timer * obj = new ak::ui::qt::timer();
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create timer"); }
		// Set parameter
		my_signalLinker->addLink(obj);
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTimer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTimer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTimer()"); }
}

ak::UID ak::ui::objectManager::createTree(
	ak::UID									_creatorUid
) {
	try {
		// Create object
		ak::ui::widget::tree * obj = new ak::ui::widget::tree(my_messenger, my_uidManager, my_iconManager, my_colorStyle);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Set parameter
		if (my_colorStyle != nullptr) { obj->setColorStyle(my_colorStyle); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTree()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTree()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTree()"); }
}

ak::UID ak::ui::objectManager::createUiManager(
	ak::UID								_creatorUid
) {
	try {
		// Create new ui manager
		ak::ui::uiManager * ui = nullptr;
		ui = new ak::ui::uiManager(my_messenger, my_uidManager, my_iconManager, this, my_colorStyle);
		if (ui == nullptr) { throw ak::Exception("Failed to create", "Create ui manager"); }
		// Store data
		my_mapObjects.insert_or_assign(ui->uid(), ui);
		addCreatedUid(_creatorUid, ui->uid());
		return ui->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createUiManager()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createUiManager()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createUiManager()"); }
}

ak::UID ak::ui::objectManager::createTabView(
	ak::UID												_creatorUid
) {
	try {
		// Create object
		ak::ui::widget::tabView * obj = new ak::ui::widget::tabView(my_iconManager, my_messenger, my_uidManager, this, my_colorStyle);
		if (obj == nullptr) { throw ak::Exception("Failed to create", "Create action"); }
		// Store data
		my_mapObjects.insert_or_assign(obj->uid(), obj);
		addCreatedUid(_creatorUid, obj->uid());
		return obj->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTabView()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTabView()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTabView()"); }
}

ak::UID ak::ui::objectManager::createTabToolBarSubContainer(
	ak::UID								_creatorUid,
	ak::UID								_parentUid,
	const QString &									_text
) {
	try {
		// Find parent object
		my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check ui manager UID"); }
		// Check object type
		if (itm->second->objectType() != ak::ui::core::objectType::oTabToolbarPage &&
			itm->second->objectType() != ak::ui::core::objectType::oTabToolbarGroup &&
			itm->second->objectType() != ak::ui::core::objectType::oTabToolbarSubgroup
		) {
			// Check if the provided object is located at a main window
			if (itm->second->objectType() == ak::ui::core::objectType::oMainWindow) {
				return createTabToolBarPage(_creatorUid, _parentUid, _text);
			}

			throw ak::Exception("Invalid object type", "Check object type");
		}
		// Cast container
		ak::ui::core::ttbContainer * cont = nullptr;
		cont = dynamic_cast<ak::ui::core::ttbContainer *>(itm->second);
		if (cont == nullptr) { throw ak::Exception("Cast failed", "Cast container"); }
		// Create subcontainer
		ak::ui::core::ttbContainer * sub = cont->createSubContainer(_text);
		assert(sub != nullptr); // Invalid information received
		// Store data
		if (my_colorStyle != nullptr) { sub->setColorStyle(my_colorStyle); }
		my_mapObjects.insert_or_assign(sub->uid(), sub);
		addCreatedUid(_creatorUid, sub->uid());
		return sub->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTabToolBarSubContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTabToolBarSubContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTabToolBarSubContainer()"); }
}

ak::UID ak::ui::objectManager::createTabToolBarPage(
	ak::UID								_creatorUid,
	ak::UID								_uiManagerUid,
	const QString &									_text
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check ui manager UID"); }
		// Check object type
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) {
			throw ak::Exception("Invalid object type", "Check object type");
		}
		// Cast ui
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast ui manager"); }
		// Create container
		ak::ui::core::ttbContainer * cont = ui->createTabToolbarSubContainer(_text);
		assert(cont != nullptr); // Invalid information received
		// Store data
		if (my_colorStyle != nullptr) { cont->setColorStyle(my_colorStyle); }
		my_mapObjects.insert_or_assign(cont->uid(), cont);
		addCreatedUid(_creatorUid, cont->uid());
		return cont->uid();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::createTabToolBarPage()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::createTabToolBarPage()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::createTabToolBarPage()"); }
}

// ###############################################################################################################################################

// Object setter

void ak::ui::objectManager::obj_setAlias(
	ak::UID												_objectUid,
	const QString &										_alias
) {
	try {
		// Find parent object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		obj->second->setAlias(_alias);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setAlias()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setAlias()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setAlias()"); }
}

void ak::ui::objectManager::obj_addObjectToContainer(
	ak::UID								_parentUid,
	ak::UID								_objectUid
) {
	try {
		// Find parent object
		my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check parent UID"); }
		// Find object
		my_mapObjectsIterator itmO = my_mapObjects.find(_objectUid);
		if (itmO == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabToolbarPage:
		case ak::ui::core::objectType::oTabToolbarGroup:
		case ak::ui::core::objectType::oTabToolbarSubgroup:
		{
			// Cast container
			ak::ui::core::ttbContainer * cont = nullptr;
			cont = dynamic_cast<ak::ui::core::ttbContainer *>(itm->second);
			if (cont == nullptr) { throw ak::Exception("Cast failed", "Cast tab toolbar container"); }
			// Add object
			cont->addChild(itmO->second);
			break;
		}
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}

		
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addObjectToContainer()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addObjectToContainer()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addObjectToContainer()"); }
}

void ak::ui::objectManager::obj_setCentralWidget(
	ak::UID								_parentUid,
	ak::UID								_objectUid
) {
	try {
		// Find parent object
		my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check parent UID"); }
		// Find object
		my_mapObjectsIterator itmO = my_mapObjects.find(_objectUid);
		if (itmO == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (!itmO->second->isWidgetType()) { throw ak::Exception("Object is not widget type", "Check object type"); }
		ak::ui::core::aWidget * widget = dynamic_cast<ak::ui::core::aWidget *>(itmO->second);

		switch (itm->second->objectType())
		{
		case ak::ui::core::objectType::oDock:
		{
			// Cast dock
			ak::ui::qt::dock * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			obj->setWidget(widget->widget());
		}
		break;
		case ak::ui::core::objectType::oMainWindow:
		{
			// Cast dock
			ak::ui::uiManager * obj = nullptr;
			obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast ui manager"); }
			obj->setCentralWidget(widget->widget());
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check parent object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setCentralWidget(UID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setCentralWidget(UID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setCentralWidget(UID)"); }
}

void ak::ui::objectManager::obj_setCentralWidget(
	ak::UID									_parentUid,
	QWidget *								_widget
) {
	try {
		// Find parent object
		my_mapObjectsIterator itm = my_mapObjects.find(_parentUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check parent UID"); }

		switch (itm->second->objectType())
		{
		case ak::ui::core::objectType::oDock:
		{
			// Cast dock
			ak::ui::qt::dock * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			obj->setWidget(_widget);
		}
		break;
		case ak::ui::core::objectType::oMainWindow:
		{
			// Cast dock
			ak::ui::uiManager * obj = nullptr;
			obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast ui manager"); }
			obj->setCentralWidget(_widget);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check parent object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setCentralWidget(QWidget)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setCentralWidget(QWidget)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setCentralWidget(QWidget)"); }
}

void ak::ui::objectManager::obj_setText(
	ak::UID									_objectUid,
	const QString &										_text
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			obj->setText(_text);
		}
		break;
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			obj->setText(_text);
		}
		break;
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::colorEditButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			obj->overrideText(_text);
		}
		break;
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			obj->setText(_text);
		}
		break;
		case ak::ui::core::objectType::oComboButtonItem:
		{
			// Cast object
			ak::ui::qt::comboButtonItem * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button item"); }
			obj->setText(_text);
		}
		break;
		case ak::ui::core::objectType::oDock:
		{
			// Cast object
			ak::ui::qt::dock * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			obj->setWindowIconText(_text);
		}
		break;
		case ak::ui::core::objectType::oMainWindow:
		{
			// Cast object
			ak::ui::uiManager * obj = nullptr;
			obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast main window"); }
			obj->setWindowTitle(_text);
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			obj->setText(_text);
		}
		break;
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			obj->setText(_text);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setText()"); }
}

void ak::ui::objectManager::obj_setChecked(
	ak::UID									_objectUid,
	bool												_checked
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			obj->setChecked(_checked);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setChecked()"); }
}

void ak::ui::objectManager::obj_setTristate(
	ak::UID									_objectUid,
	bool												_isTristate
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			obj->setTristate(_isTristate);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setTristate()"); }
}

void ak::ui::objectManager::obj_setFilterVisible(
	ak::UID									_objectUid,
	bool												_vis
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			obj->setFilterVisible(_vis);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setFilterVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setFilterVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setFilterVisible()"); }
}

void ak::ui::objectManager::obj_setFilterCaseSensitive(
	ak::UID												_objectUid,
	bool												_caseSensitive,
	bool												_refresh
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			obj->setFilterCaseSensitive(_caseSensitive, _refresh);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setFilterCaseSensitive()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setFilterCaseSensitive()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setFilterCaseSensitive()"); }
}

void ak::ui::objectManager::obj_setFilterRefreshOnChange(
	ak::UID												_objectUid,
	bool												_refreshOnChange
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			obj->setFilterRefreshOnChange(_refreshOnChange);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setFilterRefreshOnChange()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setFilterRefreshOnChange()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setFilterRefreshOnChange()"); }
}

void ak::ui::objectManager::obj_setItems(
	ak::UID									_objectUid,
	const std::vector<ak::ui::qt::comboButtonItem> &	_items
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			obj->setItems(_items);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setItems(comboButtonItems)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setItems(comboButtonItems)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setItems(comboButtonItems)"); }
}

void ak::ui::objectManager::obj_setColor(
	ak::UID									_objectUid,
	const ak::ui::color &								_color
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::colorEditButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			obj->setColor(_color);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setColor()"); }
}

void ak::ui::objectManager::obj_setIcon(
	ak::UID									_objectUid,
	const QIcon &										_icon
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			obj->setIcon(_icon);
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			obj->setIcon(_icon);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setIcon()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setIcon()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setIcon()"); }
}

void ak::ui::objectManager::obj_setReadOnly(
	ak::UID									_objectUid,
	bool									_readOnly
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast textEdit"); }
			obj->setReadOnly(_readOnly);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setReadOnly()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setReadOnly()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setReadOnly()"); }
}

void ak::ui::objectManager::obj_appendText(
	ak::UID									_objectUid,
	const QString &										_text
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			obj->setText(obj->text() + _text);
		}
		break;
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			obj->setText(obj->text() + _text);
		}
		break;
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			obj->setText(obj->text() + _text);
		}
		break;
		case ak::ui::core::objectType::oComboButtonItem:
		{
			// Cast object
			ak::ui::qt::comboButtonItem * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button item"); }
			obj->setText(obj->text() + _text);
		}
		break;
		case ak::ui::core::objectType::oDock:
		{
			// Cast object
			ak::ui::qt::dock * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			obj->setWindowIconText(obj->windowIconText() + _text);
		}
		break;
		case ak::ui::core::objectType::oMainWindow:
		{
			// Cast object
			ak::ui::uiManager * obj = nullptr;
			obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast main window"); }
			obj->setWindowTitle(obj->windowTitle() + _text);
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			obj->setText(obj->text() + _text);
		}
		break;
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			obj->append(_text);
			
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_appendText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_appendText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_appendText()"); }
}

void ak::ui::objectManager::obj_setAutoScrollToBottomEnabled(
	ak::UID									_objectUid,
	bool									_enabled
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			obj->setAutoScrollToBottom(_enabled);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setAutoScrollToBottomEnabled()"); }
}

void ak::ui::objectManager::obj_setTabToolBarVisible(
	ak::UID									_uiManagerUid,
	bool												_vis
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) {
			throw ak::Exception("Invalid object type, expected MainWindow", "Check object type");
		}
		// Cast ui manager
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setTabToolBarVisible(_vis);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setTabToolBarVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setTabToolBarVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setTabToolBarVisible()"); }
}

void ak::ui::objectManager::obj_setTabLocation(
	ak::UID											_objectUid,
	ak::ui::core::tabLocation						_location
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			obj->setTabLocation(_location);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setTabLocation()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setTabLocation()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setTabLocation()"); }
}

void ak::ui::objectManager::obj_setTabFocused(
	ak::UID											_objectUid,
	ak::ID											_tab
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			obj->focusTab(_tab);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setTabFocused()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setTabFocused()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setTabFocused()"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	bool											_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(bool)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(bool)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(bool)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	int												_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(int)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(int)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(int)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	double											_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(double)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(double)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(double)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const char *									_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(char *)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(char *)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(char *)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const QString &									_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(QString)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(QString)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(QString)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID								_objectUid,
	const QString &									_itemName,
	const ak::ui::color &								_value,
	bool											_isMultipleValues
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _value, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(color)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(color)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(color)"); }
}

void ak::ui::objectManager::obj_addProperty(
	ak::UID												_objectUid,
	const QString &										_itemName,
	const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
	const QString &										_selectedValue,
	bool												_isMultipleValues
) {
	try {
		if (_selection.size() == 0) { throw ak::Exception("Selection is empty", "Check selection count"); }
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oPropertyGrid) {
			throw ak::Exception("Invalid object type, expected property grid", "Check object type");
		}
		// Cast property grid
		ak::ui::widget::propertyGrid * p = nullptr;
		p = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
		if (p == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
		// Add property
		p->createItem(_itemName, _selection, _selectedValue, _isMultipleValues);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addProperty(comboButtonItem)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addProperty(comboButtonItem)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addProperty(comboButtonItem)"); }
}

ak::ID ak::ui::objectManager::obj_createItem(
	ak::UID											_objectUid,
	ak::ID											_parentId,
	const QString &									_text,
	ak::ui::core::textAlignment						_textAlignment,
	QIcon											_icon
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oTree) {
			throw ak::Exception("Invalid object type, expected tree", "Check object type");
		}
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
		// Add object
		return t->add(_parentId, _text, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_createItem(parent)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_createItem(parent)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_createItem(parent)"); }
}

ak::ID ak::ui::objectManager::obj_createItem(
	ak::UID											_objectUid,
	const QString &									_cmd,
	char											_delimiter,
	ak::ui::core::textAlignment						_textAlignment,
	const QIcon  &									_icon
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oTree) {
			throw ak::Exception("Invalid object type, expected tree", "Check object type");
		}
		// Cast tree
		ak::ui::widget::tree * t = nullptr;
		t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
		if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
		// Add object
		return t->add(_cmd, _delimiter, _textAlignment, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_createItem(command)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_createItem(command)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_createItem(command)"); }
}

void ak::ui::objectManager::obj_setMultiSelectionEnabled(
	ak::UID											_objectUid,
	bool											_multiSelection
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setMultiSelectionEnabled(_multiSelection);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setMultiSelectionEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setMultiSelectionEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setMultiSelectionEnabled()"); }
}

void ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled(
	ak::UID											_objectUid,
	bool											_enabled
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setSelectAndDeselectChildrenEnabled(_enabled);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
}

void ak::ui::objectManager::obj_setAutoExpandSelectedItems(
	ak::UID											_objectUid,
	bool											_enabled
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setAutoExpandSelectedItemsEnabled(_enabled);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setAutoExpandSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setAutoExpandSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setAutoExpandSelectedItems()"); }
}

void ak::ui::objectManager::obj_expandAllItems(
	ak::UID											_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->expandAllItems();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_expandAllItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_expandAllItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_expandAllItems()"); }
}

void ak::ui::objectManager::obj_collapseAllItems(
	ak::UID											_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->collapseAllItems();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_collapseAllItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_collapseAllItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_collapseAllItems()"); }
}

void ak::ui::objectManager::obj_deselectAllItems(
	ak::UID											_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->deselectAllItems(true);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_deselectAllItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_deselectAllItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_deselectAllItems()"); }
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID											_objectUid,
	ak::UID											_widgetUid,
	const QString &									_title,
	const QIcon &									_icon
) {
	try {
		QWidget * Widget = castToWidget(_widgetUid);
		return obj_addTab(_objectUid, Widget, _title, _icon);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addTab(UID)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addTab(UID)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addTab(UID)"); }
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID											_objectUid,
	QWidget *										_widget,
	const QString &									_title,
	const QIcon &									_icon
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTabView:
		{
			// Cast tree
			ak::ui::widget::tabView * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tabView"); }
			// Add object
			return t->createTab(_widget, _title, _icon);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addTab()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addTab()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addTab()"); }
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID											_objectUid,
	ak::UID											_widgetUid,
	const QString &									_title,
	const QString &									_iconName,
	const QString &									_iconSize
) {
	try {
		return obj_addTab(_objectUid, _widgetUid, _title, *my_iconManager->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addTab(icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addTab(icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addTab(icon)"); }
}

ak::ID ak::ui::objectManager::obj_addTab(
	ak::UID											_objectUid,
	QWidget *										_widget,
	const QString &									_title,
	const QString &									_iconName,
	const QString &									_iconSize
) {
	try {
		return obj_addTab(_objectUid, _widget, _title, *my_iconManager->icon(_iconName, _iconSize));
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_addTab(widget, icon)"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_addTab(widget, icon)"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_addTab(widget, icon)"); }
}

void ak::ui::objectManager::obj_setEnabled(
	ak::UID											_objectUid,
	bool											_enabled
) {
	try {
		// Find object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * o = nullptr;
			o = dynamic_cast<ak::ui::qt::action *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * o = nullptr;
			o = dynamic_cast<ak::ui::qt::checkBox *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * o = nullptr;
			o = dynamic_cast<ak::ui::widget::colorEditButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * o = nullptr;
			o = dynamic_cast<ak::ui::qt::comboButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oComboButtonItem:
		{
			// Cast object
			ak::ui::qt::comboButtonItem * o = nullptr;
			o = dynamic_cast<ak::ui::qt::comboButtonItem *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast combo button item"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oDock:
		{
			// Cast object
			ak::ui::qt::dock * o = nullptr;
			o = dynamic_cast<ak::ui::qt::dock *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * o = nullptr;
			o = dynamic_cast<ak::ui::qt::pushButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			o->setEnabled(_enabled);
		}
		break;
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * o = nullptr;
			o = dynamic_cast<ak::ui::qt::textEdit *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			o->setEnabled(_enabled);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_setAutoSelectAndDeselectChildrenEnabled()"); }
}

void ak::ui::objectManager::obj_clear(
	ak::UID											_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			// Add object
			return t->clear();
		}
		break;
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast tree
			ak::ui::widget::propertyGrid * t = nullptr;
			t = dynamic_cast<ak::ui::widget::propertyGrid *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			// Add object
			return t->clear();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_clear()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_clear()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_clear()"); }
}

void ak::ui::objectManager::obj_deleteItems(
	ak::UID												_objectUid,
	const std::vector<ak::ID> &							_items
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			// Add object
			t->deleteItems(_items);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_deleteItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_deleteItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_deleteItems()"); }
}

void ak::ui::objectManager::obj_start(
	ak::UID												_objectUid,
	int													_value
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTimer:
		{
			// Cast tree
			ak::ui::qt::timer * t = nullptr;
			t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast timer"); }
			// Add object
			t->setInterval(_value);
			t->setSingleShot(false);
			t->start();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_start()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_start()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_start()"); }
}

void ak::ui::objectManager::obj_stop(
	ak::UID												_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTimer:
		{
			// Cast tree
			ak::ui::qt::timer * t = nullptr;
			t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast timer"); }
			// Add object
			t->stop();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_stop()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_stop()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_stop()"); }
}

void ak::ui::objectManager::obj_shoot(
	ak::UID												_objectUid,
	int													_value
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTimer:
		{
			// Cast tree
			ak::ui::qt::timer * t = nullptr;
			t = dynamic_cast<ak::ui::qt::timer *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast timer"); }
			// Add object
			t->setInterval(_value);
			t->setSingleShot(true);
			t->start();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
			break;
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_shoot()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_shoot()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_shoot()"); }
}

void ak::ui::objectManager::obj_delete(
	ak::UID									_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		// Check if is UI manager
		if (obj->second->objectType() == ak::ui::core::objectType::oMainWindow) {
			ak::ui::uiManager * ui = nullptr;
			ui = dynamic_cast<ak::ui::uiManager *>(obj->second);
			if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
			ui->close();
		}
		// Destroy object
		ak::ui::core::aObject * o = obj->second;
		delete o;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_delete()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_delete()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_delete()"); }
}

// ###############################################################################################################################################

// Object getter

QString ak::ui::objectManager::obj_getAlias(
	ak::UID												_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		return obj->second->alias();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getAlias()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getAlias()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getAlias()"); }
}

ak::ui::core::aObject * ak::ui::objectManager::obj_get(
	ak::UID									_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		return obj->second;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_get()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_get()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_get()"); }
}

QWidget * ak::ui::objectManager::obj_getWidget(
	ak::UID									_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }

		// Check type
		if (!obj->second->isWidgetType()) { throw ak::Exception("Object is not widget type", "Check object type"); }

		// Cast widget
		ak::ui::core::aWidget * w = nullptr;
		w = dynamic_cast<ak::ui::core::aWidget *>(obj->second);

		// Check cast
		if (w == nullptr) { throw ak::Exception("Cast failed", "Cast widget"); }

		return w->widget();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getWidget()"); }
}

QAction * ak::ui::objectManager::obj_getAction(
	ak::UID									_actionUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_actionUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (obj->second->objectType() != ak::ui::core::objectType::oAction) { throw ak::Exception("Invalid object type", "Check object type"); }

		// Cast action
		QAction * ret = nullptr;
		ret = dynamic_cast<QAction *>(obj->second);
		if (ret == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
		return ret;
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getAction()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getAction()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getAction()"); }
}

ak::ui::core::objectType ak::ui::objectManager::obj_getType(
	ak::UID									_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		return obj->second->objectType();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getType()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getType()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getType()"); }
}

QString ak::ui::objectManager::obj_getText(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::action *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			return obj->text();
		}
		break;
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			return obj->text();
		}
		break;
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::colorEditButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			return obj->color().toRGBString(".");
		}
		break;
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			return obj->text();
		}
		break;
		case ak::ui::core::objectType::oComboButtonItem:
		{
			// Cast object
			ak::ui::qt::comboButtonItem * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::comboButtonItem *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast combo button item"); }
			return obj->text();
		}
		break;
		case ak::ui::core::objectType::oDock:
		{
			// Cast object
			ak::ui::qt::dock * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::dock *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			return obj->windowIconText();
		}
		break;
		case ak::ui::core::objectType::oMainWindow:
		{
			// Cast object
			ak::ui::uiManager * obj = nullptr;
			obj = dynamic_cast<ak::ui::uiManager *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast main window"); }
			return obj->windowTitle();
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::pushButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			return obj->text();
		}
		break;
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			return obj->toPlainText();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getText()"); }
}

bool ak::ui::objectManager::obj_getTristate(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast check box"); }
			return obj->isTristate();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getTristate()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getTristate()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getTristate()"); }
}

bool ak::ui::objectManager::obj_getChecked(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::checkBox *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast check box"); }
			return obj->isChecked();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getChecked()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getChecked()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getChecked()"); }
}

ak::ui::color ak::ui::objectManager::obj_getColor(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::colorEditButton *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			return obj->color();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getColor()"); }
}

std::vector<ak::ID> ak::ui::objectManager::obj_getSelectedItems(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->selectedItems();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getSelectedItems()"); }
}

bool ak::ui::objectManager::obj_getAutoScrollToBottomEnabled(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * obj = nullptr;
			obj = dynamic_cast<ak::ui::qt::textEdit *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			return obj->autoScrollToBottom();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getAutoScrollToBottomEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getAutoScrollToBottomEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getAutoScrollToBottomEnabled()"); }
}

bool ak::ui::objectManager::obj_getEnabled(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType()) {
		case ak::ui::core::objectType::oAction:
		{
			// Cast object
			ak::ui::qt::action * o = nullptr;
			o = dynamic_cast<ak::ui::qt::action *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast action"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oCheckBox:
		{
			// Cast object
			ak::ui::qt::checkBox * o = nullptr;
			o = dynamic_cast<ak::ui::qt::checkBox *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast checkbox"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oColorEditButton:
		{
			// Cast object
			ak::ui::widget::colorEditButton * o = nullptr;
			o = dynamic_cast<ak::ui::widget::colorEditButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast color edit button"); }
			return o->enabled();
		}
		break;
		case ak::ui::core::objectType::oComboButton:
		{
			// Cast object
			ak::ui::qt::comboButton * o = nullptr;
			o = dynamic_cast<ak::ui::qt::comboButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast combo button"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oComboButtonItem:
		{
			// Cast object
			ak::ui::qt::comboButtonItem * o = nullptr;
			o = dynamic_cast<ak::ui::qt::comboButtonItem *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast combo button item"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oDock:
		{
			// Cast object
			ak::ui::qt::dock * o = nullptr;
			o = dynamic_cast<ak::ui::qt::dock *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oPushButton:
		{
			// Cast object
			ak::ui::qt::pushButton * o = nullptr;
			o = dynamic_cast<ak::ui::qt::pushButton *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast push button"); }
			return o->isEnabled();
		}
		break;
		case ak::ui::core::objectType::oTextEdit:
		{
			// Cast object
			ak::ui::qt::textEdit * o = nullptr;
			o = dynamic_cast<ak::ui::qt::textEdit *>(obj->second);
			if (o == nullptr) { throw ak::Exception("Cast failed", "Cast text edit"); }
			return o->isEnabled();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getEnabled()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getEnabled()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getEnabled()"); }
}

ak::ID ak::ui::objectManager::obj_getItem(
	ak::UID									_objectUid,
	const QString &							_text
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			std::vector<QString> titles = obj->tabTitles();
			for (int i = 0; i < titles.size(); i++) {
				if (titles.at(i) == _text) { return i; }
			}
			return ak::invalidID;
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getItem()"); }
}

int ak::ui::objectManager::obj_getItemCount(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			return obj->tabCount();
		}
		break;
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->itemCount();
		}
		break;
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			return obj->itemCount();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getItemCount()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getItemCount()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getItemCount()"); }
}

int ak::ui::objectManager::obj_getFocusedTab(
	ak::UID									_objectUid
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			return obj->focusedTab();
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getFocusedTab()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getFocusedTab()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getFocusedTab()"); }
}

bool ak::ui::objectManager::obj_getAutoExpandSelectedItems(
	ak::UID											_objectUid
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			return t->autoExpandSelectedItemsEnabled();
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::obj_getAutoExpandSelectedItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::obj_getAutoExpandSelectedItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::obj_getAutoExpandSelectedItems()"); }
}

// ###############################################################################################################################################

// Item setter

void ak::ui::objectManager::itm_setSelected(
	ak::UID											_objectUid,
	ak::ID											_itemId,
	bool											_selected
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setItemSelected(_itemId, _selected);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_setSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_setSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_setSelected()"); }
}

void ak::ui::objectManager::itm_setSingleSelected(
	ak::UID											_objectUid,
	ak::ID											_itemId,
	bool											_selected
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setSingleItemSelected(_itemId, _selected);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_setSingleSelected()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_setSingleSelected()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_setSingleSelected()"); }
}

void ak::ui::objectManager::itm_toggleSelection(
	ak::UID											_objectUid,
	ak::ID											_itemId
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->toggleItemSelection(_itemId);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_toggleSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_toggleSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_toggleSelection()"); }
}

void ak::ui::objectManager::itm_setText(
	ak::UID											_objectUid,
	ak::ID											_itemId,
	const QString &									_text
) {
	try {
		// Get object
		my_mapObjectsIterator obj = my_mapObjects.find(_objectUid);
		if (obj == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (obj->second->objectType())
		{
		case ak::ui::core::objectType::oTree:
		{
			// Cast tree
			ak::ui::widget::tree * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tree *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }

			t->setItemText(_itemId, _text);
		}
		break;
		case ak::ui::core::objectType::oTabView:
		{
			// Cast tree
			ak::ui::widget::tabView * t = nullptr;
			t = dynamic_cast<ak::ui::widget::tabView *>(obj->second);
			if (t == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			t->setTabText(_itemId, _text);
		}
		break;
		default:
			throw ak::Exception("Invalid object type", "Check object type");
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_setText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_setText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_setText()"); }
}

// ###############################################################################################################################################

// Item getter

std::vector<QString> ak::ui::objectManager::itm_getPath(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->getItemPath(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getPath()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getPath()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getPath()"); }
}

QString ak::ui::objectManager::itm_getPathString(
	ak::UID									_objectUid,
	ak::ID									_itemId,
	char									_delimiter
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->getItemPathString(_itemId, _delimiter);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getPathString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getPathString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getPathString()"); }
}

QString ak::ui::objectManager::itm_getText(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->getItemText(_itemId);
		}
		break;
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getPropertyName(_itemId);
		}
		break;
		case ak::ui::core::objectType::oTabView:
		{
			// Cast object
			ak::ui::widget::tabView * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tabView *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tab view"); }
			return obj->tabText(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getText()"); }
}

ak::core::valueType ak::ui::objectManager::itm_getValueType(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getValueType(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueType()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueType()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueType()"); }
}

ak::ID ak::ui::objectManager::itm_getID(
	ak::UID									_objectUid,
	const QString &							_itemPath,
	char									_delimiter
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oTree:
		{
			// Cast object
			ak::ui::widget::tree * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::tree *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast tree"); }
			return obj->getItemID(_itemPath, _delimiter);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getID()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getID()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getID()"); }
}

bool ak::ui::objectManager::itm_getValueBoolean(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getBool(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueBoolean()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueBoolean()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueBoolean()"); }
}

ak::ui::color ak::ui::objectManager::itm_getValueColor(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getColor(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueColor()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueColor()"); }
}

double ak::ui::objectManager::itm_getValueDouble(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getDouble(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueDouble()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueDouble()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueDouble()"); }
}

int ak::ui::objectManager::itm_getValueInteger(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getInt(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueInteger()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueInteger()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueInteger()"); }
}

std::vector<QString> ak::ui::objectManager::itm_getValuePossibleSelection(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getSelectionPossibleValues(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValuePossibleSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValuePossibleSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValuePossibleSelection()"); }
}

QString ak::ui::objectManager::itm_getValueSelection(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getSelection(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueSelection()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueSelection()"); }
}

QString ak::ui::objectManager::itm_getValueString(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getString(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueString()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueString()"); }
}

bool ak::ui::objectManager::itm_getValueIsMultivalued(
	ak::UID									_objectUid,
	ak::ID									_itemId
) {
	try {
		// Find object
		my_mapObjectsIterator itm = my_mapObjects.find(_objectUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		switch (itm->second->objectType()) {
		case ak::ui::core::objectType::oPropertyGrid:
		{
			// Cast object
			ak::ui::widget::propertyGrid * obj = nullptr;
			obj = dynamic_cast<ak::ui::widget::propertyGrid *>(itm->second);
			if (obj == nullptr) { throw ak::Exception("Cast failed", "Cast property grid"); }
			return obj->getIsMultivalued(_itemId);
		}
		break;
		default: throw ak::Exception("Invalid object type", "Check object type");
		}

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::itm_getValueIsMultivalued()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::itm_getValueIsMultivalued()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::itm_getValueIsMultivalued()"); }
}

// ###############################################################################################################################################

void ak::ui::objectManager::addDock(
	ak::UID									_uiManagerUid,
	ak::UID									_dockUid,
	ak::ui::core::dockLocation				_dockLocation
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		// Find dock
		itm = my_mapObjects.find(_dockUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check dock UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oDock) { throw ak::Exception("Invalid object type, expected Dock", "Check object type"); }
		// Cast dock
		ak::ui::qt::dock * d = nullptr;
		d = dynamic_cast<ak::ui::qt::dock *>(itm->second);
		if (d == nullptr) { throw ak::Exception("Cast failed", "Cast dock"); }

		// Add dock
		ui->addDock(d, _dockLocation);

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::addDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::addDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::addDock()"); }
}

void ak::ui::objectManager::tabifyDock(
	ak::UID									_uiManagerUid,
	ak::UID									_parentUid,
	ak::UID									_dockUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->tabifyDock(_parentUid, _dockUid);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::tabifyDock()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::tabifyDock()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::tabifyDock()"); }
}

void ak::ui::objectManager::setDockBottomLeftPriority(
	ak::UID									_uiManagerUid,
	ak::ui::core::dockLocation				_dockLocation
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setDockPriorityBottomLeft(_dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setDockBottomLeftPriority()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setDockBottomLeftPriority()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setDockBottomLeftPriority()"); }
}

void ak::ui::objectManager::setDockBottomRightPriority(
	ak::UID									_uiManagerUid,
	ak::ui::core::dockLocation				_dockLocation
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setDockPriorityBottomRight(_dockLocation);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setDockBottomRightPriority()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setDockBottomRightPriority()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setDockBottomRightPriority()"); }
}

void ak::ui::objectManager::setStatusLabelVisible(
	ak::UID									_uiManagerUid,
	bool									_visible
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setStatusLabelVisible(_visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setStatusLabelVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setStatusLabelVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setStatusLabelVisible()"); }
}

void ak::ui::objectManager::setStatusProgressVisible(
	ak::UID									_uiManagerUid,
	bool									_visible
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setStatusBarVisible(_visible);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setStatusProgressVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setStatusProgressVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setStatusProgressVisible()"); }
}

void ak::ui::objectManager::setStatusLabelText(
	ak::UID									_uiManagerUid,
	const QString &							_text
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setStatusLabelText(_text);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setStatusLabelText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setStatusLabelText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setStatusLabelText()"); }
}

void ak::ui::objectManager::setStatusProgressValue(
	ak::UID									_uiManagerUid,
	int										_value
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setStatusBarProgress(_value);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setStatusProgressValue()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setStatusProgressValue()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setStatusProgressValue()"); }
}

void ak::ui::objectManager::setStatusProgressContinuous(
	ak::UID									_uiManagerUid,
	bool									_continuous
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->setStatusBarContinuous(_continuous);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setStatusProgressContinuous()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setStatusProgressContinuous()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setStatusProgressContinuous()"); }
}

bool ak::ui::objectManager::getStatusLabelVisible(
	ak::UID									_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }
		
		return ui->getStatusLabelVisible();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::getStatusLabelVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::getStatusLabelVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::getStatusLabelVisible()"); }
}

bool ak::ui::objectManager::getStatusProgressVisible(
	ak::UID									_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		return ui->getStatusBarVisible();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::getStatusProgressVisible()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::getStatusProgressVisible()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::getStatusProgressVisible()"); }
}

QString ak::ui::objectManager::getStatusLabelText(
	ak::UID									_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		return ui->getStatusLabelText();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::getStatusLabelText()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::getStatusLabelText()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::getStatusLabelText()"); }
}

int ak::ui::objectManager::getStatusProgressValue(
	ak::UID									_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		return ui->getStatusBarProgress();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::getStatusProgressValue()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::getStatusProgressValue()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::getStatusProgressValue()"); }
}

bool ak::ui::objectManager::getStatusProgressContinuous(
	ak::UID									_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		return ui->getStatusBarContinuous();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::getStatusProgressContinuous()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::getStatusProgressContinuous()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::getStatusProgressContinuous()"); }
}

void ak::ui::objectManager::close(
	ak::UID												_uiManagerUid
) {
	try {
		// Find ui manager
		my_mapObjectsIterator itm = my_mapObjects.find(_uiManagerUid);
		if (itm == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check UI manager UID"); }
		if (itm->second->objectType() != ak::ui::core::objectType::oMainWindow) { throw ak::Exception("Invalid object type, expected MainWindow", "Check object type"); }
		// Cast UI
		ak::ui::uiManager * ui = nullptr;
		ui = dynamic_cast<ak::ui::uiManager *>(itm->second);
		if (ui == nullptr) { throw ak::Exception("Cast failed", "Cast UI manager"); }

		ui->close();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::close()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::close()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::close()"); }
}

// ###############################################################################################################################################

void ak::ui::objectManager::creatorDestroyed(
	ak::UID									_creatorUid
) {
	try {
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
		catch (const ak::Exception & e) { my_notifier->enable(); throw e; }
		catch (const std::exception & e) { my_notifier->enable(); throw e; }
		catch (...) { my_notifier->enable(); throw ak::Exception("Unknown error", "Perform delete"); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::creatorDestroyed()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::creatorDestroyed()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::creatorDestroyed()"); }
}

void ak::ui::objectManager::setColorStyle(
	ak::ui::colorStyle *								_colorStyle
) {
	try {
		if (_colorStyle == nullptr) { throw ak::Exception("Is nullptr", "Check colorStyle"); }
		my_colorStyle = _colorStyle;

		for (my_mapObjectsIterator obj = my_mapObjects.begin(); obj != my_mapObjects.end(); obj++) {
			if (obj->second->isPaintableType()) {
				// Cast paintable
				ak::ui::core::aPaintable * itm = nullptr;
				itm = dynamic_cast<ak::ui::core::aPaintable *>(obj->second);
				if (itm == nullptr) { throw ak::Exception("Cast failed", "Cast paintable"); }
				itm->setColorStyle(my_colorStyle);
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::setColorStyle()"); }
}

void ak::ui::objectManager::destroyAll(void) {
	try {

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
			} else {
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
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::destroyAll()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::destroyAll()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::destroyAll()"); }
}

// ###############################################################################################################################################

void ak::ui::objectManager::addCreatedUid(
	ak::UID									_creatorUid,
	ak::UID									_createdUid
) {
	try {
		my_mapCreatorsIterator itm = my_mapCreators.find(_creatorUid);
		if (itm == my_mapCreators.end()) {
			// First object created by this creator
			std::vector<ak::UID> *	v = nullptr;
			v = new (std::nothrow) std::vector<ak::UID>();
			if (v == nullptr) { throw ak::Exception("Failed to create", "Create creator vector", ak::Exception::exceptionType::BadAlloc); }

			// Store data
			v->push_back(_createdUid);
			my_mapCreators.insert_or_assign(_creatorUid, v);
		}
		else {
			// Object(s) already created by this creator before

			// Check for duplicate
			for (int i = 0; i < itm->second->size(); i++) {
				if (itm->second->at(i) == _createdUid) { throw ak::Exception("UID already stored", "Check created UID", ak::Exception::exceptionType::InvalidInput); }
			}
			// Store data
			itm->second->push_back(_createdUid);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::addCreatedUid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::addCreatedUid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::addCreatedUid()"); }
}

QWidget * ak::ui::objectManager::castToWidget(
	ak::UID									_objectUid
) {
	try {
		// Get widget
		my_mapObjectsIterator wid = my_mapObjects.find(_objectUid);
		if (wid == my_mapObjects.end()) { throw ak::Exception("Invalid UID", "Check object UID"); }
		if (!wid->second->isWidgetType()) { throw ak::Exception("Provided object is not widget type", "Check object type"); }
		ak::ui::core::aWidget * Widget = nullptr;
		Widget = dynamic_cast<ak::ui::core::aWidget *>(wid->second);
		if (Widget == nullptr) { throw ak::Exception("Cast failed", "Cast widget"); }
		return Widget->widget();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::objectManager::castToWidget()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::objectManager::castToWidget()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::objectManager::castToWidget()"); }
}
