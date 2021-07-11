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
	: aWidget(otPropertyGrid), m_currentID(invalidID),
	m_groupHeaderBackColor(80,80,80), m_groupHeaderForeColor(0,0,0), m_itemDefaultBackgroundColor(230,230,230),
	m_itemTextColorError(255,0,0), m_itemTextColorNormal(0,0,0),
	m_layout(nullptr), m_infoTextEdit(nullptr), m_isEnabled(true)
{
	// Create central widget
	setContentsMargins(0, 0, 0, 0);
	m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);

	// Create info field
	m_infoTextEdit = new aTextEditWidget("No items");
	m_infoTextEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);
	m_infoTextEdit->setReadOnly(true);
	m_infoTextEdit->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	m_layout->addWidget(m_infoTextEdit);

	// Create table
	m_table = new aTableWidget(0, 2);
	m_table->verticalHeader()->setVisible(false);
	m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);

	QStringList lst;
	lst.push_back("Name");
	lst.push_back("Value");
	m_table->setHorizontalHeaderLabels(lst);

	m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
	m_table->horizontalHeader()->setEnabled(false);

	// Create default group
	m_defaultGroup = new aPropertyGridGroup(m_table, "");
	m_defaultGroup->setItemsBackColor(m_itemDefaultBackgroundColor, false);
	m_defaultGroup->setItemsTextColors(m_itemTextColorNormal, m_itemTextColorError, false);
	m_defaultGroup->setHeaderColors(m_groupHeaderForeColor, m_groupHeaderBackColor);
	m_defaultGroup->setGroupHeaderVisible(false);
	m_defaultGroup->activate();

	connect(m_table, &aTableWidget::focusLost, this, &aPropertyGridWidget::slotFocusLost);
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
	m_colorStyle = _aColorStyle;
	//m_table->setStyleSheet(m_colorStyle->getStylesheet(ak::aColorStyle::styleableObject::sTable));
	m_groupHeaderBackColor = m_colorStyle->getHeaderBackgroundColor().toQColor();
	m_groupHeaderForeColor = m_colorStyle->getHeaderForegroundColor().toQColor();
	m_itemDefaultBackgroundColor = m_colorStyle->getControlsMainBackgroundColor().toQColor();
	m_itemTextColorNormal = m_colorStyle->getControlsMainForegroundColor().toQColor();
	m_itemTextColorError = m_colorStyle->getControlsErrorFrontForegroundColor().toQColor();

	for (auto itm : m_groups) { 
		itm.second->setHeaderColors(m_groupHeaderForeColor, m_groupHeaderBackColor);
		itm.second->setColorStyle(m_colorStyle, true);
	}

	m_defaultGroup->setHeaderColors(m_groupHeaderForeColor, m_groupHeaderBackColor);
	m_defaultGroup->setItemsTextColors(m_itemTextColorNormal, m_itemTextColorError, false);
	m_defaultGroup->setItemsBackColor(m_itemDefaultBackgroundColor, true);

	QString sheet("QHeaderView{border: none;}\n");

	sheet.append(m_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader, "QHeaderView::section{","border: 0px}"
		"QHeaderView::section:first{border-right: 1px solid black;}"));

	m_table->horizontalHeader()->setStyleSheet(sheet);
	m_table->setColorStyle(m_colorStyle);
	m_infoTextEdit->setColorStyle(m_colorStyle);

	for (auto itm : m_items) { itm.second->setColorStyle(m_colorStyle); }

}

void ak::aPropertyGridWidget::setAlias(
	const QString &							_alias
) {
	aObject::setAlias(_alias);
	m_table->setObjectName(m_alias + "__Table");
	m_infoTextEdit->setObjectName(m_alias + "__TextEdit");
}

// ##############################################################################################################

// Item creation and manipulation

void ak::aPropertyGridWidget::addGroup(
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	auto itm = m_groups.find(_group);
	assert(itm == m_groups.end());	// Group already exists
	aPropertyGridGroup * newGroup = new aPropertyGridGroup(m_table, _group);
	newGroup->setItemsBackColor(m_itemDefaultBackgroundColor, false);
	newGroup->setHeaderColors(m_groupHeaderForeColor, m_groupHeaderBackColor);
	newGroup->setStateIcons(&m_groupIconExpanded, &m_groupIconCollapsed);
	if (m_colorStyle != nullptr) {
		newGroup->setColorStyle(m_colorStyle, true);
	}
	newGroup->activate();
	m_groups.insert_or_assign(_group, newGroup);
}

void ak::aPropertyGridWidget::addGroup(
	const QColor &									_aColor,
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	auto itm = m_groups.find(_group);
	assert(itm == m_groups.end());	// Group already exists
	aPropertyGridGroup * newGroup = new aPropertyGridGroup(m_table, _group);
	newGroup->setItemsBackColor(_aColor, false);
	newGroup->setHeaderColors(m_groupHeaderForeColor, m_groupHeaderBackColor);
	newGroup->setStateIcons(&m_groupIconExpanded, &m_groupIconCollapsed);
	if (m_colorStyle != nullptr) {
		newGroup->setColorStyle(m_colorStyle, true);
	}
	newGroup->activate();
	m_groups.insert_or_assign(_group, newGroup);
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	bool											_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aColor &								_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	double											_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	int												_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const QString &									_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aDate &									_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aTime &									_value
) { return newItemCreated(m_defaultGroup->addItem(-1, _isMultipleValues, _settingName, _value)); }

