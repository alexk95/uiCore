/*
 * ak_ui_qt_aComboButtonWidget.cpp
 *
 *  Created on: March 19, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akCore/aException.h>
#include <akGui/aColorStyle.h>
#include <akWidgets/aComboButtonWidget.h>

// Qt header
#include <qmenu.h>					// dropDownMenu

ak::aComboButtonWidget::aComboButtonWidget(
	const QString &				_initialText,
	const ak::aColorStyle *		_colorStyle,
	QWidget *					_parent
) : QPushButton(_initialText, _parent),
	ak::aWidget(otComboButton, _colorStyle),
	my_menu(nullptr)
{
	try {
		// Create new QMenu to be a dropdown menu of the pushbutton
		my_menu = new QMenu;
		if (my_menu == nullptr) { throw aException("Failed to create", "Create menu"); }

		// Apply color style and menu
		if (my_colorStyle != nullptr) { setColorStyle(_colorStyle); }
		setMenu(my_menu);
		my_itemsUIDmanager.setLatestUid(1);
	}
	catch (const aException & e) { throw aException(e, "ak::aComboButtonWidget::aComboButtonWidget()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aComboButtonWidget::aComboButtonWidget()"); }
	catch (...) { throw aException("Unknown error", "ak::aComboButtonWidget::aComboButtonWidget()"); }
}

//! @brief Deconstructor
ak::aComboButtonWidget::~aComboButtonWidget() { A_OBJECT_DESTROYING clearItems(); }

// #######################################################################################################
// Event handling

void ak::aComboButtonWidget::keyPressEvent(QKeyEvent *_event)
{
	QPushButton::keyPressEvent(_event);
	emit keyPressed(_event);
}

void ak::aComboButtonWidget::keyReleaseEvent(QKeyEvent * _event) {
	QPushButton::keyReleaseEvent(_event);
	emit keyReleased(_event);
}

// #######################################################################################################

void ak::aComboButtonWidget::clearItems(void) {
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

void ak::aComboButtonWidget::setItems(
	const std::vector<QString> &		_menu
) {
	try {
		clearItems();
		for (int i = 0; i < _menu.size(); i++) {
			// Create new item
			ak::aComboButtonWidgetItem * n_itm = nullptr;
			n_itm = new ak::aComboButtonWidgetItem(_menu.at(i));
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
	catch (const aException & e) { throw aException(e, "ak::aComboButtonWidget::setItems()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aComboButtonWidget::setItems()"); }
	catch (...) { throw aException("Unknown error", "ak::aComboButtonWidget::setItems()"); }
}

int ak::aComboButtonWidget::addItem(
	const QString &						_other
) {
	try {
		// Create new item
		ak::aComboButtonWidgetItem * n_itm = nullptr;
		n_itm = new ak::aComboButtonWidgetItem(_other);
		if (n_itm == nullptr) { throw aException("Failed to create", "Create aComboButtonWidgetItem"); }
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
	catch (const aException & e) { throw aException(e, "ak::aComboButtonWidget::addItem()"); }
	catch (const std::exception & e) { throw aException(e.what(), "ak::aComboButtonWidget::addItem()"); }
	catch (...) { throw aException("Unknown error", "ak::aComboButtonWidget::addItem()"); }
}

int ak::aComboButtonWidget::getItemCount(void) const { return my_items.size(); }

void ak::aComboButtonWidget::setColorStyle(
	const aColorStyle *								_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	QString sheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls));
	sheet.append(my_colorStyle->toStyleSheet(cafForegroundColorControls |
		cafBackgroundColorControls | cafBackgroundColorAlternate, "QPushButton QMenu{", "}"));
	this->setStyleSheet(sheet);
}

void ak::aComboButtonWidget::slotItemTriggered() {
	// Cast the QObject to the aComboButtonWidgetItem
	ak::aComboButtonWidgetItem * itm = dynamic_cast<ak::aComboButtonWidgetItem *>(sender());
	assert(itm != nullptr); // failed to cast. but item was expected
	auto obj = my_itemUids.find(itm->uid());
	assert(obj != my_itemUids.end()); // Invalid data stored
	setText(my_items.at(obj->second)->text());
	emit changed();
}

QWidget * ak::aComboButtonWidget::widget(void) { return this; }
