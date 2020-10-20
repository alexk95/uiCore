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

// Qt header
#include <qevent.h>
#include <qscrollbar.h>

ak::ui::qt::list::list()
	: ak::ui::core::aWidget(ak::ui::core::oList), my_currentId(ak::invalidID), my_verticalScrollbarAlwaysVisible(true)
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

void ak::ui::qt::list::keyPressEvent(QKeyEvent *_event)
{ QListWidget::keyPressEvent(_event); emit keyPressed(_event); }

void ak::ui::qt::list::keyReleaseEvent(QKeyEvent * _event)
{ QListWidget::keyReleaseEvent(_event); emit keyReleased(_event); }

void ak::ui::qt::list::mouseMoveEvent(QMouseEvent * _event)
{ QListWidget::mouseMoveEvent(_event); emit mouseMove(_event); }

void ak::ui::qt::list::enterEvent(QEvent *_event)
{
	QListWidget::leaveEvent(_event);
	if (!my_verticalScrollbarAlwaysVisible) {
		verticalScrollBar()->setVisible(true);
	}
	emit leave(_event);
}

void ak::ui::qt::list::leaveEvent(QEvent *_event)
{
	QListWidget::leaveEvent(_event);
	if (!my_verticalScrollbarAlwaysVisible) {
		verticalScrollBar()->setVisible(false);
	}
	emit leave(_event);
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

ak::ui::qt::listItem * ak::ui::qt::list::Item(
	ak::ID							_id
) {
	my_itemsIterator itm = my_items.find(_id);
	assert(itm != my_items.end()); // Invalid item id provided
	assert(itm->second != nullptr);	// nullptr stored
	return itm->second;
}

QString ak::ui::qt::list::itemText(
	ak::ID							_id
) {
	listItem * itm = Item(_id);
	return itm->text();
}

void ak::ui::qt::list::setItemText(
	ak::ID							_id,
	const QString &					_text
) {
	listItem * itm = Item(_id);
	itm->setText(_text);
}

void ak::ui::qt::list::setItemIcon(
	ak::ID							_id,
	const QIcon &					_icon
) {
	listItem * itm = Item(_id);
	itm->setIcon(_icon);
}

void ak::ui::qt::list::removeItem(
	ak::ID							_id
) {
	listItem * itm = Item(_id);
	removeItemWidget(itm);
	delete itm;
	my_items.erase(_id);
}

void ak::ui::qt::list::Clear() { clear(); my_currentId = ak::invalidID; memFree(); }

void ak::ui::qt::list::setVerticalScrollbarAlwaysVisible(
	bool							_vis
) {
	my_verticalScrollbarAlwaysVisible = _vis;
	verticalScrollBar()->setVisible(false);
}

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