// ++++++++++++++++++++++++++++++++++++++++++

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	bool											_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aColor &									_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	double											_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	int												_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const std::vector<QString> &					_possibleSelection,
	const QString &									_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _possibleSelection, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aDate &									_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

ak::ID ak::aPropertyGridWidget::addItem(
	bool											_isMultipleValues,
	const QString &									_groupName,
	const QString &									_settingName,
	const aTime &									_value
) {
	auto group = m_groups.find(_groupName);
	assert(group != m_groups.end());	// Invalid group name
	return newItemCreated(group->second->addItem(-1, _isMultipleValues, _settingName, _value));
}

void ak::aPropertyGridWidget::setItemReadOnly(
	ak::ID											_itemID,
	bool											_readOnly
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	itm->second->setReadOnly(_readOnly);
}

bool ak::aPropertyGridWidget::itemIsReadOnly(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->isReadOnly();
}

void ak::aPropertyGridWidget::setGroupStateIcons(
	const QIcon &									_groupExpanded,
	const QIcon &									_groupCollapsed
) {
	m_groupIconCollapsed = _groupCollapsed;
	m_groupIconExpanded = _groupExpanded;

	for (auto itm : m_groups) {
		itm.second->setStateIcons(&m_groupIconExpanded, &m_groupIconCollapsed);
	}
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	const QString &									_valueToReset
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	int												_valueToReset
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::resetItemAsError(
	ak::ID											_itemID,
	double											_valueToReset
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	itm->second->resetAsError(_valueToReset);
}

void ak::aPropertyGridWidget::showItemAsError(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	itm->second->showAsError();
}

void ak::aPropertyGridWidget::setEnabled(
	bool											_enabled
) {
	m_isEnabled = _enabled;
	if (m_isEnabled) {
		for (auto itm : m_itemStateMap) { itm.first->setEnabled(itm.second); }
	}
	else {
		for (auto itm : m_itemStateMap) { itm.first->setEnabled(false); }
	}
}

// ##############################################################################################################

// Clear items

void ak::aPropertyGridWidget::clear(
	bool											_keepGroups
) {
	slotFocusLost();

	if (_keepGroups) {
		for (auto itm : m_groups) { itm.second->clear(); }
	}
	else {
		auto oldGroups = m_groups;
		m_groups.clear();

		for (auto itm : oldGroups) { delete itm.second; }
	}
	m_defaultGroup->clear();
	m_items.clear();
	m_itemStateMap.clear();
	m_currentID = ak::invalidID;
	itemCountChanged();
	emit cleared();
}

// ##############################################################################################################

// Item information gathering

QString ak::aPropertyGridWidget::getItemGroup(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getGroup();
}

bool ak::aPropertyGridWidget::getItemIsMultipleValues(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getIsMultipleValues();
}

QString ak::aPropertyGridWidget::getItemName(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getName();
}

std::vector<QString> ak::aPropertyGridWidget::getItemPossibleSelection(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getPossibleSelection();
}

bool ak::aPropertyGridWidget::getItemValueBool(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueBool();
}

ak::aColor ak::aPropertyGridWidget::getItemValueColor(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueColor();
}

double ak::aPropertyGridWidget::getItemValueDouble(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueDouble();
}

int ak::aPropertyGridWidget::getItemValueInteger(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueInteger();
}

QString ak::aPropertyGridWidget::getItemValueSelection(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueSelection();
}

QString ak::aPropertyGridWidget::getItemValueString(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueString();
}

ak::aDate ak::aPropertyGridWidget::getItemValueDate(
	ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueDate();
}

ak::aTime ak::aPropertyGridWidget::getItemValueTime(
	ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
	return itm->second->getValueTime();
}

ak::valueType ak::aPropertyGridWidget::getItemValueType(
	ak::ID											_itemID
) {
	auto itm = m_items.find(_itemID);
	assert(itm != m_items.end()); // Invalid item ID
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
	if (m_checkItemVisibilityRequired) {
		m_checkItemVisibilityRequired = false;
		if (m_items.size() == 0) {
			if (m_table->isVisible()) {
				m_layout->removeWidget(m_table);
				m_layout->addWidget(m_infoTextEdit);
				m_infoTextEdit->setVisible(true);
				m_table->setVisible(false);
				//m_layout->setContentsMargins(2, 2, 2, 2);
			}
		}
		else {
			if (!m_table->isVisible()) {
				m_layout->removeWidget(m_infoTextEdit);
				m_layout->addWidget(m_table);
				m_infoTextEdit->setVisible(false);
				m_table->setVisible(true);
				//m_layout->setContentsMargins(0, 0, 0, 0);
			}

		}
	}
}

void ak::aPropertyGridWidget::slotFocusLost(void) {
	for (auto group : m_groups) { group.second->deselect(); }
}

// ##############################################################################################################

// Private members

void ak::aPropertyGridWidget::itemCountChanged(void) {
	m_checkItemVisibilityRequired = true;
	// Queue the request to avoid flickering when clearing property grid and refilling it with new items
	QMetaObject::invokeMethod(this, "slotCheckItemVisibility", Qt::QueuedConnection);
}

ak::ID ak::aPropertyGridWidget::newItemCreated(aPropertyGridItem * _item) {
	_item->setId(++m_currentID);
	//if (m_colorStyle != nullptr) { _item->setColorStyle(m_colorStyle); }
	m_items.insert_or_assign(m_currentID, _item);
	m_itemStateMap.insert_or_assign(_item, true);
	_item->setEnabled(m_isEnabled);

	itemCountChanged();

	connect(_item, &aPropertyGridItem::changed, this, &aPropertyGridWidget::slotItemChanged);
	return m_currentID;
}

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

// ##############################################################################################################********************************************************

ak::aPropertyGridGroup::aPropertyGridGroup(
	aTableWidget *						_propertyGridTable,
	const QString &						_groupName
) : m_propertyGridTable(_propertyGridTable), m_isActivated(false), m_isVisible(true), m_isAlternateBackground(false), m_headerIsVisible(true),
	m_colorTextNormal(0, 0, 0), m_colorTextError(255, 0, 0), m_colorStyle(nullptr), m_colorStyleAlt(nullptr), m_externColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);
	m_item = new QTableWidgetItem;
	m_name = _groupName;
	m_item->setText(_groupName);
	Qt::ItemFlags flags = m_item->flags();
	flags.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	flags.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_item->setFlags(flags);

	int r = m_propertyGridTable->rowCount();
	m_propertyGridTable->insertRow(r);
	m_propertyGridTable->setSpan(r, 0, 1, 2);
	m_propertyGridTable->setItem(r, 0, m_item);
	QFont font = m_item->font();
	font.setBold(true);
	m_item->setFont(font);

	// Initialize icons
	m_iconCollapsed = nullptr;
	m_iconExpanded = nullptr;

	setItemsBackColor(m_colorItemBackground, false);
	connect(m_propertyGridTable, &QTableWidget::itemDoubleClicked, this, &aPropertyGridGroup::slotDoubleClicked);
}

