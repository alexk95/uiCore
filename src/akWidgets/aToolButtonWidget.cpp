/*
 *	File:		aToolButtonWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: November 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aAction.h>
#include <akGui/aColorStyle.h>
#include <akGui/aContextMenuItem.h>
#include <akWidgets/aToolButtonWidget.h>

// Qt header
#include <qmenu.h>

ak::aToolButtonWidget::aToolButtonWidget()
	: QToolButton(), aWidget(otToolButton),
	my_action(nullptr)
{
	my_action = new aAction();
	ini();
}

ak::aToolButtonWidget::aToolButtonWidget(
	const QString &				_text
)	: QToolButton(), ak::aWidget(otToolButton),
	my_action(nullptr)
{
	my_action = new aAction(_text);
	ini();
}

ak::aToolButtonWidget::aToolButtonWidget(
	const QIcon &				_icon,
	const QString &				_text
)	: QToolButton(), ak::aWidget(otToolButton),
	my_action(nullptr)
{
	my_action = new aAction(_icon, _text);
	ini();
}

ak::aToolButtonWidget::~aToolButtonWidget() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Event handling

void ak::aToolButtonWidget::keyPressEvent(QKeyEvent *_event)
{
	QToolButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::aToolButtonWidget::keyReleaseEvent(QKeyEvent * _event) {
	QToolButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

// Base class function

void ak::aToolButtonWidget::setColorStyle(
	aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus |
		cafBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected |
		cafBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorHeader |
		cafBackgroundColorHeader | cafBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

	if (my_menu != nullptr) {
		sheet = my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu{", "}");
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorDialogWindow | cafBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorFocus | cafBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorSelected | cafBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_menu->setStyleSheet(sheet);
	}
}

// #######################################################################################################

void ak::aToolButtonWidget::SetToolTip(
	const QString &						_text
) {
	setToolTip(_text);
	setWhatsThis(_text);
	my_action->setToolTip(_text);
	my_action->setWhatsThis(_text);
}

QString ak::aToolButtonWidget::ToolTip(void) const { return toolTip(); }

ak::ID ak::aToolButtonWidget::addMenuItem(
	aContextMenuItem *					_item
) {
	if (my_menu == nullptr) {
		my_menu = new QMenu();
		setMenu(my_menu);
	}
	_item->setId(my_menuItems.size());
	my_menuItems.push_back(_item);

	my_menu->addAction(_item);

	connect(_item, SIGNAL(triggered(bool)), this, SLOT(slotMenuItemClicked()));
	connect(_item, SIGNAL(toggled(bool)), this, SLOT(slotMenuItemCheckedChanged()));

	return _item->id();
}

void ak::aToolButtonWidget::addMenuSeperator(void) {
	if (my_menu == nullptr) {
		my_menu = new QMenu();
		setMenu(my_menu);
	}
	my_menu->addSeparator();
}

void ak::aToolButtonWidget::clearMenu(void) {
	if (my_menu != nullptr) {
		for (auto itm : my_menuItems) { aContextMenuItem * item = itm; delete itm; }
		setMenu(nullptr);
		delete my_menu;
		my_menu = nullptr;
		my_menuItems.clear();
	}
}

void ak::aToolButtonWidget::setMenuItemChecked(
	ak::ID								_itemID,
	bool								_checked
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	aContextMenuItem * itm = my_menuItems[_itemID];
	itm->setCheckable(true);
	itm->setChecked(_checked);
}

void ak::aToolButtonWidget::setMenuItemNotCheckable(
	ak::ID								_itemID
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	aContextMenuItem * itm = my_menuItems[_itemID];
	itm->setCheckable(false);
}

QString ak::aToolButtonWidget::getMenuItemText(
	ak::ID								_itemID
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	aContextMenuItem * itm = my_menuItems[_itemID];
	return itm->text();
}

// #######################################################################################################

// Slots

void ak::aToolButtonWidget::slotClicked() {
	if (my_menuItems.size() != 0) {
		showMenu();
	}
	else { emit btnClicked(); }
}

void ak::aToolButtonWidget::slotMenuItemClicked() {
	aContextMenuItem * itm = nullptr;
	itm = dynamic_cast<aContextMenuItem *>(sender());
	assert(itm != nullptr); // Cast failed
	emit menuItemClicked(itm->id());
}

void ak::aToolButtonWidget::slotMenuItemCheckedChanged() {
	aContextMenuItem * itm = nullptr;
	itm = dynamic_cast<aContextMenuItem *>(sender());
	assert(itm != nullptr); // Cast failed
	emit menuItemCheckedChanged(itm->id(), itm->isChecked());
}

// #######################################################################################################

// Private member

void ak::aToolButtonWidget::ini(void) {
	setDefaultAction(my_action);
	setToolTip("");
	setWhatsThis("");
	my_action->setToolTip("");
	my_action->setWhatsThis("");
	my_menu = nullptr;
	my_action->setMenuRole(QAction::MenuRole::NoRole);
	my_action->setIconVisibleInMenu(true);
	connect(my_action, SIGNAL(triggered(bool)), this, SLOT(slotClicked()));
}
