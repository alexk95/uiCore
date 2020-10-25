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

#define ALTERNATE_ROW_COLOR_VALUE 40

ak::ui::widget::propertyGrid::propertyGrid(
	messenger *				_messenger,
	uidManager *			_uidManager
)
	: ak::ui::core::aWidgetManager(ak::ui::core::oPropertyGrid, _messenger, _uidManager), my_currentID(ak::invalidID),
	my_groupHeaderBackColor(80,80,80), my_groupHeaderForeColor(0,0,0), my_itemDefaultBackgroundColor(230,230,230),
	my_itemTextColorError(255,0,0), my_itemTextColorNormal(0,0,0)
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
	my_defaultGroup->setItemsBackColor(my_itemDefaultBackgroundColor);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
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
	//my_table->setStyleSheet(my_colorStyle->getStylesheet(ak::ui::colorStyle::styleableObject::sTable));
	my_groupHeaderBackColor = my_colorStyle->getHeaderBackgroundColor().toQColor();
	my_groupHeaderForeColor = my_colorStyle->getHeaderForegroundColor().toQColor();
	my_itemDefaultBackgroundColor = my_colorStyle->getControlsMainBackcolor().toQColor();
	my_itemTextColorNormal = my_colorStyle->getControlsMainForecolor().toQColor();
	my_itemTextColorError = my_colorStyle->getControlsErrorForecolor().toQColor();

	for (auto itm : my_groups) {
		itm.second->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
		itm.second->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	}

	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
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
	newGroup->setItemsBackColor(my_itemDefaultBackgroundColor);
	newGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	newGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	newGroup->activate();
	my_groups.insert_or_assign(_group, newGroup);
}

void ak::ui::widget::propertyGrid::addGroup(
	const QColor &									_color,
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	my_groupsIterator itm = my_groups.find(_group);
	assert(itm == my_groups.end());	// Group already exists
	propertyGridGroup * newGroup = new propertyGridGroup(my_table, _group);
	newGroup->setItemsBackColor(_color);
	newGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	newGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
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

// Clear items

void ak::ui::widget::propertyGrid::clear(void) {
	for (auto itm : my_groups) {
		itm.second->clear();
	}
	my_defaultGroup->clear();
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

bool ak::ui::widget::propertyGrid::getItemIsMultipleValues(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getIsMultipleValues();
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
) : my_propertyGridTable(_propertyGridTable), my_isActivated(false), my_isVisible(true), my_isAlternateBackground(false)
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
	my_item->setFont(font);

	connect(my_propertyGridTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotDoubleClicked(QTableWidgetItem *)));

}

ak::ui::widget::propertyGridGroup::~propertyGridGroup() {
	my_propertyGridTable->removeRow(my_item->row());
}

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

void ak::ui::widget::propertyGridGroup::setHeaderColors(
	const QColor &				_foreColor,
	const QColor &				_backColor
) {
	my_backColor = _backColor;
	my_foreColor = _foreColor;

	my_item->setBackgroundColor(my_backColor);
	my_item->setTextColor(my_foreColor);
}

void ak::ui::widget::propertyGridGroup::setItemsBackColor(
	const QColor &									_backgroudColor
) {
	my_colorItemBackground = _backgroudColor;
	int r = my_colorItemBackground.red() + ALTERNATE_ROW_COLOR_VALUE;
	int g = my_colorItemBackground.green() + ALTERNATE_ROW_COLOR_VALUE;
	int b = my_colorItemBackground.blue() + ALTERNATE_ROW_COLOR_VALUE;
	int a = my_colorItemBackground.alpha() - ALTERNATE_ROW_COLOR_VALUE;
	if (r < 0) { r = 0; }
	if (r > 255) { r = 255; }
	if (g < 0) { g = 0; }
	if (g > 255) { g = 255; }
	if (b < 0) { b = 0; }
	if (b > 255) { b = 255; }
	if (a < 0) { a = 0; }
	if (a > 255) { a = 255; }
	my_colorItemBackgroundAlternate.setRgb(r, g, b, a);
}

void ak::ui::widget::propertyGridGroup::setItemsTextColors(
	const QColor &									_textColorNormal,
	const QColor &									_textColorError
) {

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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
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
	newItem->setTextColors(my_colorTextNormal, my_colorTextError);
	if (my_isAlternateBackground) { newItem->setBackgroundColor(my_colorItemBackgroundAlternate); }
	else { newItem->setBackgroundColor(my_colorItemBackground); }
	my_isAlternateBackground = !my_isAlternateBackground;
	checkVisibility();
	newItem->setId(_itemId);
	return newItem;
}

