/*
 * ak_ui_widget_propertyGrid.cpp
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <ak_ui_widget_propertyGrid.h>
#include <ak_ui_core.h>
#include <ak_messenger.h>
#include <ak_ui_colorStyle.h>
#include <ak_ui_qt_table.h>
#include <ak_ui_qt_checkBox.h>
#include <ak_ui_qt_colorEditButton.h>
#include <ak_ui_qt_comboButton.h>
#include <ak_ui_dialog_prompt.h>

// Qt header
#include <qheaderview.h>
#include <qflags.h>
#include <qfont.h>
#include <qstringlist.h>

ak::ui::widget::propertyGrid::propertyGrid(
	messenger *				_messenger,
	uidManager *			_uidManager
)
	: ak::ui::core::aWidgetManager(ak::ui::core::oPropertyGrid, _messenger, _uidManager),
	my_alternateForeColor(180, 180, 180), my_alternateBackColor(0, 0, 0), my_isAlternateGroup(false),
	my_currentID(ak::invalidID)
{
	assert(_messenger != nullptr); // nullptr provided
	assert(_uidManager != nullptr); // nullptr provided

	my_table = new qt::table(0, 2);
	my_table->verticalHeader()->setVisible(false);

	my_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setEnabled(false);

	QStringList headerText;
	headerText.push_back("Name");
	headerText.push_back("Value");
	my_table->setHorizontalHeaderLabels(headerText);

	my_defaultGroup = new propertyGridGroup(my_table, "");
	my_defaultGroup->activate();

}

ak::ui::widget::propertyGrid::~propertyGrid() {

}

// ##############################################################################################################

// base class functions

QWidget * ak::ui::widget::propertyGrid::widget(void) {
	return my_table;
}

void ak::ui::widget::propertyGrid::setColorStyle(
	const ak::ui::colorStyle *						_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	my_table->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTable));
	my_alternateBackColor = my_colorStyle->getAlternateBackColor().toQColor();
	my_alternateForeColor = my_colorStyle->getAlternateForeColor().toQColor();
	my_isAlternateGroup = false;
	for (auto itm : my_groups) {
		my_isAlternateGroup = !my_isAlternateGroup;
		if (my_isAlternateGroup) {
			itm.second->setColors(my_alternateForeColor, my_alternateBackColor);
		}
	}
}

// ##############################################################################################################

// Item creation and manipulation

void ak::ui::widget::propertyGrid::addGroup(
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	my_groupsIterator itm = my_groups.find(_group);
	assert(itm == my_groups.end());	// Group already exists
	propertyGridGroup * newGroup = new propertyGridGroup(my_table, _group);
	my_isAlternateGroup = !my_isAlternateGroup;
	if (my_isAlternateGroup) {
		newGroup->setColors(my_alternateForeColor, my_alternateBackColor);
	}
	newGroup->activate();
	my_groups.insert_or_assign(_group, newGroup);
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const ui::color &								_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _possibleSelection, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) {
	my_currentID++;
	ui::widget::propertyGridItem * newItem = my_defaultGroup->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

// ++++++++++++++++++++++++++++++++++++++++++

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const ui::color &									_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _possibleSelection, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

ak::ID ak::ui::widget::propertyGrid::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	my_currentID++;
	ui::widget::propertyGridItem * newItem = group->second->addItem(my_currentID, _isMultipleValues, _settingName, _value);
	my_items.insert_or_assign(my_currentID, newItem);
	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

// ##############################################################################################################

// Item information gathering

QString ak::ui::widget::propertyGrid::getItemGroup(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getGroup();
}

QString ak::ui::widget::propertyGrid::getItemName(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getName();
}

std::vector<QString> ak::ui::widget::propertyGrid::getItemPossibleSelection(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getPossibleSelection();
}

bool ak::ui::widget::propertyGrid::getItemValueBool(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueBool();
}

ak::ui::color ak::ui::widget::propertyGrid::getItemValueColor(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueColor();
}

double ak::ui::widget::propertyGrid::getItemValueDouble(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueDouble();
}

int ak::ui::widget::propertyGrid::getItemValueInteger(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueInteger();
}

QString ak::ui::widget::propertyGrid::getItemValueSelection(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueSelection();
}

QString ak::ui::widget::propertyGrid::getItemValueString(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueString();
}

ak::core::valueType ak::ui::widget::propertyGrid::getItemValueType(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueType();
}

void ak::ui::widget::propertyGrid::slotItemChanged(void) {
	ui::widget::propertyGridItem * actualSender = nullptr;
	actualSender = dynamic_cast<ui::widget::propertyGridItem *>(sender());
	assert(actualSender != nullptr); // Cast failed
	assert(my_messenger != nullptr); // That should not happen
	my_messenger->sendMessage(my_uid, ak::core::eventType::eChanged, actualSender->getId());
}

// ##############################################################################################################

// ##############################################################################################################

// ##############################################################################################################

ak::ui::widget::propertyGridGroup::propertyGridGroup(
	qt::table *							_propertyGridTable,
	const QString &						_groupName
) : my_propertyGridTable(_propertyGridTable), my_isActivated(false), my_isVisible(true), my_colorWasSet(false)
{
	assert(my_propertyGridTable != nullptr);
	my_item = new QTableWidgetItem;
	my_name = _groupName;
	my_item->setText(_groupName);
	Qt::ItemFlags flags = my_item->flags();
	flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	flags.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_item->setFlags(flags);

	int r = my_propertyGridTable->rowCount();
	my_propertyGridTable->insertRow(r);
	my_propertyGridTable->setSpan(r, 0, 1, 2);
	my_propertyGridTable->setItem(r, 0, my_item);
	QFont font = my_item->font();
	font.setBold(true);
	font.setUnderline(true);
	my_item->setFont(font);

	connect(my_propertyGridTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotDoubleClicked(QTableWidgetItem *)));

}

ak::ui::widget::propertyGridGroup::~propertyGridGroup() {}

QString ak::ui::widget::propertyGridGroup::name(void) const { return my_name; }

void ak::ui::widget::propertyGridGroup::setName(
	const QString &						_groupName
) {
	my_name = _groupName;
}

void ak::ui::widget::propertyGridGroup::activate(void) {
	my_isActivated = true;
	checkVisibility();
}

void ak::ui::widget::propertyGridGroup::setColors(
	const QColor &				_foreColor,
	const QColor &				_backColor
) {
	my_colorWasSet = true;
	my_backColor = _backColor;
	my_foreColor = _foreColor;

	my_item->setBackgroundColor(my_backColor);
	my_item->setTextColor(my_foreColor);

	for (auto itm : my_items) {
		itm->setBackColor(my_backColor);
		itm->setTextColor(my_foreColor);
	}
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const ui::color &								_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _possibleSelection, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

ak::ui::widget::propertyGridItem * ak::ui::widget::propertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

// ##############################################################################################################

// slots

void ak::ui::widget::propertyGridGroup::slotDoubleClicked(QTableWidgetItem * _item) {
	if (_item == my_item) {
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), my_isVisible);
		}
		my_isVisible = !my_isVisible;
	}
}

// ##############################################################################################################

// private members

void ak::ui::widget::propertyGridGroup::checkVisibility(void) {
	if (!my_isActivated) { return; }
	if (my_items.size() > 0) {
		my_propertyGridTable->setRowHidden(my_item->row(), false);
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), false);
		}
	}
	else {
		my_propertyGridTable->setRowHidden(my_item->row(), true);
	}
}

// ##############################################################################################################++++++++++++++++++++++++++++++

// ##############################################################################################################++++++++++++++++++++++++++++++

// ##############################################################################################################++++++++++++++++++++++++++++++

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	bool								_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vBool),
	my_name(_settingName), my_valueBool(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetBool = new ui::qt::checkBox();
	if (my_isMultipleValues) {
		my_widgetBool->setTristate(true);
	}
	else {
		my_widgetBool->setChecked(_value);
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetBool);

	connect(my_widgetBool, SIGNAL(stateChanged(int)), this, SLOT(slotValueWidgetEvent()));
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const ui::color &					_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vColor),
	my_name(_settingName), my_valueColor(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetColor = new ui::qt::colorEditButton(_value);
	if (my_isMultipleValues) {
		my_widgetColor->overrideText("...");
	}
	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetColor->widget());

	connect(my_widgetColor, SIGNAL(changed()), this, SLOT(slotValueWidgetEvent()));
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	double								_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vDouble),
	my_name(_settingName), my_valueDouble(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_cellValue = new QTableWidgetItem(QString::number(_value));
	if (my_isMultipleValues) {
		my_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotTableCellChanged(QTableWidgetItem *)));
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	int									_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vInt),
	my_name(_settingName), my_valueInteger(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_cellValue = new QTableWidgetItem(QString::number(_value));
	if (my_isMultipleValues) {
		my_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotTableCellChanged(QTableWidgetItem *)));
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const std::vector<QString> &		_possibleSlection,
	const QString &						_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vSelection),
	my_name(_settingName), my_valueSelection(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	if (my_isMultipleValues) {
		my_widgetSelection = new qt::comboButton("...");
	}
	else {
		my_widgetSelection = new qt::comboButton(_value);
	}
	my_valuePossibleSelection = _possibleSlection;
	my_widgetSelection->setItems(my_valuePossibleSelection);

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetSelection);

	connect(my_widgetSelection, SIGNAL(changed()), this, SLOT(slotValueWidgetEvent()));
}

ak::ui::widget::propertyGridItem::propertyGridItem(
	qt::table *							_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const QString &						_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(ak::core::valueType::vString),
	my_name(_settingName), my_valueString(_value), my_isMultipleValues(_isMultipleValues)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_cellValue = new QTableWidgetItem(_value);
	if (my_isMultipleValues) {
		my_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main propertyGrid table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(slotTableCellChanged(QTableWidgetItem *)));
}

ak::ui::widget::propertyGridItem::~propertyGridItem() {
	my_propertyGridTable->removeRow(my_cellSettingName->row());
	delete my_cellSettingName;
	delete my_cellValue;
}

int ak::ui::widget::propertyGridItem::row() const {
	return my_cellSettingName->row();
}

void ak::ui::widget::propertyGridItem::setId(
	ak::ID								_ID
) { my_id = _ID; }

ak::ID ak::ui::widget::propertyGridItem::getId(void) const { return my_id; }

void ak::ui::widget::propertyGridItem::setTextColor(
	const QColor &						_color
) {
	my_cellSettingName->setTextColor(_color);
	my_cellValue->setTextColor(_color);
}

void ak::ui::widget::propertyGridItem::setBackColor(
	const QColor &						_color
) {
	my_cellSettingName->setBackgroundColor(_color);
	my_cellValue->setBackgroundColor(_color);
}

// #################################################################################

// Information gathering

QString ak::ui::widget::propertyGridItem::getGroup() const { return my_group; }

QString ak::ui::widget::propertyGridItem::getName() const { return my_name; }

ak::core::valueType ak::ui::widget::propertyGridItem::getValueType() const { return my_valueType; }

std::vector<QString> ak::ui::widget::propertyGridItem::getPossibleSelection() const {
	assert(my_valueType == ak::core::valueType::vSelection);	// Wrong value type for this item
	return my_valuePossibleSelection;
}

bool ak::ui::widget::propertyGridItem::getValueBool() const {
	assert(my_valueType == ak::core::valueType::vBool);	// Wrong value type for this item
	return my_valueBool;
}

ak::ui::color ak::ui::widget::propertyGridItem::getValueColor() const {
	assert(my_valueType == ak::core::valueType::vColor);	// Wrong value type for this item
	return my_valueColor;
}

double ak::ui::widget::propertyGridItem::getValueDouble() const {
	assert(my_valueType == ak::core::valueType::vDouble);	// Wrong value type for this item
	return my_valueDouble;
}

int ak::ui::widget::propertyGridItem::getValueInteger() const {
	assert(my_valueType == ak::core::valueType::vInt);	// Wrong value type for this item
	return my_valueInteger;
}

QString ak::ui::widget::propertyGridItem::getValueSelection() const {
	assert(my_valueType == ak::core::valueType::vSelection);	// Wrong value type for this item
	return my_valueSelection;
}

QString ak::ui::widget::propertyGridItem::getValueString() const {
	assert(my_valueType == ak::core::valueType::vString);	// Wrong value type for this item
	return my_valueString;
}

bool ak::ui::widget::propertyGridItem::isMultipleValues(void) const { return my_isMultipleValues; }

// #################################################################################

// Slots

void ak::ui::widget::propertyGridItem::slotValueWidgetEvent() {
	switch (my_valueType)
	{
	case ak::core::vBool:
		if (my_isMultipleValues) {
			my_isMultipleValues = false;
			my_valueBool = true;
			my_widgetBool->setTristate(false);
			my_widgetBool->setChecked(my_valueBool);
			emit changed();
		}
		else {
			my_valueBool = my_widgetBool->isChecked();
			emit changed();
		}
		break;
	case ak::core::vColor:
		my_valueColor = my_widgetColor->color();
		my_isMultipleValues = false;
		emit changed();
		break;
	case ak::core::vSelection:
		my_valueSelection = my_widgetSelection->text();
		my_isMultipleValues = false;
		emit changed();
		break;
	default:
		assert(0); // This should not have happened
		break;
	}
}

void ak::ui::widget::propertyGridItem::slotTableCellChanged(
	QTableWidgetItem *									_item
) {
	if (my_ignoreCellEvent) { return; }
	if (_item == my_cellValue) {
		QString theText(_item->text());
		switch (my_valueType)
		{
		case ak::core::vDouble:
		{
			my_ignoreCellEvent = true;
			double v = theText.toDouble();
			if (theText != QString::number(v)) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected numeric.");
				ui::dialog::prompt dia(msg, "Error", ui::core::promptType::promptOk);
				dia.showDialog();
				if (my_isMultipleValues) {
					my_cellValue->setText("...");
				}
				else {
					my_cellValue->setText(QString::number(my_valueDouble));
				}
				my_cellValue->setTextColor(my_colorErrorForeground.toQColor());
				my_ignoreCellEvent = false;
			}
			else {
				my_valueDouble = v;
				my_isMultipleValues = false;
				my_cellValue->setTextColor(my_colorNormalForeground.toQColor());
				my_ignoreCellEvent = false;
				emit changed();
			}
		}
			break;
		case ak::core::vInt:
		{
			my_ignoreCellEvent = true;
			int v = theText.toInt();
			if (theText != QString::number(v)) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected natural number.");
				ui::dialog::prompt dia(msg, "Error", ui::core::promptType::promptOk);
				dia.showDialog();	
				if (my_isMultipleValues) {
					my_cellValue->setText("...");
				}
				else {
					my_cellValue->setText(QString::number(my_valueInteger));
				}
				my_cellValue->setTextColor(my_colorErrorForeground.toQColor());
				my_ignoreCellEvent = false;
			}
			else {
				my_valueInteger = v;
				my_isMultipleValues = false;
				my_cellValue->setTextColor(my_colorNormalForeground.toQColor());
				my_ignoreCellEvent = false;
				emit changed();
			}
		}
			break;
		case ak::core::vString:
			my_valueString = theText;
			emit changed();
			break;
		default:
			assert(0); // Invalid type
			break;
		}
	}
}

void ak::ui::widget::propertyGridItem::ini(void) {
	my_widgetBool = nullptr;
	my_widgetColor = nullptr;
	my_widgetSelection = nullptr;
	my_ignoreCellEvent = false;
	my_colorErrorForeground.setRGBA(255, 0, 0);
}