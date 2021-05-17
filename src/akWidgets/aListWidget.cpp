/*
 *	File:		aListWidget.cpp
 *	Package:	akWidgets
 *
 *  Created on: October 12, 2020
 *	Author: Alexander Kuester
 *  Copyright (c) 2020 Alexander Kuester
 *	This file is part of the uiCore component.
 *	This file is subject to the terms and conditions defined in
 *	file 'LICENSE', which is part of this source code package.
 */

// AK header
#include <akGui/aColorStyle.h>
#include <akWidgets/aListWidget.h>

// Qt header
#include <qevent.h>
#include <qscrollbar.h>

ak::aListWidget::aListWidget()
	: aWidget(otList), my_currentId(invalidID), my_verticalScrollbarAlwaysVisible(true)
{}

ak::aListWidget::~aListWidget() {
	A_OBJECT_DESTROYING
	memFree();
}

QWidget * ak::aListWidget::widget(void) { return this; }

void ak::aListWidget::setColorStyle(
	const aColorStyle *			_colorStyle
) {
	assert(_colorStyle != nullptr); // nullptr provided
	my_colorStyle = _colorStyle;
	if (my_alias.length() > 0) {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
			cafBackgroundColorControls | cafBackgroundColorAlternate,
			"#" + my_alias + "{", "}"));
	}
	else {
		this->setStyleSheet(my_colorStyle->toStyleSheet(cafForegroundColorControls |
			cafBackgroundColorControls | cafBackgroundColorAlternate));
	}
}

void ak::aListWidget::keyPressEvent(QKeyEvent *_event)
{ QListWidget::keyPressEvent(_event); emit keyPressed(_event); }

void ak::aListWidget::keyReleaseEvent(QKeyEvent * _event)
{ QListWidget::keyReleaseEvent(_event); emit keyReleased(_event); }

void ak::aListWidget::mouseMoveEvent(QMouseEvent * _event)
{ QListWidget::mouseMoveEvent(_event); emit mouseMove(_event); }

void ak::aListWidget::enterEvent(QEvent *_event)
{
	QListWidget::leaveEvent(_event);
	if (!my_verticalScrollbarAlwaysVisible) {
		verticalScrollBar()->setVisible(true);
	}
	emit leave(_event);
}

void ak::aListWidget::leaveEvent(QEvent *_event)
{
	QListWidget::leaveEvent(_event);
	if (!my_verticalScrollbarAlwaysVisible) {
		verticalScrollBar()->setVisible(false);
	}
	emit leave(_event);
}

// ###########################################################################################################################################

//! @brief Will add a new item to this aListWidget
				//! @param _text The item text
ak::ID ak::aListWidget::AddItem(
	const QString &					_text
) {
	aListWidgetItem * nItm = new aListWidgetItem(_text, ++my_currentId, this);
	my_items.insert_or_assign(nItm->id(), nItm);
	//addItem(nItm);
	return my_currentId;
}

//! @brief Will add a new item to this aListWidget
//! @param _icon The item icon
//! @param _text The item text
ak::ID ak::aListWidget::AddItem(
	const QIcon &					_icon,
	const QString &					_text
) {
	aListWidgetItem * nItm = new aListWidgetItem(_icon, _text, ++my_currentId, this);
	my_items.insert_or_assign(nItm->id(), nItm);
	//addItem(nItm);
	return my_currentId;
}

ak::aListWidgetItem * ak::aListWidget::Item(
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

ak::aListWidgetItem * ak::aListWidget::Item(
	ak::ID							_id
) {
	auto itm = my_items.find(_id);
	assert(itm != my_items.end()); // Invalid item id provided
	assert(itm->second != nullptr);	// nullptr stored
	return itm->second;
}

QString ak::aListWidget::itemText(
	ak::ID							_id
) {
	aListWidgetItem * itm = Item(_id);
	return itm->text();
}

void ak::aListWidget::setItemText(
	ak::ID							_id,
	const QString &					_text
) {
	aListWidgetItem * itm = Item(_id);
	itm->setText(_text);
}

void ak::aListWidget::setItemIcon(
	ak::ID							_id,
	const QIcon &					_icon
) {
	aListWidgetItem * itm = Item(_id);
	itm->setIcon(_icon);
}

void ak::aListWidget::removeItem(
	ak::ID							_id
) {
	aListWidgetItem * itm = Item(_id);
	removeItemWidget(itm);
	delete itm;
	my_items.erase(_id);
}

void ak::aListWidget::Clear() { clear(); my_currentId = ak::invalidID; memFree(); }

void ak::aListWidget::setVerticalScrollbarAlwaysVisible(
	bool							_vis
) {
	my_verticalScrollbarAlwaysVisible = _vis;
	verticalScrollBar()->setVisible(false);
}

void ak::aListWidget::memFree(void) {
	for (auto itm = my_items.begin(); itm != my_items.end(); itm++) {
		aListWidgetItem * actualItem = itm->second;
		assert(actualItem != nullptr);	// Nullptr stored
		delete actualItem;
	}
	my_items.clear();
}

// ###########################################################################################################################################

// ###########################################################################################################################################

// ###########################################################################################################################################

ak::aListWidgetItem::aListWidgetItem(
	ak::ID				_id,
	aListWidget *				_view,
	int					_type
) : QListWidgetItem(_view, _type), my_id(_id)
{}

ak::aListWidgetItem::aListWidgetItem(
	const QString &		_text,
	ak::ID				_id,
	aListWidget *				_view,
	int					_type
) : QListWidgetItem(_text, _view, _type), my_id(_id)
{}

ak::aListWidgetItem::aListWidgetItem(
	const QIcon &		_icon,
	const QString &		_text,
	ak::ID				_id,
	aListWidget *				_view,
	int					_type
) : QListWidgetItem(_icon, _text, _view, _type), my_id(_id)
{}

ak::aListWidgetItem::~aListWidgetItem() {}

void ak::aListWidgetItem::setId(
	ak::ID				_id
) { my_id = _id; }

ak::ID ak::aListWidgetItem::id(void) const { return my_id; }
