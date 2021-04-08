/*
 * ak_ui_qt_toolButton.cpp
 *
 *  Created on: November 07, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_toolButton.h>		// Corresponding header
#include <ak_ui_qt_action.h>			// action
#include <ak_ui_colorStyle.h>
#include <ak_ui_qt_contextMenuItem.h>

// Qt header
#include <qmenu.h>

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::toolButton::toolButton()
	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action();
	ini();
}

ak::ui::qt::toolButton::toolButton(
	const QString &				_text
)	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action(_text);
	ini();
}

ak::ui::qt::toolButton::toolButton(
	const QIcon &				_icon,
	const QString &				_text
)	: QToolButton(), ak::ui::core::aWidget(ak::ui::core::objectType::oToolButton),
	my_action(nullptr)
{
	my_action = new qt::action(_icon, _text);
	ini();
}

ak::ui::qt::toolButton::~toolButton() { A_OBJECT_DESTROYING }

// #######################################################################################################

// Event handling

void ak::ui::qt::toolButton::keyPressEvent(QKeyEvent *_event)
{
	QToolButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::toolButton::keyReleaseEvent(QKeyEvent * _event) {
	QToolButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

// Base class function

void ak::ui::qt::toolButton::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;

	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls, "QToolButton{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus |
		TYPE_COLORAREA::caBackgroundColorFocus, "QToolButton:hover:!pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected |
		TYPE_COLORAREA::caBackgroundColorSelected, "QToolButton:pressed{", "}"));

	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorHeader |
		TYPE_COLORAREA::caBackgroundColorHeader | TYPE_COLORAREA::caBorderColorHeader,
		"QToolTip{", "border: 1px;}"));
	setStyleSheet(sheet);

	if (my_menu != nullptr) {
		sheet = my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu{", "}");
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorDialogWindow | TYPE_COLORAREA::caBackgroundColorDialogWindow, "QMenu::item{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorFocus | TYPE_COLORAREA::caBackgroundColorFocus, "QMenu::item:selected{", "}"));
		sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorSelected | TYPE_COLORAREA::caBackgroundColorSelected, "QMenu::item:pressed{", "}"));
		my_menu->setStyleSheet(sheet);
	}
}

// #######################################################################################################

void ak::ui::qt::toolButton::SetToolTip(
	const QString &						_text
) {
	setToolTip(_text);
	setWhatsThis(_text);
	my_action->setToolTip(_text);
	my_action->setWhatsThis(_text);
}

QString ak::ui::qt::toolButton::ToolTip(void) const { return toolTip(); }

ak::ID ak::ui::qt::toolButton::addMenuItem(
	contextMenuItem *					_item
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

void ak::ui::qt::toolButton::addMenuSeperator(void) {
	if (my_menu == nullptr) {
		my_menu = new QMenu();
		setMenu(my_menu);
	}
	my_menu->addSeparator();
}

void ak::ui::qt::toolButton::clearMenu(void) {
	if (my_menu != nullptr) {
		for (auto itm : my_menuItems) { contextMenuItem * item = itm; delete itm; }
		setMenu(nullptr);
		delete my_menu;
		my_menu = nullptr;
		my_menuItems.clear();
	}
}

void ak::ui::qt::toolButton::setMenuItemChecked(
	ak::ID								_itemID,
	bool								_checked
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	contextMenuItem * itm = my_menuItems[_itemID];
	itm->setCheckable(true);
	itm->setChecked(_checked);
}

void ak::ui::qt::toolButton::setMenuItemNotCheckable(
	ak::ID								_itemID
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	contextMenuItem * itm = my_menuItems[_itemID];
	itm->setCheckable(false);
}

QString ak::ui::qt::toolButton::getMenuItemText(
	ak::ID								_itemID
) {
	assert(_itemID >= 0 && _itemID < my_menuItems.size());	// Index out of range
	contextMenuItem * itm = my_menuItems[_itemID];
	return itm->text();
}

// #######################################################################################################

// Slots

void ak::ui::qt::toolButton::slotClicked() {
	if (my_menuItems.size() != 0) {
		showMenu();
	}
	else { emit btnClicked(); }
}

void ak::ui::qt::toolButton::slotMenuItemClicked() {
	contextMenuItem * itm = nullptr;
	itm = dynamic_cast<contextMenuItem *>(sender());
	assert(itm != nullptr); // Cast failed
	emit menuItemClicked(itm->id());
}

void ak::ui::qt::toolButton::slotMenuItemCheckedChanged() {
	contextMenuItem * itm = nullptr;
	itm = dynamic_cast<contextMenuItem *>(sender());
	assert(itm != nullptr); // Cast failed
	emit menuItemCheckedChanged(itm->id(), itm->isChecked());
}

// #######################################################################################################

// Private member

void ak::ui::qt::toolButton::ini(void) {
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