ak::aPropertyGridGroup::~aPropertyGridGroup() {
	int r = m_item->row();
	clear();
	delete m_item;
	m_propertyGridTable->removeRow(r);
}

QString ak::aPropertyGridGroup::name(void) const { return m_name; }

void ak::aPropertyGridGroup::setName(
	const QString &						_groupName
) {
	m_name = _groupName;
}

void ak::aPropertyGridGroup::activate(void) {
	m_isActivated = true;
	checkVisibility();
}

void ak::aPropertyGridGroup::setHeaderColors(
	const QColor &				_foreColor,
	const QColor &				_backColor
) {
	m_backColor = _backColor;
	m_foreColor = _foreColor;

	m_item->setBackgroundColor(m_backColor);
	m_item->setTextColor(m_foreColor);
}

void ak::aPropertyGridGroup::setItemsBackColor(
	const QColor &									_backgroudColor,
	bool											_repaint
) {
	m_colorItemBackground = _backgroudColor;
	int r = m_colorItemBackground.red() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int g = m_colorItemBackground.green() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int b = m_colorItemBackground.blue() + AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	int a = m_colorItemBackground.alpha() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;

	if (r > 255 || g > 255 || b > 255) {
		r = m_colorItemBackground.red() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
		g = m_colorItemBackground.green() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
		b = m_colorItemBackground.blue() - AK_INTERN_ALTERNATE_ROW_COLOR_VALUE;
	}
	if (r < 0) { r = 0; }
	if (g < 0) { g = 0; }
	if (b < 0) { b = 0; }
	if (a < 0) { a = 0; }
	if (a > 255) { a = 255; }
	m_colorItemBackgroundAlternate.setRgb(r, g, b, a);

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
	m_colorTextNormal = _textColorNormal;
	m_colorTextError = _textColorError;

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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _possibleSelection, _value);
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
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
	ID												_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aDate &									_value
) {
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

ak::aPropertyGridItem * ak::aPropertyGridGroup::addItem(
	ID												_itemId,
	bool											_isMultipleValues,
	const QString &									_settingName,
	const aTime &									_value
) {
	int r = m_item->row();
	if (m_items.size() > 0) {
		r = m_items.back()->row();
	}
	m_propertyGridTable->insertRow(r + 1);
	aPropertyGridItem * newItem = new aPropertyGridItem(m_propertyGridTable, m_name, r + 1, _isMultipleValues, _settingName, _value);
	itemCreated(newItem);
	newItem->setId(_itemId);
	return newItem;
}

void ak::aPropertyGridGroup::setGroupHeaderVisible(
	bool											_isVisible
) {
	assert(m_item != nullptr);	// This should never happen
	m_headerIsVisible = _isVisible;
	m_propertyGridTable->setRowHidden(m_item->row(), !m_headerIsVisible);
}

void ak::aPropertyGridGroup::setColorStyle(
	aColorStyle *									_colorStyle,
	bool											_repaint
) {
	m_externColorStyle = _colorStyle;

	if (_repaint) {
		rebuildStyleSheets();
		repaint();
	}
}

void ak::aPropertyGridGroup::clear(void) {
	auto oldItems = m_items;
	m_items.clear();

	for (auto itm : oldItems) {
		aPropertyGridItem * actualItem = itm;
		delete actualItem;
	}

	m_isAlternateBackground = false;
}

void ak::aPropertyGridGroup::deselect(void) {
	for (auto itm : m_items) { itm->deselect(); }
	m_item->setSelected(false);
}

void ak::aPropertyGridGroup::setStateIcons(
	QIcon *											_expanded,
	QIcon *											_collapsed
) {
	assert(_expanded != nullptr); // Nullptr provided
	assert(_collapsed != nullptr); // Nullptr provided
	m_iconCollapsed = _collapsed;
	m_iconExpanded = _expanded;
	refreshIcon();
}

// ##############################################################################################################

// slots

void ak::aPropertyGridGroup::slotDoubleClicked(QTableWidgetItem * _item) {
	if (_item == m_item) {
		for (auto itm : m_items) {
			m_propertyGridTable->setRowHidden(itm->row(), m_isVisible);
		}
		m_isVisible = !m_isVisible;
		refreshIcon();
	}
}

// ##############################################################################################################

// private members

void ak::aPropertyGridGroup::itemCreated(aPropertyGridItem * _item) {
	m_items.push_back(_item);
	if (m_isAlternateBackground) {
		_item->setColorStyle(m_colorStyleAlt);
		_item->setBackgroundColor(m_colorItemBackgroundAlternate);
	}
	else {
		_item->setColorStyle(m_colorStyle);
		_item->setBackgroundColor(m_colorItemBackground);
	}
	_item->setTextColors(m_colorTextNormal, m_colorTextError);
	m_isAlternateBackground = !m_isAlternateBackground;
	checkVisibility();
}

void ak::aPropertyGridGroup::checkVisibility(void) {
	if (!m_isActivated) { return; }
	if (m_items.size() > 0) {
		// Show group header (if is setVisible) and items
		m_propertyGridTable->setRowHidden(m_item->row(), !m_headerIsVisible);
		for (auto itm : m_items) {
			m_propertyGridTable->setRowHidden(itm->row(), !m_isVisible);
		}
	}
	else {
		// Hide group header
		m_propertyGridTable->setRowHidden(m_item->row(), true);
		m_isVisible = true;
	}
}

void ak::aPropertyGridGroup::rebuildStyleSheets() {
	if (m_colorStyle == nullptr) {
		m_colorStyle = new aCustomizableColorStyle;
	}
	if (m_colorStyleAlt == nullptr) {
		m_colorStyleAlt = new aCustomizableColorStyle;
	}

	// Set default colors
	m_colorStyle->setControlsMainForegroundColor(m_colorTextNormal);
	m_colorStyle->setControlsMainBackgroundColor(m_colorItemBackground);
	m_colorStyle->setAlternateForegroundColor(m_colorTextNormal);
	m_colorStyle->setAlternateBackgroundColor(m_colorItemBackgroundAlternate);
	m_colorStyle->setHeaderBackgroundColor(m_colorItemBackground);
	m_colorStyle->setHeaderForegroundColor(m_colorTextNormal);

	m_colorStyleAlt->setControlsMainForegroundColor(m_colorTextNormal);
	m_colorStyleAlt->setControlsMainBackgroundColor(m_colorItemBackground);
	m_colorStyleAlt->setAlternateForegroundColor(m_colorTextNormal);
	m_colorStyleAlt->setAlternateBackgroundColor(m_colorItemBackgroundAlternate);
	m_colorStyleAlt->setHeaderBackgroundColor(m_colorItemBackground);
	m_colorStyleAlt->setHeaderForegroundColor(m_colorTextNormal);

	// Set sheets
	m_colorStyle->setSheet(cafBackgroundColorControls, aColor::toHexString(m_colorItemBackground, true, "background-color: #"));
	m_colorStyle->setSheet(cafBackgroundColorWindow, aColor::toHexString(m_colorItemBackground, true, "background-color: #"));
	m_colorStyle->setSheet(cafBackgroundColorButton, aColor::toHexString(m_colorItemBackground, true, "background-color: #"));
	m_colorStyle->setSheet(cafBackgroundColorAlternate, aColor::toHexString(m_colorItemBackgroundAlternate, true, "alternate-background-color: #"));
	m_colorStyle->setSheet(cafBackgroundColorDialogWindow, aColor::toHexString(m_colorItemBackgroundAlternate, true, "background-color: #"));
	m_colorStyle->setSheet(cafForegroundColorControls, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyle->setSheet(cafForegroundColorButton, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyle->setSheet(cafForegroundColorWindow, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyle->setSheet(cafForegroundColorDialogWindow, aColor::toHexString(m_colorTextNormal, true, "color: #"));

	m_colorStyleAlt->setSheet(cafBackgroundColorControls, aColor::toHexString(m_colorItemBackgroundAlternate, true, "background-color: #"));
	m_colorStyleAlt->setSheet(cafBackgroundColorWindow, aColor::toHexString(m_colorItemBackgroundAlternate, true, "background-color: #"));
	m_colorStyleAlt->setSheet(cafBackgroundColorButton, aColor::toHexString(m_colorItemBackgroundAlternate, true, "background-color: #"));
	m_colorStyleAlt->setSheet(cafBackgroundColorAlternate, aColor::toHexString(m_colorItemBackgroundAlternate, true, "alternate-background-color: #"));
	m_colorStyleAlt->setSheet(cafBackgroundColorDialogWindow, aColor::toHexString(m_colorItemBackgroundAlternate, true, "background-color: #"));
	m_colorStyleAlt->setSheet(cafForegroundColorControls, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyleAlt->setSheet(cafForegroundColorButton, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyleAlt->setSheet(cafForegroundColorWindow, aColor::toHexString(m_colorTextNormal, true, "color: #"));
	m_colorStyleAlt->setSheet(cafForegroundColorDialogWindow, aColor::toHexString(m_colorTextNormal, true, "color: #"));

	if (m_externColorStyle == nullptr) {
		m_colorStyle->setSheet(cafBackgroundColorFocus, "");
		m_colorStyleAlt->setSheet(cafBackgroundColorFocus, "");

		m_colorStyle->setSheet(cafForegroundColorFocus, "");
		m_colorStyleAlt->setSheet(cafForegroundColorFocus, "");

		m_colorStyle->setSheet(cafBackgroundColorSelected, "");
		m_colorStyleAlt->setSheet(cafBackgroundColorSelected, "");

		m_colorStyle->setSheet(cafForegroundColorSelected, "");
		m_colorStyleAlt->setSheet(cafForegroundColorSelected, "");

		m_colorStyle->setSheet(cafBorderColorWindow, "");
		m_colorStyleAlt->setSheet(cafBorderColorWindow, "");

		m_colorStyle->setSheet(cafDefaultBorderWindow, "");
		m_colorStyleAlt->setSheet(cafDefaultBorderWindow, "");
	}
	else {
		m_colorStyle->setSheet(cafBackgroundColorFocus, m_externColorStyle->toStyleSheet(cafBackgroundColorFocus));
		m_colorStyleAlt->setSheet(cafBackgroundColorFocus, m_externColorStyle->toStyleSheet(cafBackgroundColorFocus));

		m_colorStyle->setSheet(cafForegroundColorFocus, m_externColorStyle->toStyleSheet(cafForegroundColorFocus));
		m_colorStyleAlt->setSheet(cafForegroundColorFocus, m_externColorStyle->toStyleSheet(cafForegroundColorFocus));

		m_colorStyle->setSheet(cafBackgroundColorSelected, m_externColorStyle->toStyleSheet(cafBackgroundColorSelected));
		m_colorStyleAlt->setSheet(cafBackgroundColorSelected, m_externColorStyle->toStyleSheet(cafBackgroundColorSelected));

		m_colorStyle->setSheet(cafForegroundColorSelected, m_externColorStyle->toStyleSheet(cafForegroundColorSelected));
		m_colorStyleAlt->setSheet(cafForegroundColorSelected, m_externColorStyle->toStyleSheet(cafForegroundColorSelected));

		m_colorStyle->setSheet(cafBorderColorWindow, m_externColorStyle->toStyleSheet(cafBorderColorWindow));
		m_colorStyleAlt->setSheet(cafBorderColorWindow, m_externColorStyle->toStyleSheet(cafBorderColorWindow));

		m_colorStyle->setSheet(cafDefaultBorderWindow, m_externColorStyle->toStyleSheet(cafDefaultBorderWindow));
		m_colorStyleAlt->setSheet(cafDefaultBorderWindow, m_externColorStyle->toStyleSheet(cafDefaultBorderWindow));
	}

}

void ak::aPropertyGridGroup::repaint(void) {
	m_isAlternateBackground = false;
	for (auto itm : m_items) {
		if (m_isAlternateBackground) {
			itm->setBackgroundColor(m_colorItemBackgroundAlternate);
			itm->setColorStyle(m_colorStyleAlt);
		}
		else { 
			itm->setBackgroundColor(m_colorItemBackground);
			itm->setColorStyle(m_colorStyle);
		}
		itm->setTextColors(m_colorTextNormal, m_colorTextError);
		m_isAlternateBackground = !m_isAlternateBackground;
	}
}

void ak::aPropertyGridGroup::refreshIcon(void) {
	if (m_iconCollapsed != nullptr) {
		assert(m_iconExpanded != nullptr); // This should never happen
		if (m_isVisible) { m_item->setIcon(*m_iconExpanded); }
		else { m_item->setIcon(*m_iconCollapsed); }
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
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtBool),
	m_name(_settingName), m_valueBool(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_widgetBool = new aCheckBoxWidget();
	if (m_isMultipleValues) {
		m_widgetBool->setTristate(true);
		m_widgetBool->setCheckState(Qt::PartiallyChecked);
	}
	else {
		m_widgetBool->setChecked(_value);
	}
	QString sheet("QCheckBox{background-color:#");
	sheet.append(m_colorBackground.toHexString(true));
	sheet.append(";}\n");
	m_widgetBool->setStyleSheet(sheet);

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setCellWidget(_row, 1, m_widgetBool);

	connect(m_widgetBool, &QCheckBox::stateChanged, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const aColor &					_value
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtColor),
	m_name(_settingName), m_valueColor(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_widgetColor = new aColorEditButtonWidget(_value);
	if (m_isMultipleValues) {
		m_widgetColor->overrideText("...");
	}
	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setCellWidget(_row, 1, m_widgetColor->widget());

	connect(m_widgetColor, &aColorEditButtonWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	double								_value
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtDouble),
	m_name(_settingName), m_valueDouble(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_cellValue = new QTableWidgetItem(QString::number(_value));
	if (m_isMultipleValues) {
		m_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setItem(_row, 1, m_cellValue);
	connect(m_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	int									_value
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtInt),
	m_name(_settingName), m_valueInteger(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_cellValue = new QTableWidgetItem(QString::number(_value));
	if (m_isMultipleValues) {
		m_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setItem(_row, 1, m_cellValue);
	connect(m_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const std::vector<QString> &		_possibleSlection,
	const QString &						_value
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtSelection),
	m_name(_settingName), m_valueSelection(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	if (m_isMultipleValues) {
		m_widgetSelection = new aComboButtonWidget("...");
	}
	else {
		m_widgetSelection = new aComboButtonWidget(_value);
	}
	m_valuePossibleSelection = _possibleSlection;
	m_widgetSelection->setItems(m_valuePossibleSelection);
	m_widgetSelection->setAutoFillBackground(true);

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setCellWidget(_row, 1, m_widgetSelection);

	connect(m_widgetSelection, &aComboButtonWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_aPropertyGridWidgetTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const QString &						_value
) : m_propertyGridTable(_aPropertyGridWidgetTable), m_group(_group), m_valueType(vtString),
	m_name(_settingName), m_valueString(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_cellValue = new QTableWidgetItem(_value);
	if (m_isMultipleValues) {
		m_cellValue->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setItem(_row, 1, m_cellValue);
	connect(m_propertyGridTable, &QTableWidget::itemChanged, this, &aPropertyGridItem::slotTableCellChanged);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const aDate &						_value
) : m_propertyGridTable(_propertyGridTable), m_group(_group), m_valueType(vtDate),
	m_name(_settingName), m_valueDate(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr),
	m_cellSettingName(nullptr), m_cellValue(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_widgetDate = new aDatePickWidget(_value, dfDDMMYYYY);

	if (m_isMultipleValues) {
		m_widgetDate->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setCellWidget(_row, 1, m_widgetDate);

	connect(m_widgetDate, &aDatePickWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::aPropertyGridItem(
	aTableWidget *						_propertyGridTable,
	const QString &						_group,
	int									_row,
	bool								_isMultipleValues,
	const QString &						_settingName,
	const aTime &						_value
) : m_propertyGridTable(_propertyGridTable), m_group(_group), m_valueType(vtTime),
	m_name(_settingName), m_valueTime(_value), m_isMultipleValues(_isMultipleValues), m_globalColorStyle(nullptr)
{
	assert(m_propertyGridTable != nullptr);

	ini();

	// Create new cell items
	m_cellSettingName = new QTableWidgetItem(_settingName);
	m_widgetTime = new aTimePickWidget(_value, tfHHMM);

	if (m_isMultipleValues) {
		m_widgetTime->setText("...");
	}

	// Make the first cell read only
	Qt::ItemFlags f = m_cellSettingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	m_cellSettingName->setFlags(f);

	// Add cell items to the main aPropertyGridWidget table
	m_propertyGridTable->setItem(_row, 0, m_cellSettingName);
	m_propertyGridTable->setCellWidget(_row, 1, m_widgetTime);

	connect(m_widgetTime, &aTimePickWidget::changed, this, &aPropertyGridItem::slotValueWidgetEvent);
}

ak::aPropertyGridItem::~aPropertyGridItem() {
	assert(m_cellSettingName != nullptr); // This should never happen
	if (m_cellValue != nullptr) { delete m_cellValue; }
	if (m_widgetBool != nullptr) { delete m_widgetBool; }
	if (m_widgetColor != nullptr) { delete m_widgetColor; }
	if (m_widgetSelection != nullptr) { delete m_widgetSelection; }
	int r = m_cellSettingName->row();
	delete m_cellSettingName;
	m_propertyGridTable->removeRow(r);
}

int ak::aPropertyGridItem::row() const {
	return m_cellSettingName->row();
}

void ak::aPropertyGridItem::setId(
	ak::ID								_ID
) { m_id = _ID; }

ak::ID ak::aPropertyGridItem::getId(void) const { return m_id; }

bool ak::aPropertyGridItem::getIsCurrentlyError() const { return m_isCurrentlyError; }

void ak::aPropertyGridItem::setBackgroundColor(
	const QColor &					_backgroundColor
) {
	m_colorBackground = _backgroundColor; 
	repaint();
}

void ak::aPropertyGridItem::setTextColors(
	const QColor &					_foregroundNormal,
	const QColor &					_foregroundError
) {
	m_colorErrorForeground = _foregroundError;
	m_colorNormalForeground = _foregroundNormal;
	repaint();
}

void ak::aPropertyGridItem::deselect(void) {
	m_cellSettingName->setSelected(false);
	if (m_cellValue != nullptr) { m_cellValue->setSelected(false); }
}

// #################################################################################

// Information gathering

QString ak::aPropertyGridItem::getGroup(void) const { return m_group; }

bool ak::aPropertyGridItem::getIsMultipleValues(void) const { return m_isMultipleValues; }

QString ak::aPropertyGridItem::getName(void) const { return m_name; }

ak::valueType ak::aPropertyGridItem::getValueType(void) const { return m_valueType; }

std::vector<QString> ak::aPropertyGridItem::getPossibleSelection(void) const {
	assert(m_valueType == vtSelection);	// Wrong value type for this item
	return m_valuePossibleSelection;
}

bool ak::aPropertyGridItem::getValueBool(void) const {
	assert(m_valueType == vtBool);	// Wrong value type for this item
	return m_valueBool;
}

ak::aColor ak::aPropertyGridItem::getValueColor(void) const {
	assert(m_valueType == vtColor);	// Wrong value type for this item
	return m_valueColor;
}

double ak::aPropertyGridItem::getValueDouble(void) const {
	assert(m_valueType == vtDouble);	// Wrong value type for this item
	return m_valueDouble;
}

int ak::aPropertyGridItem::getValueInteger(void) const {
	assert(m_valueType == vtInt);	// Wrong value type for this item
	return m_valueInteger;
}

QString ak::aPropertyGridItem::getValueSelection(void) const {
	assert(m_valueType == vtSelection);	// Wrong value type for this item
	return m_valueSelection;
}

QString ak::aPropertyGridItem::getValueString(void) const {
	assert(m_valueType == vtString);	// Wrong value type for this item
	return m_valueString;
}

ak::aDate ak::aPropertyGridItem::getValueDate(void) const {
	assert(m_valueType == vtDate);
	return m_valueDate;
}

ak::aTime ak::aPropertyGridItem::getValueTime(void) const {
	assert(m_valueType == vtTime);
	return m_valueTime;
}

bool ak::aPropertyGridItem::isMultipleValues(void) const { return m_isMultipleValues; }

void ak::aPropertyGridItem::setReadOnly(
	bool					_readOnly
) {
	m_ignoreCellEvent = true;
	m_isReadOnly = _readOnly;
	switch (m_valueType)
	{
	case vtBool:
		assert(m_widgetBool != nullptr); // This should not happen
		m_widgetBool->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtColor:
		assert(m_widgetColor != nullptr); // This should not happen
		m_widgetColor->SetEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtSelection:
		assert(m_widgetSelection != nullptr); // This should not happen
		m_widgetSelection->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtDate:
		assert(m_widgetDate != nullptr); // This should not happen
		m_widgetDate->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtTime:
		assert(m_widgetTime != nullptr); // This should not happen
		m_widgetTime->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtDouble:
	case vtInt:
	case vtString:
	{
		Qt::ItemFlags f = m_cellValue->flags();
		f.setFlag(Qt::ItemFlag::ItemIsEditable, !m_isReadOnly && m_isEnabled);
		m_cellValue->setFlags(f);
	}
	break;
	default:
		assert(0); // Invalid value type
		break;
	}
	m_ignoreCellEvent = false;
}

bool ak::aPropertyGridItem::isReadOnly() { return m_isReadOnly; }

void ak::aPropertyGridItem::setEnabled(
	bool					_enabled
) {
	m_ignoreCellEvent = true;
	m_isEnabled = _enabled;
	switch (m_valueType)
	{
	case vtBool:
		assert(m_widgetBool != nullptr); // This should not happen
		m_widgetBool->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtColor:
		assert(m_widgetColor != nullptr); // This should not happen
		m_widgetColor->SetEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtSelection:
		assert(m_widgetSelection != nullptr); // This should not happen
		m_widgetSelection->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtDate:
		assert(m_widgetDate != nullptr); // This should not happen
		m_widgetDate->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtTime:
		assert(m_widgetTime != nullptr); // This should not happen
		m_widgetTime->setEnabled(!m_isReadOnly && m_isEnabled);
		break;
	case vtDouble:
	case vtInt:
	case vtString:
	{
		Qt::ItemFlags f = m_cellValue->flags();
		f.setFlag(Qt::ItemFlag::ItemIsEditable, !m_isReadOnly && m_isEnabled);
		m_cellValue->setFlags(f);
	}
	break;
	default:
		assert(0); // Invalid value type
		break;
	}
	m_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::setColorStyle(
	aColorStyle *	_style
) {
	assert(_style != nullptr); // Nullptr provided
	m_globalColorStyle = _style;
	repaint();
}

void ak::aPropertyGridItem::resetAsError(
	const QString &									_valueToReset
) {
	assert(m_valueType == vtString);
	m_ignoreCellEvent = true;
	m_valueString = _valueToReset;
	m_isCurrentlyError = true;
	m_cellValue->setText(m_valueString);
	repaint();
	m_cellValue->setSelected(false);
	m_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::resetAsError(
	int												_valueToReset
) {
	assert(m_valueType == vtInt);
	m_ignoreCellEvent = true;
	m_valueInteger = _valueToReset;
	m_isCurrentlyError = true;
	m_cellValue->setText(QString::number(m_valueInteger));
	repaint();
	m_cellValue->setSelected(false);
	m_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::resetAsError(
	double											_valueToReset
) {
	assert(m_valueType == vtDouble);
	m_ignoreCellEvent = true;
	m_valueDouble = _valueToReset;
	m_isCurrentlyError = true;
	m_cellValue->setText(QString::number(m_valueDouble));
	repaint();
	m_cellValue->setSelected(false);
	m_ignoreCellEvent = false;
}

void ak::aPropertyGridItem::showAsError(void) {
	m_ignoreCellEvent = true;
	m_isCurrentlyError = true;
	m_cellValue->setSelected(false);
	repaint();
	m_ignoreCellEvent = false;
}

// #################################################################################

// Slots

void ak::aPropertyGridItem::slotValueWidgetEvent() {
	switch (m_valueType)
	{
	case vtBool:
		if (m_isMultipleValues) {
			m_isMultipleValues = false;
			m_valueBool = true;
			m_widgetBool->setTristate(false);
			m_widgetBool->setChecked(m_valueBool);
			emit changed();
		}
		else {
			m_valueBool = m_widgetBool->isChecked();
			emit changed();
		}
		break;
	case vtColor:
		m_valueColor = m_widgetColor->color();
		m_isMultipleValues = false;
		emit changed();
		break;
	case vtDate:
		m_valueDate = m_widgetDate->currentDate();
		m_isMultipleValues = false;
		emit changed();
		break;
	case vtSelection:
		m_valueSelection = m_widgetSelection->text();
		m_isMultipleValues = false;
		emit changed();
		break;
	case vtTime:
		m_valueTime = m_widgetTime->currentTime();
		m_isMultipleValues = false;
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
	if (m_ignoreCellEvent) { return; }
	if (_item == m_cellValue) {

		if (m_isCurrentlyError) {
			m_isCurrentlyError = false;
			repaint();
		}

		QString theText(_item->text());
		switch (m_valueType)
		{
		case vtDouble:
		{
			m_ignoreCellEvent = true;
			bool failed = false;
			double v;
			v = toNumber<double>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(m_name);
				msg.append(". Expected numeric.");
				aPromptDialog dia(msg, "Error", promptType::promptOk);
				dia.showDialog();
				m_cellValue->setSelected(false);
				if (m_isMultipleValues) {
					m_cellValue->setText("...");
				}
				else {
					m_cellValue->setText(QString::number(m_valueDouble));
				}
				m_cellValue->setTextColor(m_colorErrorForeground.toQColor());
				m_ignoreCellEvent = false;
			}
			else {
				m_valueDouble = v;
				m_isMultipleValues = false;
				m_cellValue->setTextColor(m_colorNormalForeground.toQColor());
				m_ignoreCellEvent = false;
				emit changed();
			}
		}
			break;
		case vtInt:
		{
			m_ignoreCellEvent = true;
			bool failed = false;
			int v;
			v = toNumber<int>(theText.toStdString(), failed);
			if (failed) {
				QString msg("Invalid value format provided for setting ");
				msg.append(m_name);
				msg.append(". Expected natural number.");
				aPromptDialog dia(msg, "Error", promptType::promptOk);
				dia.showDialog();
				m_cellValue->setSelected(false);
				if (m_isMultipleValues) {
					m_cellValue->setText("...");
				}
				else {
					m_cellValue->setText(QString::number(m_valueInteger));
				}
				m_cellValue->setTextColor(m_colorErrorForeground.toQColor());
				m_ignoreCellEvent = false;
			}
			else {
				m_valueInteger = v;
				m_isMultipleValues = false;
				m_cellValue->setTextColor(m_colorNormalForeground.toQColor());
				m_ignoreCellEvent = false;
				emit changed();
			}
		}
			break;
		case vtString:
			m_valueString = theText;
			emit changed();
			break;
		default:
			assert(0); // Invalid type
			break;
		}
	}
}

void ak::aPropertyGridItem::ini(void) {
	m_cellSettingName = nullptr;
	m_cellValue = nullptr;
	m_widgetBool = nullptr;
	m_widgetColor = nullptr;
	m_widgetSelection = nullptr;
	m_ignoreCellEvent = false;
	m_isCurrentlyError = false;
	m_isEnabled = true;
	m_isReadOnly = false;
	m_colorErrorForeground = QColor(255, 0, 0);
	m_colorNormalForeground = QColor(0, 0, 0);
}

void ak::aPropertyGridItem::repaint(void) {
	if (m_cellSettingName != nullptr) {
		m_cellSettingName->setBackgroundColor(m_colorBackground.toQColor());
		m_cellSettingName->setTextColor(m_colorNormalForeground.toQColor());
	}
	if (m_cellValue != nullptr) {
		m_ignoreCellEvent = true;
		m_cellValue->setBackgroundColor(m_colorBackground.toQColor());
		if (m_isCurrentlyError) {
			m_cellValue->setTextColor(m_colorErrorForeground.toQColor());
		}
		else {
			m_cellValue->setTextColor(m_colorNormalForeground.toQColor());
		}
		m_ignoreCellEvent = false;
	}
	else {
		switch (m_valueType)
		{
		case vtBool:
		{
			assert(m_widgetBool != nullptr); // Something went wrong
			QString sheet = m_globalColorStyle->toStyleSheet(cafBackgroundColorControls | cafForegroundColorControls, "QCheckBox{", "}");
			m_widgetBool->setStyleSheet(sheet);
			/*QString sheet("QCheckBox{background-color:#");
			sheet.append(m_colorBackground.toHexString(true));
			sheet.append(";}\n");
			m_widgetBool->setStyleSheet(sheet);*/
		}
			break;
		case vtColor:
		{
			assert(m_widgetColor != nullptr); // Something went wrong
			m_widgetColor->setColorStyle(m_globalColorStyle);
			m_widgetColor->fillBackground(m_colorBackground);
			/*QString sheet("QPushButton{background-color: #");
			sheet.append(m_colorBackground.toHexString(true));
			sheet.append("; color: #");
			sheet.append(m_colorNormalForeground.toHexString(true));
			sheet.append("; }");

			if (m_globalColorStyle != nullptr) {
				sheet.append(m_globalColorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus, "QPushButton:hover{", "}"));
				sheet.append(m_globalColorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected, "QPushButton:pressed{", "}"));
			}

			m_widgetColor->setPushButtonStyleSheet(sheet);*/
		}
			break;
		case vtDate:
			assert(m_widgetDate != nullptr);	// Something went wrong
			m_widgetDate->setColorStyle(m_globalColorStyle);
			break;
		case vtTime:
			assert(m_widgetTime != nullptr);	// Something went wrong
			m_widgetTime->setColorStyle(m_globalColorStyle);
			break;
		case vtSelection:
		{
			assert(m_widgetSelection != nullptr); // Something went wrong
			m_widgetSelection->setColorStyle(m_globalColorStyle);
			/*QString sheet("QPushButton{color: #");
			aColor fore(m_colorNormalForeground);
			aColor back(m_colorBackground);
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append(";} QPushButton QMenu{color: #");
			sheet.append(fore.toHexString(true));
			sheet.append("; background-color: #");
			sheet.append(back.toHexString(true));
			sheet.append("; alternate-background-color: red; border 1px solid black; }");

			if (m_globalColorStyle != nullptr) {
				sheet.append(m_globalColorStyle->toStyleSheet(cafBackgroundColorFocus | cafForegroundColorFocus, "QPushButton:hover{", "}"));
				sheet.append(m_globalColorStyle->toStyleSheet(cafBackgroundColorSelected | cafForegroundColorSelected, "QPushButton:pressed{", "}"));
			}
			
			m_widgetSelection->setStyleSheet(sheet);
			*/
		}
			break;
		default:
			break;
		}
	}
}