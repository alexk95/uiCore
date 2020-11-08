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
#include <ak_ui_qt_textEdit.h>

// Qt header
#include <qwidget.h>
#include <qlayout.h>
#include <qheaderview.h>
#include <qflags.h>
#include <qfont.h>
#include <qstringlist.h>

#define ALTERNATE_ROW_COLOR_VALUE 40
#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::widget::propertyGrid::propertyGrid(
	messenger *				_messenger,
	uidManager *			_uidManager
)
	: ak::ui::core::aWidgetManager(ak::ui::core::oPropertyGrid, _messenger, _uidManager), my_currentID(ak::invalidID),
	my_groupHeaderBackColor(80,80,80), my_groupHeaderForeColor(0,0,0), my_itemDefaultBackgroundColor(230,230,230),
	my_itemTextColorError(255,0,0), my_itemTextColorNormal(0,0,0),
	my_widget(nullptr), my_layout(nullptr), my_infoTextEdit(nullptr)
{
	assert(_messenger != nullptr); // nullptr provided
	assert(_uidManager != nullptr); // nullptr provided

	// Create central widget
	my_widget = new QWidget();
	my_widget->setContentsMargins(0, 0, 0, 0);
	my_layout = new QVBoxLayout(my_widget);
	my_layout->setContentsMargins(0, 0, 0, 0);

	// Create info field
	my_infoTextEdit = new qt::textEdit("No items");
	my_infoTextEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	my_infoTextEdit->setReadOnly(true);
	my_infoTextEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	my_layout->addWidget(my_infoTextEdit);

	// Create table
	my_table = new qt::table(0, 2);
	my_table->verticalHeader()->setVisible(false);
	my_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

	QStringList lst;
	lst.push_back("Name");
	lst.push_back("Value");
	my_table->setHorizontalHeaderLabels(lst);

	my_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setEnabled(false);

	// Create default group
	my_defaultGroup = new propertyGridGroup(my_table, "");
	my_defaultGroup->setItemsBackColor(my_itemDefaultBackgroundColor);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	my_defaultGroup->setGroupHeaderVisible(false);
	my_defaultGroup->activate();

	connect(my_table, SIGNAL(focusLost()), this, SLOT(slotFocusLost()));

}

ak::ui::widget::propertyGrid::~propertyGrid() {
	A_OBJECT_DESTROYING

	clear();
}

// ##############################################################################################################

// base class functions

QWidget * ak::ui::widget::propertyGrid::widget(void) {
	return my_widget;
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

	for (auto itm : my_groups) { itm.second->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor); }

	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError);
	my_defaultGroup->setItemsBackColor(my_itemDefaultBackgroundColor);

	QString sheet("QHeaderView{border: none;}\n");

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader |
		TYPE_COLORAREA::caBackgroundColorHeader, "QHeaderView::section{","border: 0px}"
		"QHeaderView::section:first{border-right: 1px solid black;}"));

	my_table->horizontalHeader()->setStyleSheet(sheet);
	my_table->setColorStyle(my_colorStyle);
	my_infoTextEdit->setColorStyle(my_colorStyle);
}

void ak::ui::widget::propertyGrid::setAlias(
	const QString &							_alias
) {
	ui::core::aObject::setAlias(_alias);
	my_table->setObjectName(my_alias + "__Table");
	my_infoTextEdit->setObjectName(my_alias + "__TextEdit");
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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

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

	itemCountChanged();

	connect(newItem, SIGNAL(changed()), this, SLOT(slotItemChanged()));
	return my_currentID;
}

void ak::ui::widget::propertyGrid::setItemReadOnly(
	ak::ID											_itemID,
	bool											_readOnly
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setReadOnly(_readOnly);
}

bool ak::ui::widget::propertyGrid::itemIsReadOnly(
	ak::ID											_itemID
) {
	my_itemsIterator itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->isReadOnly();
}

// ##############################################################################################################

// Clear items

