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
#include <propertyGrid.h>
#include <ak_ui_core.h>
#include <ak_ui_colorStyle.h>

// Qt header
#include <qheaderview.h>
#include <qflags.h>
#include <qfont.h>

propertyGrid::propertyGrid()
	: ak::ui::core::aWidget(ak::ui::core::oPropertyGrid),
	my_alternateForeColor(180, 180, 180), my_alternateBackColor(0, 0, 0), my_isAlternateGroup(false)
{
	my_table = new QTableWidget(0, 2);
	my_table->verticalHeader()->setVisible(false);

	my_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
	my_table->horizontalHeader()->setEnabled(false);

	my_defaultGroup = new propertyGridGroup("", my_table);
	my_defaultGroup->activate();

}

propertyGrid::~propertyGrid() {

}

// ##############################################################################################################

// base class functions

QWidget * propertyGrid::widget(void) {
	return my_table;
}

void propertyGrid::setColorStyle(
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

void propertyGrid::addGroup(
	const QString &									_group
) {
	assert(_group.length() > 0); // Empty group name provided
	my_groupsIterator itm = my_groups.find(_group);
	assert(itm == my_groups.end());	// Group already exists
	propertyGridGroup * newGroup = new propertyGridGroup(_group, my_table);
	my_isAlternateGroup = !my_isAlternateGroup;
	if (my_isAlternateGroup) {
		newGroup->setColors(my_alternateForeColor, my_alternateBackColor);
	}
	newGroup->activate();
	my_groups.insert_or_assign(_group, newGroup);
}

ak::ID propertyGrid::addItem(
	const QString &									_settingName,
	const QString &									_value
) {
	my_defaultGroup->addItem(_settingName, _value, ++my_currentID);
	return my_currentID;
}

ak::ID propertyGrid::addItem(
	const QString &									_groupName,
	const QString &									_settingName,
	const QString &									_value
) {
	my_groupsIterator group = my_groups.find(_groupName);
	assert(group != my_groups.end());	// Invalid group name
	group->second->addItem(_settingName, _value, ++my_currentID);
	return my_currentID;
}

// ##############################################################################################################

// ##############################################################################################################

// ##############################################################################################################

propertyGridGroup::propertyGridGroup(
	const QString &						_groupName,
	QTableWidget *						_propertyGridTable
) :	my_propertyGridTable(_propertyGridTable), my_isActivated(false), my_isVisible(true), my_colorWasSet(false)
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

propertyGridGroup::~propertyGridGroup() {}

QString propertyGridGroup::name(void) const { return my_name; }

void propertyGridGroup::setName(
	const QString &						_groupName
) { my_name = _groupName; }

void propertyGridGroup::activate(void) {
	my_isActivated = true;
	checkVisibility();
}

void propertyGridGroup::setColors(
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

void propertyGridGroup::addItem(
	const QString &									_settingName,
	const QString &									_value,
	ak::ID											_itemId
) {
	int r = my_item->row();
	if (my_items.size() > 0) {
		r = my_items.at(my_items.size() - 1)->row();
	}
	my_propertyGridTable->insertRow(r + 1);
	propertyGridItem * newItem = new propertyGridItem(my_propertyGridTable, r + 1, _settingName, _value);
	my_items.push_back(newItem);
	if (my_colorWasSet) {
		newItem->setBackColor(my_backColor);
		newItem->setTextColor(my_backColor);
	}
	checkVisibility();
}

// ##############################################################################################################

// slots

void propertyGridGroup::slotDoubleClicked(QTableWidgetItem * _item) {
	if (_item == my_item) {
		for (auto itm : my_items) {
			my_propertyGridTable->setRowHidden(itm->row(), my_isVisible);
		}
		my_isVisible = !my_isVisible;
	}
}

// ##############################################################################################################

// private members

void propertyGridGroup::checkVisibility(void) {
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

propertyGridItem::propertyGridItem(
	QTableWidget *						_propertyGridTable,
	int									_row,
	const QString &						_settingName,
	const QString &						_value
) :	my_propertyGridTable(_propertyGridTable)
{
	assert(my_propertyGridTable != nullptr);

	my_settingName = new QTableWidgetItem(_settingName);
	my_value = new QTableWidgetItem(_value);

	Qt::ItemFlags f = my_settingName->flags();
	f.setFlag(Qt::ItemFlag::ItemIsEditable, false);
	f.setFlag(Qt::ItemFlag::ItemIsSelectable, false);
	my_settingName->setFlags(f);

	my_propertyGridTable->setItem(_row, 0, my_settingName);
	my_propertyGridTable->setItem(_row, 1, my_value);
}

propertyGridItem::~propertyGridItem() {
	my_propertyGridTable->removeRow(my_settingName->row());
	delete my_settingName;
	delete my_value;
}

int propertyGridItem::row() const {
	return my_settingName->row();
}

void propertyGridItem::setTextColor(
	const QColor &						_color
) {
	my_settingName->setTextColor(_color);
	my_value->setTextColor(_color);
}

void propertyGridItem::setBackColor(
	const QColor &						_color
) {
	my_settingName->setBackgroundColor(_color);
	my_value->setBackgroundColor(_color);
}
