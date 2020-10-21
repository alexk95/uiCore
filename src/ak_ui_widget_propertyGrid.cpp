/*
 * ak_ui_widget_propertyGrid.cpp
 *
 *  Created on: March 29, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_widget_propertyGrid.h>		// corresponding class
#include <ak_exception.h>					// error handling
#include <ak_notifierPropertyGrid.h>		// notifierPropertyGrid
#include <ak_ui_colorStyle.h>				// colorStyle
#include <ak_ui_core.h>						// objectType
#include <ak_ui_widget_propertyGridItem.h>	// propertyGridItem
#include <ak_ui_widget_table.h>				// table
#include <ak_messenger.h>					// messenger
#include <ak_uidMangager.h>					// UID manager
#include <ak_ui_signalLinker.h>

// Widgets and qt items
#include <ak_ui_qt_checkBox.h>
#include <ak_ui_qt_comboButton.h>

#include <ak_ui_widget_colorEditButton.h>

// Qt header
#include <qcolordialog.h>					// QColorDialog
#include <qheaderview.h>					// QHeaderView
#include <qmessagebox.h>					// QMessageBox

ak::ui::widget::propertyGrid::propertyGrid(
	ak::messenger *						_messenger,
	ak::uidManager *					_uidManager,
	const ak::ui::colorStyle *			_colorStyle
) : ak::ui::core::aWidgetManager(ak::ui::core::objectType::oPropertyGrid, nullptr, nullptr, _colorStyle),
	my_externalMessanger(nullptr), my_table(nullptr), my_externalUidManager(nullptr),
	my_showMessageboxOnSyntaxError(true), my_signalLinker(nullptr)
{
	try {
		
		// Check arguments
		if (_messenger == nullptr) { throw ak::Exception("Is nullptr", "Check messenger"); }
		if (_uidManager == nullptr) { throw ak::Exception("Is nullptr", "Check UID manager"); }

		my_signalLinker = new ui::signalLinker(my_messenger, my_uidManager);

		// Set variables
		my_externalMessanger = _messenger;
		my_externalUidManager = _uidManager;
		my_uid = my_externalUidManager->getId();
		my_colorStyle = _colorStyle;

		// Create new table
		my_table = new ak::ui::widget::table(my_messenger, my_uidManager, my_colorStyle, 0, 2);
		if (my_table == nullptr) { throw ak::Exception("Failed to create", "Create table"); }
	
		// Set colorstyle
		if (my_colorStyle != nullptr) { setColorStyle(my_colorStyle); }

		// Initialize table
		my_table->setColumnHeader(0, "Property");
		my_table->setColumnHeader(1, "Value");
		my_table->setRowHeaderVisible(false);
		my_table->setColumnResizeMode(0, QHeaderView::ResizeMode::Stretch);
		my_table->setColumnResizeMode(1, QHeaderView::ResizeMode::Stretch);
		my_table->setColumnHeaderEnabled(false);
		
		// Create notifier
		my_internalNotifier = new ak::notifierPropertyGrid(this, my_table->uid());
		if (my_internalNotifier == nullptr) { throw ak::Exception("Failed to create", "Create internal notifier"); }

		// Register notifier
		my_messenger->registerUidReceiver(my_table->uid(), my_internalNotifier);

	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::propertyGrid()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::propertyGrid()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::propertyGrid()"); }
}

ak::ui::widget::propertyGrid::~propertyGrid() {
	for (int i = 0; i < my_items.size(); i++) {
		ak::ui::widget::propertyGridItem * itm = my_items.at(i);
		delete itm;
	}
	delete my_internalNotifier;
	if (my_externalMessanger != nullptr) {
		try { my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eDestroyed); } catch (...) {}
	}
}

QWidget * ak::ui::widget::propertyGrid::widget(void) { return my_table->widget(); }

void ak::ui::widget::propertyGrid::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	try {
		assert(_colorStyle != nullptr); // No color style provided
		my_colorStyle = _colorStyle;
		my_table->setColorStyle(my_colorStyle);
		my_colorNormal = my_colorStyle->getControlsMainForecolor();
		my_colorInvalidInput = my_colorStyle->getControlsErrorForecolor();
		for (auto itm : my_items) {
			ak::ui::core::aWidget * wid = itm->widget();
			if (wid != nullptr) { wid->setColorStyle(my_colorStyle); }
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::colorEditButton::setColorStyle()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::colorEditButton::setColorStyle()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::colorEditButton::setColorStyle()"); }
}

// ###########################################################################################################################

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	bool											_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount(), _itemName, ak::core::valueType::vBool,_isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		pItm->setBool(_value);
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		//Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);

		// Create checkbox
		ak::ui::qt::checkBox * nObj = new ak::ui::qt::checkBox;
		pItm->setWidget(nObj);

		// Setup checkbox
		if (_isMultipleValues) { nObj->setTristate(true); }
		else { nObj->setChecked(_value); }

		// Finalize checkbox
		my_signalLinker->addLink(nObj);
		my_table->setCellWidget(nObj, my_table->rowCount() - 1, 1);
		my_messenger->registerUidReceiver(nObj->uid(), my_internalNotifier);

		// Store UID information
		my_UIDmap.insert_or_assign(nObj->uid(), my_items.size() - 1);

		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) { 
		my_internalNotifier->enable(); 
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(bool)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(bool)");
	}
	catch (...) {
		my_internalNotifier->enable(); 
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(bool)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	int												_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount(), _itemName, ak::core::valueType::vInt, _isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		pItm->setInt(_value);
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		if (_isMultipleValues) { my_table->setCellText(my_table->rowCount() - 1, 1, "..."); }
		else { my_table->setCellText(my_table->rowCount() - 1, 1, QString::number(_value)); }
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);
	
		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(int)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(int)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(int)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	double											_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount(), _itemName, ak::core::valueType::vDouble, _isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		pItm->setDouble(_value);
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();
		
		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		if (_isMultipleValues) { my_table->setCellText(my_table->rowCount() - 1, 1, "..."); }
		else { my_table->setCellText(my_table->rowCount() - 1, 1, QString::number(_value)); }
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);
	
		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(double)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(double)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(double)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	const char *									_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount(), _itemName, ak::core::valueType::vString, _isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		pItm->setString(QString(_value));
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		if (_isMultipleValues) { my_table->setCellText(my_table->rowCount() - 1, 1, "..."); }
		else { my_table->setCellText(my_table->rowCount() - 1, 1, QString(_value)); }
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);

		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(string)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(string)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(string)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	const QString &									_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount() - 1, _itemName, ak::core::valueType::vString, _isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		pItm->setString(_value);
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		if (_isMultipleValues) { my_table->setCellText(my_table->rowCount() - 1, 1, "..."); }
		else { my_table->setCellText(my_table->rowCount() - 1, 1, _value); }
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);

		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(string)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(string)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(string)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &									_itemName,
	const ak::ui::color &								_value,
	bool											_isMultipleValues
) {
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount() - 1, _itemName, ak::core::valueType::vColor, _isMultipleValues);
		if (_isMultipleValues) { pItm->setColor(ak::ui::color(210, 210, 210, 0)); }
		else { pItm->setColor(_value); }
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);
		my_table->setCellEditable(my_table->rowCount() - 1, 1, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 1, false);

		// Add item
		ui::widget::colorEditButton * nObj = nullptr;
		if (_isMultipleValues) { nObj = new ui::widget::colorEditButton(my_messenger, my_uidManager, _value, "..."); }
		else { nObj = new ui::widget::colorEditButton(my_messenger, my_uidManager, _value); }
		nObj->setUid(my_uidManager->getId());
		my_table->setCellWidget(nObj->widget(), my_table->rowCount() -1, 1);
		my_messenger->registerUidReceiver(nObj->uid(), my_internalNotifier);	

		// Store UID information
		pItm->setWidget(nObj);
		my_UIDmap.insert_or_assign(nObj->uid(), my_items.size() - 1);

		// Enable the notifier again
		my_internalNotifier->enable();

	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(color)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(color)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(color)");
	}
}

void ak::ui::widget::propertyGrid::createItem(
	const QString &										_itemName,
	const std::vector<ak::ui::qt::comboButtonItem> &	_selection,
	const QString &										_selectedValue,
	bool												_isMultipleValues
) {	
	try {
		// Store data
		ak::ui::widget::propertyGridItem * pItm = nullptr;
		pItm = new ak::ui::widget::propertyGridItem(my_table->rowCount() - 1, _itemName, ak::core::valueType::vSelection, _isMultipleValues);
		if (pItm == nullptr) { throw ak::Exception("Failed to create", "Create new ak::ui::proprtyGridItem"); }
		std::vector<QString> nItms;
		for (int c = 0; c < _selection.size(); c++) { nItms.push_back(_selection.at(c).text()); }
		pItm->setPossibleSelection(nItms);
		pItm->setSelection(_selectedValue);
		my_items.push_back(pItm);

		// Disable the notifier to avoid change message on creation
		my_internalNotifier->disable();

		// Add to table
		my_table->addRow();
		my_table->setCellText(my_table->rowCount() - 1, 0, _itemName);
		my_table->setCellEditable(my_table->rowCount() - 1, 0, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 0, false);
		my_table->setCellEditable(my_table->rowCount() - 1, 1, false);
		my_table->setCellSelectable(my_table->rowCount() - 1, 1, false);

		// Add item
		ak::ui::qt::comboButton * nObj = nullptr;
		if (_isMultipleValues) { nObj = new ui::qt::comboButton("...", my_colorStyle); }
		else { nObj = new ui::qt::comboButton(_selectedValue, my_colorStyle); }
		nObj->setItems(_selection);
		my_signalLinker->addLink(nObj);

		my_messenger->registerUidReceiver(nObj->uid(), my_internalNotifier);
		my_table->setCellWidget(nObj, my_table->rowCount() - 1, 1);

		// Store UID information
		pItm->setWidget(nObj);
		my_UIDmap.insert_or_assign(nObj->uid(), my_items.size() - 1);

		// Enable the notifier again
		my_internalNotifier->enable();
	}
	catch (const ak::Exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e, "ak::ui::widget::propertyGrid::createItem(color)");
	}
	catch (const std::exception & e) {
		my_internalNotifier->enable();
		throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::createItem(color)");
	}
	catch (...) {
		my_internalNotifier->enable();
		throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::createItem(color)");
	}
}

// ############################################################################################################

// Manipulate existing

void ak::ui::widget::propertyGrid::clear(void) { 
	// Disconnect all items
	my_internalNotifier->disable();
	if (my_signalLinker != nullptr) { delete my_signalLinker; my_signalLinker = nullptr; }
	my_signalLinker = new ak::ui::signalLinker(my_messenger, my_uidManager);

	// delete all items created
	for (auto itm : my_items) {
		ak::ui::widget::propertyGridItem * item = itm;
		ak::ui::core::aWidget * widget = item->widget();
		delete item; delete widget;
	}
	
	// Clear up all entries
	my_UIDmap.clear();
	my_messenger->clearAll();
	my_table->clear();
	my_items.clear();
	
	my_table->setColumnHeader(0, "Property");
	my_table->setColumnHeader(1, "Value");
	my_uidManager->setLatestUid(my_table->uid() + 1);
	my_messenger->registerUidReceiver(my_table->uid(), my_internalNotifier);
	my_internalNotifier->enable();
}

// ############################################################################################################

// Information

void ak::ui::widget::propertyGrid::setShowMessageboxOnSyntaxError(
	bool								_show
) { my_showMessageboxOnSyntaxError = _show; }

void ak::ui::widget::propertyGrid::focusRow(int _row) const {
	try {
		my_table->setCellSelected(_row, 1, true);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::focusRow()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::focusRow()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::focusRow()"); }
}

void ak::ui::widget::propertyGrid::setSyntaxError(int _row, const QString & _message) {
	try
	{
		try {
			if (_row < 0 || _row >= my_items.size()) { throw ak::Exception("Index out of range", "Check row index"); }
			switch (my_items.at(_row)->valueType())
			{
			case ak::core::valueType::vBool: throw ak::Exception("Array can not be set to state: Syntax error", "Check value type"); break;
			case ak::core::valueType::vArray: throw ak::Exception("Bool can not be set to state: Syntax error", "Check value type"); break;
			case ak::core::valueType::vSelection: throw ak::Exception("Selection can not be set to state: Syntax error", "Check value type"); break;
			case ak::core::valueType::vDouble:
			case ak::core::valueType::vInt:
			case ak::core::valueType::vString:
				my_internalNotifier->disable();
				my_table->setCellForecolor(_row, 0, my_colorInvalidInput.toQColor());
				my_table->setCellForecolor(_row, 1, my_colorInvalidInput.toQColor());
				if (_message.length() > 0) {
					QMessageBox msg; msg.setText(_message);
					msg.setWindowTitle("Syntax error"); msg.exec();
				}
				my_internalNotifier->enable();
				break;
			default:
				assert(0); // Hot implemented yet
				break;
			}
		}
		catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::setSyntaxError()"); }
		catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::setSyntaxError()"); }
		catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::setSyntaxError()"); }
	}
	catch (const ak::Exception e) {
		my_internalNotifier->enable();
		throw ak::Exception(e);
	}
}

int ak::ui::widget::propertyGrid::itemCount(void) const { return my_table->rowCount(); }

bool ak::ui::widget::propertyGrid::showMessageboxOnSyntaxError(void) const { return my_showMessageboxOnSyntaxError; }

ak::core::valueType ak::ui::widget::propertyGrid::getValueType(
	ak::ID											_itemID
) const {
	try {
		checkRow(_itemID, true); // Check row index and throw if out of range
		return my_items.at(_itemID)->valueType();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getValueType()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getValueType()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getValueType()"); }
}

QString ak::ui::widget::propertyGrid::getPropertyName(
	ak::ID											_itemID
) const {
	try {
		checkRow(_itemID, true); // Check row index and throw if out of range
		return my_items.at(_itemID)->propertyName();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getPropertyName()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getPropertyName()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getPropertyName()"); }
}

bool ak::ui::widget::propertyGrid::getIsMultivalued(
	ak::ID											_itemID
) const {
	try {
		checkRow(_itemID, true); // Check row index and throw if out of range, also check value type
		return my_items.at(_itemID)->isMultivalued();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getIsMultivalued()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getIsMultivalued()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getIsMultivalued()"); }
}

// Get values

bool ak::ui::widget::propertyGrid::getBool(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vBool, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getBool();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getBool()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getBool()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getBool()"); }
}

int ak::ui::widget::propertyGrid::getInt(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vInt, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getInt();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getInt()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getInt()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getInt()"); }
}

double ak::ui::widget::propertyGrid::getDouble(ak::ID _row) const  {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vDouble, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getDouble();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getDouble()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getDouble()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getDouble()"); }
}

QString ak::ui::widget::propertyGrid::getString(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vString, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getString();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getString()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getString()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getString()"); }
}

QString ak::ui::widget::propertyGrid::getSelection(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vSelection, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getSelection();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getSelection()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getSelection()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getSelection()"); }
}

std::vector<QString> ak::ui::widget::propertyGrid::getSelectionPossibleValues(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vSelection, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		std::vector<QString> out;
		return my_items.at(_row)->getSelectionPossible();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getSelectionPossibleValues()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getSelectionPossibleValues()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getSelectionPossibleValues()"); }
}

ak::ui::color ak::ui::widget::propertyGrid::getColor(ak::ID _row) const {
	try {
		checkItemTypeEqual(_row, ak::core::valueType::vColor, true); // Check row index and throw if out of range, also check value type
		if (my_items.at(_row)->isMultivalued()) { throw ak::Exception("Is multivalued", "Check value"); }
		return my_items.at(_row)->getColor();
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::getColor()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::getColor()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::getColor()"); }
}

// ############################################################################################################
// Event handling

void ak::ui::widget::propertyGrid::raiseCellEvent(
	int									_row,
	ak::core::eventType						_event,
	bool								_forceMessage
) const {
	try {
		if (_forceMessage) {
			my_externalMessanger->sendMessage(my_uid, _event, _row, 0);
		}
		else {
			// Check if the message is a changed message
			if (_event == ak::core::eventType::eChanged) {
				
				assert(_row >= 0 && _row < my_items.size());

				switch (my_items.at(_row)->valueType()) 
				{
				case ak::core::valueType::vColor:
				case ak::core::valueType::vBool:
				case ak::core::valueType::vSelection:
					return;

				case ak::core::valueType::vDouble:
				{
					QString txt = my_table->cellText(_row, 1);
					if (!ak::core::numbers::isDecimal(txt)) {
						my_table->setCellForecolor(_row, 0, my_colorInvalidInput.toQColor());
						my_table->setCellForecolor(_row, 1, my_colorInvalidInput.toQColor());
						// Show notification messagebox
						if (my_showMessageboxOnSyntaxError && my_items.at(_row)->lastText() != txt) {
							my_items.at(_row)->setLastText(txt);
							QMessageBox msg;
							msg.setWindowTitle("Syntax error");
							msg.setText("At property '" + my_items.at(_row)->propertyName() + "' type [double] was expected");
							msg.exec();
						}
						return;
					}
					double v = txt.toDouble();
					my_items.at(_row)->setLastText(txt);
					my_table->setCellForecolor(_row, 0, my_colorNormal.toQColor());
					my_table->setCellForecolor(_row, 1, my_colorNormal.toQColor());

					if (my_items.at(_row)->getDouble() != v) {
						my_items.at(_row)->setDouble(v);
						my_table->setCellText(_row, 1, QString::number(v));	
						my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, _row, 0);
					}
					else {
						my_table->setCellText(_row, 1, QString::number(v));
					}
				}
				break;
				case ak::core::valueType::vInt:
				{
					QString txt = my_table->cellText(_row, 1);
					if (!ak::core::numbers::isInteger(txt)) {
						my_table->setCellForecolor(_row, 0, my_colorInvalidInput.toQColor());
						my_table->setCellForecolor(_row, 1, my_colorInvalidInput.toQColor());
						// Show notification messagebox
						if (my_showMessageboxOnSyntaxError && my_items.at(_row)->lastText() != txt) {
							my_items.at(_row)->setLastText(txt);
							QMessageBox msg;
							msg.setWindowTitle("Syntax error");
							msg.setText("At property '" + my_items.at(_row)->propertyName() + "' type [int] was expected");
							msg.exec();
						}
						return;
					}
					int v = txt.toInt();
					
					my_items.at(_row)->setLastText(txt);
					my_table->setCellForecolor(_row, 0, my_colorNormal.toQColor());
					my_table->setCellForecolor(_row, 1, my_colorNormal.toQColor());

					if (my_items.at(_row)->getInt() != v) {
						my_items.at(_row)->setInt(v);
						my_table->setCellText(_row, 1, QString::number(v));
						my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, _row, 0);
					}
					else {
						my_table->setCellText(_row, 1, QString::number(v));
					}
				}
					break;
				case ak::core::valueType::vString:
				{
					QString txt = my_table->cellText(_row, 1);
					my_items.at(_row)->setString(txt);
					my_externalMessanger->sendMessage(my_uid,_event, _row, 0); 
					break;
				}
				default:
					assert(0); // Not implemented value type
					break;
				}
			}
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::raiseCellEvent()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::raiseCellEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::raiseCellEvent()"); }
}

void ak::ui::widget::propertyGrid::raiseWidgetEvent(
	ak::UID						_widgetUid,
	ak::core::eventType						_eventType
) {
	try {
		if (_eventType == ak::core::eventType::eChanged || _eventType == ak::core::eventType::eToggeledChecked ||
			_eventType == ak::core::eventType::eToggeledUnchecked || _eventType == ak::core::eventType::eStateChanged ||
			_eventType == ak::core::eventType::eClicked || _eventType == ak::core::eventType::eIndexChanged) {
			
			my_UIDmapIterator itm = my_UIDmap.find(_widgetUid);
			if (itm == my_UIDmap.end()) { throw ak::Exception("Invalid UID", "Check widget UID"); }
			assert(itm->second >= 0 && itm->second < my_items.size()); // Invalid index stored

			ak::ui::widget::propertyGridItem * ptItm = my_items.at(itm->second);
			assert(ptItm != nullptr);

			switch (my_items.at(itm->second)->valueType())
			{
			case ak::core::valueType::vBool:
			{
				if (_eventType != ak::core::eventType::eClicked) {
					
					// Cast actual object
					ak::ui::qt::checkBox * actualObject = nullptr;
					actualObject = dynamic_cast<ak::ui::qt::checkBox *>(ptItm->widget());
					assert(actualObject != nullptr); // Cast failed

					// Check if was multivalued
					if (ptItm->isMultivalued()) {
						// Update data
						ptItm->setIsMultivalued(false);
						actualObject->setTristate(false);
						ptItm->setBool(actualObject->isChecked());
						// Send message
						my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, itm->second);
					}
					else {
						bool newState = actualObject->isChecked();
						if (ptItm->getBool() != newState) {
							ptItm->setBool(newState);
							my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, itm->second);
						}
					}
				}
			}
			break;
			case ak::core::valueType::vColor:
			{
				// Get item
				if (ptItm->isMultivalued()) { ptItm->setIsMultivalued(false); }

				// Cast actual object
				ak::ui::widget::colorEditButton * actualObject = nullptr;
				actualObject = dynamic_cast<ak::ui::widget::colorEditButton *>(ptItm->widget());
				assert(actualObject != nullptr);

				// Save changes
				ptItm->setColor(actualObject->color());
				// Send message
				my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, itm->second);
			}
			break;
			case ak::core::valueType::vSelection:
			{
				// Cast actual object
				ak::ui::qt::comboButton * actualObject = nullptr;
				actualObject = dynamic_cast<ak::ui::qt::comboButton *>(ptItm->widget());
				assert(actualObject != nullptr); // Cast failed

				// Get item
				if (ptItm->isMultivalued()) {
					ptItm->setIsMultivalued(false);
					// Check procedure
					my_items.at(itm->second)->setSelection(actualObject->text());
					my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, itm->second);
				}
				else {
					QString newState = actualObject->text();
					if (ptItm->getSelection() != newState) {
						ptItm->setSelection(newState);
						my_externalMessanger->sendMessage(my_uid, ak::core::eventType::eChanged, itm->second);
					}
				}
			}
			break;
			default:
				assert(0); // Check why?
				break;
			}
		}
		else {
			//my_externalMessanger->sendMessage(my_uid, ak::core::messageType::mEvent, _eventType, 0, 0);
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::raiseWidgetEvent()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::raiseWidgetEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::raiseWidgetEvent()"); }
}

void ak::ui::widget::propertyGrid::keyEvent(
	ak::ui::core::keyType							_key,
	bool											_keyDown
) {
	try {
		if (_keyDown) { my_messenger->sendMessage(my_uid, ak::core::eKeyPressed, 0, _key); }
		else { my_messenger->sendMessage(my_uid, ak::core::eKeyReleased, 0, _key); }
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::keyPressedEvent()"); }
	catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::keyPressedEvent()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::keyPressedEvent()"); }
}

bool ak::ui::widget::propertyGrid::checkRow(
	int												_row,
	bool											_throwException
) const {
	if (_row < 0 || _row >= my_items.size()) {
		if (_throwException) { throw ak::Exception("Index out of range", "Check row index"); }
		else { return false; }
	} else { return true; }
}

bool ak::ui::widget::propertyGrid::checkItemTypeEqual(
	int												_row,
	ak::core::valueType									_requestedType,
	bool											_throwException
) const{
	try {
		try {
			// Call check row. Will throw an exception if out of range
			checkRow(_row, true);

			if (my_items.at(_row)->valueType() == _requestedType) {
				return true;
			}
			else if (_throwException) {
				QString msg = "Expected object type " + toQString(my_items.at(_row)->valueType()) +
					", but provided " + toQString(_requestedType);
				throw ak::Exception(msg.toStdString(), "Check type");
			}
			else { return false; }
		}
		catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::widget::propertyGrid::checkItemTypeEqual()"); }
		catch (std::exception & e) { throw ak::Exception(e.what(), "ak::ui::widget::propertyGrid::checkItemTypeEqual()"); }
		catch (...) { throw ak::Exception("Unknown error", "ak::ui::widget::propertyGrid::checkItemTypeEqual()"); }
	}
	catch (const ak::Exception & e) {
		if (_throwException) { throw e; }
		else { return false; }
	}
}