void ak::ui::widget::propertyGridGroup::clear(void) {
	for (auto itm : my_items) {
		propertyGridItem * actualItem = itm;
		delete actualItem;
	}
	my_items.clear();
	my_isAlternateBackground = false;
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

void ak::ui::widget::propertyGridGroup::repaint(void) {
	my_isAlternateBackground = false;
	for (auto itm : my_items) {
		if (my_isAlternateBackground) { itm->setBackgroundColor(my_colorItemBackgroundAlternate); }
		else { itm->setBackgroundColor(my_colorItemBackground); }
		itm->setTextColors(my_colorTextNormal, my_colorTextError);
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
	QString sheet("QCheckBox{background-color:#");
	sheet.append(my_colorBackground.toHexString(true));
	sheet.append(";}\n");
	my_widgetBool->setStyleSheet(sheet);
	my_widgetBool->setAutoFillBackground(true);

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
	my_widgetSelection->setAutoFillBackground(true);

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
	assert(my_cellSettingName != nullptr); // This should never happen
	if (my_cellValue != nullptr) { delete my_cellValue; }
	if (my_widgetBool != nullptr) { delete my_widgetBool; }
	if (my_widgetColor != nullptr) { delete my_widgetColor; }
	if (my_widgetSelection != nullptr) { delete my_widgetSelection; }
	int r = my_cellSettingName->row();
	delete my_cellSettingName;
	my_propertyGridTable->removeRow(r);
}

int ak::ui::widget::propertyGridItem::row() const {
	return my_cellSettingName->row();
}

void ak::ui::widget::propertyGridItem::setId(
	ak::ID								_ID
) { my_id = _ID; }

ak::ID ak::ui::widget::propertyGridItem::getId(void) const { return my_id; }

bool ak::ui::widget::propertyGridItem::getIsCurrentlyError() const { return my_isCurrentlyError; }

void ak::ui::widget::propertyGridItem::setBackgroundColor(
	const QColor &					_backgroundColor
) { my_colorBackground = _backgroundColor; repaint(); }

void ak::ui::widget::propertyGridItem::setTextColors(
	const QColor &					_foregroundNormal,
	const QColor &					_foregroundError
) {
	my_colorErrorForeground = _foregroundError;
	my_colorNormalForeground = _foregroundNormal;
	repaint();
}

// #################################################################################

// Information gathering

QString ak::ui::widget::propertyGridItem::getGroup() const { return my_group; }

bool ak::ui::widget::propertyGridItem::getIsMultipleValues() const { return my_isMultipleValues; }

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
				my_cellValue->setTextColor(my_colorErrorForeground);
				my_ignoreCellEvent = false;
			}
			else {
				my_valueDouble = v;
				my_isMultipleValues = false;
				my_cellValue->setTextColor(my_colorNormalForeground);
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
				my_cellValue->setTextColor(my_colorErrorForeground);
				my_ignoreCellEvent = false;
			}
			else {
				my_valueInteger = v;
				my_isMultipleValues = false;
				my_cellValue->setTextColor(my_colorNormalForeground);
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
	my_cellSettingName = nullptr;
	my_cellValue = nullptr;
	my_widgetBool = nullptr;
	my_widgetColor = nullptr;
	my_widgetSelection = nullptr;
	my_ignoreCellEvent = false;
	my_isCurrentlyError = false;
	my_colorErrorForeground = QColor(255, 0, 0);
	my_colorNormalForeground = QColor(0, 0, 0);
}

void ak::ui::widget::propertyGridItem::repaint(void) {
	if (my_cellSettingName != nullptr) {
		my_cellSettingName->setBackgroundColor(my_colorBackground.toQColor());
		my_cellSettingName->setTextColor(my_colorNormalForeground);
	}
	if (my_cellValue != nullptr) {
		my_cellValue->setBackgroundColor(my_colorBackground.toQColor());
		if (my_isCurrentlyError) {
			my_cellValue->setTextColor(my_colorErrorForeground);
		}
		else {
			my_cellValue->setTextColor(my_colorNormalForeground);
		}
	}
	else {
		switch (my_valueType)
		{
		case ak::core::vBool:
		{
			assert(my_widgetBool != nullptr); // Something went wrong
			QString sheet("QCheckBox{background-color:#");
			sheet.append(my_colorBackground.toHexString(true));
			sheet.append(";}\n");
			my_widgetBool->setStyleSheet(sheet);
		}
			break;
		case ak::core::vColor:
		{
			assert(my_widgetColor != nullptr); // Something went wrong
			my_widgetColor->fillBackground(my_colorBackground);
		}
			break;
		case ak::core::vSelection:
			break;
		default:
			break;
		}
	}
}