void ak::ui::widget::propertyGrid::clear(void) {
	for (auto itm : my_groups) {
		propertyGridGroup * group = itm.second;
		delete group;
	}
	my_defaultGroup->clear();
	my_items.clear();
	my_groups.clear();
	my_currentID = ak::invalidID;
	itemCountChanged();
	my_messenger->sendMessage(my_uid, ak::core::eventType::eCleared);
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

// ##############################################################################################################

// Slots

void ak::ui::widget::propertyGrid::slotItemChanged(void) {
	ui::widget::propertyGridItem * actualSender = nullptr;
	actualSender = dynamic_cast<ui::widget::propertyGridItem *>(sender());
	assert(actualSender != nullptr); // Cast failed
	assert(my_messenger != nullptr); // That should not happen
	my_messenger->sendMessage(my_uid, ak::core::eventType::eChanged, actualSender->getId());
}

void ak::ui::widget::propertyGrid::slotCheckItemVisibility(void) {
	if (my_checkItemVisibilityRequired) {
		my_checkItemVisibilityRequired = false;
		if (my_items.size() == 0) {
			if (my_table->isVisible()) {
				my_layout->removeWidget(my_table);
				my_layout->addWidget(my_infoTextEdit);
				my_infoTextEdit->setVisible(true);
				my_table->setVisible(false);
				//my_layout->setContentsMargins(2, 2, 2, 2);
			}
		}
		else {
			if (!my_table->isVisible()) {
				my_layout->removeWidget(my_infoTextEdit);
				my_layout->addWidget(my_table);
				my_infoTextEdit->setVisible(false);
				my_table->setVisible(true);
				//my_layout->setContentsMargins(0, 0, 0, 0);
			}

		}
	}
}

void ak::ui::widget::propertyGrid::slotFocusLost(void) {
	for (auto group : my_groups) { group.second->deselect(); }
}

// ##############################################################################################################

// Private members

void ak::ui::widget::propertyGrid::itemCountChanged(void) {
	my_checkItemVisibilityRequired = true;
	// Queue the request to avoid flickering when clearing property grid and refilling it with new items
	QMetaObject::invokeMethod(this, "slotCheckItemVisibility", Qt::QueuedConnection);
}

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

ak::ui::widget::propertyGridGroup::propertyGridGroup(
	qt::table *							_propertyGridTable,
	const QString &						_groupName
) : my_propertyGridTable(_propertyGridTable), my_isActivated(false), my_isVisible(true), my_isAlternateBackground(false), my_headerIsVisible(true),
	my_colorTextNormal(0, 0, 0), my_colorTextError(255, 0, 0)
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

	setItemsBackColor(my_colorItemBackground);

	connect(my_propertyGridTable, SIGNAL(itemDoubleClicked(QTableWidgetItem *)), this, SLOT(slotDoubleClicked(QTableWidgetItem *)));

}

ak::ui::widget::propertyGridGroup::~propertyGridGroup() {
	int r = my_item->row();
	clear();
	delete my_item;
	my_propertyGridTable->removeRow(r);
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
	if (r > 255) { r = my_colorItemBackground.red() - ALTERNATE_ROW_COLOR_VALUE; }
	if (g > 255) { g = my_colorItemBackground.green() - ALTERNATE_ROW_COLOR_VALUE; }
	if (b > 255) { b = my_colorItemBackground.blue() - ALTERNATE_ROW_COLOR_VALUE; }
	if (a < 0) { a = 0; }
	if (a > 255) { a = 255; }
	my_colorItemBackgroundAlternate.setRgb(r, g, b, a);
	repaint();
}

