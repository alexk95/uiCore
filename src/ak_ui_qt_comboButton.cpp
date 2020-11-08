/*
 * ak_ui_qt_comboButton.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_comboButton.h>	// corresponding class
#include <ak_ui_colorStyle.h>		// colorStyle
#include <ak_exception.h>			// error handling

// Qt header
#include <qmenu.h>					// dropDownMenu

#define TYPE_COLORAREA ak::ui::core::colorAreaFlag

ak::ui::qt::comboButton::comboButton(
	const QString &				_initialText,
	const ak::ui::colorStyle *	_colorStyle,
	QWidget *					_parent
) : QPushButton(_initialText, _parent),
	ak::ui::core::aWidget(ak::ui::core::objectType::oComboButton, _colorStyle),
	my_menu(nullptr)
{
	try {
		// Create new QMenu to be a dropdown menu of the pushbutton
		my_menu = new QMenu;
		if (my_menu == nullptr) { throw ak::Exception("Failed to create", "Create menu"); }

		// Apply color style and menu
		if (my_colorStyle != nullptr) { setColorStyle(_colorStyle); }
		setMenu(my_menu);
		my_itemsUIDmanager.setLatestUid(1);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::comboButton::comboButton()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::comboButton::comboButton()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::comboButton::comboButton()"); }
}

//! @brief Deconstructor
ak::ui::qt::comboButton::~comboButton() { A_OBJECT_DESTROYING clearItems(); }

// #######################################################################################################
// Event handling

void ak::ui::qt::comboButton::keyPressEvent(QKeyEvent *_event)
{
	QPushButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::ui::qt::comboButton::keyReleaseEvent(QKeyEvent * _event) {
	QPushButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

void ak::ui::qt::comboButton::clearItems(void) {
	for (int i = 0; i < my_items.size(); i++){
		// Remove the action from the menu of this combo button
		my_menu->removeAction(my_items.at(i));
		// Disconnect the signal
		disconnect(my_items.at(i), SIGNAL(triggered()), this, SLOT(slotItemTriggered()));
		// Remove entry from the vector
		delete my_items.at(i);
	}
	my_items.clear();
	my_itemUids.clear();
	my_itemsUIDmanager.setLatestUid(1);
}

void ak::ui::qt::comboButton::setItems(
	const std::vector<QString> &		_menu
) {
	try {
		clearItems();
		for (int i = 0; i < _menu.size(); i++) {
			// Create new item
			ak::ui::qt::comboButtonItem * n_itm = nullptr;
			n_itm = new ak::ui::qt::comboButtonItem(_menu.at(i));
			// Set the items UID
			n_itm->setUid(my_itemsUIDmanager.getId());
			// Add the new item to the menu
			my_menu->addAction(n_itm);
			// Store data
			my_items.push_back(n_itm);
			my_itemUids.insert_or_assign(n_itm->uid(), my_items.size() - 1);
			// Connect the triggered signal of the new item
			connect(n_itm, SIGNAL(triggered()), this, SLOT(slotItemTriggered()));
		}
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::comboButton::setItems()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::comboButton::setItems()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::comboButton::setItems()"); }
}

int ak::ui::qt::comboButton::addItem(
	const QString &						_other
) {
	try {
		// Create new item
		ak::ui::qt::comboButtonItem * n_itm = nullptr;
		n_itm = new ak::ui::qt::comboButtonItem(_other);
		if (n_itm == nullptr) { throw ak::Exception("Failed to create", "Create comboButtonItem"); }
		// Set the items UID
		n_itm->setUid(my_itemsUIDmanager.getId());
		// Add the new item to the menu
		my_menu->addAction(n_itm);
		// Store data
		my_items.push_back(n_itm);
		my_itemUids.insert_or_assign(n_itm->uid(), my_items.size() - 1);
		// Connect the triggered signal of the new item
		connect(n_itm, SIGNAL(triggered()), this, SLOT(slotItemTriggered()));
		// Return the new items index in the menu
		return (my_items.size() - 1);
	}
	catch (const ak::Exception & e) { throw ak::Exception(e, "ak::ui::qt::comboButton::addItem()"); }
	catch (const std::exception & e) { throw ak::Exception(e.what(), "ak::ui::qt::comboButton::addItem()"); }
	catch (...) { throw ak::Exception("Unknown error", "ak::ui::qt::comboButton::addItem()"); }
}

int ak::ui::qt::comboButton::getItemCount(void) const { return my_items.size(); }

void ak::ui::qt::comboButton::setColorStyle(
	const ak::ui::colorStyle *								_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls));
	sheet.append(my_colorStyle->toStyleSheet(TYPE_COLORAREA::caForegroundColorControls |
		TYPE_COLORAREA::caBackgroundColorControls | TYPE_COLORAREA::caBackgroundColorAlternate, "QPushButton QMenu{", "}"));
	this->setStyleSheet(sheet);
}

void ak::ui::qt::comboButton::slotItemTriggered() {
	// Cast the QObject to the comboButtonItem
	ak::ui::qt::comboButtonItem * itm = dynamic_cast<ak::ui::qt::comboButtonItem *>(sender());
	assert(itm != nullptr); // failed to cast. but item was expected
	my_itemUidsIterator obj = my_itemUids.find(itm->uid());
	assert(obj != my_itemUids.end()); // Invalid data stored
	setText(my_items.at(obj->second)->text());
	emit changed();
}

QWidget * ak::ui::qt::comboButton::widget(void) { return this; }
