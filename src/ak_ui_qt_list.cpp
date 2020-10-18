/*
 * ak_ui_qt_list.cpp
 *
 *  Created on: October 12, 2020
 *	Last modified on: October 12, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <ak_ui_qt_list.h>
#include <ak_ui_colorStyle.h>

ak::ui::qt::list::list()
	: ak::ui::core::aWidget(ak::ui::core::oList), my_currentId(ak::invalidID)
{}

ak::ui::qt::list::~list() {
	memFree();
}

QWidget * ak::ui::qt::list::widget(void) { return this; }

void ak::ui::qt::list::setColorStyle(
	const ak::ui::colorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr);	// nullptr provided
	my_colorStyle = _colorStyle;
	setStyleSheet(_colorStyle->getStylesheet(ui::colorStyle::styleableObject::sList));
}

// ###########################################################################################################################################

//! @brief Will add a new item to this list
				//! @param _text The item text
ak::ID ak::ui::qt::list::AddItem(
	const QString &					_text
) {
	listItem * nItm = new listItem(_text, ++my_currentId, this);
	my_items.insert_or_assign(nItm->id(), nItm);
	//addItem(nItm);
	return my_currentId;
}

//! @brief Will add a new item to this list
//! @param _icon The item icon
//! @param _text The item text
ak::ID ak::ui::qt::list::AddItem(
	const QIcon &					_icon,
	const QString &					_text
) {
	listItem * nItm = new listItem(_icon, _text, ++my_currentId, this);
	my_items.insert_or_assign(nItm->id(), nItm);
	//addItem(nItm);
	return my_currentId;
}

ak::ui::qt::listItem * ak::ui::qt::list::Item(
	const QString &					_text
) {
	for (auto itm : my_items) {
		if (itm.second->text() == _text) {
			return itm.second;
		}
	}
	assert(0); // Item does not exist
	return nullptr;
}

void ak::ui::qt::list::Clear() { clear(); memFree(); }

void ak::ui::qt::list::memFree(void) {
	for (my_itemsIterator itm = my_items.begin(); itm != my_items.end(); itm++) {
		listItem * actualItem = itm->second;
		assert(actualItem != nullptr);	// Nullptr stored
		delete actualItem;
	}
	my_items.clear();
}

// ###########################################################################################################################################

// ###########################################################################################################################################

// ###########################################################################################################################################

ak::ui::qt::listItem::listItem(
	ak::ID				_id,
	list *				_view,
	int					_type
) : QListWidgetItem(_view, _type), my_id(_id)
{}

ak::ui::qt::listItem::listItem(
	const QString &		_text,
	ak::ID				_id,
	list *				_view,
	int					_type
) : QListWidgetItem(_text, _view, _type), my_id(_id)
{}

ak::ui::qt::listItem::listItem(
	const QIcon &		_icon,
	const QString &		_text,
	ak::ID				_id,
	list *				_view,
	int					_type
) : QListWidgetItem(_icon, _text, _view, _type), my_id(_id)
{}

ak::ui::qt::listItem::~listItem() {}

void ak::ui::qt::listItem::setId(
	ak::ID				_id
) { my_id = _id; }

ak::ID ak::ui::qt::listItem::id(void) const { return my_id; }