void ak::ui::widget::propertyGridGroup::setItemsTextColors(
	const QColor &									_textColorNormal,
	const QColor &									_textColorError
) {
	my_colorTextNormal = _textColorNormal;
	my_colorTextError = _textColorError;
	repaint();
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

void ak::ui::widget::propertyGridGroup::setGroupHeaderVisible(
	bool											_isVisible
) {
	assert(my_item != nullptr);	// This should never happen
	my_headerIsVisible = _isVisible;
	my_propertyGridTable->setRowHidden(my_item->row(), !my_headerIsVisible);
}

void ak::ui::widget::propertyGridGroup::clear(void) {
	for (auto itm : my_items) {
		propertyGridItem * actualItem = itm;
		delete actualItem;
	}
	my_items.clear();
	my_isAlternateBackground = false;
}

void ak::ui::widget::propertyGridGroup::deselect(void) {
	for (auto itm : my_items) { itm->deselect(); }
	my_item->setSelected(false);
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
		// Show group header (if is setVisible) and items
		my_propertyGridTable->setRowHidden(my_item->row(), !my_headerIsVisible);
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), false);
		}
	}
	else {
		// Hide group header
		my_propertyGridTable->setRowHidden(my_item->row(), true);
	}
}

void ak::ui::widget::propertyGridGroup::repaint(void) {
	my_isAlternateBackground = false;
	for (auto itm : my_items) {
		if (my_isAlternateBackground) { itm->setBackgroundColor(my_colorItemBackgroundAlternate); }
		else { itm->setBackgroundColor(my_colorItemBackground); }
		itm->setTextColors(my_colorTextNormal, my_colorTextError);
		my_isAlternateBackground = !my_isAlternateBackground;
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

void ak::ui::widget::propertyGridItem::deselect(void) {
	my_cellSettingName->setSelected(false);
	if (my_cellValue != nullptr) { my_cellValue->setSelected(false); }
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

void ak::ui::widget::propertyGridItem::setReadOnly(
	bool					_readOnly
) {
	my_ignoreCellEvent = true;
	switch (my_valueType)
	{
	case ak::core::vBool:
		assert(my_widgetBool != nullptr); // This should not happen
		my_widgetBool->setEnabled(!_readOnly);
		break;
	case ak::core::vColor:
		assert(my_widgetColor != nullptr); // This should not happen
		my_widgetColor->SetEnabled(!_readOnly);
		break;
	case ak::core::vSelection:
		assert(my_widgetSelection != nullptr); // This should not happen
		my_widgetSelection->setEnabled(!_readOnly);
		break;
	case ak::core::vDouble:
	case ak::core::vInt:
	case ak::core::vString:
	{
		Qt::ItemFlags f = my_cellValue->flags();
		f.setFlag(Qt::ItemFlag::ItemIsEditable, !_readOnly);
		my_cellValue->setFlags(f);
	}
		break;
	default:
		assert(0); // Invalid value type
		break;
	}
	my_ignoreCellEvent = false;
}

bool ak::ui::widget::propertyGridItem::isReadOnly() { return my_isReadOnly; }

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

#include <string>
#include <sstream>

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
			bool failed = false;
			double v;
			v = ak::core::numbers::validateNumber<double>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected numeric.");
				ui::dialog::prompt dia(msg, "Error", ui::core::promptType::promptOk);
				dia.showDialog();
				my_cellValue->setSelected(false);
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
			bool failed = false;
			int v;
			v = ak::core::numbers::validateNumber<int>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected natural number.");
				ui::dialog::prompt dia(msg, "Error", ui::core::promptType::promptOk);
				dia.showDialog();
				my_cellValue->setSelected(false);
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
		my_ignoreCellEvent = true;
		my_cellValue->setBackgroundColor(my_colorBackground.toQColor());
		if (my_isCurrentlyError) {
			my_cellValue->setTextColor(my_colorErrorForeground);
		}
		else {
			my_cellValue->setTextColor(my_colorNormalForeground);
		}
		my_ignoreCellEvent = false;
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
		{
			assert(my_widgetSelection != nullptr); // Something went wrong
			QString sheet("QPushButton{color: #");
			ui::color fore(my_colorNormalForeground);
			ui::color back(my_colorBackground);
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append(";} QPushButton QMenu{color: #");
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append("; alternate-background-color: red;}");
			my_widgetSelection->setStyleSheet(sheet);
		}
			break;
		default:
			break;
		}
	}
}