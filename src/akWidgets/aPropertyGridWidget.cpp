/*
 *	File:		aPropertyGridWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: October 21, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

 // AK header
#include <akGui/aColor.h>
#include <akGui/aColorStyle.h>
#include <akGui/aCustomizableColorStyle.h>

#include <akDialogs/aPromptDialog.h>

#include <akWidgets/aCheckBoxWidget.h>
#include <akWidgets/aColorEditButtonWidget.h>
#include <akWidgets/aComboButtonWidget.h>
#include <akWidgets/aPropertyGridWidget.h>
#include <akWidgets/aTableWidget.h>
#include <akWidgets/aTextEditWidget.h>
#include <akWidgets/aDatePickWidget.h>
#include <akWidgets/aTimePickWidget.h>

// Qt header
#include <qwidget.h>
#include <qlayout.h>
#include <qheaderview.h>
#include <qflags.h>
#include <qfont.h>
#include <qstringlist.h>

const int AK_INTERN_ALTERNATE_ROW_COLOR_VALUE = 40;

ak::aPropertyGridWidget::aPropertyGridWidget()
	: aWidget(otPropertyGrid), my_currentID(invalidID),
	my_groupHeaderBackColor(80,80,80), my_groupHeaderForeColor(0,0,0), my_itemDefaultBackgroundColor(230,230,230),
	my_itemTextColorError(255,0,0), my_itemTextColorNormal(0,0,0),
	my_layout(nullptr), my_infoTextEdit(nullptr), my_isEnabled(true)
{
	// Create central widget
	setContentsMargins(0, 0, 0, 0);
	my_layout = new QVBoxLayout(this);
	my_layout->setContentsMargins(0, 0, 0, 0);

	// Create info field
	my_infoTextEdit = new aTextEditWidget("No items");
	my_infoTextEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	my_infoTextEdit->setReadOnly(true);
	my_infoTextEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	my_layout->addWidget(my_infoTextEdit);

	// Create table
	my_table = new aTableWidget(0, 2);
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
	my_defaultGroup = new aPropertyGridGroup(my_table, "");
	my_defaultGroup->setItemsBackColor(my_itemDefaultBackgroundColor, false);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError, false);
	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	my_defaultGroup->setGroupHeaderVisible(false);
	my_defaultGroup->activate();

	connect(my_table, &aTableWidget::focusLost, this, &aPropertyGridWidget::slotFocusLost);
}

ak::aPropertyGridWidget::~aPropertyGridWidget() {
	A_OBJECT_DESTROYING

	clear();
}

// ##############################################################################################################

// base class functions

QWidget * ak::aPropertyGridWidget::widget(void) { return this; }

void ak::aPropertyGridWidget::setColorStyle(
	aColorStyle *						_aColorStyle
) {
	assert(_aColorStyle != nullptr); // nullptr provided
	my_colorStyle = _aColorStyle;
	//my_table->setStyleSheet(my_colorStyle->getStylesheet(ak::aColorStyle::styleableObject::sTable));
	my_groupHeaderBackColor = my_colorStyle->getHeaderBackgroundColor().toQColor();
	my_groupHeaderForeColor = my_colorStyle->getHeaderForegroundColor().toQColor();
	my_itemDefaultBackgroundColor = my_colorStyle->getControlsMainBackgroundColor().toQColor();
	my_itemTextColorNormal = my_colorStyle->getControlsMainForegroundColor().toQColor();
	my_itemTextColorError = my_colorStyle->getControlsErrorFrontForegroundColor().toQColor();

	for (auto itm : my_groups) { 
		itm.second->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
		itm.second->setColorStyle(my_colorStyle, true);
	}

	my_defaultGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	my_defaultGroup->setItemsTextColors(my_itemTextColorNormal, my_itemTextColorError, false);
	my_defaultGroup->setItemsBackColor(my_itemDefaultBackgroundColor, true);

	QString sheet("QHeaderView{border: none;}\n");

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader, "QHeaderView::section{","border: 0px}"
		"QHeaderView::section:first{border-right: 1px solid black;}"));

	my_table->horizontalHeader()->setStyleSheet(sheet);
	my_table->setColorStyle(my_colorStyle);
	my_infoTextEdit->setColorStyle(my_colorStyle);

	for (auto itm : my_items) { itm.second->setColorStyle(my_colorStyle); }

}

void ak::aPropertyGridWidget::setAlias(
	const QString &							_alias
) {
	aObject::setAlias(_alias);
	my_table->setObjectName(my_alias + "__Table");
	my_infoTextEdit->setObjectName(my_alias + "__TextEdit");
}

// ##############################################################################################################

// Item creation and manipulation

void ak::aPropertyGridWidget::addGroup(
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	auto itm = my_groups.find(_group);
	assert(itm == my_groups.end());	// Group already exists
	aPropertyGridGroup * newGroup = new aPropertyGridGroup(my_table, _group);
	newGroup->setItemsBackColor(my_itemDefaultBackgroundColor, false);
	newGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	newGroup->setStateIcons(&my_groupIconExpanded, &my_groupIconCollapsed);
	if (my_colorStyle != nullptr) {
		newGroup->setColorStyle(my_colorStyle, true);
	}
	newGroup->activate();
	my_groups.insert_or_assign(_group, newGroup);
}

void ak::aPropertyGridWidget::addGroup(
	const QColor &									_aColor,
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	auto itm = my_groups.find(_group);
	assert(itm == my_groups.end());	// Group already exists
	aPropertyGridGroup * newGroup = new aPropertyGridGroup(my_table, _group);
	newGroup->setItemsBackColor(_aColor, false);
	newGroup->setHeaderColors(my_groupHeaderForeColor, my_groupHeaderBackColor);
	newGroup->setStateIcons(&my_groupIconExpanded, &my_groupIconCollapsed);
	if (my_colorStyle != nullptr) {
		newGroup->setColorStyle(my_colorStyle, true);
	}
	newGroup->activate();
	my_groups.insert_or_assign(_group, newGroup);
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aColor &								_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QDate &									_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QTime &									_value
) { return newItemCreated(my_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

// ++++++++++++++++++++++++++++++++++++++++++

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aColor &									_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _possibleSelection, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QDate &									_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QTime &									_value
) {
	auto group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

void ak::aPropertyGridWidget::setItemReadOnly(
	ak::ID											_itemID,
	bool											_readOnly
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->setReadOnly(_readOnly);
}

bool ak::aPropertyGridWidget::itemIsReadOnly(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->isReadOnly();
}

void ak::aPropertyGridWidget::setGroupStateIcons(
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) {
	my_groupIconCollapsed = _groupCollapsed;
	my_groupIconExpanded = _groupExpanded;

	for (auto itm : my_groups) {
		itm.second->setStateIcons(&my_groupIconExpanded, &my_groupIconCollapsed);
	}
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	const QString &									_valueToReset
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	int												_valueToReset
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	double											_valueToReset
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::showItemAsError(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	itm->second->showAsError();
}

void ak::aPropertyGridWidget::setEnabled(
	bool											_enabled
) {
	my_isEnabled = _enabled;
	if (my_isEnabled) {
		for (auto itm : my_itemStateMap) { itm.first->setEnabled(itm.second); }
	}
	else {
		for (auto itm : my_itemStateMap) { itm.first->setEnabled(false); }
	}
}

// ##############################################################################################################

// Clear items

void ak::aPropertyGridWidget::clear(
	bool											_keepGroups
) {
	slotFocusLost();

	if (_keepGroups) {
		for (auto itm : my_groups) { itm.second->clear(); }
	}
	else {
		for (auto itm : my_groups) { delete itm.second; }
		my_groups.clear();
	}
	my_defaultGroup->clear();
	my_items.clear();
	my_itemStateMap.clear();
	my_currentID = ak::invalidID;
	itemCountChanged();
	emit cleared();
}

// ##############################################################################################################

// Item information gathering

QString ak::aPropertyGridWidget::getItemGroup(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getGroup();
}

bool ak::aPropertyGridWidget::getItemIsMultipleValues(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getIsMultipleValues();
}

QString ak::aPropertyGridWidget::getItemName(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getName();
}

std::vector<QString> ak::aPropertyGridWidget::getItemPossibleSelection(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getPossibleSelection();
}

bool ak::aPropertyGridWidget::getItemValueBool(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueBool();
}

ak::aColor ak::aPropertyGridWidget::getItemValueColor(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueColor();
}

double ak::aPropertyGridWidget::getItemValueDouble(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueDouble();
}

int ak::aPropertyGridWidget::getItemValueInteger(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueInteger();
}

QString ak::aPropertyGridWidget::getItemValueSelection(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueSelection();
}

QString ak::aPropertyGridWidget::getItemValueString(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueString();
}

QDate ak::aPropertyGridWidget::getItemValueDate(
	ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueDate();
}

QTime ak::aPropertyGridWidget::getItemValueTime(
	ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueTime();
}

ak::valueType ak::aPropertyGridWidget::getItemValueType(
	ak::ID											_itemID
) {
	auto itm = my_items.find(_itemID);
	assert(itm != my_items.end()); // Invalid item ID
	return itm->second->getValueType();
}

// ##############################################################################################################

// Slots

void ak::aPropertyGridWidget::slotItemChanged(void) {
	aPropertyGridItem * actualSender = nullptr;
	actualSender = dynamic_cast<aPropertyGridItem *>(sender());
	assert(actualSender != nullptr); // Cast failed
	emit itemChanged(actualSender->getId());
}

void ak::aPropertyGridWidget::slotCheckItemVisibility(void) {
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

void ak::aPropertyGridWidget::slotFocusLost(void) {
	for (auto group : my_groups) { group.second->deselect(); }
}

// ##############################################################################################################

// Private members

void ak::aPropertyGridWidget::itemCountChanged(void) {
	my_checkItemVisibilityRequired = true;
	// Queue the request to avoid flickering when clearing property grid and refilling it with new items
	QMetaObject::invokeMethod(this, "slotCheckItemVisibility", Qt::QueuedConnection);
}

ak::ID ak::aPropertyGridWidget::newItemCreated(aPropertyGridItem * _item) {
	_item->setId(++my_currentID);
	//if (my_colorStyle != nullptr) { _item->setColorStyle(my_colorStyle); }
	my_items.insert_or_assign(my_currentID, _item);
	my_itemStateMap.insert_or_assign(_item, true);
	_item->setEnabled(my_isEnabled);

	itemCountChanged();

	connect(_item, &aPropertyGridItem::changed, this, &aPropertyGridWidget::slotItemChanged);
	return my_currentID;
}

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

ak::aPropertyGridGroup::aPropertyGridGroup(
	aTableWidget *						_propertyGridTable,
	const QString &						_groupName
) : my_propertyGridTable(_propertyGridTable), my_isActivated(false), my_isVisible(true), my_isAlternateBackground(false), my_headerIsVisible(true),
	my_colorTextNormal(0, 0, 0), my_colorTextError(255, 0, 0), my_colorStyle(nullptr), my_colorStyleAlt(nullptr), my_externColorStyle(nullptr)
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

	// Initialize icons
	my_iconCollapsed = nullptr;
	my_iconExpanded = nullptr;

	setItemsBackColor(my_colorItemBackground, false);
	connect(my_propertyGridTable, &QTableWidget::itemDoubleClicked, this, &aPropertyGridGroup::slotDoubleClicked);
}

ak::aPropertyGridGroup::~aPropertyGridGroup() {
	int r = my_item->row();
	clear();
	delete my_item;
	my_propertyGridTable->removeRow(r);
}

QString ak::aPropertyGridGroup::name(void) const { return my_name; }

void ak::aPropertyGridGroup::setName(
	const QString &						_groupName
) {
	my_name = _groupName;
}

void ak::aPropertyGridGroup::activate(void) {
	my_isActivated = true;
	checkVisibility();
}

void ak::aPropertyGridGroup::setHeaderColors(
	const QColor &				_foreColor,
	const QColor &				_backColor
) {
	my_backColor = _backColor;
	my_foreColor = _foreColor;

	my_item->setBackgroundColor(my_backColor);
	my_item->setTextColor(my_foreColor);
}

void ak::aPropertyGridGroup::setItemsBackColor(
	const QColor &									_backgroudColor,
	bool											_repaint
) {
	my_colorItemBackground = _backgroudColor;
	int r = my_colorItemBackground.red() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int g = my_colorItemBackground.green() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int b = my_colorItemBackground.blue() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int a = my_colorItemBackground.alpha() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;

	if (r > 255 || g > 255 || b > 255) {
		r = my_colorItemBackground.red() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
		g = my_colorItemBackground.green() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
		b = my_colorItemBackground.blue() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	}
	if (r < 0) { r = 0; }
	if (g < 0) { g = 0; }
	if (b < 0) { b = 0; }
	if (a < 0) { a = 0; }
	if (a > 255) { a = 255; }
	my_colorItemBackgroundAlternate.setRgb(r, g, b, a);

	if (_repaint) {
		rebuildStyleSheets();
		repaint();
	}
}

void ak::aPropertyGridGroup::setItemsTextColors(
	const QColor &									_textColorNormal,
	const QColor &									_textColorError,
	bool											_repaint
) {
	my_colorTextNormal = _textColorNormal;
	my_colorTextError = _textColorError;

	if (_repaint) {
		rebuildStyleSheets();
		repaint();
	}
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
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
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
	ak::ID											_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aColor &								_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
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
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
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
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
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
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _possibleSelection, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
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
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
	ID												_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QDate &									_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
	ID												_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QTime &									_value
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.back()->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(my_propertyGridTable, my_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

void ak::aPropertyGridGroup::setGroupHeaderVisible(
	bool											_isVisible
) {
	assert(my_item != nullptr);	// This should never happen
	my_headerIsVisible = _isVisible;
	my_propertyGridTable->setRowHidden(my_item->row(), !my_headerIsVisible);
}

void ak::aPropertyGridGroup::setColorStyle(
	aColorStyle *									_colorStyle,
	bool											_repaint
) {
	my_externColorStyle = _colorStyle;

	if (_repaint) {
		rebuildStyleSheets();
		repaint();
	}
}

void ak::aPropertyGridGroup::clear(void) {
	for (auto itm : my_items) {
		aPropertyGridItem * actualItem = itm;
		delete actualItem;
	}
	my_items.clear();
	my_isAlternateBackground = false;
}

void ak::aPropertyGridGroup::deselect(void) {
	for (auto itm : my_items) { itm->deselect(); }
	my_item->setSelected(false);
}

void ak::aPropertyGridGroup::setStateIcons(
	QIcon *											_expanded,
	QIcon *											_collapsed
) {
	assert(_expanded != nullptr); // Nullptr provided
	assert(_collapsed != nullptr); // Nullptr provided
	my_iconCollapsed = _collapsed;
	my_iconExpanded = _expanded;
	refreshIcon();
}

// ##############################################################################################################

// slots

void ak::aPropertyGridGroup::slotDoubleClicked(QTableWidgetItem * _item) {
	if (_item == my_item) {
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), my_isVisible);
		}
		my_isVisible = !my_isVisible;
		refreshIcon();
	}
}

// ##############################################################################################################

// private members

void ak::aPropertyGridGroup::itemCreated(aPropertyGridItem * _item) {
	my_items.push_back(_item);
	if (my_isAlternateBackground) {
		_item->setColorStyle(my_colorStyleAlt);
		_item->setBackgroundColor(my_colorItemBackgroundAlternate);
	}
	else {
		_item->setColorStyle(my_colorStyle);
		_item->setBackgroundColor(my_colorItemBackground);
	}
	_item->setTextColors(my_colorTextNormal, my_colorTextError);
	my_isAlternateBackground = !my_isAlternateBackground;
	checkVisibility();
}

void ak::aPropertyGridGroup::checkVisibility(void) {
	if (!my_isActivated) { return; }
	if (my_items.size() > 0) {
		// Show group header (if is setVisible) and items
		my_propertyGridTable->setRowHidden(my_item->row(), !my_headerIsVisible);
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), !my_isVisible);
		}
	}
	else {
		// Hide group header
		my_propertyGridTable->setRowHidden(my_item->row(), true);
		my_isVisible = true;
	}
}

void ak::aPropertyGridGroup::rebuildStyleSheets() {
	if (my_colorStyle == nullptr) {
		my_colorStyle = new aCustomizableColorStyle;
	}
	if (my_colorStyleAlt == nullptr) {
		my_colorStyleAlt = new aCustomizableColorStyle;
	}

	// Set default colors
	my_colorStyle->setControlsMainForegroundColor(my_colorTextNormal);
	my_colorStyle->setControlsMainBackgroundColor(my_colorItemBackground);
	my_colorStyle->setAlternateForegroundColor(my_colorTextNormal);
	my_colorStyle->setAlternateBackgroundColor(my_colorItemBackgroundAlternate);
	my_colorStyle->setHeaderBackgroundColor(my_colorItemBackground);
	my_colorStyle->setHeaderForegroundColor(my_colorTextNormal);

	my_colorStyleAlt->setControlsMainForegroundColor(my_colorTextNormal);
	my_colorStyleAlt->setControlsMainBackgroundColor(my_colorItemBackground);
	my_colorStyleAlt->setAlternateForegroundColor(my_colorTextNormal);
	my_colorStyleAlt->setAlternateBackgroundColor(my_colorItemBackgroundAlternate);
	my_colorStyleAlt->setHeaderBackgroundColor(my_colorItemBackground);
	my_colorStyleAlt->setHeaderForegroundColor(my_colorTextNormal);

	// Set sheets
	my_colorStyle->setSheet(cafBackgroundColorControls, aColor::toHexString(my_colorItemBackground, true, "background-color: #"));
	my_colorStyle->setSheet(cafBackgroundColorWindow, aColor::toHexString(my_colorItemBackground, true, "background-color: #"));
	my_colorStyle->setSheet(cafBackgroundColorButton, aColor::toHexString(my_colorItemBackground, true, "background-color: #"));
	my_colorStyle->setSheet(cafForegroundColorControls, aColor::toHexString(my_colorTextNormal, true, "color: #"));
	my_colorStyle->setSheet(cafForegroundColorButton, aColor::toHexString(my_colorTextNormal, true, "color: #"));
	my_colorStyle->setSheet(cafForegroundColorWindow, aColor::toHexString(my_colorTextNormal, true, "color: #"));

	my_colorStyleAlt->setSheet(cafBackgroundColorControls, aColor::toHexString(my_colorItemBackgroundAlternate, true, "background-color: #"));
	my_colorStyleAlt->setSheet(cafBackgroundColorWindow, aColor::toHexString(my_colorItemBackgroundAlternate, true, "background-color: #"));
	my_colorStyleAlt->setSheet(cafBackgroundColorButton, aColor::toHexString(my_colorItemBackgroundAlternate, true, "background-color: #"));
	my_colorStyleAlt->setSheet(cafForegroundColorControls, aColor::toHexString(my_colorTextNormal, true, "color: #"));
	my_colorStyleAlt->setSheet(cafForegroundColorButton, aColor::toHexString(my_colorTextNormal, true, "color: #"));
	my_colorStyleAlt->setSheet(cafForegroundColorWindow, aColor::toHexString(my_colorTextNormal, true, "color: #"));

	if (my_externColorStyle == nullptr) {
		my_colorStyle->setSheet(cafBackgroundColorFocus, "");
		my_colorStyleAlt->setSheet(cafBackgroundColorFocus, "");

		my_colorStyle->setSheet(cafForegroundColorFocus, "");
		my_colorStyleAlt->setSheet(cafForegroundColorFocus, "");

		my_colorStyle->setSheet(cafBackgroundColorSelected, "");
		my_colorStyleAlt->setSheet(cafBackgroundColorSelected, "");

		my_colorStyle->setSheet(cafForegroundColorSelected, "");
		my_colorStyleAlt->setSheet(cafForegroundColorSelected, "");

		my_colorStyle->setSheet(cafBorderColorWindow, "");
		my_colorStyleAlt->setSheet(cafBorderColorWindow, "");

		my_colorStyle->setSheet(cafDefaultBorderWindow, "");
		my_colorStyleAlt->setSheet(cafDefaultBorderWindow, "");
	}
	else {
		my_colorStyle->setSheet(cafBackgroundColorFocus, my_externColorStyle->toStyleSheet(cafBackgroundColorFocus));
		my_colorStyleAlt->setSheet(cafBackgroundColorFocus, my_externColorStyle->toStyleSheet(cafBackgroundColorFocus));

		my_colorStyle->setSheet(cafForegroundColorFocus, my_externColorStyle->toStyleSheet(cafForegroundColorFocus));
		my_colorStyleAlt->setSheet(cafForegroundColorFocus, my_externColorStyle->toStyleSheet(cafForegroundColorFocus));

		my_colorStyle->setSheet(cafBackgroundColorSelected, my_externColorStyle->toStyleSheet(cafBackgroundColorSelected));
		my_colorStyleAlt->setSheet(cafBackgroundColorSelected, my_externColorStyle->toStyleSheet(cafBackgroundColorSelected));

		my_colorStyle->setSheet(cafForegroundColorSelected, my_externColorStyle->toStyleSheet(cafForegroundColorSelected));
		my_colorStyleAlt->setSheet(cafForegroundColorSelected, my_externColorStyle->toStyleSheet(cafForegroundColorSelected));

		my_colorStyle->setSheet(cafBorderColorWindow, my_externColorStyle->toStyleSheet(cafBorderColorWindow));
		my_colorStyleAlt->setSheet(cafBorderColorWindow, my_externColorStyle->toStyleSheet(cafBorderColorWindow));

		my_colorStyle->setSheet(cafDefaultBorderWindow, my_externColorStyle->toStyleSheet(cafDefaultBorderWindow));
		my_colorStyleAlt->setSheet(cafDefaultBorderWindow, my_externColorStyle->toStyleSheet(cafDefaultBorderWindow));
	}

}

void ak::aPropertyGridGroup::repaint(void) {
	my_isAlternateBackground = false;
	for (auto itm : my_items) {
		if (my_isAlternateBackground) {
			itm->setBackgroundColor(my_colorItemBackgroundAlternate);
			itm->setColorStyle(my_colorStyleAlt);
		}
		else { 
			itm->setBackgroundColor(my_colorItemBackground);
			itm->setColorStyle(my_colorStyle);
		}
		itm->setTextColors(my_colorTextNormal, my_colorTextError);
		my_isAlternateBackground = !my_isAlternateBackground;
	}
}

void ak::aPropertyGridGroup::refreshIcon(void) {
	if (my_iconCollapsed != nullptr) {
		assert(my_iconExpanded != nullptr); // This should never happen
		if (my_isVisible) { my_item->setIcon(*my_iconExpanded); }
		else { my_item->setIcon(*my_iconCollapsed); }
	}
}

// ##############################################################################################################++++++++++++++++++++++++++++++

// ##############################################################################################################++++++++++++++++++++++++++++++

// ##############################################################################################################++++++++++++++++++++++++++++++

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	bool								_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtBool),
	my_name(_settingName), my_valueBool(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetBool = new aCheckBoxWidget();
	if (my_isMultipleValues) {
		my_widgetBool->setTristate(true);
		my_widgetBool->setCheckState(Qt::PartiallyChecked);
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

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetBool);

	connect(my_widgetBool, &QCheckBox::stateChanged, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const aColor &					_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtColor),
	my_name(_settingName), my_valueColor(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetColor = new aColorEditButtonWidget(_value);
	if (my_isMultipleValues) {
		my_widgetColor->overrideText("...");
	}
	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetColor->widget());

	connect(my_widgetColor, &aColorEditButtonWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	double								_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtDouble),
	my_name(_settingName), my_valueDouble(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
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

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	int									_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtInt),
	my_name(_settingName), my_valueInteger(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
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

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const std::vector<QString> &		_possibleSlection,
	const QString &						_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtSelection),
	my_name(_settingName), my_valueSelection(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	if (my_isMultipleValues) {
		my_widgetSelection = new aComboButtonWidget("...");
	}
	else {
		my_widgetSelection = new aComboButtonWidget(_value);
	}
	my_valuePossibleSelection = _possibleSlection;
	my_widgetSelection->setItems(my_valuePossibleSelection);
	my_widgetSelection->setAutoFillBackground(true);

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetSelection);

	connect(my_widgetSelection, &aComboButtonWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const QString &						_value
) : my_propertyGridTable(_aPropertyGridWidgetTable), my_group(_group), my_valueType(vtString),
	my_name(_settingName), my_valueString(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
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

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setItem(_row, 1, my_cellValue);
	connect(my_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const QDate &						_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(vtDate),
	my_name(_settingName), my_valueDate(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr),
	my_cellSettingName(nullptr), my_cellValue(nullptr)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetDate = new aDatePickWidget(_value, dfDDMMYYYY);

	if (my_isMultipleValues) {
		my_widgetDate->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetDate);

	connect(my_widgetDate, &aDatePickWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const QTime &						_value
) : my_propertyGridTable(_propertyGridTable), my_group(_group), my_valueType(vtTime),
	my_name(_settingName), my_valueTime(_value), my_isMultipleValues(_isMultipleValues), my_globalColorStyle(nullptr)
{
	assert(my_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	my_cellSettingName = new QTableWidgetItem(_settingName);
	my_widgetTime = new aTimePickWidget(_value, tfHHMM);

	if (my_isMultipleValues) {
		my_widgetTime->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = my_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	my_propertyGridTable->setItem(_row, 0, my_cellSettingName);
	my_propertyGridTable->setCellWidget(_row, 1, my_widgetTime);

	connect(my_widgetTime, &aTimePickWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::~aPropertyGridItem() {
	assert(my_cellSettingName != nullptr); // This should never happen
	if (my_cellValue != nullptr) { delete my_cellValue; }
	if (my_widgetBool != nullptr) { delete my_widgetBool; }
	if (my_widgetColor != nullptr) { delete my_widgetColor; }
	if (my_widgetSelection != nullptr) { delete my_widgetSelection; }
	int r = my_cellSettingName->row();
	delete my_cellSettingName;
	my_propertyGridTable->removeRow(r);
}

int ak::aPropertyGridItem::row() const {
	return my_cellSettingName->row();
}

void ak::aPropertyGridItem::setId(
	ak::ID								_ID
) { my_id = _ID; }

ak::ID ak::aPropertyGridItem::getId(void) const { return my_id; }

bool ak::aPropertyGridItem::getIsCurrentlyError() const { return my_isCurrentlyError; }

void ak::aPropertyGridItem::setBackgroundColor(
	const QColor &					_backgroundColor
) {
	my_colorBackground = _backgroundColor; 
	repaint();
}

void ak::aPropertyGridItem::setTextColors(
	const QColor &					_foregroundNormal,
	const QColor &					_foregroundError
) {
	my_colorErrorForeground = _foregroundError;
	my_colorNormalForeground = _foregroundNormal;
	repaint();
}

void ak::aPropertyGridItem::deselect(void) {
	my_cellSettingName->setSelected(false);
	if (my_cellValue != nullptr) { my_cellValue->setSelected(false); }
}

// #################################################################################

// Information gathering

QString ak::aPropertyGridItem::getGroup(void) const { return my_group; }

bool ak::aPropertyGridItem::getIsMultipleValues(void) const { return my_isMultipleValues; }

QString ak::aPropertyGridItem::getName(void) const { return my_name; }

ak::valueType ak::aPropertyGridItem::getValueType(void) const { return my_valueType; }

std::vector<QString> ak::aPropertyGridItem::getPossibleSelection(void) const {
	assert(my_valueType == vtSelection);	// Wrong value type for this item
	return my_valuePossibleSelection;
}

bool ak::aPropertyGridItem::getValueBool(void) const {
	assert(my_valueType == vtBool);	// Wrong value type for this item
	return my_valueBool;
}

ak::aColor ak::aPropertyGridItem::getValueColor(void) const {
	assert(my_valueType == vtColor);	// Wrong value type for this item
	return my_valueColor;
}

double ak::aPropertyGridItem::getValueDouble(void) const {
	assert(my_valueType == vtDouble);	// Wrong value type for this item
	return my_valueDouble;
}

int ak::aPropertyGridItem::getValueInteger(void) const {
	assert(my_valueType == vtInt);	// Wrong value type for this item
	return my_valueInteger;
}

QString ak::aPropertyGridItem::getValueSelection(void) const {
	assert(my_valueType == vtSelection);	// Wrong value type for this item
	return my_valueSelection;
}

QString ak::aPropertyGridItem::getValueString(void) const {
	assert(my_valueType == vtString);	// Wrong value type for this item
	return my_valueString;
}

QDate ak::aPropertyGridItem::getValueDate(void) const {
	assert(my_valueType == vtDate);
	return my_valueDate;
}

QTime ak::aPropertyGridItem::getValueTime(void) const {
	assert(my_valueType == vtTime);
	return my_valueTime;
}

bool ak::aPropertyGridItem::isMultipleValues(void) const { return my_isMultipleValues; }

void ak::aPropertyGridItem::setReadOnly(
	bool					_readOnly
) {
	my_ignoreCellEvent = true;
	my_isReadOnly = _readOnly;
	switch (my_valueType)
	{
	case vtBool:
		assert(my_widgetBool != nullptr); // This should not happen
		my_widgetBool->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtColor:
		assert(my_widgetColor != nullptr); // This should not happen
		my_widgetColor->SetEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtSelection:
		assert(my_widgetSelection != nullptr); // This should not happen
		my_widgetSelection->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtDate:
		assert(my_widgetDate != nullptr); // This should not happen
		my_widgetDate->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtTime:
		assert(my_widgetTime != nullptr); // This should not happen
		my_widgetTime->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtDouble:
	case vtInt:
	case vtString:
	{
		Qt::ItemFlags f = my_cellValue->flags();
		f.setFlag(Qt::ItemFlag::ItemIsEditable, !my_isReadOnly && my_isEnabled);
		my_cellValue->setFlags(f);
	}
	break;
	default:
		assert(0); // Invalid value type
		break;
	}
	my_ignoreCellEvent = false;
}

bool ak::aPropertyGridItem::isReadOnly() { return my_isReadOnly; }

void ak::aPropertyGridItem::setEnabled(
	bool					_enabled
) {
	my_ignoreCellEvent = true;
	my_isEnabled = _enabled;
	switch (my_valueType)
	{
	case vtBool:
		assert(my_widgetBool != nullptr); // This should not happen
		my_widgetBool->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtColor:
		assert(my_widgetColor != nullptr); // This should not happen
		my_widgetColor->SetEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtSelection:
		assert(my_widgetSelection != nullptr); // This should not happen
		my_widgetSelection->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtDate:
		assert(my_widgetDate != nullptr); // This should not happen
		my_widgetDate->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtTime:
		assert(my_widgetTime != nullptr); // This should not happen
		my_widgetTime->setEnabled(!my_isReadOnly && my_isEnabled);
		break;
	case vtDouble:
	case vtInt:
	case vtString:
	{
		Qt::ItemFlags f = my_cellValue->flags();
		f.setFlag(Qt::ItemFlag::ItemIsEditable, !my_isReadOnly && my_isEnabled);
		my_cellValue->setFlags(f);
	}
	break;
	default:
		assert(0); // Invalid value type
		break;
	}
	my_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::setColorStyle(
	aColorStyle *	_style
) {
	assert(_style != nullptr); // Nullptr provided
	my_globalColorStyle = _style;
	repaint();
}

void ak::aPropertyGridItem::resetAsError(
	const QString &									_valueToReset
) {
	assert(my_valueType == vtString);
	my_ignoreCellEvent = true;
	my_valueString = _valueToReset;
	my_isCurrentlyError = true;
	my_cellValue->setText(my_valueString);
	repaint();
	my_cellValue->setSelected(false);
	my_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::resetAsError(
	int												_valueToReset
) {
	assert(my_valueType == vtInt);
	my_ignoreCellEvent = true;
	my_valueInteger = _valueToReset;
	my_isCurrentlyError = true;
	my_cellValue->setText(QString::number(my_valueInteger));
	repaint();
	my_cellValue->setSelected(false);
	my_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::resetAsError(
	double											_valueToReset
) {
	assert(my_valueType == vtDouble);
	my_ignoreCellEvent = true;
	my_valueDouble = _valueToReset;
	my_isCurrentlyError = true;
	my_cellValue->setText(QString::number(my_valueDouble));
	repaint();
	my_cellValue->setSelected(false);
	my_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::showAsError(void) {
	my_ignoreCellEvent = true;
	my_isCurrentlyError = true;
	my_cellValue->setSelected(false);
	repaint();
	my_ignoreCellEvent = false;
}

// #################################################################################

// Slots

void ak::aPropertyGridItem::slotValueWidgetEvent() {
	switch (my_valueType)
	{
	case vtBool:
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
	case vtColor:
		my_valueColor = my_widgetColor->color();
		my_isMultipleValues = false;
		emit changed();
		break;
	case vtDate:
		my_valueDate = my_widgetDate->currentDate();
		my_isMultipleValues = false;
		emit changed();
		break;
	case vtSelection:
		my_valueSelection = my_widgetSelection->text();
		my_isMultipleValues = false;
		emit changed();
		break;
	case vtTime:
		my_valueTime = my_widgetTime->currentTime();
		my_isMultipleValues = false;
		emit changed();
		break;
	default:
		assert(0); // This should not have happened
		break;
	}
}

void ak::aPropertyGridItem::slotTableCellChanged(
	QTableWidgetItem *									_item
) {
	if (my_ignoreCellEvent) { return; }
	if (_item == my_cellValue) {

		if (my_isCurrentlyError) {
			my_isCurrentlyError = false;
			repaint();
		}

		QString theText(_item->text());
		switch (my_valueType)
		{
		case vtDouble:
		{
			my_ignoreCellEvent = true;
			bool failed = false;
			double v;
			v = toNumber<double>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected numeric.");
				aPromptDialog dia(msg, "Error", promptType::promptOk);
				dia.showDialog();
				my_cellValue->setSelected(false);
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
		case vtInt:
		{
			my_ignoreCellEvent = true;
			bool failed = false;
			int v;
			v = toNumber<int>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(my_name);
				msg.append(". Expected natural number.");
				aPromptDialog dia(msg, "Error", promptType::promptOk);
				dia.showDialog();
				my_cellValue->setSelected(false);
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
		case vtString:
			my_valueString = theText;
			emit changed();
			break;
		default:
			assert(0); // Invalid type
			break;
		}
	}
}

void ak::aPropertyGridItem::ini(void) {
	my_cellSettingName = nullptr;
	my_cellValue = nullptr;
	my_widgetBool = nullptr;
	my_widgetColor = nullptr;
	my_widgetSelection = nullptr;
	my_ignoreCellEvent = false;
	my_isCurrentlyError = false;
	my_isEnabled = true;
	my_isReadOnly = false;
	my_colorErrorForeground = QColor(255, 0, 0);
	my_colorNormalForeground = QColor(0, 0, 0);
}

void ak::aPropertyGridItem::repaint(void) {
	if (my_cellSettingName != nullptr) {
		my_cellSettingName->setBackgroundColor(my_colorBackground.toQColor());
		my_cellSettingName->setTextColor(my_colorNormalForeground.toQColor());
	}
	if (my_cellValue != nullptr) {
		my_ignoreCellEvent = true;
		my_cellValue->setBackgroundColor(my_colorBackground.toQColor());
		if (my_isCurrentlyError) {
			my_cellValue->setTextColor(my_colorErrorForeground.toQColor());
		}
		else {
			my_cellValue->setTextColor(my_colorNormalForeground.toQColor());
		}
		my_ignoreCellEvent = false;
	}
	else {
		switch (my_valueType)
		{
		case vtBool:
		{
			assert(my_widgetBool != nullptr); // Something went wrong
			QString sheet = my_globalColorStyle->toStyleSheet(cafBackgroundColorControls | cafForegroundColorControls, "QCheckBox{", "}");
			my_widgetBool->setStyleSheet(sheet);
			/*QString sheet("QCheckBox{background-color:#");
			sheet.append(my_colorBackground.toHexString(true));
			sheet.append(";}\n");
			my_widgetBool->setStyleSheet(sheet);*/
		}
			break;
		case vtColor:
		{
			assert(my_widgetColor != nullptr); // Something went wrong
			my_widgetColor->setColorStyle(my_globalColorStyle);
			my_widgetColor->fillBackground(my_colorBackground);
			/*QString sheet("QPushButton{background-color: #");
			sheet.append(my_colorBackground.toHexString(true));
			sheet.append("; color: #");
			sheet.append(my_colorNormalForeground.toHexString(true));
			sheet.append("; }");

			if (my_globalColorStyle != nullptr) {
				sheet.append(my_globalColorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus, "QPushButton:hover{", "}"));
				sheet.append(my_globalColorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected, "QPushButton:pressed{", "}"));
			}

			my_widgetColor->setPushButtonStyleSheet(sheet);*/
		}
			break;
		case vtDate:
			assert(my_widgetDate != nullptr);	// Something went wrong
			my_widgetDate->setColorStyle(my_globalColorStyle);
			break;
		case vtTime:
			assert(my_widgetTime != nullptr);	// Something went wrong
			my_widgetTime->setColorStyle(my_globalColorStyle);
			break;
		case vtSelection:
		{
			assert(my_widgetSelection != nullptr); // Something went wrong
			my_widgetSelection->setColorStyle(my_globalColorStyle);
			/*QString sheet("QPushButton{color: #");
			aColor fore(my_colorNormalForeground);
			aColor back(my_colorBackground);
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append(";} QPushButton QMenu{color: #");
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append("; alternate-background-color: red; border 1px solid black; }");

			if (my_globalColorStyle != nullptr) {
				sheet.append(my_globalColorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus, "QPushButton:hover{", "}"));
				sheet.append(my_globalColorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected, "QPushButton:pressed{", "}"));
			}
			
			my_widgetSelection->setStyleSheet(sheet);
			*/
		}
			break;
		default:
			break;
		}
	}
}